#include <iostream>
#include <string>
#include <limits>
#include <optional>
#include "classes/tasks.h"
#include <fstream>

using namespace std;

// Date: 07.05.2025

/*
Authors:
Alina Gaybdjanova (U2410084)
Ilya Kolesnikov (U2410113)
Amir Madjidov (U2410125)
Jahongir Davronov (U2410066)
*/

bool checkingInput1and0(int choice, int correct[])
{
    return true;
}

bool is_empty(std::ifstream &pFile)
{
    return pFile.peek() == std::ifstream::traits_type::eof();
}

void writeFile()
{
    Task t;
    ofstream fout("files/tasks.dat", ios::binary | ios::app);
    if (!fout.is_open())
    {
        cerr << "Error: Couldn't open 'tasks.dat' for writing" << endl;
        return;
    }
    t.setData();
    // add serialization
    fout.close();
}

void deleting() {}

void editing() {}

void display() {}

int main()
{
    int mainMenuChoice;

    do
    {
        cout << "~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        // change it to display by default and asking to create a new one if no is there
        cout << "Hi! I'm your task manager." << endl;
        // check whether task file is empty or not, if empty prompt user to create his first task
        display();
    } while (mainMenuChoice != 0);
}