//
// Created by ad3st on 23.05.2021.
//
#include "Console.h"
#include "ClientInput.h"

int widthLineFiles = WidthConsole - LINES_FOR_BORDER*2;
int lastLineFiles = HeightConsole - LINES_FOR_BORDER - 1;


void updateAll(){

}

void markLine(int x, int y, int width, BOOLEAN clean, BOOLEAN skipBorder) {
    WORD style;
    if (clean)
        style = FG_WHITE | BG_BLACK;
    else
        style = FG_WHITE | BG_DARK_GREEN;
    int start = x;
    int end = width;
//    if (skipBorder) {
//        start++;
//        width--;
//    }
    for (int i = start; i < width + start; i++) {
        m_bufScreen[y * m_nScreenWidth + i].Attributes = style;
    }
}

void makeBorder(int x, int y, int width, int height) {
    WORD style = FG_WHITE;
    WCHAR ch = PIXEL_HALF;
    for (int i = x; i < width + x; i++) {
        m_bufScreen[i + y * m_nScreenWidth].Char.UnicodeChar = ch;
        m_bufScreen[i + y * m_nScreenWidth].Attributes = style;
        m_bufScreen[(height - 1 + y) * m_nScreenWidth + i].Char.UnicodeChar = ch;
        m_bufScreen[(height - 1 + y) * m_nScreenWidth + i].Attributes = style;
    }
    for (int j = y; j < height + y; j++) {
        m_bufScreen[j * m_nScreenWidth + x].Char.UnicodeChar = ch;
        m_bufScreen[j * m_nScreenWidth + x].Attributes = style;
        m_bufScreen[j * m_nScreenWidth + x + width - 1].Char.UnicodeChar = ch;
        m_bufScreen[j * m_nScreenWidth + x + width - 1].Attributes = style;
    }
}

int CreateConsole(int width, int height, int fontw, int fonth) {
    m_nScreenWidth = WidthConsole;
    m_nScreenHeight = HeightConsole;
    setlocale(LC_ALL, "rus");
    SetConsoleCP(866);
    SetConsoleOutputCP(866);
    m_bufScreen = calloc(m_nScreenHeight * m_nScreenWidth, sizeof(CHAR_INFO));
    currentLineFiles = 1;
    m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (m_hConsole == INVALID_HANDLE_VALUE)
        return -1;

    m_nScreenWidth = width;
    m_nScreenHeight = height;
    SetConsoleWindowInfo(m_hConsole, TRUE, &m_rectWindow);

    coord.X = (short) m_nScreenWidth;
    coord.Y = (short) m_nScreenHeight;
    m_rectWindow.Left = 0;
    m_rectWindow.Top = 0;
    m_rectWindow.Right = (short) m_nScreenWidth - 1;
    m_rectWindow.Bottom = (short) m_nScreenHeight - 1;
    bufcoord.X = 0;
    bufcoord.Y = 0;

    if (!SetConsoleScreenBufferSize(m_hConsole, coord))
        return -1;

    // Assign screen buffer to the console
    if (!SetConsoleActiveScreenBuffer(m_hConsole))
        return -1;

    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = fontw;
    cfi.dwFontSize.Y = fonth;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;

    //    wcscpy_s(cfi.FaceName,1, L"Consolas");
    //    if (!SetCurrentConsoleFontEx(m_hConsole, false, &cfi))
    //        return -1;

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(m_hConsole, &csbi))
        return -1;
    if (m_nScreenHeight > csbi.dwMaximumWindowSize.Y)
        return -1;
    if (m_nScreenWidth > csbi.dwMaximumWindowSize.X)
        return -1;

    // Set Physical Console Window Size
    SMALL_RECT b1m_rectWindow = {0, 0, (short) m_nScreenWidth - 1, (short) m_nScreenHeight - 1};
    if (!SetConsoleWindowInfo(m_hConsole, TRUE, &b1m_rectWindow))
        return -1;



    // Allocate memory for screen buffer
//    m_bufScreen = calloc(1, sizeof(CHAR_INFO) * width * height);
//    m_bufScreen = malloc( sizeof(CHAR_INFO) * width * height);
    //memset(m_bufScreen, 0, sizeof(CHAR_INFO) * m_nScreenWidth * m_nScreenHeight);

    //SetConsoleCtrlHandler((PHANDLER_ROUTINE)CloseHandler, TRUE);
    HWND consoleWindow = GetConsoleWindow();
    SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(m_hConsole, &info);
    for (int i = 0; i < m_nScreenHeight; i++) {
        markLine(0, i, m_nScreenWidth, true, false);
    }
    markLine(0, currentLineFiles, m_nScreenWidth, false, true);
//    makeBorder(0, 0, m_nScreenWidth, m_nScreenHeight);
    updateConsole();
    return 1;
}

void writeText(int x, int y, wchar_t *text, int width, int height, int startLine) {
    cleanLines(y, width, x, height);
    size_t counter = 0;
    size_t textLength = wcslen(text);
    size_t startOfNline;
    if (startLine > 1) {
        startOfNline = getNlineInText(text, startLine);
        counter += startOfNline;
    }
    wchar_t nextLine = L'\n';
    for (size_t j = y; j < height + y; j++) {
        if (counter >= textLength)
            break;
        for (size_t i = x; i < width + x; i++) {
            if (text[counter] == nextLine) {
                i = width + x - 1;
                counter++;
                continue;
            }
            m_bufScreen[i + j * m_nScreenWidth].Char.UnicodeChar = text[counter];
            counter++;
            if (counter >= textLength)
                break;
        }
    }
}

void drawVLine(int x, int y, int height) {
    WORD style = FG_WHITE;
    WCHAR ch = L'|';
    for (int j = y; j < height; j++) {
        m_bufScreen[j * m_nScreenWidth + x].Char.UnicodeChar = ch;
        m_bufScreen[j * m_nScreenWidth + x].Attributes = style;
    }
}

void cleanLines(int numberLine, int widthLine, int x, int amount) {
    for (int i = 0; i < amount; i++)
        cleanLine(numberLine + i, widthLine, x);
}

void cleanLine(int numberLine, int widthLine, int x) {
    WORD style = FG_WHITE | BG_BLACK;
    for (int k = 0; k < widthLine; k++) {
        m_bufScreen[m_nScreenWidth * numberLine + k + x].Char.UnicodeChar = 0x0;
        m_bufScreen[m_nScreenWidth * numberLine + k + x].Attributes = style;
    }
}

void updateConsole() {
    WriteConsoleOutput(m_hConsole, m_bufScreen, coord, bufcoord, &m_rectWindow);
}

void updateFilesBox(wchar_t *path) {
//    free(arrayOfFilesAndDir);
//    getListOfFilesAndDir(path);

    currentLineFiles = 1;
    scrollCounterFiles = 1;

    writeListOfFiles(m_nScreenWidth - LINES_FOR_BORDER * 2, m_nScreenHeight - LINES_FOR_BORDER - 1, scrollCounterFiles);
    markLine(LINES_FOR_BORDER, currentLineFiles, widthLineFiles, false, true);
    updateConsole();
}