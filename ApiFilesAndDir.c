//
// Created by ad3st on 23.05.2021.
//

#define _CRT_NON_CONFORMING_WCSTOK
#define _CRT_SECURE_NO_WARNINGS

#include "ApiFilesAndDir.h"
#include "Console.h"

HANDLE hFind;
void freeArrayFilesDir(ArrayOfFiles *arrayOfFiles) {
    free(arrayOfFiles->wFile);
    free(arrayOfFiles);
}

void addTimeToFileTime(FILETIME *f, double seconds) {
    ULARGE_INTEGER u;
    memcpy(&u, f, sizeof(u));

    const double c_dSecondsPer100nsInterval = 100. * 1.E-9;
    u.QuadPart += seconds / c_dSecondsPer100nsInterval;

    memcpy(f, &u, sizeof(FILETIME));
}

void addTimeToSystemTime(SYSTEMTIME *s, double seconds) {

    FILETIME f;
    SystemTimeToFileTime(s, &f);

    ULARGE_INTEGER u;
    memcpy(&u, &f, sizeof(u));

    const double c_dSecondsPer100nsInterval = 100. * 1.E-9;
    u.QuadPart += seconds / c_dSecondsPer100nsInterval;

    memcpy(&f, &u, sizeof(f));

    FileTimeToSystemTime(&f, s);
}

size_t getNlineInText(wchar_t *text, int number) {
    int amountOfLines = 1;
    size_t startOfNline = 1;
    size_t afterLastLine = 0;
    for (size_t i = 0; i < wcslen(text); i++) {
        if (text[i] == L'\n') {
            amountOfLines++;
            afterLastLine = 0;
            if (amountOfLines == number)
                startOfNline = i + 1;
        } else if (afterLastLine == WidthMessage - LINES_FOR_BORDER * 2) {
            afterLastLine = 0;
            amountOfLines++;
            if (amountOfLines == number)
                startOfNline = i;
        }
        afterLastLine++;
    }
    if (number == -1)
        return amountOfLines;
    return startOfNline;
}

size_t getLinesInText(wchar_t *text) {
    size_t amountOfLines = 1;
    size_t afterLastLine = 0;
    for (size_t i = 0; i < wcslen(text); i++) {
        if (text[i] == L'\n') {
            amountOfLines++;
            afterLastLine = 0;
        } else
            afterLastLine++;
        if (afterLastLine > WidthMessage - LINES_FOR_BORDER * 2) {
            afterLastLine = 0;
            amountOfLines++;
        }
    }
    return amountOfLines;
}

void getFirstAndLastTokens(wchar_t *text, wchar_t *bufFirst, wchar_t *bufLast, wchar_t *delimiter) {
    wcscpy(bufFirst, text);
    size_t lengthFirst = wcslen(text);
    wchar_t *token = wcstok(bufFirst, delimiter);
    token = wcstok(NULL, delimiter);
    if (token != NULL) {
        do {
            wcscpy(bufLast, token);
            token = wcstok(NULL, delimiter);
        } while (token != NULL);
    }
    if (wcslen(bufLast) > 0)
        lengthFirst = wcslen(text) - wcslen(bufLast) - wcslen(delimiter);
//    wcsncpy(bufFirst, text, lengthFirst); // Strange behavior, copies more, than should
    for (int i = 0; i < lengthFirst; i++)
        bufFirst[i] = text[i];
    bufFirst[lengthFirst] = '\0';
}

void writeTime(int x, SYSTEMTIME *systemtime) {
    wchar_t wTime[9];
    wchar_t wDate[11];
    GetTimeFormatEx(NULL, 0, systemtime, L"HH':'mm':'ss", wTime, 9);
    GetDateFormatEx(NULL, 0, systemtime, L"dd'.'MM'.'yyyy", wDate, 11, NULL);
    for (int i = 0; i < wcslen(wTime); i++) {
        m_bufScreen[x + i].Char.UnicodeChar = wTime[i];
    }
    for (int i = 0; i < wcslen(wDate); i++) {
        m_bufScreen[x + wcslen(wTime) + 1 + i].Char.UnicodeChar = wDate[i];
    }
}

