#include<bits/stdc++.h>
using namespace std;
//Bank class
class Bank{
    int accNo;
    char name[25];
    char father[25];
    char Aadhar[25];
    char mobile[25];
    char email[25];
    float amount;
    int newAmount;
    fstream file, file1;

public:
    int search;

    void createAccount(void);
    void depositAmount(void);
    void withdrawAmount(void);
    void checkInfo(void);
    bool isValidEmail(const string &email);
    bool isDuplicate(const string &Aadhar, const string &email, const string &phone);
};
//check email valid ya invalid
bool Bank::isValidEmail(const string &email) {
    return (email.find('@') != string::npos && email.find('.') != string::npos && email.find(' ') == string::npos);
}
//check duplicate user
bool Bank::isDuplicate(const string &cnicVal, const string &emailVal, const string &phoneVal) {
    fstream f;
    f.open("bankdata.txt", ios::in);
    if (!f)
        return false;

    int acc;
    char n[25], fn[25], c[25], p[25], e[25];
    float amt;
    while(f>>acc>>n>>fn>>c>>p>>e>>amt){
        if(strcmp(c, cnicVal.c_str()) == 0 || strcmp(e, emailVal.c_str()) == 0 || strcmp(p, phoneVal.c_str()) == 0) {
            f.close();
            return true;
        }
    }
    f.close();
    return false;
}
//create Account
void Bank::createAccount(){
    srand(time(0));
    accNo = rand()*rand() + rand()*rand();

    cout << "Enter Your name -: ";
    cin >> name;
    cout << "Enter Your Father name -: ";
    cin >> father;
    cout << "Enter Your Aadhar no. -: ";
    cin >> Aadhar;
    cout << "Enter Your phone no. -: ";
    cin >> mobile;
    cout << "Enter Your email -: ";
    cin >> email;

    // Email Validation
    if(!isValidEmail(email)){
        cout << "\n Invalid email format! Use a valid email (e.g., abc@gmail.com)\n";
        return;
    }

    // Duplicate Check
    if(isDuplicate(Aadhar, email, mobile)){
        cout << "\n Account with same Aadhar, Email or Phone already exists!\n";
        return;
    }
    cout << "Enter Your amount to deposit -: ";
    cin>>amount;
    cout << endl
         << accNo << " This is your account number...\n";
    cout << "Please save it.\n\n";

    file.open("bankdata.txt", ios::out | ios::app);
    file << accNo << "\t" << name << "\t" << father << "\t" << Aadhar << "\t" << mobile << "\t" << email << "\t" << amount << endl;
    file.close();

    cout << "✅ Account created successfully!\n";
}
//deposit
void Bank::depositAmount() {
    cout << "Enter amount to deposit -: ";
    cin >> newAmount;

    file.open("bankdata.txt", ios::in);
    file1.open("bankdata1.txt", ios::out | ios::app);
    file >> accNo >> name >> father >> Aadhar >> mobile >> email >> amount;

    bool found = false;
    while(!file.eof()){
        if(accNo == search){
            found = true;
            cout << "\nCurrent amount -: " << amount;
            amount = amount + newAmount;
            cout << "\nUpdated amount -: " << amount << endl;
        }
        file1 << accNo << "\t" << name << "\t" << father << "\t" << Aadhar << "\t" << mobile << "\t" << email << "\t" << amount << endl;
        file >> accNo >> name >> father >> Aadhar >> mobile >> email >> amount;
    }

    if (!found)
        cout << "\n Account not found!\n";

    file.close();
    file1.close();
    remove("bankdata.txt");
    rename("bankdata1.txt", "bankdata.txt");
}
//withdraw
void Bank::withdrawAmount() {
    cout << "Enter amount to withdraw -: ";
    cin >> newAmount;

    file.open("bankdata.txt", ios::in);
    file1.open("bankdata1.txt", ios::out | ios::app);
    file >> accNo >> name >> father >> Aadhar >> mobile >> email >> amount;

    bool found = false;
    while(!file.eof()){
        if(accNo == search){
            found = true;
            cout << "\nCurrent amount -: " << amount;
            if(newAmount > amount) {
                cout << "\n Insufficient balance!\n";
            }else {
                amount = amount - newAmount;
                cout << "\nUpdated amount -: " << amount << endl;
            }
        }
        file1 << accNo << "\t" << name << "\t" << father << "\t" << Aadhar << "\t" << mobile << "\t" << email << "\t" << amount << endl;
        file >> accNo >> name >> father >> Aadhar >> mobile >> email >> amount;
    }

    if (!found)
        cout << "\n Account not found!\n";

    file.close();
    file1.close();
    remove("bankdata.txt");
    rename("bankdata1.txt", "bankdata.txt");
}
//check info    
void Bank::checkInfo() {
    fstream file;
    file.open("bankdata.txt", ios::in);
    if (!file) {
        cout << " File opening error!";
        return;
    }
    file >> accNo >> name >> father >> Aadhar >> mobile >> email >> amount;
    bool found = false;
    while (!file.eof()) {
        if (accNo == search) {
            found = true;
            cout << "\n_________________________________________\n";
            cout << "\t# Account Number - " << accNo << endl;
            cout << "\t# User Name      - " << name << endl;
            cout << "\t# Father Name    - " << father << endl;
            cout << "\t# Aadhar number  - " << Aadhar << endl;
            cout << "\t# Phone Number   - " << mobile << endl;
            cout << "\t# Email          - " << email << endl;
            cout << "\t# Current amount - " << amount << endl;
            cout << "___________________________________________\n\n";
        }
        file >> accNo >> name >> father >> Aadhar >> mobile >> email >> amount;
    }

    if (!found)
        cout << "\n Account not found!\n";

    file.close();
}
//main function
int main() {
    Bank obj;
    char choice;

    cout << "\n\n\n\t\tWelcome! choose your Option :";
    cout << "\n\t\t1. press 1 to Login  Account ";
    cout << "\n\t\t2. press 2 to Create Account";
    cout << "\n\t\t3. press 0 to Exit           ";
    cout << "\n\t\t >Enter choice... :\t";
    cin >> choice;
//choice
    switch (choice) {
    case '1':
        cout << "Enter your account no -: ";
        cin >> obj.search;
        while (1) {
            cout << "\n\n\n\t\tWelcome! choose your Option :";
            cout << "\n\t\t1. press 1 to Deposit  Amount ";
            cout << "\n\t\t2. press 2 to Withdraw Amount";
            cout << "\n\t\t3. press 3 to Check    Info";
            cout << "\n\t\t4. press 0 to Exit     Menu";
            cout << "\n\t\t>Enter Choice... :\t";
            cin >> choice;

            switch (choice) {
            case '1':
                obj.depositAmount();
                break;
            case '2':
                obj.withdrawAmount();
                break;
            case '3':
                obj.checkInfo();
                break;
            case '0':
                return 0;
            default:
                cout << "Invalid Choice...!";
                break;
            }
            system("pause");
            system("cls");
        }
        break;
    case '2':
        obj.createAccount();
        break;
    case '0':
        cout << "Exiting... Thank you!\n";
        break;
    default:
        cout << "\n Invalid choice...! ";
        break;
    }
    return 0;
}
