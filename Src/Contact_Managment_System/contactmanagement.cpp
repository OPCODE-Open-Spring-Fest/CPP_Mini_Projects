#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;
class ContactManager {
private:
    string phoneNo, name, address, description;
    fstream file;

public:
    void menu();
    void addContact();
    void showAll();
    void searchContact();
};
// Function to display menu 
void ContactManager::menu() {
    char choice;
    do {
        system("cls");
        cout << "   # CONTACT MANAGEMENT APP   \n";
        cout << "________________________________\n";
        cout << "1 → Add Contact\n";
        cout << "2 → Show All Contacts\n";
        cout << "3 → Search Contact\n";
        cout << "0 → Exit\n";
        cout << "________________________________\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case '1':
            addContact();
            break;
        case '2':
            showAll();
            break;
        case '3':
            searchContact();
            break;
        case '0':
            cout << "\nExiting the program... Goodbye!\n";
            break;
        default:
            cout << "\nInvalid choice! Please try again.\n";
        }

        if (choice != '0') {
            cout << "\nPress Enter to continue...";
            cin.get();
        }

    } while (choice != '0');
}
// Function to add new contact
void ContactManager::addContact() {
    cout << "\nEnter Phone Number: ";
    getline(cin, phoneNo);
    cout << "Enter Name: ";
    getline(cin, name);
    cout << "Enter Address: ";
    getline(cin, address);
    cout << "Enter Description: ";
    getline(cin, description);

    file.open("contacts.csv", ios::out | ios::app);
    if (!file){
        cerr<<"Error opening file for writing!\n";
        return;
    }

    file << phoneNo << "," << name << "," << address << "," << description << "\n";
    file.close();

    cout << "\nContact added successfully!";
}
// Function to display all contacts
void ContactManager::showAll() {
    file.open("contacts.csv", ios::in);
    if (!file) {
        cerr << "Error opening file! No data found.\n";
        return;
    }

    cout << "\n    CONTACT LIST      \n";
    cout << left << setw(15) << "Phone No"
         << setw(20) << "Name"
         << setw(25) << "Address"
         << setw(30) << "Description" << endl;
    cout << "________________________________\n";

    bool found = false;
    while(getline(file, phoneNo, ',')){
        getline(file, name, ',');
        getline(file, address, ',');
        getline(file, description, '\n');

        cout << left << setw(15) << phoneNo
             << setw(20) << name
             << setw(25) << address
             << setw(30) << description << endl;

        found = true;
    }
    if(!found)
        cout << "\nNo contacts found.\n";

    file.close();
}
// Function to search a contact by phone number
void ContactManager::searchContact() {
    cout << "\nEnter Phone Number to Search: ";
    string searchPhone;
    getline(cin, searchPhone);

    file.open("contacts.csv", ios::in);
    if (!file) {
        cerr << "Error opening file! No data found.\n";
        return;
    }

    bool found = false;
    while (getline(file, phoneNo, ',')) {
        getline(file, name, ',');
        getline(file, address, ',');
        getline(file, description, '\n');

        if (phoneNo == searchPhone) {
            cout << "\n Contact Found!\n";
            cout << "Phone Number: " << phoneNo << endl;
            cout << "Name: " << name << endl;
            cout << "Address: " << address << endl;
            cout << "Description: " << description << endl;
            found = true;
            break;
        }
    }

    if (!found)
        cout << "\n No contact found with that phone number.\n";

    file.close();
}

int main() {
    ContactManager app;
    app.menu();
    return 0;
}
