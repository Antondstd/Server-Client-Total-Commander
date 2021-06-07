//
// Created by ad3st on 24.05.2021.
//

#include "ClientInput.h"
#include "Console.h"
#include "serverApiFiles.h"
#include "client.h"

#define _CRT_NON_CONFORMING_WCSTOK
#define _CRT_SECURE_NO_WARNINGS

BOOLEAN bActiveText = false;
BOOLEAN bActiveDownloads = false;
BOOLEAN bActiveUploads = false;
BOOLEAN bActiveErrorMessage = false;
BOOLEAN greatLoop = true;

int currentLineFiles = 1;
int startedLineFiles = 1;
int scrollCounterFiles = 1;
int savedScrollCounterFiles = 1;

int startedLineText;
int currentLineText = 1;
int scrollCounterText = 1;
int savedScrollCounterText = 1;
wchar_t *text;
wchar_t uploadPath[MAX_PATH];
int currentUploadPath = 0;
int amountText;

typedef struct MyDownloadData {
    int val1;
    int val2;
} MyDownloadData, *PMyDownloadData;

void serverListener() {
    Message *messageConsole = calloc(1, sizeof(Message));
    while (greatLoop) {
        if ((recv(mainPair.socket, (void *) messageConsole, sizeof(Message), 0)) == SOCKET_ERROR) {
            puts("recv failed");
        }
        if (messageConsole->type == Text) {
            bActiveText = TRUE;
            text = calloc(messageConsole->amount, sizeof(wchar_t));
            if (messageConsole->amount > 0) {
                if ((recv(mainPair.socket, (void *) text, sizeof(wchar_t) * messageConsole->amount, 0)) ==
                    SOCKET_ERROR) {
                    puts("recv failed");
                }
            }
            amountText = getNlineInText(text, -1);
            int xStartMessage = (m_nScreenWidth - WidthMessage) / 2;
            int yStartMessage = (m_nScreenHeight - LINES_FOR_BORDER * 2) / 2 - HeightMessage / 2;
            int xStartLineMessage = xStartMessage + LINES_FOR_BORDER;
            int yStartLineMessage = yStartMessage + LINES_FOR_BORDER;
            int width_linesMessage = WidthMessage - LINES_FOR_BORDER * 2;
            int height_linesMessage = HeightMessage - LINES_FOR_BORDER - 1;
            cleanLines(yStartMessage, WidthMessage,
                       xStartMessage, HeightMessage);
            makeBorder(xStartMessage, yStartMessage, WidthMessage, HeightMessage);
//            text = getFileText(filePath);
//            amountText = getNlineInText(text, -1);
            writeText(xStartLineMessage, yStartLineMessage, text, width_linesMessage, height_linesMessage,
                      savedScrollCounterText);
            markLine(xStartLineMessage, currentLineText + yStartLineMessage - 1, width_linesMessage, false, true);
            updateConsole();
        }
//        if(messageConsole->type == MOVE || messageConsole->type == UPLOAD || messageConsole->type == Back){
        if (messageConsole->type == UPDATE) {
            arrayOfFilesAndDir->count = messageConsole->amount;
            arrayOfFilesAndDir->wFile = calloc(messageConsole->amount, sizeof(WFile));
            if ((recv(mainPair.socket, (char *) arrayOfFilesAndDir->wFile, sizeof(WFile) * messageConsole->amount,
                      0)) == SOCKET_ERROR) {
                puts("recv failed");
            }
            updateFilesBox(L"path");
        }
        if (messageConsole->type == DOWNLOAD) {

        }
    }
    free(messageConsole);
}

void downloadSome() {
    Message *messageConsole = calloc(1, sizeof(Message));
    wchar_t *newFile = calloc(MAX_PATH + 1, sizeof(wchar_t));
    wcscpy(newFile, L"C:\\Downloads");
    wcscat(newFile, L"\\");
    wcscat(newFile, arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].name);
    if (wcslen(arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].extension) > 0) {
        wcscat(newFile, L".");
        wcscat(newFile, arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].extension);
    }
