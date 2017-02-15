/*********************************************************************

          File: IPC.cpp

   Description: IPC Source Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2017/02/06

 Copyright (c) 2017, MinLC(Minimalist Library Collections) Project,
 All rights reserved.
 Third party copyrights are property of their respective owners.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in
    the documentation and/or other materials provided with the
    distribution.
 3. Neither the name of the copyright holder nor the names of its
    contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 OF THE POSSIBILITY OF SUCH DAMAGE.

*********************************************************************/

#include "IPC.hpp"

#if defined(_IPC_hpp_)

#include "Timer.hpp"

// for Transceiver
//@private
IPC::Transceiver *G_Transceiver;

void IPC::Transceiver::_Initialize() {
  _FileSrc = "";
  _TransceiverStarted = false;
  _IsConnected = false;
  _TransceiverThreadLoopIn = false;
  _RequesterThreadLoopIn = false;

  G_Transceiver = this;

  _TransceiverThread = mlc::Ptr<Thread>(new Thread(), DeletePointer<Thread>);
  _RequesterThread = mlc::Ptr<Thread>(new Thread(), DeletePointer<Thread>);

  _SyncSignal_TransmitMsgQueue = mlc::Ptr<SyncSignal>(new SyncSignal(), DeletePointer<SyncSignal>);
  _SyncSignal_TransceiverThreadLoopIn = mlc::Ptr<SyncSignal>(new SyncSignal(), DeletePointer<SyncSignal>);
  _SyncSignal_RequesterThreadWakeUp = mlc::Ptr<SyncSignal>(new SyncSignal(), DeletePointer<SyncSignal>);
}

void IPC::Transceiver::_Deinitialize() {
  _FileSrc = "";

  G_Transceiver = NULL;

  Stop_Transceiver();
}

