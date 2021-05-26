//#include <stdio.h>


#pragma warning(disable : 4996)

//#include <windows.h>
//#include <locale.h>
//#include <stdbool.h>
//#include <conio.h>
#include "ApiFilesAndDir.h"



int main() {


    //    m_bufScreen = calloc(1, sizeof(CHAR_INFO) * 250 * 50);
    CreateConsole(WidthConsole, HeightConsole, 15, 15);
    makeBorder(0, 0, m_nScreenWidth, m_nScreenHeight);

    wchar_t *path = calloc(265,sizeof (wchar_t));
    path[0] = L'\0';
    wcscpy(path,L"D:\\University\\6_semester\\SPO\\Test");
    getListOfFilesAndDir(path);
    inputClient(path);
    return 0;
}
