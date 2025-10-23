#ifndef LIBRARYMANAGEMENT_H
#define LIBRARYMANAGEMENT_H

#include <bits/stdc++.h>
using namespace std;

// ---------------- Book ----------------
class Book {
    int id;
    string title, author, category;
    int totalCopies, availableCopies;

public:
    Book();
    Book(int id, string title, string author, string category, int copies);

    int getId() const;
    string getTitle() const;
    string getAuthor() const;
    string getCategory() const;
    int getTotalCopies() const;
    int getAvailableCopies() const;

    void setTitle(const string&);
    void setAuthor(const string&);
    void setCategory(const string&);
    void setTotalCopies(int);

    bool issueOne();
    bool returnOne();

    string toCSV() const;
    static Book fromCSV(const string&);
};

// ---------------- Member ----------------
class Member {
    int id;
    string name, email;

public:
    Member();
    Member(int id, string name, string email);

    int getId() const;
    string getName() const;
    string getEmail() const;

    void setName(const string&);
    void setEmail(const string&);

    string toCSV() const;
    static Member fromCSV(const string&);
};

// ---------------- Transaction ----------------
struct Transaction {
    int memberId;
    int bookId;
    string date;
    bool returned;
};

// ---------------- Library ----------------
class Library {
    unordered_map<int, Book> books;
    unordered_map<int, Member> members;
    vector<Transaction> transactions;

    int nextBookId = 1;
    int nextMemberId = 1;

    const string booksFile = "books.csv";
    const string membersFile = "members.csv";
    const string txFile = "transactions.csv";

    void loadAll();
    void saveAll();

public:
    Library();
    ~Library();

    // Books
    int addBook(const string&, const string&, const string&, int);
    void updateBook(int, const optional<string>&, const optional<string>&, const optional<string>&, const optional<int>&);
    void deleteBook(int);
    vector<Book> listBooks() const;
    vector<Book> searchByTitle(const string&) const;
    vector<Book> searchByAuthor(const string&) const;
    vector<Book> searchByCategory(const string&) const;

    // Members
    int addMember(const string&, const string&);
    void updateMember(int, const optional<string>&, const optional<string>&);
    void deleteMember(int);
    vector<Member> listMembers() const;

    // Transactions
    void issueBook(int memberId, int bookId);
    void returnBook(int memberId, int bookId);
};

#endif
