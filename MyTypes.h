//
// Created by ad3st on 30.05.2021.
//

#ifndef LAB3_MYTYPES_H
#define LAB3_MYTYPES_H
enum MessageType{
    SETUP,
    MOVE,
    UPLOAD,
    DOWNLOAD,
    M_ERROR,
    CHANGEID,
    UPDATE,
    OK_ID,
    Text,
    Back
};
typedef struct Message{
    enum MessageType type;
    int id; // Для получения и смены ID
    wchar_t message[MAX_PATH + 1];
    size_t amount;
}Message;

typedef struct WFile {
    wchar_t name[260];
    wchar_t extension[260];
    SYSTEMTIME stLastWriteTime;
    BOOLEAN type;
} WFile;

typedef struct ArrayOfFiles {
    int count;
    WFile *wFile;
} ArrayOfFiles;

typedef struct SocketMutexPair{
    SOCKET socket;
    HANDLE mutex;
} SocketMutexPair;


typedef struct MyData {
    int val1;
    wchar_t val2[MAX_PATH + 1];
} MYDATA, *PMYDATA;

typedef struct ListUsers{
    int id;
    wchar_t path[MAX_PATH + 1];
    SocketMutexPair mainPair;
    SocketMutexPair filesPair;
} ListUsers;


#endif //LAB3_MYTYPES_H