//                HANDLE hFile = CreateFileW(newFile,GENERIC_WRITE,0,NULL,                   // default security
//                            CREATE_NEW,             // create new file only
//                            FILE_ATTRIBUTE_NORMAL,  // normal file
//                            NULL);
    FILE *fileF = _wfopen(newFile, L"wb");
    WaitForSingleObject(
            filesPair.mutex,    // handle to mutex
            INFINITE);
    if ((recv(filesPair.socket, (char *) messageConsole, sizeof(Message),
              0)) == SOCKET_ERROR) {
        puts("recv failed1");
    }
    void *file = calloc(messageConsole->amount, sizeof(void));
    if ((recv(filesPair.socket, file, sizeof(void) * messageConsole->amount,
              0)) == SOCKET_ERROR) {
        puts("recv failed2");
    }
    ReleaseMutex(filesPair.mutex);
    fwrite(file, sizeof(void), sizeof(void) * messageConsole->amount, fileF);
    fclose(fileF);
//                BOOL bErrorFlag = WriteFile(
//                        hFile,           // open file handle
//                        file,      // start of data to write
//                        messageConsole->amount,  // number of bytes to write
//                        NULL, // number of bytes that were written
//                        NULL);
//                CloseHandle(hFile);
    free(file);
    free(newFile);
    free(messageConsole);
}


void inputReadFile(wchar_t *pathFile) {
    BOOLEAN loop = true;
    int started_line = WidthMessage + LINES_FOR_BORDER;
    int last_line = HeightMessage - LINES_FOR_BORDER - 1;
    int width_line = WidthMessage - LINES_FOR_BORDER * 2;

    int xLine = (m_nScreenWidth - WidthMessage) / 2;
    int yLine = (m_nScreenHeight - LINES_FOR_BORDER * 2) / 2 - HeightMessage / 2;

    int startYLine = yLine + LINES_FOR_BORDER;
    int lastYLine = yLine + HeightMessage - LINES_FOR_BORDER - 1;
    int scrollCounter = 1;
    int currentLineText = 1;
    int savedScrollCounter = scrollCounter;
//    cleanLine((m_nScreenHeight - LINES_FOR_BORDER * 2) / 2 - HeightMessage / 2, WidthMessage,
//              (m_nScreenWidth - WidthMessage) / 2);

    cleanLines(yLine, WidthMessage,
               xLine, HeightMessage);
    makeBorder(xLine, yLine, WidthMessage, HeightMessage);

    wchar_t *text = getFileText(pathFile);
    size_t amountLinesText = getNlineInText(text, -1);
//    wchar_t *text = getFileText(L"D:\\University\\6_semester\\SPO\\Test\\ppp.eee.txt");
//    wchar_t text[255] =  L"12345676565656565656565466661\n16\n6666\n66666\n66666\n6666\n66666\n6666\n666\n66666\n66\n666\n666\n6666\n666\n666\n66\n66\n66\n666\n77\n888";
    writeText(xLine + LINES_FOR_BORDER, yLine + LINES_FOR_BORDER, text, width_line, last_line, 0);
    markLine(xLine + LINES_FOR_BORDER, currentLineText + startYLine - 1, width_line, false, true);
    updateConsole();
    while (loop) {
        int key = _getch();
        if (key == 27) {
            loop = false;
            continue;
        }
        if (key == 224) {
            key = _getch();
            if (key == 72) {
                // ARROW UP
                markLine(xLine + LINES_FOR_BORDER, currentLineText + startYLine - 1, width_line, true, true);
                currentLineText--;
                scrollCounter--;
                if (scrollCounter < 1 && currentLineText < 1) {
                    scrollCounter = amountLinesText;
                    if (amountLinesText < last_line)
                        currentLineText = amountLinesText;
                    else
                        currentLineText = last_line;
                    savedScrollCounter = scrollCounter - currentLineText + 1;
                    writeText(xLine + LINES_FOR_BORDER, startYLine, text, width_line, last_line, savedScrollCounter);
                } else if (currentLineText < 1) {
                    currentLineText++;
                    savedScrollCounter = scrollCounter;
                    writeText(xLine + LINES_FOR_BORDER, startYLine, text, width_line, last_line, savedScrollCounter);
                }
            } else if (key == 80) {
                markLine(xLine + LINES_FOR_BORDER, currentLineText + startYLine - 1, width_line, true, true);
                currentLineText++;
                scrollCounter++;
                if ((currentLineText > last_line || currentLineText > amountLinesText) &&
                    scrollCounter > amountLinesText) {
                    currentLineText = 1;
                    scrollCounter = 1;
                    savedScrollCounter = scrollCounter;
                    writeText(xLine + LINES_FOR_BORDER, startYLine, text, width_line, last_line, savedScrollCounter);
                } else if (currentLineText > last_line) {
                    currentLineText--;
                    savedScrollCounter = scrollCounter - last_line + 1;
                    writeText(xLine + LINES_FOR_BORDER, startYLine, text, width_line, last_line, savedScrollCounter);
                }
            }
            markLine(xLine + LINES_FOR_BORDER, currentLineText + startYLine - 1, width_line, false, true);
            updateConsole();
            continue;
        }
    }
    free(text);
}

