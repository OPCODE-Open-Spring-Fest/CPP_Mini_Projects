// InvoiceGenerator.cpp
// Works on Dev-C++, Old MinGW, Linux, Windows
// Compile: g++ InvoiceGenerator.cpp -o InvoiceGenerator

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>

#ifdef _WIN32
#include <direct.h> // for _mkdir
#else
#include <sys/stat.h> // for mkdir
#endif

using namespace std;

// ----------- Utility: Create Directory -------------
void makeDirectory(const string &dirName)
{
#ifdef _WIN32
    _mkdir(dirName.c_str()); // Windows
#else
    mkdir(dirName.c_str(), 0777); // Linux
#endif
}

// Check if file exists
bool fileExists(const string &filename)
{
    ifstream f(filename.c_str());
    return f.good();
}

// ----------- Item Class -------------
class Item
{
public:
    string name;
    int quantity;
    double price;

    Item() : quantity(0), price(0) {}

    Item(string n, int q, double p)
        : name(n), quantity(q), price(p) {}

    double getTotal() const
    {
        return quantity * price;
    }
};

// ----------- Invoice Class -------------
class Invoice
{
public:
    int invoiceID;
    string customerName;
    vector<Item> items;
    double subtotal, tax, total;
    double taxRate; // NEW: user-entered tax rate %

    Invoice(int id, string cname, double tr)
        : invoiceID(id), customerName(cname), subtotal(0),
          tax(0), total(0), taxRate(tr) {}

    void addItem(const Item &it)
    {
        items.push_back(it);
    }

    void calculateTotals()
    {
        subtotal = 0;
        for (auto &it : items)
            subtotal += it.getTotal();

        tax = subtotal * (taxRate / 100.0);
        total = subtotal + tax;
    }

    string filename() const
    {
        return "invoices/invoice_" + to_string(invoiceID) + ".txt";
    }

    void saveToFile()
    {
        makeDirectory("invoices");

        ofstream out(filename().c_str());
        if (!out)
        {
            cout << "Error saving invoice file!\n";
            return;
        }

        out << "----------------------------------------\n";
        out << "Invoice ID: " << invoiceID << "\n";
        out << "Customer Name: " << customerName << "\n";
        out << "----------------------------------------\n\n";

        out << left << setw(20) << "Item Name"
            << setw(10) << "Qty"
            << setw(10) << "Price"
            << setw(10) << "Total" << "\n";
        out << "--------------------------------------------------\n";

        for (auto &it : items)
        {
            out << left << setw(20) << it.name
                << setw(10) << it.quantity
                << setw(10) << fixed << setprecision(2) << it.price
                << setw(10) << fixed << setprecision(2) << it.getTotal()
                << "\n";
        }

        out << "--------------------------------------------------\n\n";
        out << "Subtotal: " << subtotal << "\n";
        out << "Tax (" << taxRate << "%): " << tax << "\n";
        out << "Total: " << total << "\n";
        out << "----------------------------------------\n";

        out.close();
    }
};

// ----------- Invoice Manager -------------
class InvoiceManager
{
public:
    string indexFile;

    InvoiceManager()
    {
        indexFile = "invoices/index.txt";
        makeDirectory("invoices");

        if (!fileExists(indexFile))
        {
            ofstream make(indexFile.c_str());
            make.close();
        }
    }

    int generateID()
    {
        ifstream f(indexFile.c_str());
        string line;
        int maxID = 1000;

        while (getline(f, line))
        {
            if (line.empty())
                continue;

            stringstream ss(line);
            string idStr;
            getline(ss, idStr, '|');

            int id = stoi(idStr);
            if (id > maxID)
                maxID = id;
        }
        return maxID + 1;
    }

    void saveToIndex(int id, string cname, string fname)
    {
        ofstream f(indexFile.c_str(), ios::app);
        f << id << "|" << cname << "|" << fname << "\n";
        f.close();
    }

    void createInvoice()
    {
        cin.ignore();
        cout << "Enter customer name: ";
        string cname;
        getline(cin, cname);

        cout << "Enter tax percentage (%): ";
        double taxPercent;
        cin >> taxPercent;
        cin.ignore();

        int id = generateID();
        Invoice inv(id, cname, taxPercent);

        while (true)
        {
            cout << "\nEnter item name (leave blank to finish): ";
            string iname;
            getline(cin, iname);

            if (iname.empty())
                break;

            cout << "Quantity: ";
            int qty;
            cin >> qty;

            cout << "Price: ";
            double price;
            cin >> price;
            cin.ignore();

            inv.addItem(Item(iname, qty, price));
        }

        if (inv.items.empty())
        {
            cout << "No items added. Invoice cancelled.\n";
            return;
        }

        inv.calculateTotals();
        inv.saveToFile();

        saveToIndex(inv.invoiceID, inv.customerName, inv.filename());

        cout << "\nInvoice created and saved successfully!\n";
    }

    void viewInvoiceByID()
    {
        cout << "Enter Invoice ID: ";
        int id;
        cin >> id;

        ifstream f(indexFile.c_str());
        string line;

        while (getline(f, line))
        {
            stringstream ss(line);
            string idStr, cname, fname;

            getline(ss, idStr, '|');
            getline(ss, cname, '|');
            getline(ss, fname, '|');

            if (stoi(idStr) == id)
            {
                ifstream invFile(fname.c_str());
                cout << "\n----- Invoice Content -----\n\n";
                string ln;
                while (getline(invFile, ln))
                    cout << ln << "\n";
                return;
            }
        }

        cout << "Invoice not found!\n";
    }

    void showAllInvoices()
    {
        ifstream f(indexFile.c_str());
        string line;

        cout << "\nStored Invoices:\n";
        cout << "-------------------------------------------\n";

        while (getline(f, line))
        {
            if (line.empty())
                continue;

            stringstream ss(line);
            string id, cname, fname;
            getline(ss, id, '|');
            getline(ss, cname, '|');
            getline(ss, fname, '|');

            cout << "ID: " << id << " | Name: " << cname
                 << " | File: " << fname << "\n";
        }
    }
};

// ----------- MAIN MENU -------------
int main()
{
    InvoiceManager manager;

    while (true)
    {
        cout << "\n----------------------------------------\n";
        cout << "         INVOICE GENERATOR SYSTEM        \n";
        cout << "----------------------------------------\n";
        cout << "1. Create Invoice\n";
        cout << "2. View Invoice by ID\n";
        cout << "3. Show All Invoices\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";

        int choice;
        cin >> choice;

        switch (choice)
        {
        case 1:
            manager.createInvoice();
            break;
        case 2:
            manager.viewInvoiceByID();
            break;
        case 3:
            manager.showAllInvoices();
            break;
        case 4:
            cout << "Exiting...\n";
            return 0;
        default:
            cout << "Invalid choice!\n";
        }
    }
}
