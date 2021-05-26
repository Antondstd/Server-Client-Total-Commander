//
// Created by ad3st on 24.05.2021.
//
#define _CRT_SECURE_NO_WARNINGS
#include "serverApiFiles.h"
wchar_t* getFileText(wchar_t *pathFile){
    FILE *file = _wfopen(pathFile, L"r, ccs=UTF-8");
//    __int64 fileSize = 0;
//    LARGE_INTEGER lFileSize;
//    GetFileSizeEx(hFind,&lFileSize);
//    fileSize = lFileSize.QuadPart;
    fseek(file, 0, SEEK_END); // seek to end of file
    long fileSize = ftell(file); // get current file pointer
    fseek(file, 0, SEEK_SET);
    wchar_t *text = calloc(fileSize + 1,sizeof (wchar_t ));
    fread(text,sizeof (wchar_t),fileSize,file);
    text[fileSize] = L'\0';
    return text;
}