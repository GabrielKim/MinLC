/*********************************************************************

          File: proto_ipc.def.hpp

   Description: proto_ipc.def Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2017/02/16

 Copyright (c) 2017, MinLC(Minimalist Library Collections) Project,
 All rights reserved.

 ** The "MinLC" is distributed under the "3-clause BSD" license.
    See details COPYING.

*********************************************************************/

#ifndef _proto_ipc_def_hpp_
#define _proto_ipc_def_hpp_

enum Proto_IPC {

  // internal receive state
  IPC_COMMAND_WAIT = -1, IPC_DATA_WAIT = -2,
  // send Receiver(Receiver -> Transceiver).
  IPC_COMMIT_OK = 131,
  // internal command state
  IPC_NO_COMMAND = -10,
  // send Transceiver(Transceiver -> Receiver).
  IPC_CONNECT = 161, IPC_DISCONNECT = 162,
  IPC_PACKET_SIZE = 171, IPC_PACKET_SEG_SIZE = 172,
  IPC_TRANS_DATA = 181, IPC_TRANS_END = 182
  //IPC_TRANS_START = 181,
};

#define IPC_POLL_CONNECTIONS 1
#define IPC_TIMEOUT 1000 // just 1 second.

#endif // _proto_ipc_def_hpp_