void greatInputClient(wchar_t *path) {
    int xStart; //xLine
    int yStart; //yLine
    int xStartLine;
    int yStartLine; //startYLine
//    int yLastLine;
    int width_lines; // width_line
    int height_lines; //last_line

    int *currentLine;
    int *scrollCounter;
    int *savedScrollCounter;

    int *amount;

    wchar_t *filePath; //TODO Удалить

    Message *messageConsole = calloc(1, sizeof(Message));

    CreateThread(
            NULL,                   // default security attributes
            0,                      // use default stack size
            (LPTHREAD_START_ROUTINE) serverListener,       // thread function name
            NULL,          // argument to thread function
            0,                      // use default creation flags
            NULL);
    while (greatLoop) {
        wint_t key = _getwch();
        if (bActiveText == true) {
            xStart = (m_nScreenWidth - WidthMessage) / 2;
            yStart = (m_nScreenHeight - LINES_FOR_BORDER * 2) / 2 - HeightMessage / 2;
            xStartLine = xStart + LINES_FOR_BORDER;
            yStartLine = yStart + LINES_FOR_BORDER;
            width_lines = WidthMessage - LINES_FOR_BORDER * 2;
            height_lines = HeightMessage - LINES_FOR_BORDER - 1;
            currentLine = &currentLineText;
            scrollCounter = &scrollCounterText;
            savedScrollCounter = &savedScrollCounterText;
            amount = &amountText;
        } else {
            xStart = 0;
            yStart = 0;
            xStartLine = xStart + LINES_FOR_BORDER;
            yStartLine = yStart + LINES_FOR_BORDER;
            width_lines = m_nScreenWidth - LINES_FOR_BORDER * 2;
            height_lines = m_nScreenHeight - LINES_FOR_BORDER - 1;
            currentLine = &currentLineFiles;
            scrollCounter = &scrollCounterFiles;
            savedScrollCounter = &savedScrollCounterFiles;
            amount = &arrayOfFilesAndDir->count;
        }
        if (key == 224) {
            key = _getwch();
            if (key == 72) {
                // ARROW UP
                markLine(xStartLine, *currentLine, m_nScreenWidth, true, true);
                *currentLine = *currentLine - 1;
                *scrollCounter = *scrollCounter - 1;
                if (*scrollCounter < 1 && *currentLine < 1) {
                    *scrollCounter = *amount;
                    if (*amount < height_lines) // Height_lines с last_line ??
                        *currentLine = *amount;
                    else
                        *currentLine = height_lines;
                    *savedScrollCounter = *scrollCounter - *currentLine + 1;
                } else if (*currentLine < 1) {
                    *currentLine = *currentLine + 1;
                    *savedScrollCounter = *scrollCounter;
                }
                if (bActiveText)
                    writeText(xStartLine, yStartLine, text, width_lines, height_lines, *savedScrollCounter);
                else
                    writeListOfFiles(width_lines, height_lines, *savedScrollCounter);
            } else if (key == 80) {
                markLine(xStartLine, *currentLine, m_nScreenWidth, true, true);
                *currentLine = *currentLine + 1;
                *scrollCounter = *scrollCounter + 1;
                if ((*currentLine > height_lines || *currentLine > *amount) &&
                    *scrollCounter > *amount) {
                    *currentLine = 1;
                    *scrollCounter = 1;
                    *savedScrollCounter = 1;
                } else if (*currentLine > height_lines) {
                    *currentLine = *currentLine - 1;
                    *savedScrollCounter = *scrollCounter - height_lines + 1;
                }
                if (bActiveText)
                    writeText(xStartLine, yStartLine, text, width_lines, height_lines, *savedScrollCounter);
                else
                    writeListOfFiles(width_lines, height_lines, *savedScrollCounter);
            }
            markLine(xStartLine, *currentLine + yStartLine - 1, width_lines, false, true);
            updateConsole();
            continue;
        }

        if (key == 13 && !(bActiveText || bActiveDownloads || bActiveErrorMessage || bActiveUploads) &&
            wcscmp(arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].extension, L"txt") == 0) {
            bActiveText = true;
            savedScrollCounterText = 1;
            scrollCounterText = 1;
            currentLineText = 1;
            markLine(xStartLine, *currentLine, width_lines, true, true);
            int lengthPath = wcslen(arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].name) + 1 +
                             wcslen(arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].extension) + 1;
            filePath = calloc(lengthPath, sizeof(wchar_t));
            wcscat(filePath, arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].name);
            wcscat(filePath, L".");
            wcscat(filePath, arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].extension);
            filePath[lengthPath - 1] = L'\0';
            wcscpy(messageConsole->message, filePath);
            messageConsole->type = Text;
            messageConsole->id = mainID;
