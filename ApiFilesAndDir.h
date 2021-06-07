//
// Created by ad3st on 23.05.2021.
//
#pragma once
#define UNICODE 1
#define _WIN32_WINNT 0x0500
#define _CRT_SECURE_NO_WARNINGS



#include <stdio.h>
#include <windows.h>
#include <locale.h>
#include <stdbool.h>
#include <conio.h>

//typedef struct WFile {
//    wchar_t name[260];
//    wchar_t extension[260];
//    SYSTEMTIME stLastWriteTime;
//    BOOLEAN type;
//} WFile;
//
//typedef struct ArrayOfFiles {
//    int count;
//    WFile *wFile;
//} ArrayOfFiles;
//
//typedef struct SocketMutexPair{
//    SOCKET socket;
//    HANDLE mutex;
//} SocketMutexPair;

#include "MyTypes.h"
//int currentLineFiles = 1;
ArrayOfFiles *arrayOfFilesAndDir;
WIN32_FIND_DATA findedFile;

void addTimeToFileTime(FILETIME *f, double seconds);

void freeArrayFilesDir(ArrayOfFiles *arrayOfFiles);

void writeListOfFiles(int widthLines, int heightLines, int startingLine);

void getListOfFilesAndDir(wchar_t *path);

size_t getNlineInText(wchar_t *text, int number);

void getFirstAndLastTokens(wchar_t *text,wchar_t *bufFirst, wchar_t *bufLast, wchar_t *delimiter);

size_t getLinesInText(wchar_t *text);



