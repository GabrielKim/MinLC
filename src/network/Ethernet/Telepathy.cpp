/*********************************************************************

          File: Telepathy.cpp

   Description: Telepathy Source Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/18

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

#include "Telepathy.hpp"

#if defined(_Telepathy_hpp_)

#include "MacroTools.hpp"

#pragma region Server Class
// Telepathy Server Class Area.
Telepathy::Server *G_TelepathyServer;

#pragma region Constructor & Destructor
// constructor
Telepathy::Server::Server() {
	IsInitializeServer = false;
	IsServerStarted = false;
}

// destructor
Telepathy::Server::~Server() {
	if (IsInitializeServer == true) {
    Close_Server();
	}
}
#pragma endregion Constructor & Destructor

#pragma region Server Threads

void * Telepathy::Server::Server_ConnectionThread(void *Param) {
  Telepathy::Server *_TServer = (Telepathy::Server *) Param;

	while (_TServer->IsServerStarted == true) {
    bool _TCompressFds = false;
    int _TFds = poll(_TServer->_Fds, _TServer->_ConnectionNumber, -1);

    if (_TFds < 0) {
      // error.
    }

    int _TCurrentSize = _TServer->_ConnectionNumber;
    for (int i=0; i<_TCurrentSize; i++) {
      if (_TServer->_Fds[i].revents == 0)
        continue;

      if (_TServer->_Fds[i].fd == _TServer->_ServerSocket) {
        int _TSocket = G_TelepathyServer->_AddClient();
        if (_TSocket == -1) {
          // Fail that Socket Connect.
          continue;
        }
        else {
          _TServer->_Fds[_TServer->_ConnectionNumber].fd = _TSocket;
          _TServer->_Fds[_TServer->_ConnectionNumber].events = POLLIN;
          _TServer->_ConnectionNumber++;
        }
      }
      else {
        if (G_TelepathyServer->_Receive(_TServer->_Fds[i].fd) == false) {
          close(_TServer->_Fds[i].fd);
          _TServer->_Fds[i].fd = -1;
          _TCompressFds = true;
        }
      }
    }

    if (_TCompressFds == true) {
      for (int i=0; i<_TServer->_ConnectionNumber; i++) {
        if (_TServer->_Fds[i].fd == -1) {
          for (int j=i; j<_TServer->_ConnectionNumber; j++)
            _TServer->_Fds[j].fd = _TServer->_Fds[j+1].fd;
          i--;
          _TServer->_ConnectionNumber--;
        }
      }
    }
  }

  // When, server closed that closing sockets.
  for (int i=0; i<_TServer->_ConnectionNumber; i++){
    if (_TServer->_Fds[i].fd >= 0) {
      close(_TServer->_Fds[i].fd);
    }
  }
#if defined(WINDOWS_SYS)
	_endthread();
#elif defined(POSIX_SYS)

#endif
	return 0;
}

#pragma endregion Server Threads

// Server initializations.
bool Telepathy::Server::Initialize_Server() {
#if defined(WINDOWS_SYS)
	// WSAStartUp
	if (WSAStartup(0x101, &_WSAData) != 0)
		return false;
#endif
	// using IPv4
	_ServerAddress.sin_family = AF_INET;
	// 32bit IPv4 address
	_ServerAddress.sin_addr.s_addr =
#if defined(POSIX_SYS)
      htonl(
#endif
      INADDR_ANY
#if defined(POSIX_SYS)
      )
#endif
      ;

	// Using port.
	_ServerAddress.sin_port = htons((u_short)DEFAULT_TCP_PORT);
	// Socket Create.
	_ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	int _TOptionValue = 1;
	// TCP No Delay Option.
	setsockopt(_ServerSocket, IPPROTO_TCP, TCP_NODELAY, (const char *)&_TOptionValue, sizeof(_TOptionValue));

	// if, wrong socket..
	if (_ServerSocket ==
#if defined(WINDOWS_SYS)
      INVALID_SOCKET
#elif defined(POSIX_SYS)
      -1
#endif
      )
		return false;

	// socket bind.
	if (bind(_ServerSocket, (sockaddr *)&_ServerAddress, sizeof(_ServerAddress)) != 0) {
    Close_Server();
		return false;
	}

	// socket listen.
	if (listen(_ServerSocket, DEFAULT_TCP_LISTEN_QUEUE) != 0) {
    Close_Server();
		return false;
	}

	// 외부 Receive 함수용.
	G_TelepathyServer = this;
	IsInitializeServer = true;

  // poll init.
  _Fds[0].fd = _ServerSocket;
  _Fds[0].events = POLLIN;
  _ConnectionNumber = 1;

	return true;
}

// Start the Server.
bool Telepathy::Server::Start_Server() {
	bool _TIsStarted = false;
	if (IsInitializeServer != true) {
		// failed started server.
		_TIsStarted = false;
	}
	else {
		// start to the Client manage Thread.
		_Thread.StartThread(Server_ConnectionThread, this);
		_TIsStarted = IsServerStarted = true;
	}
	return _TIsStarted;
}

// Close the Server.
void Telepathy::Server::Close_Server() {
	if (_ServerSocket != NULL) {
#if defined(WINDOWS_SYS)
    closesocket(_ServerSocket);
		WSACleanup();
#elif defined(POSIX_SYS)
    close(_ServerSocket);
#endif
		IsInitializeServer = false;
		IsServerStarted = false;
	}
}

int Telepathy::Server::_AddClient() {
	// if connected the Client..
  T_SOCKET _TSocket;
  T_SOCKADDR_IN _TClientAddress;
	int _TClientLength = sizeof(_TClientAddress);

	// Accepting the Socket.
	_TSocket = accept(_ServerSocket, (struct sockaddr *)&_TClientAddress,
#if defined(POSIX_SYS)
                    (socklen_t *)
#endif
      &_TClientLength);

	int _TOptionValue = 1;
	// TCP No Delay Option.
	setsockopt(_TSocket, IPPROTO_TCP, TCP_NODELAY, (const char *)&_TOptionValue, sizeof(_TOptionValue));

	if (_TSocket !=
#if defined(WINDOWS_SYS)
      INVALID_SOCKET
#elif defined(POSIX_SYS)
      -1
#endif
      ) {
		ClientsList _TClientList;
		// set Client Address.
		_TClientList.ClientAddress = _TClientAddress;
		// set Socket.
		_TClientList.ClientSocket = _TSocket;
		// initialize the Client Type.
		_TClientList.ClientType = "";
		// initialize the Client Name.
		_TClientList.ClientName = "";
		// push at lists back.
		ConnectedClientList.push_back(_TClientList);

		TAnyConnectionNotifier(_TSocket);
	}
	else
		return -1;

  return _TSocket;
}

bool Telepathy::Server::_Receive(T_SOCKET ClientSocket) {
	char _TBuffer[BUFFER_MAX_32767];
  T_SSIZE_T _TReadBufferLength;

	memset(_TBuffer, 0, sizeof(_TBuffer));

	_TReadBufferLength =
    recv(ClientSocket, _TBuffer, BUFFER_MAX_32767, 0);
	
	if (_TReadBufferLength <= 0) {
    for_IterToEnd(vector, ClientsList, ConnectedClientList, i) {
			if (_i->ClientSocket == ClientSocket) {
				// crate for the one Iterator.
				vector<ClientsList>::iterator _TClientListIter = _i;
				// It does not matter that the Iterator loses its position and returns it anyway.
        // Create another Iterator to increment this one and check whether the current position of the iterator is end.
        // If not delete.
				if ((++_TClientListIter) != ConnectedClientList.end())
					_i = ConnectedClientList.erase(_i);
				else {
          // pop the List.
          ConnectedClientList.pop_back();
          // if not break, will be dead.
          break;
        }
			}
		}

    TAnyDisconnectionNotifier(ClientSocket);
		return false;
	}
	else {
		// if contain the buffer, call the callback.
		TServerReceivedCallback(_TBuffer, ClientSocket);
	}
	return true;
}

bool Telepathy::Server::SendDataToOne(char *Str, T_SOCKET ClientSocket) {
	int _TSendStatus = 0;
  int _TIndex = 0;

	/*
    Sent to the specified Client.
    Due to the nature of the STL List, there is a problem when deleting, and I was very careful that
    Iterator can not recognize the following when erasing.
    Therefore, when deleting the connecter at the end of the List (if the iterator from begin to end)
    I made the following code assuming that an error could occur.
	*/
	for_IterToEnd(vector, ClientsList, ConnectedClientList, i) {
		// Checking for the socket to senders.
		if (_i->ClientSocket == ClientSocket) {
			_TSendStatus = send(_i->ClientSocket, Str, strlen(Str), 0);
			if (_TSendStatus == -1) {
        TAnyDisconnectionNotifier(ClientSocket);

        // First, delete the Poll Desc.
        close(_Fds[_TIndex+1].fd);
        _Fds[_TIndex+1].fd = -1;

				// crate for the one Iterator.
				vector<ClientsList>::iterator _TClientListIter = _i;
				// It does not matter that the Iterator loses its position and returns it anyway.
        // Create another Iterator to increment this one and check whether the current position of the iterator is end.
        // If not delete.
				if ((++_TClientListIter) != ConnectedClientList.end())
					_i = ConnectedClientList.erase(_i);
				else {
					// pop the List.
					ConnectedClientList.pop_back();
					// if not break, will be dead.
					break;
				}
				return false;
			}
			return true;
		}
    _TIndex++;
	}
	return true;
}

