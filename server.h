//
// Created by ad3st on 28.05.2021.
//
#pragma once

#include <stdio.h>
#include <windows.h>
#include <locale.h>
#include <stdbool.h>
#include <conio.h>

#include "MyTypes.h"

//typedef struct SocketMutexPair{
//    SOCKET socket;
//    HANDLE mutex;
//} SocketMutexPair;
//
//typedef struct WFile {
//    wchar_t name[260];
//    wchar_t extension[260];
//    SYSTEMTIME stLastWriteTime;
//    BOOLEAN type;
//} WFile;
//
//typedef struct MyData {
//    int val1;
//    int val2;
//} MYDATA, *PMYDATA;
//
//typedef struct ListUsers{
//    int id;
//    wchar_t path[MAX_PATH + 1];
//    SocketMutexPair mainPair;
//    SocketMutexPair filesPair;
//} ListUsers;

struct sockaddr_in serverAddr;
SOCKET serverSocket, serverClientBufSocket;

BOOLEAN serverLoop;

HANDLE mutexList;
fd_set readfds;

void server();
