/*********************************************************************

          File: IPC.cpp

   Description: IPC Source Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2017/02/06

 Copyright (c) 2017, MinLC(Minimalist Library Collections) Project,
 All rights reserved.

 ** The "MinLC" is distributed under the "3-clause BSD" license.
    See details COPYING.

*********************************************************************/

#include "IPC.hpp"

#if defined(_IPC_hpp_)

#include "proto_ipc.def.hpp"

#include "PacketStructer.hpp"
#include "Timer.hpp"

typedef struct pollfd IPCPollFd;

// for Transceiver
IPC::Transceiver *G_Transceiver;

//@private
void IPC::Transceiver::_Initialize() {
  _TransceiverStarted = false;
  _IsConnected = false;

  _TransmitThreadLoopIn = false;
  _RequesterThreadLoopIn = false;

  G_Transceiver = this;

  Options = mlc::Ptr<TransceiverVariables>(new TransceiverVariables(), mlc::DeletePointer<TransceiverVariables>);
  Options->FileSrc = "";

  _TransmitThread = mlc::Ptr<Thread>(new Thread(), mlc::DeletePointer<Thread>);
  _RequesterThread = mlc::Ptr<Thread>(new Thread(), mlc::DeletePointer<Thread>);

  _SyncSignal_TransmitMsgQueue = mlc::Ptr<SyncSignal>(new SyncSignal(), mlc::DeletePointer<SyncSignal>);
  _SyncSignal_TransmitThreadLoopIn = mlc::Ptr<SyncSignal>(new SyncSignal(), mlc::DeletePointer<SyncSignal>);
  _SyncSignal_RequesterThreadWakeUp = mlc::Ptr<SyncSignal>(new SyncSignal(), mlc::DeletePointer<SyncSignal>);
}

void IPC::Transceiver::_Deinitialize() {
  Options->FileSrc = "";

  G_Transceiver = NULL;

  Stop_Transceiver();
}

bool IPC::Transceiver::_OpenConnection() {
  bool _TResult = false;

  if (Options->FileSrc != "") {
    _TransceiverSocket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (_TransceiverSocket !=
#if defined(WINDOWS_SYS)
        INVALID_SOCKET
#elif defined(POSIX_SYS)
        -1
#endif
        ) {
      int _TLen = -1;

      bzero(&_TransceiverFd, sizeof(_TransceiverFd));
      _TransceiverFd.sun_family = AF_UNIX;
      strcpy(_TransceiverFd.sun_path, Options->FileSrc.c_str());

      _TLen = sizeof(_TransceiverFd);

      if (connect(_TransceiverSocket,
                  (sockaddr *)&_TransceiverFd, _TLen) < 0) {
        _CloseConnection();
      }
      else {
        _IsConnected = true;
        _TResult = true;
      }
    }
  }

  return _TResult;
}

void IPC::Transceiver::_CloseConnection() {
  if (_TransceiverSocket != NULL) {
    shutdown(_TransceiverSocket, 0x02); // BOTH.
#if defined(WINDOWS_SYS)
    closesocket(_ClientSocket);
#elif defined(POSIX_SYS)
    close(_TransceiverSocket);
#endif
  }
  _IsConnected = false;
}

void IPC::Transceiver::_SafeTransceiverThreadActivate() {
  _TransceiverStarted = true;

  _TransmitThread->StartThread(_Transceiver_TransmitThread, this);
}

void IPC::Transceiver::_SafeTransceiverThreadsExit() {
  if (_TransmitThreadLoopIn) {
    if (_SyncSignal_TransmitMsgQueue != NULL)
      _SyncSignal_TransmitMsgQueue->Signal();
    if (_SyncSignal_TransmitThreadLoopIn != NULL)
      _SyncSignal_TransmitThreadLoopIn->Wait(); // wait for exit the thread loop
  }

  if (_RequesterThreadLoopIn) {
    // if try that connection request..
    _IsConnected = true; // _IsConnected assign the true that temporary..
    _SyncSignal_RequesterThreadWakeUp->Signal(); // call signal and wake up thread.
  }
}

