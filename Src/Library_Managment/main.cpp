#include "LibraryManagement.h"

int main() {
    Library lib;

    cout << "=== Library Management System ===\n";

    while (true) {
        cout << "\n1. Add Book\n2. List Books\n3. Add Member\n4. List Members\n"
                "5. Issue Book\n6. Return Book\n7. Search Book by Title\n8. Exit\nChoice: ";
        int ch; 
        if (!(cin >> ch)) break;

        try {
            if (ch == 1) {
                string t,a,c; int copies;
                cout << "Title: "; cin.ignore(); getline(cin,t);
                cout << "Author: "; getline(cin,a);
                cout << "Category: "; getline(cin,c);
                cout << "Copies: "; cin >> copies;
                int id = lib.addBook(t,a,c,copies);
                cout << "Book added with ID " << id << "\n";
            }
            else if (ch == 2) {
                for (auto &b : lib.listBooks())
                    cout << b.getId() << ": " << b.getTitle() << " by " << b.getAuthor()
                         << " (" << b.getAvailableCopies() << "/" << b.getTotalCopies() << " available)\n";
            }
            else if (ch == 3) {
                string n,e;
                cout << "Name: "; cin.ignore(); getline(cin,n);
                cout << "Email: "; getline(cin,e);
                int id = lib.addMember(n,e);
                cout << "Member added with ID " << id << "\n";
            }
            else if (ch == 4) {
                for (auto &m : lib.listMembers())
                    cout << m.getId() << ": " << m.getName() << " <" << m.getEmail() << ">\n";
            }
            else if (ch == 5) {
                int mid,bid;
                cout << "Member ID: "; cin >> mid;
                cout << "Book ID: "; cin >> bid;
                lib.issueBook(mid,bid);
                cout << "Book issued successfully\n";
            }
            else if (ch == 6) {
                int mid,bid;
                cout << "Member ID: "; cin >> mid;
                cout << "Book ID: "; cin >> bid;
                lib.returnBook(mid,bid);
                cout << "Book returned successfully\n";
            }
            else if (ch == 7) {
                string q;
                cout << "Enter title search query: "; cin.ignore(); getline(cin,q);
                auto res = lib.searchByTitle(q);
                for (auto &b : res)
                    cout << b.getId() << ": " << b.getTitle() << " by " << b.getAuthor() << "\n";
            }
            else if (ch == 8) break;
            else cout << "Invalid choice.\n";
        } catch (const exception &e) {
            cout << "Error: " << e.what() << "\n";
        }
    }
    cout << "Goodbye!\n";
    return 0;
}
