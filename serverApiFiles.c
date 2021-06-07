//
// Created by ad3st on 24.05.2021.
//
#define _CRT_SECURE_NO_WARNINGS
#include "serverApiFiles.h"
#include "ApiFilesAndDir.h"
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

ArrayOfFiles* serverGetListOfFilesAndDir(wchar_t *path) {
    ArrayOfFiles *array = calloc(1, sizeof(ArrayOfFiles));
    array->count = 0;
    ArrayOfFiles *arrayOfFiles = calloc(1, sizeof(ArrayOfFiles));
    arrayOfFiles->count = 0;
    arrayOfFiles->wFile = calloc(1, sizeof(WFile));
    ArrayOfFiles *arrayOfDir = calloc(1, sizeof(ArrayOfFiles));
    arrayOfDir->count = 0;
    arrayOfDir->wFile = calloc(1, sizeof(WFile));
    wchar_t *bufpath = calloc(wcslen(path) + 3, sizeof(wchar_t));
    wcscpy(bufpath, path);
    wcscat(bufpath, L"\\*");
    HANDLE hFind = FindFirstFile(bufpath, &findedFile);
    free(bufpath);
//    hFind = FindFirstFile(L"D:\\University\\6_semester\\SPO\\Test\\*", &findedFile);
    if (hFind == INVALID_HANDLE_VALUE)
        printf("SADGE");
    else {
        do {
//            if (wcscmp(L".", findedFile.cFileName) == 0 || wcscmp(L"..", findedFile.cFileName) == 0)
//                continue;
            if (wcscmp(L".", findedFile.cFileName) == 0)
                continue;
            if (findedFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                arrayOfDir->count++;
                arrayOfDir->wFile = realloc(arrayOfDir->wFile, sizeof(WFile) * arrayOfDir->count);
                memset(&arrayOfDir->wFile[arrayOfDir->count - 1], 0, sizeof(WFile));
                wcscpy(arrayOfDir->wFile[arrayOfDir->count - 1].name, findedFile.cFileName);
                arrayOfDir->wFile[arrayOfDir->count - 1].type = true;
                addTimeToFileTime(&findedFile.ftLastWriteTime, 60 * 60 * 3);
                FileTimeToSystemTime(&findedFile.ftLastWriteTime,
                                     &arrayOfDir->wFile[arrayOfDir->count - 1].stLastWriteTime);
            } else {
                arrayOfFiles->count++;
                arrayOfFiles->wFile = realloc(arrayOfFiles->wFile, sizeof(WFile) * arrayOfFiles->count);
                memset(&arrayOfFiles->wFile[arrayOfFiles->count - 1], 0, sizeof(WFile));
                wchar_t *bufname = calloc(MAX_PATH+1,sizeof (wchar_t));
                wchar_t *bufExt = calloc(MAX_PATH+1,sizeof (wchar_t));
//                wcscpy(bufname, findedFile.cFileName);
//                wchar_t *token = wcstok(bufname, L".");
//                token = wcstok(NULL, L".");
//                if (token != NULL) {
//                    do {
//                        wcscpy(bufExt, token);
//                        token = wcstok(NULL, L".");
//                    } while (token != NULL);
//                }
                getFirstAndLastTokens(findedFile.cFileName, bufname, bufExt, L".");
                if (wcslen(bufExt) > 0)
                    wcscpy(arrayOfFiles->wFile[arrayOfFiles->count - 1].extension, bufExt);
//                int lengthName = wcslen(findedFile.cFileName);
//                if (wcslen(bufExt) > 0)
//                    lengthName = wcslen(findedFile.cFileName) - wcslen(bufExt) - 1;
//                wcsncpy(arrayOfFiles->wFile[arrayOfFiles->count - 1].name, findedFile.cFileName,
//                        lengthName);
                wcscpy(arrayOfFiles->wFile[arrayOfFiles->count - 1].name,bufname);
                addTimeToFileTime(&findedFile.ftLastWriteTime, 60 * 60 * 3);
                FileTimeToSystemTime(&findedFile.ftLastWriteTime,
                                     &arrayOfFiles->wFile[arrayOfFiles->count - 1].stLastWriteTime);
                arrayOfFiles->wFile[arrayOfFiles->count - 1].type = false;
                free(bufname);
                free(bufExt);
            }
        } while (FindNextFile(hFind, &findedFile) != 0);
        array->count = arrayOfDir->count + arrayOfFiles->count;
        array->wFile = calloc(array->count, sizeof(WFile));
        for (int i = 0; i < arrayOfDir->count; i++) {
            wcscpy(array->wFile[i].name, arrayOfDir->wFile[i].name);
            array->wFile[i].type = true;
            array->wFile[i].stLastWriteTime = arrayOfDir->wFile[i].stLastWriteTime;
        }
        for (int i = arrayOfDir->count; i < array->count; i++) {
            wcscpy(array->wFile[i].name, arrayOfFiles->wFile[i - arrayOfDir->count].name);
            wcscpy(array->wFile[i].extension, arrayOfFiles->wFile[i - arrayOfDir->count].extension);
            array->wFile[i].type = false;
            array->wFile[i].stLastWriteTime = arrayOfFiles->wFile[i - arrayOfDir->count].stLastWriteTime;
        }
        freeArrayFilesDir(arrayOfFiles);
        freeArrayFilesDir(arrayOfDir);
    }
    return array;
}