void IPC::Transceiver::_TransceiverCommunicationProcess() {
  mlc::Ptr<PacketStreamStructer>
      _TPacketStreamStructer(new PacketStreamStructer(), mlc::DeletePointer<PacketStreamStructer>);

  while (_TransceiverStarted) {
    if (!mlc::IsEmptySTLObject_Safe(_TransmitMsgQueue,
                                    _Mutex_TransmitMsgQueue)) {
      string _TTransStr = _TransmitMsgQueue.front();

      int _TCommandStatus = static_cast<int>(IPC_NO_COMMAND);
      bool _TTrasmitStatus = false; // true is data, false is type.
      int _TTransmitDataSize = 0;

      while (1) {
        if (!_TTrasmitStatus) {
          switch (_TCommandStatus) {
            case IPC_NO_COMMAND :
              _TCommandStatus = static_cast<int>(IPC_PACKET_SIZE); break;
            case IPC_PACKET_SIZE :
              _TCommandStatus = static_cast<int>(IPC_PACKET_SEG_SIZE); break;
            case IPC_PACKET_SEG_SIZE :
              _TCommandStatus = static_cast<int>(IPC_TRANS_DATA); break;
            case IPC_TRANS_DATA :
              {
                if (_TTransStr.length() == _TTransmitDataSize) {
                  _TCommandStatus = static_cast<int>(IPC_TRANS_END); break;
                }
              }
          }

          string _TStr = _TPacketStreamStructer->Convert(_TCommandStatus);
          write(_TransceiverSocket, (char *)_TStr.c_str(), _TStr.length());

          _TTrasmitStatus = true;
        }
        else {
          mlc::Ptr<char> _TReadBuf(new char[4], mlc::DeletePointer<char>);
          memset(_TReadBuf, 0, sizeof(_TReadBuf));

          if (read(_TransceiverSocket, _TReadBuf, 4) > 0) {
            string _TReceiveStr = string(_TReadBuf);
            int _TTypes = -1;

            _TPacketStreamStructer->Restore(_TReceiveStr, _TTypes, "int");
            if (_TTypes == static_cast<int>(IPC_COMMIT_OK)) {
              auto _TVar;
              string _TStr = "";
              bool _TIsLoopBreak = false;

              switch (_TCommandStatus) {
                case IPC_PACKET_SIZE :
                  _TVar = Options->MaxTransSize; break;
                case IPC_PACKET_SEG_SIZE :
                  _TVar = Options->SegTransSize; break;
                case IPC_TRANS_DATA :
                  {
                    mlc::Ptr<char> _TTransBuf(new char[Options->SegTransSize], mlc::DeletePointer<char>);
                    _TStr = string(_TTransStr.substr(_TTransmitDataSize, Options->SegTransSize - 1).c_str());
                    _TTransmitDataSize += Options->SegTransSize;
                  }
                case IPC_TRANS_END :
                  _TIsLoopBreak = true; break;
              }

              if (_TIsLoopBreak) break; // end of loop.

              if (_TCommandStatus != static_cast<int>(IPC_TRANS_DATA))
                _TStr = _TPacketStreamStructer->Convert(_TVar);
              write(_TransceiverSocket, (char *)_TStr.c_str(), _TStr.length());
            }
          }
          _TTrasmitStatus = false;
        }
      }

      _TransmitMsgQueue.pop();
    }
    else
      _SyncSignal_TransmitMsgQueue->Wait();
  }
}

void *IPC::Transceiver::_Transceiver_TransmitThread(void *Param) {
  IPC::Transceiver *_TTransceiver = (IPC::Transceiver *) Param;

  _TTransceiver->_TransmitThreadLoopIn = true; // check in

  _TTransceiver->_TransceiverCommunicationProcess();

  _TTransceiver->_TransmitThreadLoopIn = false; // check out
  _TTransceiver->_SyncSignal_TransmitThreadLoopIn->Signal();

  return 0;
}

