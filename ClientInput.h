//
// Created by ad3st on 24.05.2021.
//

#pragma once
#include "ApiFilesAndDir.h"
#define _CRT_SECURE_NO_WARNINGS

BOOLEAN bActiveErrorMessage;
BOOLEAN bActiveMessage;

int currentLineFiles;
int startedLineFiles;
int scrollCounterFiles;
int savedScrollCounterFiles;
//int widthLineFiles = WidthConsole - LINES_FOR_BORDER * 2;
//int lastLineFiles = HeightConsole - LINES_FOR_BORDER * 2;

void inputReadFile(wchar_t *pathFile);
void inputClient(wchar_t *path);

