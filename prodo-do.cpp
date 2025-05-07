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

bool checkingInput1and0(int choice, int correct[]){
    return true;
}


int main(){
    int mainMenuChoice;

    do{
        cout << "~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        // change it to display by default and asking to create a new one if no is there
        cout << "Do you want to enter a new task or see already existing ones?" << endl;
        cout << "Enter 0 for no, 1 for yes: ";
        cin >> mainMenuChoice;
    }
    while (mainMenuChoice!=0);
}