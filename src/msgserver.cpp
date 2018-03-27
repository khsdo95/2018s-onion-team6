#include "msgserver.h"


MsgServer::MsgServer()
{

}
int MsgServer::MsgClient(string ip, string msg)
{

    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        cout << "Socket creation error" <<endl;
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8888);

    if(inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr)<=0)
    {
        cout << "ERROR" << endl;
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        cout << "Connect Failed" << endl;
        return -1;
    }
    else
    {
        send(sock , msg.c_str() , msg.length() , 0 );
        close(sock);
    }
return 1;
}
void MsgServer::Start()
{
   // this->JsonParsor("1");
   std::thread serverRun([this] { RecvServer();});
   serverRun.detach();
}
void MsgServer::RecvServer()
{
    p_server_sock = new ServerSocket(8888);
    int err = p_server_sock->listen();
    if (err != 0)
    {
        std::cout << strerror(err) << std::endl;
        exit(err);
    }

    while (true)
    {
        p_client_sock = p_server_sock->accept();
        if (!p_client_sock->valid())
        {
            delete p_client_sock;
            continue;
        }
        std::thread acceptRun([this] { Worker(p_client_sock);});
        acceptRun.detach();

    }

    delete p_server_sock;
}

int MsgServer::JsonParsor(string msg)
{
        string * data = new string(msg);

        JSONCPP_STRING errs;
        Json::Value root;
        Json::CharReaderBuilder builder;
        Json::CharReader * reader = builder.newCharReader();
        reader->parse(data[0].c_str(), data[0].c_str()+data[0].length(), &root, &errs);

        cout << msg <<endl;
        //get json data

        Json::Value j_sender    = root["sender"];
        Json::Value j_receiver  = root["receiver"];
        Json::Value j_data      = root["data"];

        //cast json data
        string github_id    = j_sender.asCString();
        string receiver_id  = j_receiver.asCString();
        string pgp_data     = j_data.asCString();

            // MY MESSAGE
        if(receiver_id == myInfo->GetGithubId())
        {
            cout << "[!] Message arrived" << endl;
            cout << this->PGPDecrypt(pgp_data) << endl;
        }
            // NOT MY MESSAGE
        else
        {
            string nextIp = UserInfoMap[receiver_id]->GetIpAddr();
            cout << "[D]JsonParsor : pgp_data" << pgp_data <<endl; 
            this->MsgClient(nextIp,pgp_data);
            return 1;
        }

    delete data;
    data = NULL;
    return 0;

}
string MsgServer::PGPDecrypt(string msg)
{
    //try PGP decrypt

    string decryptedMsg="";// = decrypted_pgp_function;
    //try decrpyt
    if (1==1) // decrypt success -> try catch
    {
        decryptedMsg=msg;
    }
    return decryptedMsg;
}
//  UserInfo* myInfo;
//  4 unordered_map<string, UserInfo*> UserInfoMap;
//
void MsgServer::Worker(ClientSocket* client_sock)
{
    std::cout << "Got a client!" << std::endl;
    SocketAddress* addr = client_sock->get_sockaddr();
    std::cout << addr->get_address() << ":" << addr->get_port() << std::endl;
    while (true)
    {
        std::string msg;
        if (client_sock->read(msg) <= 0)
        {
            break;
        }
        string decryptedMsg = this->PGPDecrypt(msg);
        if(decryptedMsg!="")
        {
            this->JsonParsor(decryptedMsg);
        }
        std::cout << "[D]"<<msg;
    }
    delete client_sock;
    std::cout << "Client disconnected" << std::endl;

}
MsgServer::~MsgServer()
{
}

