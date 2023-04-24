/**
 * @file sc.cpp
 * @author Enyun Xuan (3245519202@qq.com)
 * @brief
 * @version 0.1
 * @date 2022-06-08
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <windows.h>
#include <iostream>
#include <process.h>
#include <string>
#include "md5.h"
#include "rsa.h"

#define KEY_LENGTH 512
#define N_LENGTH 1025
using namespace std;

#pragma comment(lib, "WS2_32.lib")

MD5 md5;
RSA server_rsa;
RSA client_rsa;
char signature[128];
char server_Pk[KEY_LENGTH];
char client_Pk[KEY_LENGTH];
char server_N[N_LENGTH];
char client_N[N_LENGTH];

struct Message
{
    char msg[N_LENGTH + 1];
    char md5text[33];
    char signature[N_LENGTH + 1];
};

/**
 * @brief 初始化套接字函数
 *
 */
void initialization()
{
    WSADATA wsaData;
    WORD sockVersion = MAKEWORD(2, 2);
    if (WSAStartup(sockVersion, &wsaData) != 0)
    {
        cout << "Fail to initializize socket." << endl;
    }
}

void serverRecv(void *param)
{
    while (1)
    {
        SOCKET remoteSocket = *(SOCKET *)(param);

        Message recvmsg;
        memset(&recvmsg, 0, sizeof(Message));
        int len = sizeof(Message);
        recv(remoteSocket, (char *)&recvmsg, len, 0);

        BigInt res(recvmsg.msg);

        md5.Update(recvmsg.msg);
        if (strcmp(md5.Tostring().c_str(), recvmsg.md5text) == 0)
        {
            cout << "receive correctly." << endl;
            cout << "[client]: " << res.gethexs() << endl;
        }
        else
        {
            cout << "this is a modified message!" << endl;
        }

        BigInt a = client_rsa.decry(recvmsg.msg);
        string temp = a.getbits();
        cout << "[client]rsa after decry: " << temp << endl;
        cout << "[client]md5: " << recvmsg.md5text << endl;
        BigInt signature_BI(recvmsg.signature);
        cout << "[client]signature: " << signature_BI.gethexs() << endl;
        BigInt b = client_rsa.decry(recvmsg.signature);
        temp.clear();
        temp = b.getbits();
        cout << "[client]signature after decry: " << temp << endl
             << endl;
    }
}

void serverSend(void *param)
{
    while (1)
    {
        SOCKET remoteSocket = *(SOCKET *)(param);

        Message sendmsg;
        memset(&sendmsg, 0, sizeof(Message));

        string temp;

        getline(cin, temp);
        BigInt a = server_rsa.encry(temp);

        // 添加RSA加密后的密文
        temp = a.getbits();
        strcpy(sendmsg.msg, temp.c_str());

        // 添加签名
        BigInt b = server_rsa.encry(signature);
        temp = b.getbits();
        strcpy(sendmsg.signature, temp.c_str());

        // 添加MD5摘要
        md5.Update(sendmsg.msg);
        strcpy(sendmsg.md5text, md5.Tostring().c_str());

        int len = sizeof(Message);
        send(remoteSocket, (char *)&sendmsg, len, 0);

        cout << "[server]: " << a.gethexs() << endl;
        cout << "[server]signature: " << b.gethexs() << endl;
        cout << "[server]md5: " << sendmsg.md5text << endl
             << endl;
    }
}

void clientRecv(void *param)
{
    while (1)
    {
        SOCKET remoteSocket = *(SOCKET *)(param);

        Message recvmsg;
        memset(&recvmsg, 0, sizeof(Message));
        int len = sizeof(Message);
        recv(remoteSocket, (char *)&recvmsg, len, 0);

        BigInt res(recvmsg.msg);

        md5.Update(recvmsg.msg);
        if (strcmp(md5.Tostring().c_str(), recvmsg.md5text) == 0)
        {
            cout << "receive correctly." << endl;
            cout << "[server]: " << res.gethexs() << endl;
        }
        else
        {
            cout << "this is a modified message!" << endl;
        }

        BigInt a = server_rsa.decry(recvmsg.msg);
        string temp = a.getbits();
        cout << "[server]rsa after decry: " << temp << endl;
        cout << "[server]md5: " << recvmsg.md5text << endl;
        BigInt signature_BI(recvmsg.signature);
        cout << "[server]signature: " << signature_BI.gethexs() << endl;
        BigInt b = server_rsa.decry(recvmsg.signature);
        temp.clear();
        temp = b.getbits();
        cout << "[server]signature after decry: " << temp << endl
             << endl;
    }
}