//            WaitForSingleObject(
//                    mainPair.mutex,    // handle to mutex
//                    INFINITE);
            send(mainPair.socket, (void *) messageConsole, sizeof(Message), 0);
//            if ((recv(mainPair.socket, (void *) messageConsole, sizeof(Message), 0)) == SOCKET_ERROR) {
//                puts("recv failed");
//            }
//            text = calloc(messageConsole->amount, sizeof(wchar_t));
//            if (messageConsole->amount > 0) {
//                if ((recv(mainPair.socket, (void *) text, sizeof(wchar_t) * messageConsole->amount, 0)) ==
//                    SOCKET_ERROR) {
//                    puts("recv failed");
//                }
//            }
//            amountText = getNlineInText(text, -1);
//            ReleaseMutex(mainPair.mutex);

//            filePath = calloc(wcslen(path) +
//                              wcslen(arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].name) + 1 +
//                              wcslen(arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].extension) + 2,
//                              sizeof(wchar_t));
//            wcscpy(filePath, path);
//            wcscat(filePath, L"\\");
//            wcscat(filePath, arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].name);
//            wcscat(filePath, L".");
//            wcscat(filePath, arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].extension);
//            int xStartMessage = (m_nScreenWidth - WidthMessage) / 2;
//            int yStartMessage = (m_nScreenHeight - LINES_FOR_BORDER * 2) / 2 - HeightMessage / 2;
//            int xStartLineMessage = xStartMessage + LINES_FOR_BORDER;
//            int yStartLineMessage = yStartMessage + LINES_FOR_BORDER;
//            int width_linesMessage = WidthMessage - LINES_FOR_BORDER * 2;
//            int height_linesMessage = HeightMessage - LINES_FOR_BORDER - 1;
//            cleanLines(yStartMessage, WidthMessage,
//                       xStartMessage, HeightMessage);
//            makeBorder(xStartMessage, yStartMessage, WidthMessage, HeightMessage);
//            text = getFileText(filePath);
//            amountText = getNlineInText(text, -1);
            free(filePath);
//            writeText(xStartLineMessage, yStartLineMessage, text, width_linesMessage, height_linesMessage,
//                      savedScrollCounterText);
//            markLine(xStartLineMessage, currentLineText + yStartLineMessage - 1, width_linesMessage, false, true);
//            updateConsole();
            continue;