void Telepathy::Server::SendDataToAll(char *Str) {
	/*
    Send a Message to everyone.
    This function may fail to send a message to any client because the transmission to the client
    may fail if the client's socket is disconnected.
    In other words, there is no guarantee that everything will succeed.
	*/
	int _TSendStatus = 0;
  int _TIndex = 0;

	for_IterToEnd(vector, ClientsList, ConnectedClientList, i) {
		_TSendStatus = send(_i->ClientSocket, Str, strlen(Str), 0);
		if (_TSendStatus == -1) {
      TAnyDisconnectionNotifier(_Fds[_TIndex+1].fd);

      // First, delete the Poll Desc.
      close(_Fds[_TIndex+1].fd);
      _Fds[_TIndex+1].fd = -1;

			// crate for the one Iterator.
			vector<ClientsList>::iterator _TClientListIter = _i;
			// It does not matter that the Iterator loses its position and returns it anyway.
      // Create another Iterator to increment this one and check whether the current position of the iterator is end.
      // If not delete.
			if ((++_TClientListIter) != ConnectedClientList.end())
				_i = ConnectedClientList.erase(_i);
			else {
				// pop the List.
				ConnectedClientList.pop_back();
				// if not break, will be dead.
				break;
			}
		}
    _TIndex++;
	}
}
#pragma endregion Server Class

