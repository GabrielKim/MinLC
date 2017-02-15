/*********************************************************************

          File: IPC.hpp

   Description: IPC Header Part.

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
    mlc::Ptr<Thread> _TransceiverThread;
    mlc::Ptr<Thread> _RequesterThread;

    string _FileSrc;

    T_SOCKET _TransceiverSocket;
    T_SOCKADDR_UN _TransceiverFd;

    queue<string> _TransmitMsgQueue;
    ThreadMutex _Mutex_TransmitMsgQueue;
    mlc::Ptr<SyncSignal> _SyncSignal_TransmitMsgQueue;

    mlc::Ptr<SyncSignal> _SyncSignal_TransceiverThreadLoopIn;
    mlc::Ptr<SyncSignal> _SyncSignal_RequesterThreadWakeUp;

    bool _TransceiverStarted;
    bool _TransceiverThreadLoopIn;
    bool _RequesterThreadLoopIn;
    bool _IsConnected;

    void _Initialize();
    void _Deinitialize();
    bool _OpenConnection();
    void _CloseConnection();

    void _SafeTransceiverThreadActivate();
    void _SafeTransceiverThreadExit();

    template <typename T> bool _IsEmptyQueue(queue<T> __Queue, ThreadMutex __Mutex);

    static void *_Transceiver_ConnectionThread(void *Param);
    static void *_Transceiver_RequesterThread(void *Param);

    static void _TimerRequestExpiredNotifier(void);
  public:
    Transceiver() { _Initialize(); }
    ~Transceiver() { _Deinitialize(); }

    bool Start_Transceiver();
    void Stop_Transceiver();
    void Send_Transceiver(string StrMsg);

    IMPLEMENT_SET(string, FileSrc, _FileSrc);
  };

  class Receiver { // like server.
  private:
    mlc::Ptr<Thread> _ReceiverThread;
    string _FileSrc;

    T_SOCKET _ReceiverSocket, _TransceiverSocket;
    T_SOCKADDR_UN _ReceiverFd, _TransceiverFd;

    queue<string> _RecvMsgQueue;
    ThreadMutex _Mutex_RecvMsgQueue;
    mlc::Ptr<SyncSignal> _SyncSignal_RecvMsgQueue;

    bool _ReceiverStarted;
    bool _IsConnected;

    void _Initialize();
    void _Deinitialize();
    bool _OpenConnection();
    bool _CloseConnection();

    template <typename T> bool _IsEmptyQueue(queue<T> __Queue, ThreadMutex __Mutex);

    static void *_Receiver_ConnectionThread(void *Param);
  public:
    Receiver() { _Initialize(); }
    ~Receiver() { _Deinitialize(); }

    bool Start_Receiver();
    bool Close_Receiver();

    IMPLEMENT_SET(string, FileSrc, _FileSrc);
  };
};

#endif // _IPC_hpp_

#endif