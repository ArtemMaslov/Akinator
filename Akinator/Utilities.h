#ifndef UTILITIES_H_
#define UTILITIES_H_


enum ConsoleColors
{
    BLACK = 0,
    DARK_BLUE = 1,
    DARK_GREEN = 2,
    DARK_CYAN = 3,
    DARK_RED = 4,
    DARK_VIOLET = 5,
    DARK_YELLOW = 6,
    DARK_WHITE = 7,
    GREY = 8,
    BLUE = 9,
    GREEN = 10,
    CYAN = 11,
    RED = 12,
    VIOLET = 13,
    YELLOW = 14,
    WHITE = 15
};


void SetConsoleColor(const int text, const int background);


#endif