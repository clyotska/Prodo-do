#include <iostream>
#include <string>
// used for cin.ignore()
#include <limits>
// used for some parameters
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

// first version of empty checker for a file
bool is_empty(std::ifstream &pFile)
{
    return pFile.peek() == std::ifstream::traits_type::eof();
}

// function to add a whole new task to a file
void writeFile()
{
    Task t;
    ofstream fout("files/tasks.dat", ios::binary | ios::app);
    // error checker
    if (!fout.is_open())
    {
        cerr << "Error: Couldn't open 'tasks.dat' for writing" << endl;
        return;
    }
    t.setData();
    t.serialize(fout);
    fout.close();
}

// function to read from file
void readFile()
{
    Task t;
    ifstream ifs("files/tasks.dat", ios::binary);
    while (ifs.peek() != EOF) // reading till we reach the end of file
    {
        t.deserialize(ifs);
        t.Display();
    }
}

void adding() // i (Alina) wanted to add an ability to create a task in specific place, but later we gave up on this idea
{
    writeFile();
}

bool isEmpty() // another version of empty checker, now we don't need parameters
// file to check is already specified in the function
{
    ifstream ifs("files/tasks.dat", ios::binary);
    return ifs.peek() == EOF;
}

// a function to read one specific task from the file
bool readTask(ifstream &ifs, Task &task)
{
    if (ifs.peek() == EOF)
    {
        return false;
    }
    return task.deserialize(ifs);
}

// a function to write one specific task to the file
void writeTask(ofstream &ofs, const Task &task)
{
    task.serialize(ofs);
}

// a function to mark as complete
void markTaskAsComplete()
{
    string taskHeader; // we will find a task by header
    cout << "Enter the header of the task you want to mark as complete: ";

    getline(cin, taskHeader);

    // here I (Alina) open files to write and read to and from
    ifstream fin("files/tasks.dat", ios::binary);
    ofstream fout_temp("files/temp.dat", ios::binary);

    // checking whether they both are open
    if (!fin.is_open())
    {
        cerr << "Couldn't open 'files/tasks.dat' to read" << endl;
        return;
    }
    if (!fout_temp.is_open())
    {
        cerr << "Couldn't open 'files/temp.dat' to write" << endl;
        fin.close();
        return;
    }

    // a flag to check whether task is found
    bool here = false;
    Task t;

    // Read tasks one by one from the original file
    while (readTask(fin, t))
    {
        if (t.getHeader() == taskHeader)
        { // if the task was found we mark it as complete
            t.markAsComplete();
            here = true; // and flag is changed too
            writeTask(fout_temp, t);
        }
        else
        { // if it's not the task we need, we still write it to a file, just without changing the parameter
            writeTask(fout_temp, t);
        }
    }

    // saving and closing the files
    fin.close();
    fout_temp.close();

    if (!here)
    { // if no such task was found
        cout << "No such task was found" << endl;
        // we delete temp too
        remove("files/temp.dat");
    }
    else
    {
        // replace the original file with the temporary file and consequently check the removing operation
        if (remove("files/tasks.dat") != 0)
        {
            cerr << "Couldn't delete original file" << endl;
        }
        // rename the file and check whether the operation was successful
        if (rename("files/temp.dat", "files/tasks.dat") != 0)
        {
            cerr << "Couldn't rename temporary file" << endl;
        }
        cout << "Changes saved to file" << endl;
    }
}

