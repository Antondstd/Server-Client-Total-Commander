//#include <stdio.h>


#pragma warning(disable : 4996)
#pragma comment(lib, "Ws2_32.lib")
//#include <windows.h>
//#include <locale.h>
//#include <stdbool.h>
//#include <conio.h>
#include "server.h"
#include "client.h"
#include "Console.h"
#include "ClientInput.h"

//void toCheckDir(){
//    HANDLE hDir = CreateFile(
//            L"D:\\University\\6_semester\\SPO\\Test", /* pointer to the file name */
//            FILE_LIST_DIRECTORY,                /* (this is important to be FILE_LIST_DIRECTORY!) access (read-write) mode */
//            FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE,  /* (file share write is needed, or else user is not able to rename file while you hold it) share mode */
//            NULL, /* security descriptor */
//            OPEN_EXISTING, /* how to create */
//            FILE_FLAG_BACKUP_SEMANTICS, /* file attributes */
//            NULL /* file with attributes to copy */
//    );
//
//    HANDLE dwChangeHandles[2];
//    dwChangeHandles[1] = FindFirstChangeNotification(
//            L"D:\\University\\6_semester\\SPO\\Test",                          // directory to watch
//            FALSE,                          // do not watch subtree
//            FILE_NOTIFY_CHANGE_FILE_NAME);  // watch file name changes
//
//    if (dwChangeHandles[1] == INVALID_HANDLE_VALUE)
//    {
//        printf("\n ERROR: FindFirstChangeNotification function failed.\n");
//        ExitProcess(GetLastError());
//    }
//
//// Watch the subtree for directory creation and deletion.
//    dwChangeHandles[0] = FindFirstChangeNotification(
//            L"D:\\University\\6_semester\\SPO\\Test",                       // directory to watch
//            TRUE,                          // watch the subtree
//            FILE_NOTIFY_CHANGE_DIR_NAME);  // watch dir name changes
//
//    if (dwChangeHandles[0] == INVALID_HANDLE_VALUE)
//    {
//        printf("\n ERROR: FindFirstChangeNotification function failed.\n");
//        ExitProcess(GetLastError());
//    }
//
//    while (TRUE)
//    {
//        // Wait for notification.
//        printf("\nWaiting for notification...\n");
//
//        DWORD dwWaitStatus = WaitForMultipleObjects(2, dwChangeHandles,
//                                                    FALSE, INFINITE);
//
//        switch (dwWaitStatus)
//        {
//            case WAIT_OBJECT_0:
//
//                // A file was created, renamed, or deleted in the directory.
//                // Restart the notification.
//                if ( FindNextChangeNotification(dwChangeHandles[0]) == FALSE )
//                {
//                    printf("\n ERROR: FindNextChangeNotification function failed.\n");
//                    ExitProcess(GetLastError());
//                }
//                else{
//                    printf("Something Happend Here");
//                }
//                break;
//
//            case WAIT_OBJECT_0 + 1:
//
//                // Restart the notification.
//                if (FindNextChangeNotification(dwChangeHandles[1]) == FALSE )
//                {
//                    printf("\n ERROR: FindNextChangeNotification function failed.\n");
//                    ExitProcess(GetLastError());
//                }
//                else{
//                    printf("Something Happend SOMEWHERE");
//                }
//                break;
//
//            default:
//                printf("\n ERROR: Unhandled dwWaitStatus.\n");
//                ExitProcess(GetLastError());
//                break;
//        }
//    }
//}

int main(int argc, char *argv[]) {
//    printf(argv[2]);
    int key = _getch();
    //if (strcmp(argv[3],"client") == 0) {
    if (key == 49){
        printf("test client");
        if (client() != 0)
            return 0;

        CreateConsole(WidthConsole, HeightConsole, 15, 15);
        makeBorder(0, 0, m_nScreenWidth, m_nScreenHeight);

        wchar_t *path = calloc(265, sizeof(wchar_t));
        path[0] = L'\0';
        wcscpy(path, L"D:\\University\\6_semester\\SPO\\Test");
//
//        getListOfFilesAndDir(path);
        inputClient(path);
    }
    if (key == 50) {
        //if (strcmp(argv[2],"server") == 0) {
        server();
    }
    return 0;
}
