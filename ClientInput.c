//
// Created by ad3st on 24.05.2021.
//

#include "ClientInput.h"

BOOLEAN bActiveErrorMessage = false;
BOOLEAN bActiveMessage = false;

int currentLineFiles = 1;
int startedLineFiles = 1;
int scrollCounterFiles = 1;
int savedScrollCounterFiles = 1;

void inputReadFile(wchar_t *pathFile) {
    BOOLEAN loop = true;
    int started_line = WidthMessage + LINES_FOR_BORDER;
    int last_line = HeightMessage - LINES_FOR_BORDER - 1;
    int width_line = WidthMessage - LINES_FOR_BORDER*2;

    int xLine = (m_nScreenWidth - WidthMessage) / 2;
    int yLine = (m_nScreenHeight - LINES_FOR_BORDER * 2) / 2 - HeightMessage / 2;

    int startYLine = yLine + LINES_FOR_BORDER;
    int lastYLine = yLine + HeightMessage - LINES_FOR_BORDER - 1;
    int scrollCounter = 1;
    int readCurrentLine = 1;
    int savedScrollCounter = scrollCounter;
//    cleanLine((m_nScreenHeight - LINES_FOR_BORDER * 2) / 2 - HeightMessage / 2, WidthMessage,
//              (m_nScreenWidth - WidthMessage) / 2);

    cleanLines(yLine, WidthMessage,
               xLine, HeightMessage);
    makeBorder(xLine, yLine, WidthMessage, HeightMessage);
    wchar_t *text = getFileText(pathFile);
    size_t amountLinesText = getNlineInText(text,-1);
//    wchar_t *text = getFileText(L"D:\\University\\6_semester\\SPO\\Test\\ppp.eee.txt");
//    wchar_t text[255] =  L"12345676565656565656565466661\n16\n6666\n66666\n66666\n6666\n66666\n6666\n666\n66666\n66\n666\n666\n6666\n666\n666\n66\n66\n66\n666\n77\n888";
    writeText(xLine + LINES_FOR_BORDER, yLine + LINES_FOR_BORDER, text, width_line, last_line,0);
    markLine(xLine + LINES_FOR_BORDER, readCurrentLine + startYLine - 1, width_line, false, true);
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
                markLine(xLine + LINES_FOR_BORDER, readCurrentLine + startYLine - 1, width_line, true, true);
                readCurrentLine--;
                scrollCounter--;
                if (scrollCounter < 1 && readCurrentLine < 1) {
                    scrollCounter = amountLinesText;
                    if (amountLinesText < last_line)
                        readCurrentLine = amountLinesText;
                    else
                        readCurrentLine = last_line;
                    savedScrollCounter = scrollCounter - readCurrentLine + 1;
                    writeText(xLine + LINES_FOR_BORDER, startYLine, text, width_line, last_line,savedScrollCounter);
                } else if (readCurrentLine < 1) {
                    readCurrentLine++;
                    savedScrollCounter = scrollCounter;
                    writeText(xLine + LINES_FOR_BORDER, startYLine, text, width_line, last_line,savedScrollCounter);
                }
            } else if (key == 80) {
                markLine(xLine + LINES_FOR_BORDER, readCurrentLine + startYLine - 1, width_line, true, true);
                readCurrentLine++;
                scrollCounter++;
                if ((readCurrentLine > last_line || readCurrentLine > amountLinesText) &&
                    scrollCounter > amountLinesText) {
                    readCurrentLine = 1;
                    scrollCounter = 1;
                    savedScrollCounter = scrollCounter;
                    writeText(xLine + LINES_FOR_BORDER, startYLine, text, width_line, last_line,savedScrollCounter);
                } else if (readCurrentLine > last_line) {
                    readCurrentLine--;
                    savedScrollCounter = scrollCounter - last_line + 1;
                    writeText(xLine + LINES_FOR_BORDER, startYLine, text, width_line, last_line,savedScrollCounter);
                }
            }
            markLine(xLine+LINES_FOR_BORDER, readCurrentLine + startYLine - 1, width_line, false, true);
            updateConsole();
            continue;
        }
    }
    free(text);
}

