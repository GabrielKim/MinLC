/*********************************************************************

          File: Telepathy.hpp

   Description: Telepathy Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/18

 Copyright (c) 2017, MinLC(Minimalist Library Collections) Project,
 All rights reserved.

 ** The "MinLC" is distributed under the "3-clause BSD" license.
    See details COPYING.

*********************************************************************/

#include "dep.inc.hpp"

#if defined(SET_COMMON_MODULE_ETHERNET) && defined(SET_COMMON_MODULE_CONTEXT)

#ifndef _Telepathy_hpp_
#define _Telepathy_hpp_

#include "required.inc.hpp"
#include "net.def.hpp"

#include "MacroTools.hpp"
#include "Thread.hpp"

typedef struct _ClientsList {
#if defined(WINDOWS_SYS)
  SOCKADDR_IN ClientAddress;
  SOCKET ClientSocket;
#elif defined(POSIX_SYS)
  sockaddr_in ClientAddress;
  int ClientSocket;
#endif
  int ClientNumber;
  // 이름 및 type의 최대 길이는 32까지.
  char *ClientType;
  char *ClientName;

  void ClientsListInitialize() {
    ClientNumber = -1;
    ClientType = new char[BUFFER_MAX_64];
    ClientName = new char[BUFFER_MAX_64];
  }

  void ClientsListDeinitialize() {
    ClientNumber = -1;
    delete ClientType;
    delete ClientName;
  }

  _ClientsList() { ClientsListInitialize(); }
  //~_ClientsList() { ClientsListDeinitialize(); }
} ClientsList;

typedef struct pollfd TCPPollFd;

class Telepathy {
private:

public:
  // Server Class
  class Server {
  private:
    Thread _Thread;

    T_SOCKADDR_IN _ServerAddress;
    T_SOCKET _ServerSocket;
#if defined(WINDOWS_SYS)
    WSADATA _WSAData;
#elif defined(POSIX_SYS)
    TCPPollFd _Fds[POLL_MAX_CONNECTIONS];
    int _ConnectionNumber;

    int _AddClient();
    bool _Receive(T_SOCKET ClientSocket);

#endif

    int _Port;
  public:
    Server();
    ~Server();

    //list<SOCKET> ConnectorsSocketList;
    vector<ClientsList> ConnectedClientList;

    bool IsInitializeServer;
    bool IsServerStarted;

    // server Callback
    typedef void (*_T_SERVERRECEIVEDCALLBACK)(char *Buffer, T_SOCKET ClientSocket);
    typedef void (*_T_ANYCONNECTIONNOTIFIER)(T_SOCKET ClientSocket);
    typedef void (*_T_ANYDISCONNECTIONNOTIFIER)(T_SOCKET ClientSocket);

    // Server Receive Callback Pointer.
    _T_SERVERRECEIVEDCALLBACK TServerReceivedCallback;
    _T_ANYCONNECTIONNOTIFIER TAnyConnectionNotifier;
    _T_ANYDISCONNECTIONNOTIFIER TAnyDisconnectionNotifier;

    static void *Server_ConnectionThread(void *Param);

    bool Initialize_Server();

    bool Start_Server();
    void Close_Server();

    bool SendDataToOne(char *Str, T_SOCKET ClientSocket);
    void SendDataToAll(char *Str);
  };

  // Client Class
  class Client {
  private:
    Thread _Thread;
    unsigned int _Address;
#if defined(WINDOWS_SYS)
    HOSTENT *_HostEntry;
    WSADATA _WSAData;
    SOCKET _ClientSocket;
    SOCKADDR_IN _ClientAddress;
#elif defined(POSIX_SYS)
    hostent *_HostEntry;
    int _ClientSocket;
    sockaddr_in _ClientAddress;
#endif

    string _IP;
    int _Port;
  public:
    Client();

    ~Client();

    bool IsInitializeClient;
    bool IsConnectedClient;

    // client Callback
    typedef void (*_T_CLIENTRECEIVEDCALLBACK)(char *Buffer);
    typedef void (*_T_CLIENTDISCONNECTEDCALLBACK)();

    // Client Receive Callback Pointer.
    _T_CLIENTRECEIVEDCALLBACK TClientReceivedCallback;
    _T_CLIENTDISCONNECTEDCALLBACK TClientDisconnectedCallback;

    static void *Client_ReceivingThread(void *Param);

    bool ClientInitialize();
    void ClientReceiveStart();

    void ClientClose();
    bool ClientReceiving();

    bool ClientConnect();
    void ClientDisconnect();
    //bool ClientReConnect();

    bool SendData(char *Str);
  };
};

#endif // _Telepathy_hpp_

#endif // SET_COMMON_MODULE_ETHERNET