#pragma region Client Class
// Telepathy Client Class Area.
Telepathy::Client *G_TelepathyClient;

#pragma region Constructor & Destructor
// constructor
Telepathy::Client::Client() {
	IsInitializeClient = false;
	IsConnectedClient = false;

	G_TelepathyClient = this;
}

// deconstructor
Telepathy::Client::~Client() {
	if (IsConnectedClient == true)
		ClientClose();
	IsInitializeClient = false;
	G_TelepathyClient = NULL;
}
#pragma endregion Constructor & Destructor

#pragma region Client Threads
void *Telepathy::Client::Client_ReceivingThread(void *Param) {
	while (1) {
		if (G_TelepathyClient->ClientReceiving() == false) {
			G_TelepathyClient->ClientDisconnect();
			break;
		}
	}
	return 0;
}
#pragma endregion Client Threads

bool Telepathy::Client::ClientInitialize() {
#if defined(WINDOWS_SYS)
  // WSAStartUp
	if (WSAStartup(0x101, &_WSAData) != 0)
		return false;
#endif
	// Socket Create.
	_ClientSocket = socket(AF_INET, SOCK_STREAM, 0);

	// if, wrong socket..
  if (_ClientSocket !=
      #if defined(WINDOWS_SYS)
      INVALID_SOCKET
      #elif defined(POSIX_SYS)
      -1
#endif
      )
		return false;

	int _TOptionValue = 1;
	// TCP No Delay Option.
	setsockopt(_ClientSocket, IPPROTO_TCP, TCP_NODELAY, (const char *)&_TOptionValue, sizeof(_TOptionValue));
	//if (setsockopt(_ClientSocket, SOL_SOCKET, SO_REUSEADDR, (const char *)&_TOptVal, sizeof(int)) == -1)
	//	return false;
	// Local IP Address.
	_Address = inet_addr(IP_ADDR_LOCAL);
	// get host entry.
	_HostEntry = gethostbyaddr((char *)&_Address, sizeof(_Address), AF_INET);

	if (_HostEntry == NULL) {
		ClientClose();
		return false;
	}

	// using IPv4
	_ClientAddress.sin_family = AF_INET;
	// 32bit IPv4 address
	_ClientAddress.sin_addr.s_addr = *((T_IN_ADDR_T *)_HostEntry->h_addr);
	//_M_ServerAddress.sin_addr.s_addr = inet_addr(IP_ADDR_LOCAL);
	_ClientAddress.sin_port = htons((u_short)DEFAULT_TCP_PORT);

	IsInitializeClient = true;
	return true;
}

void Telepathy::Client::ClientReceiveStart() {
	if (IsConnectedClient != true) {
		// failed started Client.
	}
	else {
		// Start a Client manage Thread.
		_Thread.StartThread(Client_ReceivingThread, NULL);
	}
}

void Telepathy::Client::ClientClose() {
	if (_ClientSocket != NULL) {
		shutdown(_ClientSocket, 0x02); // BOTH.
#if defined(WINDOWS_SYS)
		closesocket(_ClientSocket);
#elif defined(POSIX_SYS)
    close(_ClientSocket);
#endif
	}
	IsConnectedClient = false;
}

bool Telepathy::Client::ClientReceiving() {
	char _TBuffer[BUFFER_MAX_32767];
  T_SSIZE_T _TReadBufferLength;

	memset(_TBuffer, 0, sizeof(_TBuffer));

	_TReadBufferLength =
    recv(_ClientSocket, _TBuffer, BUFFER_MAX_32767, 0);


	if (_TReadBufferLength == -1) {
		return false;
	}
	else {
		// if contain the buffer, call the callback.
		TClientReceivedCallback(_TBuffer);
	}
	return true;
}

bool Telepathy::Client::ClientConnect() {
	if (connect(_ClientSocket, (sockaddr *)&_ClientAddress, sizeof(_ClientAddress))) {
		ClientClose();
		return false;
	}

	IsConnectedClient = true;

	return true;
}

void Telepathy::Client::ClientDisconnect() {
	ClientClose();
	TClientDisconnectedCallback();
}

bool Telepathy::Client::SendData(char *Str) {
	int _TSendStatus = 0;

	_TSendStatus = send(_ClientSocket, Str, strlen(Str), 0);

	if (_TSendStatus == -1)
		return false;

	return true;
}
#pragma endregion Client Class

#endif // _Telepathy_hpp_