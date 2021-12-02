#include <stdio.h>
#include <Windows.h>


void SetConsoleColor(const int text, const int background)
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, ((background & 0xf) << 4) | (text & 0xf));
}