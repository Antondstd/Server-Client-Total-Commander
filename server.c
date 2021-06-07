//
// Created by ad3st on 28.05.2021.
//

#include "server.h"
#include "serverFiles.h"
#include "serverApiFiles.h"
#define _CRT_NON_CONFORMING_WCSTOK
#define _CRT_SECURE_NO_WARNINGS

ListUsers *listOfUsersSockets;

void server() {
    WSADATA wsa;
    wchar_t *path = calloc(265, sizeof(wchar_t));
    path[0] = L'\0';
    wcscpy(path, L"D:\\University\\6_semester\\SPO\\Test");
    serverLoop = true;
    fd_set fdCurrent, fdBuffer;
    int count = 1;
    int c = sizeof(struct sockaddr_in);
    SOCKET socketBuf;
    int id = 1;
    Message msBuf;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return;
    }

    printf("Initialised.\n");
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
        return;
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8888);

    //Bind
    if (bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Bind failed with error code : %d", WSAGetLastError());
        return;
    }

    printf("Bind done");
    listen(serverSocket, 3);

    mutexList = CreateMutexA(NULL, FALSE, NULL);
//    listOfUsersSockets = calloc(FD_SETSIZE + 1, sizeof(ListUsers));
    FD_ZERO(&fdCurrent);
    FD_ZERO(&fdBuffer);
    FD_SET(serverSocket, &fdBuffer);
    printf("fdbufsize %d\n", fdBuffer.fd_count);
    fdCurrent = fdBuffer;
    printf("fdcur %d\n", fdCurrent.fd_count);
    int f;
    listOfUsersSockets = calloc(128, sizeof(ListUsers));
    while (serverLoop) {
        fdCurrent = fdBuffer;
        printf("loop\n");
        if ((f = select(FD_SETSIZE, &fdCurrent, NULL, NULL, NULL)) < 0) {
            printf("Something went wrong with select\n");
        }
//        printf("C IS %d\n",f);
        for (int i = 0; i < fdBuffer.fd_count; i++) {
//            printf("TEST %d\n",i);
            if (FD_ISSET(fdBuffer.fd_array[i], &fdCurrent)) {
                printf("ITS IN\n");
                if (fdBuffer.fd_array[i] == serverSocket) {
                    printf("RETARD %d\n", i);
                    socketBuf = accept(serverSocket, (struct sockaddr *) &serverClientBufSocket, &c);
                    if (socketBuf == INVALID_SOCKET) {
                        printf("accept failed with error code : %d", WSAGetLastError());
                        return;
                    }
                    msBuf.id = id;
                    msBuf.type = SETUP;
                    send(socketBuf, &msBuf, sizeof(Message), 0);
                    if (recv(socketBuf, &msBuf, sizeof(Message), 0) == SOCKET_ERROR) {
                        printf("SADGE SHIT %d\n", id);
                        continue;
                    }
                    if (msBuf.type == OK_ID) {
                        printf("BEFORE ADDED id %d\n", id);
                        FD_SET(socketBuf, &fdBuffer);
                        listOfUsersSockets[msBuf.id].id = id;
                        listOfUsersSockets[msBuf.id].mainPair.socket = socketBuf;
                        wcscpy(listOfUsersSockets[msBuf.id].path, path);
                        printf("ADDED id %d\n", id);
                        id++;
                    } else {
                        printf("SHIT %d\n", msBuf.id);
//                        FD_SET(socketBuf, &fdBuffer);
                        listOfUsersSockets[msBuf.id].filesPair.socket = socketBuf;
                        listOfUsersSockets[msBuf.id].filesPair.mutex = CreateMutexA(NULL, FALSE, NULL);
                    }
                } else {
                    if (recv(fdBuffer.fd_array[i], (char *) &msBuf, sizeof(Message), 0) == SOCKET_ERROR) {
                        printf("SADGE SHIT %d\n", id);
                        FD_CLR(fdBuffer.fd_array[i], &fdBuffer);
                        continue;
                    }
                    printf("GOT SOME MESSAGE\n");
                    if (msBuf.type == UPDATE) {
                        printf("UPDATE\n");
                        MYDATA *arguments = calloc(1, sizeof(MYDATA));
                        arguments->val1 = msBuf.id;
                        CreateThread(
                                NULL,                   // default security attributes
                                0,                      // use default stack size
                                (LPTHREAD_START_ROUTINE) sendUpdate,       // thread function name
                                arguments,          // argument to thread function
                                0,                      // use default creation flags
                                NULL);


//                        ListUsers user = listOfUsersSockets[msBuf.id];
//                        ArrayOfFiles *array = serverGetListOfFilesAndDir(user.path);
//                        WaitForSingleObject(
//                                user.mainPair.mutex,    // handle to mutex
//                                INFINITE);
//                        printf("IN UPDATE FUNCTION AFTER MUTEX\n");
//                        Message *message = calloc(1,sizeof (Message));
//                        msBuf.type = UPDATE;
//                        msBuf.amount = array->count;
//                        send(user.mainPair.socket,(char *) &msBuf,sizeof (Message),0);
//                        printf("FILE NAME IS : %ls\n",array->wFile[0].name);
//                        printf("1FILE NAME IS : %ls\n",array->wFile[1].name);
//                        printf("2FILE NAME IS : %ls\n",array->wFile[2].name);
//                        send(user.mainPair.socket,(void*) array->wFile,sizeof (WFile) * array->count,0);
//                        ReleaseMutex(user.mainPair.mutex);

                    }
                    if (msBuf.type == Text) {
                        printf("TEXT FOR ID: %d\n", msBuf.id);
                        ListUsers user = listOfUsersSockets[msBuf.id];
                        int lengthPath = wcslen(msBuf.message) + wcslen(user.path) + 1;
                        wchar_t *filePath = calloc(lengthPath, sizeof(wchar_t));
                        wcscpy(filePath, user.path);
                        wcscat(filePath, L"\\");
                        wcscat(filePath, msBuf.message);
                        printf("Message IS : %ls\n", msBuf.message);
                        printf("PATH IS : %ls\n", filePath);
                        wchar_t *text = getFileText(filePath);
                        printf("TEXT Length: %llu\n", wcslen(text));
                        msBuf.amount = wcslen(text);
                        send(user.mainPair.socket, (void *) &msBuf, sizeof(Message), 0);
                        if ((msBuf.amount) > 0)
                            send(user.mainPair.socket, (void *) text, sizeof (wchar_t ) *wcslen(text), 0);
                        free(filePath);
                        free(text);
                    }
                    if (msBuf.type == Back){
                        printf("GOING BACK FOR ID: %d",msBuf.id);
                        ListUsers *user = &listOfUsersSockets[msBuf.id];
                        MYDATA *arguments = calloc(1, sizeof(MYDATA));
                        arguments->val1 = msBuf.id;
                        wchar_t bufFirst[265];
                        wchar_t bufLast[265];
                        getFirstAndLastTokens(user->path, bufFirst, bufLast, L"\\");
                        wcscpy(user->path,bufFirst);
                        CreateThread(
                                NULL,                   // default security attributes
                                0,                      // use default stack size
                                (LPTHREAD_START_ROUTINE) sendUpdate,       // thread function name
                                arguments,          // argument to thread function
                                0,                      // use default creation flags
                                NULL);
                    }
                    if (msBuf.type == MOVE){
                        printf("MOVING FOR ID: %d to %ls\n",msBuf.id,msBuf.message);
                        ListUsers *user = &listOfUsersSockets[msBuf.id];
                        wcscat(user->path,L"\\");
                        wcscat(user->path,msBuf.message);
                        printf("New USER Path: %ls\n",user->path);
                        MYDATA *arguments = calloc(1, sizeof(MYDATA));
                        arguments->val1 = msBuf.id;
                        CreateThread(
                                NULL,                   // default security attributes
                                0,                      // use default stack size
                                (LPTHREAD_START_ROUTINE) sendUpdate,       // thread function name
                                arguments,          // argument to thread function
                                0,                      // use default creation flags
                                NULL);
                    }
                    if (msBuf.type == DOWNLOAD){
                        wchar_t *newFile = calloc(MAX_PATH+1,sizeof (wchar_t));
                        ListUsers user = listOfUsersSockets[msBuf.id];
                        wcscpy(newFile,user.path);
                        wcscat(newFile,L"\\");
                        wcscat(newFile,msBuf.message);
                        printf("DOWNLOAD FILE : %ls\n",newFile);
                        FILE* fileF = _wfopen(newFile,L"rb");
//                        HANDLE hFile = CreateFile(newFile,               // file to open
//                                           GENERIC_READ,          // open for reading
//                                           FILE_SHARE_READ,       // share for reading
//                                           NULL,                  // default security
//                                           OPEN_EXISTING,         // existing file only
//                                           FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // normal file
//                                           NULL);
//                        OVERLAPPED ol = {0};
                        fseek(fileF, 0, SEEK_END); // seek to end of file
                        long fileSize = ftell(fileF); // get current file pointer
                        printf("FILESIZE IS : %ld",fileSize);
                        fseek(fileF, 0, SEEK_SET);
                        msBuf.amount = fileSize;
                        send(user.filesPair.socket, (void *) &msBuf, sizeof(Message), 0);
                        void *filebuffer = calloc(1,sizeof (void)*fileSize);
                        fread(filebuffer,sizeof (void),fileSize,fileF);
                        send(user.filesPair.socket,filebuffer,fileSize,0);
                        free(newFile);
                        fclose(fileF);

//                        ReadFileEx(hFile, ReadBuffer, BUFFERSIZE-1, &ol, FileIOCompletionRoutine)
                    }
                    if (msBuf.type == UPLOAD){
                        wchar_t *newFile = calloc(MAX_PATH+1,sizeof (wchar_t));
                        ListUsers user = listOfUsersSockets[msBuf.id];
                        wcscpy(newFile,user.path);
                        wcscat(newFile,L"\\");
                        wcscat(newFile,msBuf.message);
                        printf("UPLOAD FILE : %ls\n",newFile);
                        FILE* fileF = _wfopen(newFile,L"wb");
//                        HANDLE hFile = CreateFile(newFile,               // file to open
//                                           GENERIC_READ,          // open for reading
//                                           FILE_SHARE_READ,       // share for reading
//                                           NULL,                  // default security
//                                           OPEN_EXISTING,         // existing file only
//                                           FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // normal file
//                                           NULL);
//                        OVERLAPPED ol = {0};
                        MYDATA *arguments = calloc(1, sizeof(MYDATA));
                        wcscpy(arguments->val2,user.path);
                        CreateThread(
                                NULL,                   // default security attributes
                                0,                      // use default stack size
                                (LPTHREAD_START_ROUTINE) sendUpdatetoAll,       // thread function name
                                arguments,          // argument to thread function
                                0,                      // use default creation flags
                                NULL);
                        if(msBuf.amount > 0) {
                            void *filebuffer = calloc(1, sizeof(void) * msBuf.amount);
                            recv(user.filesPair.socket, (void *) filebuffer, msBuf.amount, 0);
                            fwrite(filebuffer, sizeof(void), sizeof(void) * msBuf.amount, fileF);
                            free(filebuffer);
                        }
                        free(newFile);
                        fclose(fileF);

//                        ReadFileEx(hFile, ReadBuffer, BUFFERSIZE-1, &ol, FileIOCompletionRoutine)
                    }
                }
            }
        }
    }

}