bool IPC::Transceiver::_OpenConnection() {
  bool _TResult = false;

  if (_FileSrc != "") {
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
      strcpy(_TransceiverFd.sun_path, _FileSrc.c_str());

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
  _TransceiverThread->StartThread(_Transceiver_ConnectionThread, this);
}

void IPC::Transceiver::_SafeTransceiverThreadExit() {
  if (_TransceiverThreadLoopIn) {
    if (_SyncSignal_TransmitMsgQueue != NULL)
      _SyncSignal_TransmitMsgQueue->Signal();
    if (_SyncSignal_TransceiverThreadLoopIn != NULL)
      _SyncSignal_TransceiverThreadLoopIn->Wait(); // wait for exit the thread loop
  }

  if (_RequesterThreadLoopIn) {
    // if try that connection request..
    _IsConnected = true; // _IsConnected assign the true that temporary..
    _SyncSignal_RequesterThreadWakeUp->Signal(); // call signal and wake up thread.
  }
}

template<typename T>
bool IPC::Transceiver::_IsEmptyQueue(queue<T> __Queue, ThreadMutex __Mutex) {
  bool _TResult = false;

  __MUTEXLOCK(__Mutex);
  _TResult = __Queue.empty();
  __MUTEXUNLOCK(__Mutex);

  return _TResult;
}

void *IPC::Transceiver::_Transceiver_ConnectionThread(void *Param) {
  IPC::Transceiver *_TTransceiver = (IPC::Transceiver *) Param;

  _TTransceiver->_TransceiverThreadLoopIn = true; // in

  while (_TTransceiver->_TransceiverStarted) {
    if (!_TTransceiver->_IsEmptyQueue(_TTransceiver->_TransmitMsgQueue,
                                     _TTransceiver->_Mutex_TransmitMsgQueue)) {
      string _TTransStr = _TTransceiver->_TransmitMsgQueue.front();

      write(_TTransceiver->_TransceiverSocket, (char *)_TTransStr.c_str(), _TTransStr.length());
      _TTransceiver->_TransmitMsgQueue.pop();
    }
    else
      _TTransceiver->_SyncSignal_TransmitMsgQueue->Wait();
  }

  _TTransceiver->_TransceiverThreadLoopIn = false; // out
  _TTransceiver->_SyncSignal_TransceiverThreadLoopIn->Signal();
  return 0;
}

void *IPC::Transceiver::_Transceiver_RequesterThread(void *Param) {
  IPC::Transceiver *_TTransceiver = (IPC::Transceiver *) Param;
  mlc::Ptr<Timer> _TTimer(new Timer(), DeletePointer<Timer>);

  _TTransceiver->_RequesterThreadLoopIn = true; // in

  _TTimer->TTimerExpiredNotifier = _TTransceiver->_TimerRequestExpiredNotifier;
  _TTimer->Set_Timer(2000000); // 2 sec.
  _TTimer->Start_Timer();

  while(!_TTransceiver->_IsConnected) {
    _TTransceiver->_SyncSignal_RequesterThreadWakeUp->Wait();
  }

  _TTransceiver->_RequesterThreadLoopIn = true; // in

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

  _SafeTransceiverThreadExit();
  _CloseConnection();
}

void IPC::Transceiver::Send_Transceiver(string StrMsg) {
  if (_TransceiverStarted) {
    _TransmitMsgQueue.push(StrMsg);
    _SyncSignal_TransmitMsgQueue->Signal();
  }
}

// for Receiver
//@private
void IPC::Receiver::_Initialize() {
  _FileSrc = "";
  _ReceiverStarted = false;
  _IsConnected = false;

  _ReceiverThread = mlc::Ptr<Thread>(new Thread(), DeletePointer<Thread>);
  _SyncSignal_RecvMsgQueue = mlc::Ptr<SyncSignal>(new SyncSignal(), DeletePointer<SyncSignal>);
}

void IPC::Receiver::_Deinitialize() {
  _FileSrc = "";
  _ReceiverStarted = false;
  _IsConnected = false;
}

bool IPC::Receiver::_OpenConnection() {
  bool _TResult = false;

  if (_FileSrc != "") {
    if (access(_FileSrc.c_str(), F_OK) == 0) {
      unlink(_FileSrc.c_str());
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
      strcpy(_ReceiverFd.sun_path, _FileSrc.c_str());

      if (bind(_ReceiverSocket, (sockaddr *)&_ReceiverFd, sizeof(_ReceiverFd)) != 0) {
        _CloseConnection();
        return false;
      }

      if (listen(_ReceiverSocket, DEFAULT_TCP_LISTEN_QUEUE) != 0) {
        _CloseConnection();
        return false;
      }
      _TResult = true;
    }
  }
  return _TResult;
}

bool IPC::Receiver::_CloseConnection() {
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

void *IPC::Receiver::_Receiver_ConnectionThread(void *Param) {
  IPC::Receiver *_TReceiver = (IPC::Receiver *) Param;

  //char *_TCharBuf = new char[BUFFER_MAX_4096];
  mlc::Ptr<char> _TCharBuf(new char[BUFFER_MAX_4096], DeletePointer<char>);
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
      while (_TReceiver->_ReceiverStarted) {
        memset(_TCharBuf, 0, sizeof(_TCharBuf));

        if (read(_TReceiver->_TransceiverSocket, _TCharBuf, BUFFER_MAX_4096) > 0) {
          string _TStr = string(_TCharBuf);

        }
        /*
        if (_TReceiver->_IsEmptyQueue(_TReceiver->_RecvMsgQueue,
                                      _TReceiver->_Mutex_RecvMsgQueue)) {

        }
        else
          _TReceiver->_SyncSignal_RecvMsgQueue->Wait();
          */
      }
    }
  }

  return 0;
}

//@public
bool IPC::Receiver::Start_Receiver() {
  bool _TResult = false;
  if (_OpenConnection()) {
    _ReceiverStarted = true;
    _ReceiverThread->StartThread(_Receiver_ConnectionThread, this);

    _TResult = true;
  }
  return _TResult;
}

bool IPC::Receiver::Close_Receiver() {
  bool _TResult = false;
  if (_CloseConnection()) {

    _TResult = true;
  }
  return _TResult;
}

#endif