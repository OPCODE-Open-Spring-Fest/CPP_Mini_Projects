#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>

using namespace std;

class Transaction {
private:
    string category;
    double amount;
    string note;

public:
    Transaction(string c, double a, string n) {
        category = c;
        amount = a;
        note = n;
    }

    string getCategory() const { return category; }
    double getAmount() const { return amount; }
    string getNote() const { return note; }

    void display() const {
        cout << left << setw(15) << category
             << setw(10) << fixed << setprecision(2) << amount
             << setw(25) << note << endl;
    }

    string toFileString() const {
        return category + "," + to_string(amount) + "," + note + "\n";
    }

    static Transaction fromFileString(const string &line) {
        string c, a, n;
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);

        if (pos1 == string::npos || pos2 == string::npos)
            return Transaction("Invalid", 0.0, "Corrupt");

        c = line.substr(0, pos1);
        a = line.substr(pos1 + 1, pos2 - pos1 - 1);
        n = line.substr(pos2 + 1);
        return Transaction(c, stod(a), n);
    }
};

class ExpenseTracker {
private:
    vector<Transaction> transactions;
    const string filename = "expenses.txt";

    void loadFromFile() {
        ifstream fin(filename);
        if (!fin.is_open()) return;

        string line;
        while (getline(fin, line)) {
            if (!line.empty())
                transactions.push_back(Transaction::fromFileString(line));
        }
        fin.close();
    }

    void saveToFile() const {
        ofstream fout(filename);
        for (const auto &t : transactions)
            fout << t.toFileString();
        fout.close();
    }

public:
    ExpenseTracker() {
        loadFromFile();
    }

    void addTransaction() {
        string category, note;
        double amount;
        cout << "\nEnter category: ";
        cin.ignore();
        getline(cin, category);
        cout << "Enter amount: ";
        cin >> amount;
        cin.ignore();
        cout << "Enter note: ";
        getline(cin, note);

        transactions.push_back(Transaction(category, amount, note));
        saveToFile();
        cout << "\n✅ Transaction added successfully!\n";
    }

    void viewAll() const {
        if (transactions.empty()) {
            cout << "\nNo transactions recorded yet.\n";
            return;
        }
        cout << "\n================= All Transactions =================\n";
        cout << left << setw(15) << "Category" << setw(10) << "Amount" << setw(25) << "Note" << endl;
        cout << "----------------------------------------------------\n";
        for (const auto &t : transactions)
            t.display();
    }

    void viewSummary() const {
        if (transactions.empty()) {
            cout << "\nNo transactions available.\n";
            return;
        }
        double total = 0;
        for (const auto &t : transactions)
            total += t.getAmount();

        cout << "\nTotal Transactions: " << transactions.size();
        cout << "\nTotal Amount: ₹" << fixed << setprecision(2) << total << endl;
    }

    void searchByCategory() const {
        if (transactions.empty()) {
            cout << "\nNo transactions available.\n";
            return;
        }

        string search;
        cout << "\nEnter category to search: ";
        cin.ignore();
        getline(cin, search);

        bool found = false;
        cout << "\nTransactions in category \"" << search << "\":\n";
        cout << left << setw(15) << "Category" << setw(10) << "Amount" << setw(25) << "Note" << endl;
        cout << "----------------------------------------------------\n";
        for (const auto &t : transactions) {
            if (t.getCategory() == search) {
                t.display();
                found = true;
            }
        }
        if (!found)
            cout << "No transactions found in this category.\n";
    }

    void deleteTransaction() {
        if (transactions.empty()) {
            cout << "\nNo transactions to delete.\n";
            return;
        }

        viewAll();
        cout << "\nEnter transaction number to delete (1 - " << transactions.size() << "): ";
        int index;
        cin >> index;

        if (index < 1 || index > (int)transactions.size()) {
            cout << "❌ Invalid index!\n";
            return;
        }

        transactions.erase(transactions.begin() + index - 1);
        saveToFile();
        cout << "✅ Transaction deleted successfully.\n";
    }
};

int main() {
    ExpenseTracker tracker;
    int choice;

    while (true) {
        cout << "\n================= Expense Tracker =================";
        cout << "\n1. Add Transaction";
        cout << "\n2. View All Transactions";
        cout << "\n3. View Summary";
        cout << "\n4. Search by Category";
        cout << "\n5. Delete a Transaction";
        cout << "\n6. Exit";
        cout << "\n===================================================";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: tracker.addTransaction(); break;
            case 2: tracker.viewAll(); break;
            case 3: tracker.viewSummary(); break;
            case 4: tracker.searchByCategory(); break;
            case 5: tracker.deleteTransaction(); break;
            case 6:
                cout << "\n💾 Data saved. Exiting... Goodbye!\n";
                return 0;
            default:
                cout << "\nInvalid choice! Try again.\n";
        }
    }
}
