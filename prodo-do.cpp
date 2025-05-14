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

void readFile()
{
    Task t;
    ifstream ifs("files/tasks.dat", ios::binary);
    while (ifs.peek() != EOF)
    {
        t.deserialize(ifs);
        t.Display();
    }
}

void adding()
{
    writeFile();
}

bool isEmpty()
{
    ifstream ifs("files/tasks.dat", ios::binary);
    return ifs.peek() == EOF;
}

bool readTask(ifstream& ifs, Task& task) {
    if (ifs.peek() == EOF) {
        return false;
    }
    return task.deserialize(ifs);
}

void writeTask(ofstream& ofs, const Task& task) {
    task.serialize(ofs);
}


void markTaskAsComplete(){
    string taskHeader;
    cout << "Enter the header of the task you want to mark as complete: ";

    getline(cin, taskHeader);

    ifstream fin("files/tasks.dat", ios::binary);
    ofstream fout_temp("files/temp.dat", ios::binary);

    if (!fin.is_open()) {
        cerr << "Couldn't open 'files/tasks.dat' to read" << endl;
        return;
    }
    if (!fout_temp.is_open()) {
        cerr << "Couldn't open 'files/temp.dat' to write" << endl;
        fin.close();
        return;
    }

    bool here = false;
    Task t;

    // Read tasks one by one from the original file
    while (readTask(fin, t)) {
        if (t.getHeader() == taskHeader) {
            t.markAsComplete();
            here = true;
            writeTask(fout_temp, t);
        } else {
            writeTask(fout_temp, t);
        }
    }

    fin.close();
    fout_temp.close();

    if (!here) {
        cout << "No such task was found" << endl;
        remove("files/temp.dat");
    } else {
        // Replace the original file with the temporary file
        if (remove("files/tasks.dat") != 0) {
            cerr << "Couldn't delete original file" << endl;
        }
        if (rename("files/temp.dat", "files/tasks.dat") != 0) {
            cerr << "Couldn't rename temporary file" << endl;
        }
        cout << "Changes saved to file" << endl;
    }
}

void sortingComplete(){
    Task t;
    ofstream foutC("files/taskComplete.dat", ios::binary | ios::trunc);
    ofstream foutI("files/tasksIncomplete.dat", ios::binary | ios::trunc);
    ifstream fin("files/tasks.dat", ios::binary);

    if (!fin.is_open() || !foutC.is_open() || !foutI.is_open()) {
        cerr << "Error: Couldn't open necessary files for sorting." << endl;
        // Clean up any files that might have been opened
        if (fin.is_open()) fin.close();
        if (foutC.is_open()) foutC.close();
        if (foutI.is_open()) foutI.close();
        // Remove temporary files in case of error
        remove("files/taskComplete.dat");
        remove("files/tasksIncomplete.dat");
        return;
    }


    while(readTask(fin, t))
    {
        if (t.getCompletionStatus() == true){
            writeTask(foutC, t);
            if (foutC.fail()) { // Check for write errors
                cerr << "Error writing to taskComplete.dat during sorting." << endl;
                break; // Exit loop on error
            }
        }
        }
        else{
            writeTask(foutI, t);
            if (foutI.fail()) { // Check for write errors
                cerr << "Error writing to tasksIncomplete.dat during sorting." << endl;
                break; // Exit loop on error
            }
        }
    }

    if (fin.fail() && !fin.eof()) {
        cerr << "Error reading from tasks.dat during sorting." << endl;
    }

    fin.close();
    foutC.close();
    foutI.close();

    ifstream finC("files/taskComplete.dat", ios::binary);
    ifstream finI("files/tasksIncomplete.dat", ios::binary);

    ofstream foutTemp("files/tasksSorted.dat", ios::binary | ios::trunc);

    if (!foutSorted.is_open() || !finI.is_open() || !finC.is_open()) {
        cerr << "Error: Couldn't open files for merging during sorting." << endl;
         // Clean up files
        if (foutSorted.is_open()) foutSorted.close();
        if (finI.is_open()) finI.close();
        if (finC.is_open()) finC.close();
        // Remove temporary files
        remove("files/taskComplete.dat");
        remove("files/tasksIncomplete.dat");
        remove("files/tasksSorted.dat");
        return;
    
    Task TempTask;
    while(readTask(finI, TempTask)){
        writeTask(foutTemp, TempTask);
        if (foutTemp.fail()) { // Check for write errors
            cerr << "Error writing incomplete task to tasksSorted.dat." << endl;
            break; // Exit loop on error
    }
    if (finI.fail() && !finI.eof()) {
        cerr << "Error reading from tasksIncomplete.dat during merge." << endl;
    }

    while(readTask(finC, TempTask)){
        writeTask(foutTemp, TempTask);
        if (foutTemp.fail()) { // Check for write errors
            cerr << "Error writing incomplete task to tasksSorted.dat." << endl;
            break; // Exit loop on error
    }
    if (finC.fail() && !finC.eof()) {
        cerr << "Error reading from tasksComplete.dat during merge." << endl;
    }

    finI.close();
    finC.close();
    foutTemp.close();

    remove("files/taskComplete.dat");
    remove("files/tasksIncomplete.dat");


    // Replace the original file with the sorted file
    if (remove("files/tasks.dat") != 0) {
        cerr << "Error deleting original file during sorting." << endl;
         // Handle error: The original file might still exist, and the sorted temp file too.
    }
    if (rename("files/tasksTemp.dat", "files/tasks.dat") != 0) {
        cerr << "Error renaming sorted temporary file." << endl;
         // Handle error: The sorted temp file might still exist.
    }
    cout << "Tasks sorted by completion status." << endl;
}

void deleting() {}

void editing() {}

void display()
{
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
        if (isEmpty())
        {
            cout << "Let's create your FIRST task!" << endl;
            adding();
            display();
        }
        else
        {
            display();
        }
        cout << "----------------------" << endl;
        cout << "1. Add Task" << endl;
        cout << "2. Mark Task as Complete" << endl;
        cout << "3. Edit Task" << endl;
        cout << "4. Delete Task" << endl;
        cout << "5. Display Tasks" << endl;
        cout << "To QUIT enter 6: ";
        cin >> mainMenuChoice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "----------------------" << endl;

        // add sorting from complete to incomplete
        // add checker for main menu
        // add editing and deleting
        // add smth to inherit
        
        switch (mainMenuChoice)
        {
        case 1:
            adding();
            break;
        case 2:
            markTaskAsComplete();
            break;
        case 3:
            editing();
            break;
        case 4:
            deleting();
            break;
        case 5:
            display();
            break;
        case 6:
            cout << "Quitting the program \n";
            break;
        default:
            cout << "Wrong choice! Please, try again!" << endl;
            break;
        }
    } while (mainMenuChoice != 6);
}