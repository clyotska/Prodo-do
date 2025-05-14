#include <iostream>
#include <string>
// limits used for cin.ignore() only
#include <limits>
// used for some parameters
#include <optional>
#include <fstream>
// used for date created
#include <ctime>

using namespace std;

// Date: 07.05.2025

/*
Authors:
Alina Gaybdjanova (U2410084)
Ilya Kolesnikov (U2410113)
Amir Madjidov (U2410125)
Jahongir Davronov (U2410066)
*/

class Task
{

protected:
    // all the protected members
    string header;
    optional<string> body;
    time_t dateCreated;
    optional<string> dueDate;
    bool complete;
    int priority;

public:
    // Constructor, which we don't even use :)
    Task(string h = "Unknown", optional<string> b = nullopt, time_t dc = 0,
        optional<string> dd = nullopt, bool c = false, int p = 1) : header(h), body(b), dateCreated(dc), dueDate(dd), complete(c), priority(p) {};

    void setData()
    {
        // gigantic ahh function of setting all the data
        // btw this due date was written by Ilya
        // Alina (me) wrote all the comments!!
        string temp;
        cout << "Enter the task: ";
        getline(cin, header);

        // proper initialization of dateCreated using ctime
        // basically just takes date from your device
        time(&dateCreated);
        time_t t = time(nullptr);
        tm *lT = localtime(&t);

        // checking due date
        bool input_success1 = false;
        while (!input_success1)
        {
            cout << "Enter due date(DD.MM.YYYY) or leave this field empty: ";
            getline(cin, temp);
            if (cin.good())
            {
                if (!temp.empty()) // if user decided to add due date
                {
                    if (temp.length() == 10) // if format of the date is correct
                    {
                        string day = temp.substr(0, 2);   // here ilya finds day
                        string month = temp.substr(3, 2); // here ilya finds month
                        string year = temp.substr(6, 4);  // here ilya finds year

                        // below Ilya checks whether due date is correct
                        // basically the range of day, month and year and that due date is not in the past
                        if (temp[2] == '.' && temp[5] == '.' && stoi(day) >= 1 && stoi(day) <= 31 && stoi(month) >= 1 && stoi(month) <= 12 && stoi(year) >= lT->tm_year + 1900 && stoi(month) >= lT->tm_mon && stoi(day) >= lT->tm_mday)
                        {
                            input_success1 = true;
                            dueDate = temp;
                        }
                        else // if user is a little deranged (little dummy dumbo) we try again :3
                        {
                            cout << "Invalid input. Please enter the actual date" << endl;
                            cin.clear();
                        }
                    }
                    else // if user is wrong with format :(
                    {
                        cout << "Invalid input. Please enter the actual date" << endl;
                        cin.clear();
                    }
                }
                else // if user decided not to enter due Date
                {
                    dueDate = nullopt; // nullopt if from optional library, shows the compiler that nothing is here
                    input_success1 = true;
                }
            }
        }

        // checking the choice of description
        int description_choice;
        bool input_success2 = false;
        cout << "Enter 1 to add description and 0 to skip this step" << endl;
        while (!(cin >> description_choice) || description_choice > 1 || description_choice < 0) // we check the range and whether input is in good condition
        {
            // Check if the failure was due to non-integer input
            if (cin.fail())
            {
                cout << "WRONG INPUT. Please enter a number (1 or 0): ";
                cin.clear();                                         // Clear the error flags (e.g., failbit)
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard the invalid input
            }
            else
            {
                // Input was an integer, but outside the range 1-7
                cout << "WRONG NUMBER. Please enter a number (1 or 0): ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard the rest of the line
            }
        }
        switch (description_choice) // switch for adding/not adding description
        {
        case 1: // user decided to enter description
        {       // braces are here, bc it turns out I can't create a variable in not a new scope
            cin.ignore();
            cout << "Enter description: " << endl;
            string tempBody;
            getline(cin, tempBody);
            body = tempBody; // now the entered by user description is the task's description
            break;
        }
        case 0:
            cout << "Great! We continue with no optional description then!" << endl;
            cin.ignore();
            break;
        default:
            // Something went wrong and we don't know why
            cerr << "Error: There will be dragons" << endl;
        }

        // checking priority
        bool input_success3 = false;
        while (!input_success3) // here I (Alina) used a different approach to check the input
        {
            cout << "Enter priority for your task from 1 to 4: ";
            cin >> priority;
            if (cin.good()) // checking whether its integer
            {
                if (priority >= 1 && priority <= 4) // checking whether integer in range
                {
                    input_success3 = true;
                }
                else // wrong range
                {
                    cout << "Invalid input. Please enter a number from 1 to 4 ONLY" << endl;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
            else // wrong input all together
            {
                cout << "Invalid input. Please enter a number from 1 to 4 ONLY" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
    }

    void markAsComplete()
    { // we mark a task as complete
        complete = true;
    }

    bool getCompletionStatus() const
    { // const to show that this code doesn't change task parameters
        // getter for completetion status
        return complete;
    }

    void Display() const // const to show that this code doesn't change task parameters
    {
        // display function
        if (complete)
        {
            cout << "[+] ";
        }
        else
        {
            cout << "[-] ";
        }
        cout << "Task: " << header << endl;
        cout << "Date of creation: " << ctime(&dateCreated);
        if (dueDate.has_value())
        {
            cout << "Due date: " << dueDate.value() << endl;
        }
        if (body.has_value())
        {
            cout << body.value() << endl;
        }
        cout << "Priority: " << priority << endl
            << "-----------------" << endl;
    }

    string getHeader() const // another getter
    {
        return header;
    }

    time_t getDate() const // another getter
    {
        return dateCreated;
    }
    int getPrior() const // another getter
    {
        return priority;
    }

    optional<string> getBody() const // another getter
    {
        if (body.has_value())
        {
            return body.value();
        }
        return nullopt;
    }

    void serialize(ofstream &ofs) const // function to properly write all the sizes (bc we have strings, and their size can vary)
    {
        if (!ofs.is_open() || ofs.fail()) // we check whether the file is open and everything is oki
        {
            cerr << "Output file is bad" << endl;
            return;
        }

        // we write the size of header to a file
        size_t header_len = header.size();
        ofs.write((char *)&header_len, sizeof(header_len));
        ofs.write(header.c_str(), header_len);

        // we check whether description has value and then write its size to a file
        bool bodyHasValue = body.has_value();
        ofs.write((char *)&bodyHasValue, sizeof(bodyHasValue));
        if (bodyHasValue)
        {
            size_t body_len = body.value().size();
            ofs.write((char *)&body_len, sizeof(body_len));
            ofs.write(body.value().c_str(), body_len);
        }

        // we write the size of dateCreated to a file
        ofs.write((char *)&dateCreated, sizeof(dateCreated));

        // we check whether dueDate has value then write its size to a file
        bool dueDateHasValue = dueDate.has_value();
        ofs.write((char *)&dueDateHasValue, sizeof(dueDateHasValue));
        if (dueDateHasValue)
        {
            size_t dueDate_len = dueDate.value().size();
            ofs.write((char *)&dueDate_len, sizeof(dueDate_len));
            ofs.write(dueDate.value().c_str(), dueDate_len);
        }

        // we write the size of complete to a file
        ofs.write((char *)&complete, sizeof(complete));

        // we write the size of priority to a file
        ofs.write((char *)&priority, sizeof(priority));

        if (ofs.fail()) // smth went wrong with serialization
        {
            cerr << "Something went wrong with serialization" << endl;
        }
    }

    bool deserialize(ifstream &ifs) // a function to read a task and its members properly
    // after each reading of every parameter we check whether everything is oki
    {
        if (!ifs.is_open() || ifs.fail()) // we check whether the file for reading is open and everything is good
        {
            cerr << "Input file is bad" << endl;
            return false; // Indicate failure
        }

        bool oki = true;

        size_t header_len = 0;
        // we read the size of header
        if (ifs.read((char *)&header_len, sizeof(header_len)))
        {
            // we resize the length of header to fit the header we read from file
            header.resize(header_len);
            if (!ifs.read(&header[0], header_len))
            {
                cerr << "Couldn't read header data" << endl;
                oki = false;
            }
        }
        else
        {
            cerr << "Couldn't read header length" << endl;
            oki = false;
        }

        if (!oki)
            return false;

        bool body_has_value = false;
        // we read the size of description if its has value
        if (ifs.read((char *)&body_has_value, sizeof(body_has_value)))
        {
            if (body_has_value)
            {
                size_t body_len = 0;
                if (ifs.read((char *)&body_len, sizeof(body_len)))
                {
                    //
                    string temp_body;
                    temp_body.resize(body_len);
                    if (ifs.read(&temp_body[0], body_len))
                    {
                        body = temp_body;
                    }
                    else
                    {
                        cerr << "Couldn't read body data" << endl;
                        oki = false;
                    }
                }
                else
                {
                    cerr << "Couldn't read body length" << endl;
                    oki = false;
                }
            }
            else
            {
                body = nullopt;
            }
        }
        else
        {
            cerr << "Something went wrong with body" << endl;
            oki = false;
        }

        if (!oki)
            return false;

        if (!ifs.read((char *)&dateCreated, sizeof(dateCreated)))
        {
            cerr << "Couldn't read creation date" << endl;
            oki = false;
        }

        if (!oki)
            return false;

        // we read the size of due Date if its has value
        bool dueDate_has_value = false;
        if (ifs.read((char *)&dueDate_has_value, sizeof(dueDate_has_value)))
        {
            if (dueDate_has_value)
            {
                size_t dueDate_len = 0;
                if (ifs.read((char *)&dueDate_len, sizeof(dueDate_len)))
                {
                    string temp_dueDate;
                    temp_dueDate.resize(dueDate_len);
                    if (ifs.read(&temp_dueDate[0], dueDate_len))
                    {
                        dueDate = temp_dueDate;
                    }
                    else
                    {
                        cerr << "Couldn't read due date data" << endl;
                        oki = false;
                    }
                }
                else
                {
                    cerr << "Couldn't read due date length" << endl;
                    oki = false;
                }
            }
            else
            {
                dueDate = nullopt;
            }
        }
        else
        {
            cerr << "Something went wrong with due date" << endl;
            oki = false;
        }

        // we read the size of completion status
        if (!ifs.read((char *)&complete, sizeof(complete)))
        {
            cerr << "Couldn't read completion status" << endl;
            oki = false;
        }

        if (!oki)
            return false;

        // we read the size of priority
        if (!ifs.read((char *)&priority, sizeof(priority)))
        {
            cerr << "Couldn't read priority" << endl;
            oki = false;
        }

        return oki;
    }
};