//            inputReadFile(filePath);
        }
        if (key == 13 && !(bActiveText || bActiveDownloads || bActiveErrorMessage || bActiveUploads) &&
            arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].type) {
            if (wcscmp(arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].name, L"..") == 0) {
//                wchar_t bufFirst[265];
//                wchar_t bufLast[265];
//                getFirstAndLastTokens(path, bufFirst, bufLast, L"\\");
//                wcscpy(path, bufFirst);
//                updateFilesBox(path);
                messageConsole->id = mainID;
                messageConsole->type = Back;
                send(mainPair.socket, (void *) messageConsole, sizeof(Message), 0);
//                free(arrayOfFilesAndDir->wFile);
//                if ((recv(mainPair.socket, (char *) messageConsole, sizeof(Message),
//                          0)) == SOCKET_ERROR) {
//                    puts("recv failed");
//                }
//                arrayOfFilesAndDir->count = messageConsole->amount;
//                arrayOfFilesAndDir->wFile = calloc(messageConsole->amount, sizeof(WFile));
//                if ((recv(mainPair.socket, (char *) arrayOfFilesAndDir->wFile, sizeof(WFile) * messageConsole->amount,
//                          0)) == SOCKET_ERROR) {
//                    puts("recv failed");
//                }
//                updateFilesBox(path);
            } else {
//                wcscat(path, L"\\");
//                wcscat(path, arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].name);
                messageConsole->id = mainID;
                messageConsole->type = MOVE;
                wcscpy(messageConsole->message, arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].name);
                send(mainPair.socket, (void *) messageConsole, sizeof(Message), 0);
//                free(arrayOfFilesAndDir->wFile);
//                if ((recv(mainPair.socket, (char *) messageConsole, sizeof(Message),
//                          0)) == SOCKET_ERROR) {
//                    puts("recv failed");
//                }
//                arrayOfFilesAndDir->count = messageConsole->amount;
//                arrayOfFilesAndDir->wFile = calloc(messageConsole->amount, sizeof(WFile));
//                if ((recv(mainPair.socket, (char *) arrayOfFilesAndDir->wFile, sizeof(WFile) * messageConsole->amount,
//                          0)) == SOCKET_ERROR) {
//                    puts("recv failed");
//                }
//                updateFilesBox(path);
            }
            continue;
        }

        if (key == 0) {
            key = _getwch();
            if (key == 59 && arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].type == false) {
                messageConsole->id = mainID;
                messageConsole->type = DOWNLOAD;
                wcscpy(messageConsole->message, arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].name);
                if (wcslen(arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].extension) > 0) {
                    wcscat(messageConsole->message, L".");
                    wcscat(messageConsole->message, arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].extension);
                }
                send(mainPair.socket, (void *) messageConsole, sizeof(Message), 0);

                CreateThread(
                        NULL,                   // default security attributes
                        0,                      // use default stack size
                        (LPTHREAD_START_ROUTINE) downloadSome,       // thread function name
                        NULL,          // argument to thread function
                        0,                      // use default creation flags
                        NULL);