void writeExtension(int x, wchar_t *extension, BOOLEAN isDir) {
    int iStart;
    if (isDir) {
        iStart = x + ExtensionSize - wcslen(L"DIR") - 1;
        m_bufScreen[iStart].Char.UnicodeChar = L'D';
        m_bufScreen[iStart + 1].Char.UnicodeChar = L'I';
        m_bufScreen[iStart + 2].Char.UnicodeChar = L'R';
    } else {
        if (wcslen(extension) <= ExtensionSize - 1) {
            iStart = x + ExtensionSize - wcslen(extension) - 1;
            for (int i = 0; i < wcslen(extension) && i < ExtensionSize; i++) {
                m_bufScreen[iStart + i].Char.UnicodeChar = extension[i];
            }
        } else {
            iStart = x;
            for (int i = 0; i < ExtensionSize - 3; i++) {
                m_bufScreen[iStart + i].Char.UnicodeChar = extension[i];
            }
            m_bufScreen[iStart + ExtensionSize - 3].Char.UnicodeChar = L'.';
            m_bufScreen[iStart + ExtensionSize - 2].Char.UnicodeChar = L'.';
        }
    }

}

void writeListOfFiles(int widthLines, int heightLines, int startingLine) {
    int forText = heightLines - LINES_FOR_BORDER - 1;
    int isizeForName = widthLines - CellTimeSize - ExtensionSize;
    cleanLines(LINES_FOR_BORDER, widthLines, LINES_FOR_BORDER, HeightConsole - LINES_FOR_BORDER * 2); // TODO Тут ошибка с widthLines чет не оч
    drawVLine(m_nScreenWidth - LINES_FOR_BORDER - CellTimeSize - 1, 1, HeightConsole - LINES_FOR_BORDER * 2 + 1);
    for (int i = 1; i <= heightLines; i++) {
        int nFile = i + startingLine - 1 - 1;
        if (nFile >= arrayOfFilesAndDir->count)
            break;
        if (wcslen(arrayOfFilesAndDir->wFile[nFile].name) > isizeForName) {
            for (int k = 0 + LINES_FOR_BORDER; k <= isizeForName - 4; k++) {
                m_bufScreen[i * m_nScreenWidth + k].Char.UnicodeChar = arrayOfFilesAndDir->wFile[nFile].name[k - 1];
            }
            m_bufScreen[i * m_nScreenWidth + isizeForName - 3].Char.UnicodeChar = L'.';
            m_bufScreen[i * m_nScreenWidth + isizeForName - 2].Char.UnicodeChar = L'.';
            m_bufScreen[i * m_nScreenWidth + isizeForName - 1].Char.UnicodeChar = L'.';
        } else {
            for (int k = 0; k < wcslen(arrayOfFilesAndDir->wFile[nFile].name); k++) {
                m_bufScreen[i * m_nScreenWidth + k +
                            LINES_FOR_BORDER].Char.UnicodeChar = arrayOfFilesAndDir->wFile[nFile].name[k];
            }
        }
        writeExtension(i * m_nScreenWidth + isizeForName + 1, arrayOfFilesAndDir->wFile[nFile].extension,
                       arrayOfFilesAndDir->wFile[nFile].type);
        writeTime((i+1) * m_nScreenWidth - CellTimeSize + ((CellTimeSize - TextTimeSize) / 2),
                  &arrayOfFilesAndDir->wFile[nFile].stLastWriteTime);
    }
}

