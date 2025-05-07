#include <iostream>
#include <string>
#include <limits>
#include <optional>
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

class Task{
    protected:
    // misc: header is for the actual task, body is for optional description, like in Habitica App (screenshot well be added)
    // dataCreated & dueDate should probably be initialized using date library
    // priority from 1 to 4, 1 - lowest, 4 - urgent. 1 is default
        string header; optional<string> body;
        optional<long int> dateCreated, dueDate;
        int priority;

    public:
        Task(string h="Unknown", string b="", optional<long int> dc=nullopt, optional<long int> dd=nullopt, int p=1):header(h),body(b), dateCreated(dc), dueDate(dd), priority(p){};

        void setData(){
            cout << "Enter the task: ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            getline(cin, header);
            int description_choice;
            cout << "Do you want to add additional description?" << endl << "1 for yes, 0 for no: ";
            cin >> description_choice;
            bool input_success = false; // Flag to check if input was successful


            // Loop for input validation
            while (!input_success) {
                cout << "Do you want to add additional description?" << endl << "1 for yes, 0 for no: ";
                cin >> description_choice;

                if (cin.good()) { // Check if the input operation was successful
                    if (description_choice == 0 || description_choice == 1){
                        input_success = true;
                    }
                } else {
                    cout << "Invalid input. Please EITHER enter 1 for yes or 0 for no" << endl;
                    cin.clear(); // Clear the error flags (like failbit)
                    // Discard the invalid input from the buffer
                }
            }

            switch (description_choice)
            {
            case 1:
            {
                cout << "Enter description: " << endl;
                string tempBody;
                getline(cin, tempBody);
                body = tempBody;
                break;
            }
            case 0:
                cout << "Great! We continue with no optional description then!" << endl;
                break;
            default:
                cerr << "Error: There will be dragons" << endl;
                break;
            }

        }
};