//                wchar_t *newFile = calloc(MAX_PATH+1,sizeof (wchar_t));
//                wcscpy(newFile,L"C:\\Downloads");
//                wcscat(newFile,L"\\");
//                wcscat(newFile,arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].name);
//                if (wcslen(arrayOfFilesAndDir->wFile[scrollCounterFiles-1].extension) > 0) {
//                    wcscat(newFile, L".");
//                    wcscat(newFile, arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].extension);
//                }
////                HANDLE hFile = CreateFileW(newFile,GENERIC_WRITE,0,NULL,                   // default security
////                            CREATE_NEW,             // create new file only
////                            FILE_ATTRIBUTE_NORMAL,  // normal file
////                            NULL);
//                FILE* fileF = _wfopen(newFile,L"wb");
//                if ((recv(filesPair.socket, (char *) messageConsole, sizeof(Message),
//                          0)) == SOCKET_ERROR) {
//                    puts("recv failed1");
//                }
//                void *file = calloc(messageConsole->amount,sizeof (void));
//                if ((recv(filesPair.socket, file, sizeof(void) * messageConsole->amount,
//                          0)) == SOCKET_ERROR) {
//                    puts("recv failed2");
//                }
//                fwrite(file,sizeof (void),sizeof(void) *messageConsole->amount,fileF);
//                fclose(fileF);
////                BOOL bErrorFlag = WriteFile(
////                        hFile,           // open file handle
////                        file,      // start of data to write
////                        messageConsole->amount,  // number of bytes to write
////                        NULL, // number of bytes that were written
////                        NULL);
////                CloseHandle(hFile);
//                free(file);
//                free(newFile);
            }
            if (key == 60) {
                //F2
                bActiveUploads = true;
                currentUploadPath = 0;
                wcsnset(uploadPath, 0, MAX_PATH);
                int xStartMessage = (m_nScreenWidth - WidthMessage) / 2;
                int yStartMessage = (m_nScreenHeight - LINES_FOR_BORDER * 2) / 2 - HeightMessage / 2;
                int xStartLineMessage = xStartMessage + LINES_FOR_BORDER;
                int yStartLineMessage = yStartMessage + LINES_FOR_BORDER;
                int width_linesMessage = WidthMessage - LINES_FOR_BORDER * 2;
                int height_linesMessage = HeightMessage - LINES_FOR_BORDER - 1;
                cleanLines(yStartMessage, WidthMessage,
                           xStartMessage, HeightMessage);
                makeBorder(xStartMessage, yStartMessage, WidthMessage, HeightMessage);
//            text = getFileText(filePath);
//            amountText = getNlineInText(text, -1);
                updateConsole();
            }
            continue;
        }
        if (bActiveUploads == true) {
            if (key == 13) {
                //ENTER
                //UPLOAD FILE
                messageConsole->id = mainID;
                messageConsole->type = UPLOAD;
                wchar_t bufFirst[MAX_PATH];
                wchar_t bufLast[MAX_PATH];

                FILE *fileF = _wfopen(uploadPath, L"rb");
                if (fileF == NULL)
                    continue;
                fseek(fileF, 0, SEEK_END); // seek to end of file
                long fileSize = ftell(fileF); // get current file pointer
                printf("FILESIZE IS : %ld", fileSize);
                fseek(fileF, 0, SEEK_SET);
                messageConsole->amount = fileSize;
                getFirstAndLastTokens(uploadPath, bufFirst, bufLast, L"\\");
                wcscpy(messageConsole->message, bufLast);
                send(mainPair.socket, (void *) messageConsole, sizeof(Message), 0);
                if (fileSize > 0) {
                    void *filebuffer = calloc(1, sizeof(void) * fileSize);
                    fread(filebuffer, sizeof(void), fileSize, fileF);
                    send(filesPair.socket, filebuffer, fileSize, 0);
                    free(filebuffer);
                }
                fclose(fileF);
                bActiveUploads = false;
                int widthLinesFiles = m_nScreenWidth - LINES_FOR_BORDER * 2;
                int height_linesFiles = m_nScreenHeight - LINES_FOR_BORDER - 1;
                cleanLines(LINES_FOR_BORDER, widthLinesFiles, LINES_FOR_BORDER, //xStartLine = LINES_FOR_BORDER
                           height_linesFiles);
                writeListOfFiles(widthLinesFiles, height_linesFiles, savedScrollCounterFiles);
                markLine(LINES_FOR_BORDER, currentLineFiles, widthLinesFiles, false,
                         true); //xStartLine = LINES_FOR_BORDER
                drawVLine(widthLinesFiles - CellTimeSize, LINES_FOR_BORDER,
                          height_linesFiles + 1); //yStartLine = LINES_FOR_BORDER
                updateConsole();
                continue;
            }
            if (key == 8) {
                uploadPath[currentUploadPath - 1] = 0;
                currentUploadPath = currentUploadPath - 2;
            } else {
                uploadPath[currentUploadPath] = key;
            }
            if (currentUploadPath < MAX_PATH)
                currentUploadPath++;
            if (currentUploadPath < 0)
                currentUploadPath = 0;
            xStart = (m_nScreenWidth - WidthMessage) / 2;
            yStart = (m_nScreenHeight - LINES_FOR_BORDER * 2) / 2 - HeightMessage / 2;
            xStartLine = xStart + LINES_FOR_BORDER;
            yStartLine = yStart + LINES_FOR_BORDER;
            width_lines = WidthMessage - LINES_FOR_BORDER * 2;
            height_lines = HeightMessage - LINES_FOR_BORDER - 1;
            writeText(xStartLine, yStartLine, uploadPath, width_lines, height_lines, 1);
            updateConsole();
        }


        if (key == 27) {
            if (bActiveUploads == true) {
                bActiveUploads = false;
                if (bActiveText == true) {
                    int xStartMessage = (m_nScreenWidth - WidthMessage) / 2;
                    int yStartMessage = (m_nScreenHeight - LINES_FOR_BORDER * 2) / 2 - HeightMessage / 2;
                    int xStartLineMessage = xStartMessage + LINES_FOR_BORDER;
                    int yStartLineMessage = yStartMessage + LINES_FOR_BORDER;
                    int width_linesMessage = WidthMessage - LINES_FOR_BORDER * 2;
                    int height_linesMessage = HeightMessage - LINES_FOR_BORDER - 1;
                    cleanLines(yStartMessage, WidthMessage,
                               xStartMessage, HeightMessage);
                    makeBorder(xStartMessage, yStartMessage, WidthMessage, HeightMessage);
                    writeText(xStartLineMessage, yStartLineMessage, text, width_linesMessage, height_linesMessage,
                              savedScrollCounterText);
                } else {
                    int widthLinesFiles = m_nScreenWidth - LINES_FOR_BORDER * 2;
                    int height_linesFiles = m_nScreenHeight - LINES_FOR_BORDER - 1;
                    cleanLines(LINES_FOR_BORDER, widthLinesFiles, LINES_FOR_BORDER, //xStartLine = LINES_FOR_BORDER
                               height_linesFiles);
                    writeListOfFiles(widthLinesFiles, height_linesFiles, savedScrollCounterFiles);
                    markLine(LINES_FOR_BORDER, currentLineFiles, widthLinesFiles, false,
                             true); //xStartLine = LINES_FOR_BORDER

                    drawVLine(widthLinesFiles - CellTimeSize, LINES_FOR_BORDER,
                              height_linesFiles + 1); //yStartLine = LINES_FOR_BORDER
                }
                updateConsole();
            } else if (bActiveText) {
                bActiveText = false;
                int widthLinesFiles = m_nScreenWidth - LINES_FOR_BORDER * 2;
                int height_linesFiles = m_nScreenHeight - LINES_FOR_BORDER - 1;
                cleanLines(LINES_FOR_BORDER, widthLinesFiles, LINES_FOR_BORDER, //xStartLine = LINES_FOR_BORDER
                           height_linesFiles);
                writeListOfFiles(widthLinesFiles, height_linesFiles, savedScrollCounterFiles);
                markLine(LINES_FOR_BORDER, currentLineFiles, widthLinesFiles, false,
                         true); //xStartLine = LINES_FOR_BORDER
                drawVLine(widthLinesFiles - CellTimeSize, LINES_FOR_BORDER,
                          height_linesFiles + 1); //yStartLine = LINES_FOR_BORDER
                updateConsole();
            } else
                greatLoop = false;
            continue;
        }
    }
}

