#include <stdio.h>
#include <Windows.h>


#include "Akinator.h"


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    FILE* file = fopen("D:\\���� C\\Akinator\\tree1.txt", "r");

    if (file)
    {
        Akinator akinator = AkinatorConstructor(file);

        AkinatorDestructor(&akinator);
    }
    else
        puts("������ �������� �����");

    return 0;
}