void *IPC::Transceiver::_Transceiver_RequesterThread(void *Param) {
  IPC::Transceiver *_TTransceiver = (IPC::Transceiver *) Param;
  mlc::Ptr<Timer> _TTimer(new Timer(), mlc::DeletePointer<Timer>);

  _TTransceiver->_RequesterThreadLoopIn = true; // check in

  _TTimer->TTimerExpiredNotifier = _TTransceiver->_TimerRequestExpiredNotifier;
  _TTimer->Set_Timer(2000000); // 2 sec.
  _TTimer->Start_Timer();

  while(!_TTransceiver->_IsConnected) {
    _TTransceiver->_SyncSignal_RequesterThreadWakeUp->Wait();
  }

  _TTimer->Stop_Timer();

  _TTransceiver->_RequesterThreadLoopIn = false; // check out

  return 0;
}

void IPC::Transceiver::_TimerRequestExpiredNotifier(void) {
  if (G_Transceiver->_OpenConnection()) {
    // Retry ever while connected.
    G_Transceiver->_SyncSignal_RequesterThreadWakeUp->Signal();
    G_Transceiver->_SafeTransceiverThreadActivate();
  }
}

//@public
bool IPC::Transceiver::Start_Transceiver() {
  bool _TResult = false;

  if (_OpenConnection()) {
    _SafeTransceiverThreadActivate();

    _TResult = true;
  }
  else {
    _RequesterThread->StartThread(_Transceiver_RequesterThread, this);
  }

  return _TResult;
}

void IPC::Transceiver::Stop_Transceiver() {
  _TransceiverStarted = false;

  _SafeTransceiverThreadsExit();
  _CloseConnection();
}

void IPC::Transceiver::Send_Transceiver(string StrMsg) {
  if (_TransceiverStarted) {
    _TransmitMsgQueue.push(StrMsg);
    _SyncSignal_TransmitMsgQueue->Signal();
  }
}

// for Receiver
IPC::Receiver *G_Receiver;

//@private
void IPC::Receiver::_Initialize() {
  _ReceiverStarted = false;
  _IsConnected = false;

  _ReceiveThreadLoopIn = false;
  _MessageBackThreadLoopIn = false;

  G_Receiver = this;

  Options = mlc::Ptr<ReceiverVariables>(new ReceiverVariables(), mlc::DeletePointer<ReceiverVariables>);
  Options->FileSrc = "";

  _ReceiveThread = mlc::Ptr<Thread>(new Thread(), mlc::DeletePointer<Thread>);
  _MessageBackThread = mlc::Ptr<Thread>(new Thread(), mlc::DeletePointer<Thread>);

  _SyncSignal_ReceiveMsgQueue = mlc::Ptr<SyncSignal>(new SyncSignal(), mlc::DeletePointer<SyncSignal>);
}

void IPC::Receiver::_Deinitialize() {
  //_FileSrc = "";
  Options->FileSrc = "";
  _ReceiverStarted = false;
  _IsConnected = false;

  Stop_Receiver();
}

bool IPC::Receiver::_OpenConnection() {
  bool _TResult = false;

  if (Options->FileSrc != "") {
    if (access(Options->FileSrc.c_str(), F_OK) == 0) {
      unlink(Options->FileSrc.c_str());
    }

    _ReceiverSocket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (_ReceiverSocket !=
#if defined(WINDOWS_SYS)
        INVALID_SOCKET
#elif defined(POSIX_SYS)
        -1
#endif
        ) {
      bzero(&_ReceiverFd, sizeof(_ReceiverFd));
      _ReceiverFd.sun_family = AF_UNIX;
      strcpy(_ReceiverFd.sun_path, Options->FileSrc.c_str());

      if (bind(_ReceiverSocket, (sockaddr *)&_ReceiverFd, sizeof(_ReceiverFd)) != 0) {
        _CloseConnection();
        return false;
      }

      if (listen(_ReceiverSocket, DEFAULT_TCP_LISTEN_QUEUE) != 0) {
        _CloseConnection();
        return false;
      }
      _IsConnected = true;
      _TResult = true;
    }
  }
  return _TResult;
}

