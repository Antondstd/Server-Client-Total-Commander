//
// Created by ad3st on 28.05.2021.
//
#pragma once
#include "ApiFilesAndDir.h"

WSADATA wsa;
HANDLE mutexMain, mutexFiles;
struct SocketMutexPair mainPair, filesPair;
int mainID;
int client();
