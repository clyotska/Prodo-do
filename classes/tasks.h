#include <iostream>
#include <string>
#include <limits>
#include <optional>
#include <fstream>
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
    string header;
    optional<string> body;
    time_t dateCreated;
    optional<string> dueDate;
    int priority;

public:
    Task(string h = "Unknown", optional<string> b = nullopt, time_t dc = 0,
        optional<string> dd = nullopt, int p = 1) : header(h), body(b), dateCreated(dc), dueDate(dd), priority(p) {};

    void setData()
    {
        string temp;
        cout << "Enter the task: ";
        getline(cin, header);

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
                if (!temp.empty())
                {
                    if (temp.length() == 10)
                    {
                        string day = temp.substr(0, 2);
                        string month = temp.substr(3, 2);
                        string year = temp.substr(6, 4);
                        if (temp[2] == '.' && temp[5] == '.' && stoi(day) >= 1 && stoi(day) <= 31 && stoi(month) >= 1 && stoi(month) <= 12 && stoi(year) >= lT->tm_year + 1900 && stoi(month) >= lT->tm_mon && stoi(day) >= lT->tm_mday)
                        {
                            input_success1 = true;
                            dueDate = temp;
                        }
                        else
                        {
                            cout << "Invalid input. Please enter the actual date" << endl;
                            cin.clear();
                        }
                    }
                    else
                    {
                        cout << "Invalid input. Please enter the actual date" << endl;
                        cin.clear();
                    }
                }
                else
                {
                    dueDate = nullopt;
                    input_success1 = true;
                }
            }
        }

        // checking the choice of description
        int description_choice;
        bool input_success2 = false;
        while (!input_success2)
        {
            cout << "Do you want to add additional description?" << endl
                << "1 for yes, 0 for no: ";
            cin >> description_choice;
            if (cin.good())
            { // Check if the input operation was successful
                if (description_choice == 0 || description_choice == 1)
                {
                    input_success2 = true;
                    switch (description_choice)
                    {
                    case 1:
                    {
                        cin.ignore();
                        cout << "Enter description: " << endl;
                        string tempBody;
                        getline(cin, tempBody);
                        body = tempBody;
                        break;
                    }
                    case 0:
                        cout << "Great! We continue with no optional description then!" << endl;
                        cin.ignore();
                        break;
                    default:
                        cerr << "Error: There will be dragons" << endl;
                    }
                }
            }
            else
            {
                cout << "Invalid input. Please EITHER enter 1 for yes or 0 for no" << endl;
                cin.clear(); // Clear the error flags (like failbit)
                // Discard the invalid input from the buffer
            }
        }

        // checking priority
        bool input_success3 = false;
        while (!input_success3)
        {
            cout << "Enter priority for your task from 1 to 4: ";
            cin >> priority;
            if (cin.good())
            {
                if (priority >= 1 && priority <= 4)
                {
                    input_success3 = true;
                }
                else
                {
                    cout << "Invalid input. Please enter a number from 1 to 4 ONLY" << endl;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
            else
            {
                cout << "Invalid input. Please enter a number from 1 to 4 ONLY" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
    }

    void Display() const
    {
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
        cout << "Priority: " << priority << endl;
    }

    string getHeader() const
    {
        return header;
    }

    time_t getDate() const
    {
        return dateCreated;
    }
    int getPrior() const
    {
        return priority;
    }

    optional<string> getBody() const
    {
        if (body.has_value())
        {
            return body.value();
        }
        return nullopt;
    }

    void serialize(ofstream &ofs) const
    {
        if (!ofs.is_open() || ofs.fail())
        {
            cerr << "Output file is bad" << endl;
            return;
        }

        size_t header_len = header.size();
        ofs.write((char *)&header_len, sizeof(header_len));
        ofs.write(header.c_str(), header_len);

        bool bodyHasValue = body.has_value();
        ofs.write((char *)&bodyHasValue, sizeof(bodyHasValue));
        if (bodyHasValue)
        {
            size_t body_len = body.value().size();
            ofs.write((char *)&body_len, sizeof(body_len));
            ofs.write(body.value().c_str(), body_len);
        }

        ofs.write((char *)&dateCreated, sizeof(dateCreated));

        bool dueDateHasValue = dueDate.has_value();
        ofs.write((char *)&dueDateHasValue, sizeof(dueDateHasValue));
        if (dueDateHasValue)
        {
            size_t dueDate_len = dueDate.value().size();
            ofs.write((char *)&dueDate_len, sizeof(dueDate_len));
            ofs.write(dueDate.value().c_str(), dueDate_len);
        }

        ofs.write((char *)&priority, sizeof(priority));

        if (ofs.fail())
        {
            cerr << "Something went wrong with serialization" << endl;
        }
    }

    bool deserialize(ifstream &ifs)
    {
        if (!ifs.is_open() || ifs.fail())
        {
            cerr << "Input file is bad" << endl;
            return false; // Indicate failure
        }

        bool oki = true;

        size_t header_len = 0;
        if (ifs.read((char *)&header_len, sizeof(header_len)))
        {
            header.resize(header_len);
            if (!ifs.read(&header[0], header_len))
            {
                cerr << "Couldn't read header data" << endl;
                oki = false;
            }
            int getPrior(){
                return priority;
            }
};  