void clientSend(void *param)
{
    while (1)
    {
        SOCKET remoteSocket = *(SOCKET *)(param);

        Message sendmsg;
        memset(&sendmsg, 0, sizeof(Message));

        string temp;

        getline(cin, temp);
        BigInt a = client_rsa.encry(temp);
        temp = a.getbits();
        strcpy(sendmsg.msg, temp.c_str());

        BigInt b = client_rsa.encry(signature);
        temp = b.getbits();
        strcpy(sendmsg.signature, temp.c_str());

        md5.Update(sendmsg.msg);
        strcpy(sendmsg.md5text, md5.Tostring().c_str());

        int len = sizeof(Message);
        send(remoteSocket, (char *)&sendmsg, len, 0);

        cout << "[client]: " << a.gethexs() << endl;
        cout << "[client]signature: " << b.gethexs() << endl;
        cout << "[client]md5: " << sendmsg.md5text << endl
             << endl;
    }
}

void server()
{
    initialization();

    // cout << "Genarating RSA keys, please wait..." << endl;
    // server_rsa.NewRsaKeyPair(64);
    // cout << "Genarated!" << endl;
    cout << "Genarating RSA keys, please wait..." << endl;
    BigInt s_RSA;
    s_RSA.setHexs("826152E78A6C10DDA5D9EF2E774739EE4547AC3FCE3F6704075957DE95E0635599B6D6316434321C4111038B2D9B1BB9B727B7402329D1612659DB7B83B6F8A9");
    server_rsa.setN(s_RSA.getbits());
    s_RSA.setHexs("10001");
    server_rsa.setPk(s_RSA.getbits());
    s_RSA.setHexs("1631209E5E96444CA5D5317BF43B8065C90F090797407D755716E2F8F503CB84602F803D49F98543D31CE4CABDD7684712AF3CE1A0D4735C12EC1E932CF33B8D");
    server_rsa.setSk(s_RSA.getbits());
    cout << "Genarated!" << endl;

    SOCKET ServerSocket;
    if ((ServerSocket = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
    {
        cout << "Fail to create socket." << endl;
        return;
    }

    struct sockaddr_in ServerAddress;
    memset(&ServerAddress, 0, sizeof(sockaddr_in));
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    ServerAddress.sin_port = htons(8889);

    if (bind(ServerSocket, (sockaddr *)&ServerAddress, sizeof(ServerAddress)) == SOCKET_ERROR)
    {
        cout << "Bind socket error." << endl;
        return;
    }

    if (listen(ServerSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        cout << "Listen error." << endl;
        return;
    }
    else
    {
        cout << "I'm listening..." << endl;
    }

    sockaddr_in remoteAddress;
    SOCKET remoteSocket = INVALID_SOCKET;

    int addlen = sizeof(remoteAddress);
    if ((remoteSocket = accept(ServerSocket, (sockaddr *)&remoteAddress, &addlen)) == INVALID_SOCKET)
    {
        cout << "Connect error." << endl;
        return;
    }
    else
    {
        cout << "Successfully connected." << endl;
    }

    memset(server_Pk, 0, KEY_LENGTH);
    strcpy(server_Pk, server_rsa.getPk().getbits().c_str());
    send(remoteSocket, server_Pk, KEY_LENGTH, 0);
    cout << "server public key is : " << server_rsa.getPk().gethexs() << endl;

    memset(server_N, 0, N_LENGTH);
    strcpy(server_N, server_rsa.getN().getbits().c_str());
    send(remoteSocket, server_N, N_LENGTH, 0);
    cout << "server N is: " << server_rsa.getN().gethexs() << endl;

    memset(client_Pk, 0, KEY_LENGTH);
    recv(remoteSocket, client_Pk, KEY_LENGTH, 0);
    BigInt a(client_Pk);
    client_rsa.setPk(client_Pk);
    cout << "received client public key: " << a.gethexs() << endl;

    memset(client_N, 0, N_LENGTH);
    recv(remoteSocket, client_N, N_LENGTH, 0);
    BigInt b(client_N);
    client_rsa.setN(client_N);
    cout << "receive client N: " << b.gethexs() << endl;

    cout << "input your signature: " << endl;
    cin.getline(signature, 128);
    cout << "-------- <begin chatting> --------" << endl;

    _beginthread(serverSend, 0, &remoteSocket);
    _beginthread(serverRecv, 0, &remoteSocket);

    while (1)
        ;

    closesocket(remoteSocket);
    closesocket(ServerSocket);

    WSACleanup();
}

void client()
{
    initialization();

    cout << "Genarating RSA keys, please wait..." << endl;
    // client_rsa.NewRsaKeyPair(64);
    BigInt c_RSA;
    c_RSA.setHexs("895C327601D2C8EFA1E5391C2EFBD1A320A9501B860B663D9BA25135958B0391083A70935906AEA08533E9139136BEB616BE773858ECDFC5EBCC97877536F7E1");
    client_rsa.setN(c_RSA.getbits());
    c_RSA.setHexs("10001");
    client_rsa.setPk(c_RSA.getbits());
    c_RSA.setHexs("5439753388B0AFC6C7F55ECC5A3ECC6F83226E5D4217114CEB765EE74557E50A0DC6ECE7E3B4DFBC6095D227DC9E87F877A613995C68736F73842DEED1982275");
    client_rsa.setSk(c_RSA.getbits());
    cout << "Genarated!" << endl;

    SOCKET clientSocket;
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
    {
        cout << "Fail to create socket." << endl;
        return;
    }

    struct sockaddr_in ServerAddress;
    memset(&ServerAddress, 0, sizeof(sockaddr_in));
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    ServerAddress.sin_port = htons(8889);

    if (connect(clientSocket, (sockaddr *)&ServerAddress, sizeof(ServerAddress)) == SOCKET_ERROR)
    {
        cout << "Connect error." << endl;
        return;
    }
    else
    {
        cout << "Successfully connected." << endl;
    }

    memset(server_Pk, 0, KEY_LENGTH);
    recv(clientSocket, server_Pk, KEY_LENGTH, 0);
    BigInt a(server_Pk);
    server_rsa.setPk(server_Pk);
    cout << "received server public key: " << a.gethexs() << endl;

    memset(server_N, 0, N_LENGTH);
    recv(clientSocket, server_N, N_LENGTH, 0);
    BigInt b(server_N);
    server_rsa.setN(server_N);
    cout << "receive server N: " << b.gethexs() << endl;

    memset(client_Pk, 0, KEY_LENGTH);
    strcpy(client_Pk, client_rsa.getPk().getbits().c_str());
    send(clientSocket, client_Pk, KEY_LENGTH, 0);
    cout << "client public key is : " << client_rsa.getPk().gethexs() << endl;

    memset(client_N, 0, N_LENGTH);
    strcpy(client_N, client_rsa.getN().getbits().c_str());
    send(clientSocket, client_N, N_LENGTH, 0);
    cout << "client N is: " << client_rsa.getN().gethexs() << endl;

    cout << "input your signature: " << endl;
    cin.getline(signature, 128);
    cout << "-------- <begin chatting> --------" << endl;

    _beginthread(clientRecv, 0, &clientSocket);
    _beginthread(clientSend, 0, &clientSocket);

    while (1)
        ;

    closesocket(clientSocket);

    WSACleanup();
}

int main()
{
    cout << "input \"s\" to start server, \"c\" to start client:" << endl;
    char a;

    while (1)
    {
        cin.get(a);
        cin.ignore();
        if (a == 's')
        {
            server();
            break;
        }
        if (a == 'c')
        {
            client();
            break;
        }
        else
        {
            cout << "invalid input, please input again." << endl;
        }
    }
}