/*********************************************************************

          File: IPC.hpp

   Description: IPC Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2017/02/06

 Copyright (c) 2017, MinLC(Minimalist Library Collections) Project,
 All rights reserved.

 ** The "MinLC" is distributed under the "3-clause BSD" license.
    See details COPYING.

*********************************************************************/

#include "dep.inc.hpp"

#if defined(SET_COMMON_MODULE_IPC) && defined(SET_COMMON_MODULE_CONTEXT)

#ifndef _IPC_hpp_
#define _IPC_hpp_

#include "required.inc.hpp"
#include "net.def.hpp"

#include "MacroTools.hpp"

#include "base.hpp"
#include "Thread.hpp"
#include "SyncSignal.hpp"

class IPC {
  /*
    It's IPC Transceiver/Receiver made that Socket.
   */
public:
  class Transceiver { // like client.
  private:
    typedef struct _TransceiverVariables{
      string FileSrc;
      int SegTransSize;
      int MaxTransSize;
    } TransceiverVariables;

    mlc::Ptr<Thread> _TransmitThread;
    mlc::Ptr<Thread> _RequesterThread;

    T_SOCKET _TransceiverSocket;
    T_SOCKADDR_UN _TransceiverFd;

    queue<string> _TransmitMsgQueue;
    ThreadMutex _Mutex_TransmitMsgQueue;
    mlc::Ptr<SyncSignal> _SyncSignal_TransmitMsgQueue;
    mlc::Ptr<SyncSignal> _SyncSignal_TransmitThreadLoopIn;
    mlc::Ptr<SyncSignal> _SyncSignal_RequesterThreadWakeUp;

    bool _TransceiverStarted;
    bool _TransmitThreadLoopIn;
    bool _RequesterThreadLoopIn;
    bool _IsConnected;

    void _Initialize();
    void _Deinitialize();
    bool _OpenConnection();
    void _CloseConnection();

    void _SafeTransceiverThreadActivate();
    void _SafeTransceiverThreadsExit();

    void _TransceiverCommunicationProcess();

    static void *_Transceiver_TransmitThread(void *Param);
    static void *_Transceiver_RequesterThread(void *Param);

    static void _TimerRequestExpiredNotifier(void);
  public:
    mlc::Ptr<TransceiverVariables> Options;

    Transceiver() { _Initialize(); }
    ~Transceiver() { _Deinitialize(); }

    bool Start_Transceiver();
    void Stop_Transceiver();
    void Send_Transceiver(string StrMsg);

    //IMPLEMENT_SET(string, FileSrc, _FileSrc);
  };

  class Receiver { // like server.
  private:
    typedef struct _ReceiverVariables{
      string FileSrc;
    } ReceiverVariables;

    mlc::Ptr<Thread> _ReceiveThread;
    mlc::Ptr<Thread> _MessageBackThread;

    T_SOCKET _ReceiverSocket, _TransceiverSocket;
    T_SOCKADDR_UN _ReceiverFd, _TransceiverFd;

    queue<string> _ReceiveMsgQueue;
    ThreadMutex _Mutex_ReceiveMsgQueue;
    mlc::Ptr<SyncSignal> _SyncSignal_ReceiveMsgQueue;

    bool _ReceiverStarted;
    bool _ReceiveThreadLoopIn;
    bool _MessageBackThreadLoopIn;
    bool _IsConnected;

    void _Initialize();
    void _Deinitialize();
    bool _OpenConnection();
    void _CloseConnection();

    void _SafeReceiverThreadActivate();
    void _SafeReceiverThreadsExit();

    void _ReceiverCommunicationProcess();

    static void *_Receiver_ReceiveThread(void *Param);
    static void *_Receiver_MessageBackThread(void *Param);
  public:
    mlc::Ptr<ReceiverVariables> Options;

    Receiver() { _Initialize(); }
    ~Receiver() { _Deinitialize(); }

    typedef void (*_T_MESSAGEBACK)(string Msg);
    _T_MESSAGEBACK TMessageBack;

    bool Start_Receiver();
    void Stop_Receiver();

    //IMPLEMENT_SET(string, FileSrc, _FileSrc);
  };
};

#endif // _IPC_hpp_

#endif