void IPC::Receiver::_CloseConnection() {
  if (_TransceiverSocket != NULL) {
    shutdown(_TransceiverSocket, 0x02); // BOTH.
#if defined(WINDOWS_SYS)
    closesocket(_ClientSocket);
#elif defined(POSIX_SYS)
    close(_TransceiverSocket);
#endif
  }
  _IsConnected = false;
}

void IPC::Receiver::_SafeReceiverThreadActivate() {
  _ReceiverStarted = true;

  _ReceiveThread->StartThread(_Receiver_ReceiveThread, this);
  _MessageBackThread->StartThread(_Receiver_MessageBackThread, this);
}

void IPC::Receiver::_SafeReceiverThreadsExit() {
  if (_ReceiveThreadLoopIn) {
    // poll time out is automatic unlock by TIMEOUT state.
    // so, _ReceiverCommunicationProcess function be the safe exit.
  }

  if (_MessageBackThreadLoopIn) {
    if (_SyncSignal_ReceiveMsgQueue != NULL)
      _SyncSignal_ReceiveMsgQueue->Signal();
  }
}

void IPC::Receiver::_ReceiverCommunicationProcess() {
  mlc::Ptr<PacketStreamStructer>
      _TPacketStreamStructer(new PacketStreamStructer(), mlc::DeletePointer<PacketStreamStructer>);

  int _TBufferLength = BUFFER_MAX_4096;
  int _TotalPacketSize = -1;
  string _TReceiveStrBuf = "";

  int _TReceiveStatus = static_cast<int>(IPC_COMMAND_WAIT);
  int _TCommandStatus = static_cast<int>(IPC_NO_COMMAND);
  //bool _TIsReady = false;

  IPCPollFd _TIPCPollFd[IPC_POLL_CONNECTIONS];
  _TIPCPollFd[0].fd = _ReceiverSocket;
  _TIPCPollFd[0].events = POLLIN;
  _TIPCPollFd[1].fd = _TransceiverSocket;
  _TIPCPollFd[1].events = POLLIN;

  while (_ReceiverStarted) {
    int _TFds = poll(_TIPCPollFd, IPC_POLL_CONNECTIONS, IPC_TIMEOUT);

    if (_TIPCPollFd[0].revents & POLLIN) { //  | POLLERR
      if (_TFds == 0) {
        // Time out.
      }
      else {
        if(_TFds > 0) {
          mlc::Ptr<char> _TCharBuf(new char[_TBufferLength], mlc::DeletePointer<char>);
          memset(_TCharBuf, 0, sizeof(_TCharBuf));

          if (read(_TIPCPollFd[1].fd, _TCharBuf, _TBufferLength) > 0) {
            string _TReceiveStr = string(_TCharBuf);

            if (_TReceiveStatus == static_cast<int>(IPC_COMMAND_WAIT)) {
              int _TTypes = -1;
              _TPacketStreamStructer->Restore(_TReceiveStr, _TTypes, "int");

              switch (_TTypes) {
                case IPC_CONNECT :
                  _TCommandStatus = static_cast<int>(IPC_CONNECT); break;
                case IPC_DISCONNECT :
                  _TCommandStatus = static_cast<int>(IPC_DISCONNECT); break;
                case IPC_PACKET_SEG_SIZE :
                  _TCommandStatus = static_cast<int>(IPC_PACKET_SEG_SIZE); break;
                case IPC_PACKET_SIZE :
                  _TCommandStatus = static_cast<int>(IPC_PACKET_SIZE); break;
                case IPC_TRANS_DATA :
                  _TReceiveStatus = static_cast<int>(IPC_DATA_WAIT); break;
                case IPC_TRANS_END :
                  {
                    if (_TCommandStatus == static_cast<int>(IPC_PACKET_SEG_SIZE)) {
                      _TPacketStreamStructer->Restore(_TReceiveStrBuf, _TBufferLength, "int");
                    }
                    else if (_TCommandStatus == static_cast<int>(IPC_PACKET_SIZE)) {
                      _TPacketStreamStructer->Restore(_TReceiveStrBuf, _TotalPacketSize, "int");
                    }
                    else {
                      _ReceiveMsgQueue.push(_TReceiveStr);
                      _SyncSignal_ReceiveMsgQueue->Signal();
                    }
                    _TReceiveStrBuf.clear();

                    _TReceiveStatus = static_cast<int>(IPC_COMMAND_WAIT);
                    _TCommandStatus = static_cast<int>(IPC_TRANS_END);
                    _TBufferLength = BUFFER_MAX_4096;
                  }
                  break;
              }
            }
            else if(_TReceiveStatus == static_cast<int>(IPC_DATA_WAIT)) {
              // if _TReceiveStatus decided value after do that..
              _TReceiveStrBuf.append(_TReceiveStr);

              _TReceiveStatus = static_cast<int>(IPC_COMMAND_WAIT);
            }

            string _TStr = _TPacketStreamStructer->Convert(static_cast<int>(IPC_COMMIT_OK));
            write(_TransceiverSocket, _TStr.c_str(), _TStr.length());
          }
        }
      }
    }
    else {
      if ((_TIPCPollFd[0].revents & POLLHUP)
          || (_TIPCPollFd[0].revents & POLLNVAL)) {
        Stop_Receiver();
      }
      else if (_TIPCPollFd[0].revents & POLLOUT) {

      }
    }
  }
}

