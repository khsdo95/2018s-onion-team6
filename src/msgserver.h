#ifndef MSGSERVER_H
#define MSGSERVER_H

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <thread>
#include <unordered_map>

#include "json/json.h"
#include "config.h"
#include "serversocket.h"
#include "clientsocket.h"
#include "user_info.h"
#include "pgpmanager.h"
#include "socket.h"

extern UserInfo* myInfo;
extern unordered_map<string, UserInfo*> UserInfoMap;
extern PgpManager* pgpmanager;

class MsgServer
{
    public:
        MsgServer();
        ~MsgServer();

        void Start();

    private:
        ClientSocket* p_client_sock;
        ServerSocket* p_server_sock;

        int JsonParsor(string msg);
        void RecvServer();
        int MsgClient(string ip, string msg);
        void Worker(ClientSocket* client_sock);
        string PGPDecrypt(string msg);
        string SaveFile(string id, string msg);
};

#endif // MSGSERVER_H
