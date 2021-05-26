//
// Created by ad3st on 23.05.2021.
//
#pragma once
#include "ApiFilesAndDir.h"

#define UNICODE 1
#define _WIN32_WINNT 0x0500

#define LINES_FOR_BORDER 1
#define WidthConsole 150
#define HeightConsole 45

int widthLineFiles;
int lastLineFiles;

#define HeightMessage 20
#define WidthMessage 70

#define TextTimeSize 19
#define CellTimeSize 21 // Cant be less then 19
#define ExtensionSize 7

HANDLE m_hConsole;
CHAR_INFO *m_bufScreen;
COORD coord;
COORD bufcoord;
int m_nScreenWidth;
int m_nScreenHeight;
SMALL_RECT m_rectWindow;

enum COLOUR {
    FG_BLACK = 0x0000,
    FG_DARK_BLUE = 0x0001,
    FG_DARK_GREEN = 0x0002,
    FG_DARK_CYAN = 0x0003,
    FG_DARK_RED = 0x0004,
    FG_DARK_MAGENTA = 0x0005,
    FG_DARK_YELLOW = 0x0006,
    FG_GREY = 0x0007, // Thanks MS :-/
    FG_DARK_GREY = 0x0008,
    FG_BLUE = 0x0009,
    FG_GREEN = 0x000A,
    FG_CYAN = 0x000B,
    FG_RED = 0x000C,
    FG_MAGENTA = 0x000D,
    FG_YELLOW = 0x000E,
    FG_WHITE = 0x000F,
    BG_BLACK = 0x0000,
    BG_DARK_BLUE = 0x0010,
    BG_DARK_GREEN = 0x0020,
    BG_DARK_CYAN = 0x0030,
    BG_DARK_RED = 0x0040,
    BG_DARK_MAGENTA = 0x0050,
    BG_DARK_YELLOW = 0x0060,
    BG_GREY = 0x0070,
    BG_DARK_GREY = 0x0080,
    BG_BLUE = 0x0090,
    BG_GREEN = 0x00A0,
    BG_CYAN = 0x00B0,
    BG_RED = 0x00C0,
    BG_MAGENTA = 0x00D0,
    BG_YELLOW = 0x00E0,
    BG_WHITE = 0x00F0,
};

enum PIXEL_TYPE {
    PIXEL_SOLID = 0x2588,
    PIXEL_THREEQUARTERS = 0x2593,
    PIXEL_HALF = 0x2592,
    PIXEL_QUARTER = 0x2591,
};

void writeText(int x, int y, wchar_t *text, int width, int height, int startLine);
void drawVLine(int x, int y, int height);
void markLine(int x, int y, int width, BOOLEAN clean, BOOLEAN skipBorder);
int CreateConsole(int width, int height, int fontw, int fonth);
void makeBorder(int x, int y, int width, int height) ;

void cleanLines(int numberLine,int widthLine, int x, int amount);
void cleanLine(int numberLine, int widthLine, int x);

void writeListOfFiles(int widthLines, int heightLines, int startingLine);
void updateConsole();

void updateFilesBox(wchar_t *path);
