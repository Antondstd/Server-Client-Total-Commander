//
// Created by ad3st on 29.05.2021.
//

#include "serverFiles.h"
#include "serverApiFiles.h"

ListUsers *listOfUsersSockets;

ListUsers getSocketByID(int id){
    for (int i = 0; i< FD_SETSIZE; i++){
        if (listOfUsersSockets->id == id){
            return listOfUsersSockets[i];
        }
    }
}

void sendUpdate(MYDATA *params){
    printf("IN UPDATE FUNCTION\n");
//    ListUsers user = getSocketByID(params->val1);
    ListUsers user = listOfUsersSockets[params->val1];
    ArrayOfFiles *array = serverGetListOfFilesAndDir(user.path);
    WaitForSingleObject(
            user.mainPair.mutex,    // handle to mutex
            INFINITE);
    printf("USER Path: %ls\n",user.path);
    printf("IN UPDATE FUNCTION AFTER MUTEX\n");
    Message *message = calloc(1,sizeof (Message));
    message->type = UPDATE;
    message->amount = array->count;
    send(user.mainPair.socket,(void*) message,sizeof (Message),0);
    send(user.mainPair.socket,(void*) array->wFile,sizeof (WFile) * array->count,0);
    ReleaseMutex(user.mainPair.mutex);
    free(params);
}

void sendUpdatetoAll(MYDATA *params){
    for (int i = 0; i< FD_SETSIZE; i++){
        if (wcscmp(listOfUsersSockets[i].path,params->val2) == 0){
            MYDATA *arguments = calloc(1, sizeof(MYDATA));
            arguments->val1 = i;
            CreateThread(
                    NULL,                   // default security attributes
                    0,                      // use default stack size
                    (LPTHREAD_START_ROUTINE) sendUpdate,       // thread function name
                    arguments,          // argument to thread function
                    0,                      // use default creation flags
                    NULL);
        }
    }
}