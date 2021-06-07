//
// Created by ad3st on 28.05.2021.
//

#include "client.h"

WSADATA wsa;
int mainID;
HANDLE mutexMain, mutexFiles;
struct SocketMutexPair mainPair, filesPair;
Message *message;
Message *messageForFiles;

int client() {
    SOCKET sockForFiles;
    SOCKET sockForMain;
    struct sockaddr_in clientToServerAddrMain;
    struct sockaddr_in clientToServerAddrFiles;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return -1;
    }
    if ((sockForMain = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
    }
    clientToServerAddrMain.sin_family = AF_INET;
    clientToServerAddrMain.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientToServerAddrMain.sin_port = htons(8888);
    if (connect(sockForMain, (struct sockaddr *) &clientToServerAddrMain, sizeof(clientToServerAddrMain)) < 0) {
        puts("connect error");
        return -1;
    }
    mutexMain = CreateMutexA(NULL, FALSE, NULL);
    mainPair.socket = sockForMain;
    mainPair.mutex = mutexMain;
    puts("Connected");

    message = calloc(1, sizeof(Message));
    messageForFiles = calloc(1, sizeof(Message));

    if ((recv(sockForMain, (char *) message, sizeof(Message), 0)) == SOCKET_ERROR) {
        puts("recv failed");
        return -1;
    }
    printf("ID IS %d\n", message->id);
    message->type = OK_ID;
    mainID = message->id;
    send(sockForMain, (const char *) message, sizeof(Message), 0);
    //Test shit

//    message->type = UPDATE;
//    send(sockForMain,(const char *) message, sizeof(Message), 0);
//    int ms;
//    if((ms = (recv(sockForMain , (char *) message, sizeof (Message), 0))) == SOCKET_ERROR)
//    {
//        puts("recv failed");
//        return -1;
//    }
//    printf("MS IS %d",ms);
//    printf("GOT MESSAGE\n");
//    if (message->type != UPDATE)
//        return -1;
//    printf("MESSAGE GETS UPDATE\n");
//    arrayOfFilesAndDir = calloc(1,(sizeof (struct ArrayOfFiles)));
//    arrayOfFilesAndDir->count = message->amount;
//    arrayOfFilesAndDir->wFile = calloc(message->amount, sizeof (WFile));
//    if((recv(sockForMain , (void *) arrayOfFilesAndDir->wFile, sizeof (WFile)*message->amount, 0)) == SOCKET_ERROR)
//    {
//        puts("recv failed");
//    }
//    printf("ITS HAPPENING");
//    printf("AMOUNT OF FILES IS %d\n",arrayOfFilesAndDir->count);
//    printf("FILE NAME IS : %ls\n",arrayOfFilesAndDir->wFile[0].name);
//    printf("1FILE NAME IS : %ls\n",arrayOfFilesAndDir->wFile[1].name);
//    printf("2FILE NAME IS : %ls\n",arrayOfFilesAndDir->wFile[2].name);
// SECOND SOCKET
    if ((sockForFiles = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
    }
    clientToServerAddrFiles.sin_family = AF_INET;
    clientToServerAddrFiles.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientToServerAddrFiles.sin_port = htons(8888);
    if (connect(sockForFiles, (struct sockaddr *) &clientToServerAddrFiles, sizeof(clientToServerAddrFiles)) < 0) {
        puts("connect error");
        return -1;
    }
    printf("SECOND CONNECTION??");
//    mutexFiles = CreateMutexA(NULL, FALSE, NULL);
    if ((recv(sockForFiles, (char *) messageForFiles, sizeof(Message), 0)) == SOCKET_ERROR) {
        puts("recv failed");
        return -1;
    }
    printf("ID IS %d\n", messageForFiles->id);
    filesPair.socket = sockForFiles;
    filesPair.mutex = mutexFiles;
    messageForFiles->type = CHANGEID;
    messageForFiles->id = mainID;
    send(sockForFiles, (const char *) messageForFiles, sizeof(Message), 0);
    printf("GG");

    message->type = UPDATE;
    send(sockForMain,(const char *) message, sizeof(Message), 0);
    int ms;
    if((ms = (recv(sockForMain , (char *) message, sizeof (Message), 0))) == SOCKET_ERROR)
    {
        puts("recv failed");
        return -1;
    }
    printf("MS IS %d",ms);
    printf("GOT MESSAGE\n");
    if (message->type != UPDATE)
        return -1;
    printf("MESSAGE GETS UPDATE\n");
    arrayOfFilesAndDir = calloc(1,(sizeof (struct ArrayOfFiles)));
    arrayOfFilesAndDir->count = message->amount;
    arrayOfFilesAndDir->wFile = calloc(message->amount, sizeof (WFile));
    if((recv(sockForMain , (char *) arrayOfFilesAndDir->wFile, sizeof (WFile)*message->amount, 0)) == SOCKET_ERROR)
    {
        puts("recv failed");
    }
    printf("ITS HAPPENING");
    printf("AMOUNT OF FILES IS %d\n",arrayOfFilesAndDir->count);
    printf("FILE NAME IS : %ls\n",arrayOfFilesAndDir->wFile[0].name);
    printf("1FILE NAME IS : %ls\n",arrayOfFilesAndDir->wFile[1].name);
    printf("2FILE NAME IS : %ls\n",arrayOfFilesAndDir->wFile[2].name);
    return 0;
}