void inputClient(wchar_t *path) {

    BOOLEAN loop = true;

    writeListOfFiles(m_nScreenWidth - LINES_FOR_BORDER * 2, m_nScreenHeight - LINES_FOR_BORDER - 1, scrollCounterFiles);
    updateConsole();
//    CreateThread(
//            NULL,                   // default security attributes
//            0,                      // use default stack size
//            (LPTHREAD_START_ROUTINE) sendUpdate,       // thread function name
//            arguments,          // argument to thread function
//            0,                      // use default creation flags
//            NULL);
    greatInputClient(path);
}

//void inputClient(wchar_t *path) {
//
//    BOOLEAN loop = true;
//    writeListOfFiles(widthLineFiles, m_nScreenHeight, scrollCounterFiles);
//    WriteConsoleOutput(m_hConsole, m_bufScreen, coord, bufcoord, &m_rectWindow);
//    while (loop) {
//        int key = _getch();
//        if (key == 224) {
//            key = _getch();
//            if (key == 72) {
//                // ARROW UP
//                markLine(LINES_FOR_BORDER, currentLineFiles, m_nScreenWidth, true, true);
//                currentLineFiles--;
//                scrollCounterFiles--;
//                if (scrollCounterFiles < 1 && currentLineFiles < startedLineFiles) {
//                    scrollCounterFiles = arrayOfFilesAndDir->count;
//                    if (arrayOfFilesAndDir->count < lastLineFiles)
//                        currentLineFiles = arrayOfFilesAndDir->count;
//                    else
//                        currentLineFiles = lastLineFiles;
//                    savedScrollCounterFiles = scrollCounterFiles - currentLineFiles + 1;
//                    writeListOfFiles(widthLineFiles, m_nScreenHeight, savedScrollCounterFiles);
//                } else if (currentLineFiles < startedLineFiles) {
//                    currentLineFiles++;
//                    savedScrollCounterFiles = scrollCounterFiles;
//                    writeListOfFiles(widthLineFiles, m_nScreenHeight, savedScrollCounterFiles);
//                }
//            } else if (key == 80) {
//                markLine(LINES_FOR_BORDER, currentLineFiles, m_nScreenWidth, true, true);
//                currentLineFiles++;
//                scrollCounterFiles++;
//                if ((currentLineFiles > lastLineFiles || currentLineFiles > arrayOfFilesAndDir->count) &&
//                    scrollCounterFiles > arrayOfFilesAndDir->count) {
//                    currentLineFiles = startedLineFiles;
//                    scrollCounterFiles = startedLineFiles;
//                    savedScrollCounterFiles = scrollCounterFiles;
//                    writeListOfFiles(widthLineFiles, m_nScreenHeight, savedScrollCounterFiles);
//                } else if (currentLineFiles > lastLineFiles) {
//                    currentLineFiles--;
//                    savedScrollCounterFiles = scrollCounterFiles - lastLineFiles + 1;
//                    writeListOfFiles(widthLineFiles, m_nScreenHeight, savedScrollCounterFiles);
//                }
//            }
//            markLine(LINES_FOR_BORDER, currentLineFiles, widthLineFiles, false, true);
//            updateConsole();
//            continue;
//        }
//        if (key == 13 && wcscmp(arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].extension, L"txt") == 0) {
//            bActiveText = true;
//            markLine(LINES_FOR_BORDER, currentLineFiles, widthLineFiles, true, true);
//            wchar_t *filePath = calloc(wcslen(path) +
//                                       wcslen(arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].name) + 1 +
//                                       wcslen(arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].extension) + 2,
//                                       sizeof(wchar_t));
//            wcscpy(filePath, path);
//            wcscat(filePath, L"\\");
//            wcscat(filePath, arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].name);
//            wcscat(filePath, L".");
//            wcscat(filePath, arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].extension);
//            inputReadFile(filePath);
//            free(filePath);
//            cleanLines(LINES_FOR_BORDER, widthLineFiles, LINES_FOR_BORDER,
//                       lastLineFiles);
//            writeListOfFiles(widthLineFiles, m_nScreenHeight, savedScrollCounterFiles);
//            markLine(LINES_FOR_BORDER, currentLineFiles, widthLineFiles, false, true);
//            drawVLine(m_nScreenWidth - LINES_FOR_BORDER - CellTimeSize - 1, 1, m_nScreenHeight - 1);
//            updateConsole();
//        }
//        if (key == 13 && arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].type) {
//            if (wcscmp(arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].name, L"..") == 0) {
//                wchar_t bufFirst[265];
//                wchar_t bufLast[265];
//                getFirstAndLastTokens(path, bufFirst, bufLast, L"\\");
//                wcscpy(path, bufFirst);
//                updateFilesBox(path);
//            } else {
//                wcscat(path, L"\\");
//                wcscat(path, arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].name);
//                updateFilesBox(path);
//            }
//        }
//        if (key == 27) {
//            loop = false;
//            continue;
//        }
//
//        //        printf("KEY %c\n", key);
//    }
//}