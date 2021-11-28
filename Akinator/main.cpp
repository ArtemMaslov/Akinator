#include <stdio.h>
#include <Windows.h>


#include "Akinator.h"


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    FILE* file = fopen("D:\\язык C\\Akinator\\tree1.txt", "r");

    if (!file)
    {
        puts("ќшибка открыти€ файла");
        return 0;
    }

    Akinator akinator = AkinatorConstructor(file);
        
    ShowMenu(&akinator);

    AkinatorDestructor(&akinator);

    return 0;
}