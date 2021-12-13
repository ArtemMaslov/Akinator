#include <stdio.h>
#include <Windows.h>


#include "Akinator.h"
#include "Voice.h"


int main(int argc, char* argv[])
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    FILE* file = nullptr;
    
    if (argc > 1)
    {
        file = fopen(argv[1], "r");
    }
    else
        file = fopen("tree.txt", "r");

    if (!file)
    {
        puts("Ошибка открытия файла");
        return 0;
    }

    Akinator akinator = {};
    
    AkinatorConstructor(file, &akinator);
    
    fclose(file);
        
    ShowMenu(&akinator);

    AkinatorDestructor(&akinator);
         
    return 0;
}