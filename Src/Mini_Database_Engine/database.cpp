#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <limits>
#include <algorithm>

using namespace std;

class Record {
public:
    vector<string> values;
    Record(const vector<string>& vals) : values(vals) {}
    void display() const{
        for (size_t i = 0; i < values.size(); i++)
            cout << values[i] << "\t";
        cout << endl;
    }
};

class Table {
private:
    string name;
    vector<string>columns;
    vector<Record>records;

public:
    Table() {

    }
    Table(const string& tableName, const vector<string>& cols) : name(tableName), columns(cols) {

    }

    const string& getName() const { return name; }
    const vector<string>& getColumns() const { return columns; 
    }

    void insertRecord(const vector<string>& values) {
        if (values.size() != columns.size()) {
            cerr << "Error: Column count mismatch!" << endl;
            return;
        }
        records.emplace_back(values);
        cout << "Record inserted successfully.\n";
    }

    void displayAll() const {
        cout << "\nTable: " << name << endl;
        for (size_t i = 0; i < columns.size(); i++)
            cout << columns[i] << "\t";
        cout << "\n--------------------------\n";
        for (size_t i = 0; i < records.size(); i++)
            records[i].display();
    }

    void deleteRecord(int index) {
        if (index < 0 || index >= (int)records.size()) {
            cerr << "Error: Invalid record index." << endl;
            return;
        }
        records.erase(records.begin() + index);
        cout << "Record deleted successfully.\n";
    }

    void updateRecord(int index, const vector<string>& newValues) {
        if (index < 0 || index >= (int)records.size()) {
            cerr << "Error: Invalid record index." << endl;
            return;
        }
        if (newValues.size() != columns.size()) {
            cerr << "Error: Column count mismatch!" << endl;
            return;
        }
        records[index] = Record(newValues);
        cout << "Record updated successfully.\n";
    }

    void query(const string& column, const string& value) const {
        int colIndex = -1;
        for (size_t i = 0; i < columns.size(); i++) {
            if (columns[i]==column) { colIndex = i; break; }
        }
        if (colIndex == -1) { cerr << "Error: Column not found!" << endl; return; }

        cout << "\nQuery Results for "<< column<< " = " << value << ":\n";
        for (size_t i = 0; i < records.size(); i++) {
            if (records[i].values[colIndex] == value)
                records[i].display();
        }
    }

    void saveToFile(ofstream& out) const {
        out << name << "\n" << columns.size() << "\n";
        for (size_t i = 0; i < columns.size(); i++) out << columns[i] << "\n";
        out << records.size() << "\n";
        for (size_t i = 0; i < records.size(); i++) {
            for (size_t j = 0; j < records[i].values.size(); j++)
                out << records[i].values[j] << "\n";
        }
    }

    void loadFromFile(ifstream& in) {
        size_t colCount, recCount;
        in >> colCount;
        in.ignore(numeric_limits<streamsize>::max(),'\n');
        columns.clear();
        for (size_t i = 0; i < colCount; i++) {
            string col;
            getline(in, col);
            columns.push_back(col);
        }

        in >> recCount;
        in.ignore(numeric_limits<streamsize>::max(),'\n');
        records.clear();
        for (size_t i = 0; i < recCount; i++) {
            vector<string> vals;
            for (size_t j = 0; j < colCount; j++) {
                string val;
                getline(in, val);
                vals.push_back(val);
            }
            records.emplace_back(vals);
        }
    }
};

class Database {
private:
    unordered_map<string, Table> tables;

public:
    void createTable(const string& name, const vector<string>& columns) {
        if (tables.find(name) != tables.end()) { cerr << "Error: Table already exists!" << endl; return; }
        tables[name] = Table(name, columns);
        cout << "Table '" << name << "' created successfully.\n";
    }

    Table* getTable(const string& name) {
        auto it = tables.find(name);
        if (it == tables.end()) { cerr << "Error: Table not found!" << endl; return nullptr; }
        return &(it->second);
    }

    void saveDatabase(const string& filename)const {
        ofstream out(filename.c_str());
        if (!out) { cerr << "Error: Cannot open file for saving.\n"; return; }
        out << tables.size() << "\n";
        for (auto it = tables.begin(); it != tables.end(); ++it)
            it->second.saveToFile(out);
        cout << "Database saved to " << filename << endl;
    }

    void loadDatabase(const string& filename) {
        ifstream in(filename.c_str());
        if (!in) { cerr << "No previous database found. Starting fresh.\n"; return; }
        size_t tableCount;
        in >> tableCount; in.ignore(numeric_limits<streamsize>::max(), '\n');
        tables.clear();
        for (size_t i = 0; i < tableCount; i++) {
            string tableName;
            getline(in, tableName);
            Table t;
            t.loadFromFile(in);
            tables[tableName] = t;
        }
        cout << "Database loaded from " << filename << endl;
    }
};

int main() {
    Database db;
    db.loadDatabase("database.txt");

    int choice;
    while (true) {
        cout << "\n----- Mini Database Engine -----\n";
        cout << "1. Create Table\n2. Insert Record\n3. Display Table\n4. Delete Record\n";
        cout << "5. Update Record\n6. Query Data\n7. Save & Exit\nEnter choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        if (choice == 1) {
            string name;
            int cols;
            cout << "Enter table name: ";
            getline(cin, name);
            cout << "Enter number of columns: ";
            cin >> cols;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            vector<string> columns(cols);
            cout << "Enter column names (one per line):\n";
            for (int i = 0; i < cols; i++)
                getline(cin, columns[i]);

            db.createTable(name, columns);
        }
        else if (choice == 2) {
            string tname;
            cout << "Enter table name: ";
            getline(cin, tname);
            Table* t = db.getTable(tname);
            if (t) {
                vector<string> vals;
                for (size_t i = 0; i < t->getColumns().size(); i++) {
                    string val;
                    cout << t->getColumns()[i] << ": ";
                    getline(cin, val);
                    vals.push_back(val);
                }
                t->insertRecord(vals);
            }
        }
        else if (choice == 3) {
            string tname;
            cout << "Enter table name: ";
            getline(cin, tname);
            Table* t = db.getTable(tname);
            if (t) t->displayAll();
        }
        else if (choice == 4) {
            string tname;
            int index;
            cout<< "Enter table name: ";
            getline(cin, tname);
            cout<< "Enter record index (starting from 0): ";
            cin>> index;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            Table* t = db.getTable(tname);
            if (t) t->deleteRecord(index);
        }
        else if (choice == 5) {
            string tname;
            int index;
            cout << "Enter table name: ";
            getline(cin, tname);
            Table* t = db.getTable(tname);
            if (t) {
                cout<< "Enter record index: ";
                cin>> index;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                vector<string> newVals;
                for (size_t i = 0; i < t->getColumns().size(); i++) {
                    string val;
                    cout<< t->getColumns()[i] << ": ";
                    getline(cin, val);
                    newVals.push_back(val);
                }
                t->updateRecord(index, newVals);
            }
        }
        else if (choice == 6) {
            string tname, col, val;
            cout<< "Enter table name: ";
            getline(cin, tname);
            cout<< "Enter column to search: ";
            getline(cin, col);
            cout<< "Enter value: ";
            getline(cin, val);
            Table* t = db.getTable(tname);
            if (t) t->query(col, val);
        }
        else if (choice == 7) {
            db.saveDatabase("database.txt");
            cout << "Exiting...\n";
            break;
        }
        else cout << "Invalid choice. Try again.\n";
    }

    return 0;
}
#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    return main();
}
#endif
