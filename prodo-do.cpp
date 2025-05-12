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
    t.serialize(fout);
    fout.close();
}

void readFile(){
    Task t;
    ifstream ifs("files/tasks.dat", ios::binary);
    while (ifs.peek() != EOF){
        t.deserialize(ifs);
        t.Display();
    }
}

void adding(){
    writeFile();
}

bool isEmpty()
{   
    ifstream ifs("files/tasks.dat", ios::binary);
    return ifs.peek() == EOF;
}

void markAsComplete(){}

void deleting() {}

void editing() {}

void display() {
    cout << "---------- All your tasks: ----------" << endl;
    readFile();
}

int main()
{
    int mainMenuChoice;

    do
    {
        cout << "----------------------" << endl;
        // change it to display by default and asking to create a new one if no is there
        if (isEmpty()){
            cout << "Let's create your FIRST task!" << endl;
            adding();
            display();
        }
        else{
            display();
        }
        cout << "----------------------" << endl;
        cout << "1. Add Task" << endl;
        cout << "2. Mark Task as Complete" << endl;
        cout << "3. Edit Task" << endl;
        cout << "4. Delete Task" << endl;
        cout << "To QUIT enter 6: "; cin >> mainMenuChoice; cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "----------------------" << endl;
        // add checker for main menu
        // add marking complete [+] and incomplete [-] before tasks
        // add editing and deleting
        // add smth to inherit
        switch (mainMenuChoice)
        {
        case 1: adding(); break;
        case 2: markAsComplete(); break;
        case 3: editing(); break;
        case 4: deleting(); break;
        case 6: cout << "Quitting the program \n"; break;
        default:
            cout << "Wrong choice! Please, try again!" << endl; break;
        }
    } while (mainMenuChoice != 6);
}