void *IPC::Receiver::_Receiver_ReceiveThread(void *Param) {
  IPC::Receiver *_TReceiver = (IPC::Receiver *) Param;

  _TReceiver->_ReceiveThreadLoopIn = true;

  int _TLen = sizeof(_TReceiver->_TransceiverFd);

  _TReceiver->_TransceiverSocket
      = accept(_TReceiver->_ReceiverSocket,
               (struct sockaddr *)&_TReceiver->_TransceiverFd,
#if defined(POSIX_SYS)
               (socklen_t *)
#endif
                   &_TLen);

  pid_t _TPID = fork();

  if (_TPID == 0) {
    if (_TReceiver->_TransceiverSocket != -1) {
      _TReceiver->_ReceiverCommunicationProcess();
    }
  }

  _TReceiver->_ReceiveThreadLoopIn = true;

  return 0;
}

void *IPC::Receiver::_Receiver_MessageBackThread(void *Param) {
  IPC::Receiver *_TReceiver = (IPC::Receiver *) Param;

  _TReceiver->_MessageBackThreadLoopIn = true;

  while (_TReceiver->_ReceiverStarted) {
    if (mlc::IsEmptySTLObject_Safe(_TReceiver->_ReceiveMsgQueue,
                                   _TReceiver->_Mutex_ReceiveMsgQueue)) {
      string _RecvMsg = _TReceiver->_ReceiveMsgQueue.front();

      _TReceiver->TMessageBack(_RecvMsg);
      _TReceiver->_ReceiveMsgQueue.pop();
    }
    else
      _TReceiver->_SyncSignal_ReceiveMsgQueue->Wait();
  }

  _TReceiver->_MessageBackThreadLoopIn = false;

  return 0;
}

//@public
bool IPC::Receiver::Start_Receiver() {
  bool _TResult = false;

  if (_OpenConnection()) {
    _SafeReceiverThreadActivate();

    _TResult = true;
  }

  return _TResult;
}

void IPC::Receiver::Stop_Receiver() {
  _ReceiverStarted = false;

  _SafeReceiverThreadsExit();
  _CloseConnection();
}

#endif