// a function to implement sorting of tasks by completion
void sortingComplete()
{

    // Use ios::trunc to ensure temporary files are empty at the start
    ofstream foutC("files/taskComplete.dat", ios::binary | ios::trunc);
    ofstream foutI("files/tasksIncomplete.dat", ios::binary | ios::trunc);
    ifstream fin("files/tasks.dat", ios::binary);

    // Check if all necessary files opened successfully
    if (!fin.is_open() || !foutC.is_open() || !foutI.is_open())
    {
        cerr << "Error: Couldn't open necessary files for sorting." << endl;
        // Clean up any files that might have been opened
        if (fin.is_open())
            fin.close();
        if (foutC.is_open())
            foutC.close();
        if (foutI.is_open())
            foutI.close();
        // Remove temporary files in case of error
        remove("files/taskComplete.dat");
        remove("files/tasksIncomplete.dat");
        return;
    }

    Task t;
    // Read from the main file and write to the appropriate temporary file
    while (readTask(fin, t))
    {
        if (t.getCompletionStatus() == true)
        {
            writeTask(foutC, t);
            if (foutC.fail())
            { // Check for write errors
                cerr << "Error writing to taskComplete.dat during sorting." << endl;
                break; // Exit loop on error
            }
        }
        else
        {
            writeTask(foutI, t);
            if (foutI.fail())
            { // Check for write errors
                cerr << "Error writing to tasksIncomplete.dat during sorting." << endl;
                break; // Exit loop on error
            }
        }
    }

    // Check if the read loop finished due to an error other than EOF
    if (fin.fail() && !fin.eof())
    {
        cerr << "Error reading from tasks.dat during sorting." << endl;
    }

    fin.close();
    foutC.close();
    foutI.close();

    // Now, merge the temporary files into a new sorted file
    ofstream foutSorted("files/tasksSorted.dat", ios::binary | ios::trunc);
    ifstream finI("files/tasksIncomplete.dat", ios::binary);
    ifstream finC("files/taskComplete.dat", ios::binary);

    // Check if files for merging opened successfully
    if (!foutSorted.is_open() || !finI.is_open() || !finC.is_open())
    {
        cerr << "Error: Couldn't open files for merging during sorting." << endl;
        // Clean up files
        if (foutSorted.is_open())
            foutSorted.close();
        if (finI.is_open())
            finI.close();
        if (finC.is_open())
            finC.close();
        // Remove temporary files
        remove("files/taskComplete.dat");
        remove("files/tasksIncomplete.dat");
        remove("files/tasksSorted.dat");
        return;
    }

    // Copy incomplete tasks first
    Task tempTask;
    while (readTask(finI, tempTask))
    {
        writeTask(foutSorted, tempTask);
        if (foutSorted.fail())
        { // Check for write errors
            cerr << "Error writing incomplete task to tasksSorted.dat." << endl;
            break; // Exit loop on error
        }
    }
    if (finI.fail() && !finI.eof())
    {
        cerr << "Error reading from tasksIncomplete.dat during merge." << endl;
    }

    // Then copy complete tasks
    while (readTask(finC, tempTask))
    {
        writeTask(foutSorted, tempTask);
        if (foutSorted.fail())
        { // Check for write errors
            cerr << "Error writing complete task to tasksSorted.dat." << endl;
            break; // Exit loop on error
        }
    }
    if (finC.fail() && !finC.eof())
    {
        cerr << "Error reading from taskComplete.dat during merge." << endl;
    }

    finI.close();
    finC.close();
    foutSorted.close();

    // Clean up the intermediate temporary files
    remove("files/taskComplete.dat");
    remove("files/tasksIncomplete.dat");

    // Replace the original file with the sorted file
    if (remove("files/tasks.dat") != 0)
    {
        cerr << "Error deleting original file during sorting." << endl;
        // Handle error: The original file might still exist, and the sorted temp file too.
    }
    if (rename("files/tasksSorted.dat", "files/tasks.dat") != 0)
    {
        cerr << "Error renaming sorted temporary file." << endl;
         // Handle error: The sorted temp file might still exist.
    }
    cout << "Tasks sorted by completion status." << endl;
}

/*
Ilya zaraza dobav' komenty k svoemu kodu sam. Ya ustala i ne obyazana. !!!!!
*/
void deleting() {
    Task t;
    string temp;
    readFile();
    cout << "Enter the task's header you want to delete: "; getline(cin, temp);
    ofstream tempf("files/temp.dat", ios::binary);
    ifstream ifs("files/tasks.dat", ios::binary);
    while(t.deserialize(ifs)){
        if(t.getHeader() != temp){
            t.serialize(tempf);
        }
    }
    ifs.close();
    tempf.close();
    remove("files/tasks.dat");
    rename("files/temp.dat", "files/tasks.dat");
    sortingComplete();
}

void editing() {
    Task t;
    Task task;
    string temp;
    cout << "Enter the task's header: "; getline(cin, temp);
    ifstream ifs("files/tasks.dat", ios::binary);
    ofstream tempf("files/temp.dat", ios::binary);
    while(t.deserialize(ifs)){
        if(t.getHeader() != temp){
            t.serialize(tempf);
            task.setData();
            task.serialize(tempf);
        }
    }
    ifs.close();
    tempf.close();
    remove("files/tasks.dat");
    rename("files/temp.dat", "files/tasks.dat");
    sortingComplete();
    

}

// a display function, where I (Alina) included sorting and used readFile
void display()
{
    cout << "---------- All your tasks: ----------" << endl;
    sortingComplete();
    readFile();
}

int main()
{
    int mainMenuChoice;

    do
    {
        cout << "----------------------" << endl;
        // change it to display by default and asking to create a new one if no is there
        if (isEmpty()) // if file is empty we jump to creation
        {
            cout << "Let's create your FIRST task!" << endl;
            adding();
            display();
        }
        else // if file is not empty, program displays the tasks
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

        while (!(cin >> mainMenuChoice) || mainMenuChoice < 1 || mainMenuChoice > 6) // we check the range and whether input is in good condition
        {
            // Check if the failure was due to non-integer input
            if (cin.fail())
            {
                cout << "WRONG INPUT. Please enter a number (from 1 to 6): ";
                cin.clear();                                         // Clear the error flags (e.g., failbit)
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard the invalid input
            }
            else
            {
                // Input was an integer, but outside the range 1-7
                cout << "WRONG NUMBER. Please enter a number (from 1 to 6): ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard the rest of the line
            }
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // add checker for main menu
        // add editing and deleting
        // add smth to inherit

        switch (mainMenuChoice) // a switch for menu options
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