//void writeListOfFiles(int widthLines, int heightLines, int startingLine) {
//    int forText = heightLines - LINES_FOR_BORDER - 1;
//    int isizeForName = widthLines - CellTimeSize - 1 - ExtensionSize;
//    cleanLines(LINES_FOR_BORDER, widthLines, LINES_FOR_BORDER, HeightConsole - LINES_FOR_BORDER * 2);
//    drawVLine(m_nScreenWidth - LINES_FOR_BORDER - CellTimeSize - 1, 1, HeightConsole - LINES_FOR_BORDER * 2);
//    for (int i = 1; i <= forText; i++) {
//        int nFile = i + startingLine - 1 - 1;
//        if (nFile >= arrayOfFilesAndDir->count)
//            break;
//        if (wcslen(arrayOfFilesAndDir->wFile[nFile].name) > isizeForName) {
//            for (int k = 0 + LINES_FOR_BORDER; k <= isizeForName - 4; k++) {
//                m_bufScreen[i * m_nScreenWidth + k].Char.UnicodeChar = arrayOfFilesAndDir->wFile[nFile].name[k - 1];
//            }
//            m_bufScreen[i * m_nScreenWidth + isizeForName - 3].Char.UnicodeChar = L'.';
//            m_bufScreen[i * m_nScreenWidth + isizeForName - 2].Char.UnicodeChar = L'.';
//            m_bufScreen[i * m_nScreenWidth + isizeForName - 1].Char.UnicodeChar = L'.';
//        } else {
//            for (int k = 0; k < wcslen(arrayOfFilesAndDir->wFile[nFile].name); k++) {
//                m_bufScreen[i * m_nScreenWidth + k +
//                            LINES_FOR_BORDER].Char.UnicodeChar = arrayOfFilesAndDir->wFile[nFile].name[k];
//            }
//        }
//        writeExtension(i * m_nScreenWidth + isizeForName + 1, arrayOfFilesAndDir->wFile[nFile].extension,
//                       arrayOfFilesAndDir->wFile[nFile].type);
//        writeTime(i * m_nScreenWidth + widthLines - CellTimeSize + ((CellTimeSize - TextTimeSize) / 2),
//                  &arrayOfFilesAndDir->wFile[nFile].stLastWriteTime);
//    }
//}

void getListOfFilesAndDir(wchar_t *path) {
    arrayOfFilesAndDir = calloc(1, sizeof(ArrayOfFiles));
    arrayOfFilesAndDir->count = 0;
    ArrayOfFiles *arrayOfFiles = calloc(1, sizeof(ArrayOfFiles));
    arrayOfFiles->count = 0;
    arrayOfFiles->wFile = calloc(1, sizeof(WFile));
    ArrayOfFiles *arrayOfDir = calloc(1, sizeof(ArrayOfFiles));
    arrayOfDir->count = 0;
    arrayOfDir->wFile = calloc(1, sizeof(WFile));
    wchar_t *bufpath = calloc(wcslen(path) + 3, sizeof(wchar_t));
    wcscpy(bufpath, path);
    wcscat(bufpath, L"\\*");
    hFind = FindFirstFile(bufpath, &findedFile);
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
        arrayOfFilesAndDir->count = arrayOfDir->count + arrayOfFiles->count;
        arrayOfFilesAndDir->wFile = calloc(arrayOfFilesAndDir->count, sizeof(WFile));
        for (int i = 0; i < arrayOfDir->count; i++) {
            wcscpy(arrayOfFilesAndDir->wFile[i].name, arrayOfDir->wFile[i].name);
            arrayOfFilesAndDir->wFile[i].type = true;
            arrayOfFilesAndDir->wFile[i].stLastWriteTime = arrayOfDir->wFile[i].stLastWriteTime;
        }
        for (int i = arrayOfDir->count; i < arrayOfFilesAndDir->count; i++) {
            wcscpy(arrayOfFilesAndDir->wFile[i].name, arrayOfFiles->wFile[i - arrayOfDir->count].name);
            wcscpy(arrayOfFilesAndDir->wFile[i].extension, arrayOfFiles->wFile[i - arrayOfDir->count].extension);
            arrayOfFilesAndDir->wFile[i].type = false;
            arrayOfFilesAndDir->wFile[i].stLastWriteTime = arrayOfFiles->wFile[i - arrayOfDir->count].stLastWriteTime;
        }
        freeArrayFilesDir(arrayOfFiles);
        freeArrayFilesDir(arrayOfDir);
    }
}