void inputClient(wchar_t *path) {

    BOOLEAN loop = true;
    writeListOfFiles(widthLineFiles, m_nScreenHeight, scrollCounterFiles);
    WriteConsoleOutput(m_hConsole, m_bufScreen, coord, bufcoord, &m_rectWindow);
    while (loop) {
        int key = _getch();
        if (key == 224) {
            key = _getch();
            if (key == 72) {
                // ARROW UP
                markLine(LINES_FOR_BORDER, currentLineFiles, m_nScreenWidth, true, true);
                currentLineFiles--;
                scrollCounterFiles--;
                if (scrollCounterFiles < 1 && currentLineFiles < startedLineFiles) {
                    scrollCounterFiles = arrayOfFilesAndDir->count;
                    if (arrayOfFilesAndDir->count < lastLineFiles)
                        currentLineFiles = arrayOfFilesAndDir->count;
                    else
                        currentLineFiles = lastLineFiles;
                    savedScrollCounterFiles = scrollCounterFiles - currentLineFiles + 1;
                    writeListOfFiles(widthLineFiles, m_nScreenHeight, savedScrollCounterFiles);
                } else if (currentLineFiles < startedLineFiles) {
                    currentLineFiles++;
                    savedScrollCounterFiles = scrollCounterFiles;
                    writeListOfFiles(widthLineFiles, m_nScreenHeight, savedScrollCounterFiles);
                }
            } else if (key == 80) {
                markLine(LINES_FOR_BORDER, currentLineFiles, m_nScreenWidth, true, true);
                currentLineFiles++;
                scrollCounterFiles++;
                if ((currentLineFiles > lastLineFiles || currentLineFiles > arrayOfFilesAndDir->count) &&
                    scrollCounterFiles > arrayOfFilesAndDir->count) {
                    currentLineFiles = startedLineFiles;
                    scrollCounterFiles = startedLineFiles;
                    savedScrollCounterFiles = scrollCounterFiles;
                    writeListOfFiles(widthLineFiles, m_nScreenHeight, savedScrollCounterFiles);
                } else if (currentLineFiles > lastLineFiles) {
                    currentLineFiles--;
                    savedScrollCounterFiles = scrollCounterFiles - lastLineFiles + 1;
                    writeListOfFiles(widthLineFiles, m_nScreenHeight, savedScrollCounterFiles);
                }
            }
            markLine(LINES_FOR_BORDER, currentLineFiles, widthLineFiles, false, true);
            updateConsole();
            continue;
        }
        if (key == 13 && wcscmp(arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].extension, L"txt") == 0) {
            bActiveMessage = true;
            markLine(LINES_FOR_BORDER, currentLineFiles, widthLineFiles, true, true);
            wchar_t *filePath = calloc(wcslen(path) +
                                       wcslen(arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].name) + 1 +
                                       wcslen(arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].extension) + 2,
                                       sizeof(wchar_t));
            wcscpy(filePath, path);
            wcscat(filePath, L"\\");
            wcscat(filePath, arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].name);
            wcscat(filePath, L".");
            wcscat(filePath, arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].extension);
            inputReadFile(filePath);
            free(filePath);
            cleanLines(LINES_FOR_BORDER, widthLineFiles, LINES_FOR_BORDER,
                       lastLineFiles);
            writeListOfFiles(widthLineFiles, m_nScreenHeight, savedScrollCounterFiles);
            markLine(LINES_FOR_BORDER, currentLineFiles, widthLineFiles, false, true);
            drawVLine(m_nScreenWidth - LINES_FOR_BORDER - CellTimeSize - 1, 1, m_nScreenHeight - 1);
            updateConsole();
        }
        if (key == 13 && arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].type){
            if (wcscmp(arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].name, L"..") == 0){
                wchar_t bufFirst[265];
                wchar_t bufLast[265];
                getFirstAndLastTokens(path,bufFirst,bufLast,L"\\");
                wcscpy(path,bufFirst);
                updateFilesBox(path);
            }
            else{
                wcscat(path,L"\\");
                wcscat(path,arrayOfFilesAndDir->wFile[scrollCounterFiles - 1].name);
                updateFilesBox(path);
            }
        }
        if (key == 27) {
            loop = false;
            continue;
        }

        //        printf("KEY %c\n", key);
    }
}