#include "LibraryManagement.h"

// ------------ helpers ------------
static inline string trim(const string &s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

static inline string lower(const string &s) {
    string t = s;
    transform(t.begin(), t.end(), t.begin(), ::tolower);
    return t;
}

// ------------ Book ------------
Book::Book() : id(-1), totalCopies(0), availableCopies(0) {}
Book::Book(int i, string t, string a, string c, int copies)
    : id(i), title(move(t)), author(move(a)), category(move(c)),
      totalCopies(copies), availableCopies(copies) {}

int Book::getId() const { return id; }
string Book::getTitle() const { return title; }
string Book::getAuthor() const { return author; }
string Book::getCategory() const { return category; }
int Book::getTotalCopies() const { return totalCopies; }
int Book::getAvailableCopies() const { return availableCopies; }

void Book::setTitle(const string &t) { title = t; }
void Book::setAuthor(const string &a) { author = a; }
void Book::setCategory(const string &c) { category = c; }
void Book::setTotalCopies(int c) {
    if (c < 0) throw runtime_error("Total copies cannot be negative");
    int issued = totalCopies - availableCopies;
    if (c < issued) throw runtime_error("Cannot set total copies less than already issued copies");
    totalCopies = c;
    availableCopies = c - issued;
}

bool Book::issueOne() {
    if (availableCopies <= 0) return false;
    --availableCopies;
    return true;
}

bool Book::returnOne() {
    if (availableCopies >= totalCopies) return false;
    ++availableCopies;
    return true;
}

string Book::toCSV() const {
    return to_string(id) + "," + title + "," + author + "," + category + "," +
           to_string(totalCopies) + "," + to_string(availableCopies);
}

Book Book::fromCSV(const string &line) {
    stringstream ss(line);
    string id, t, a, c, total, avail;
    getline(ss, id, ',');
    getline(ss, t, ',');
    getline(ss, a, ',');
    getline(ss, c, ',');
    getline(ss, total, ',');
    getline(ss, avail, ',');
    Book b;
    b = Book(stoi(id), trim(t), trim(a), trim(c), stoi(total));
    b.setTotalCopies(stoi(total));
    while (b.getAvailableCopies() > stoi(avail)) b.issueOne();
    return b;
}

// ------------ Member ------------
Member::Member() : id(-1) {}
Member::Member(int i, string n, string e) : id(i), name(move(n)), email(move(e)) {}

int Member::getId() const { return id; }
string Member::getName() const { return name; }
string Member::getEmail() const { return email; }

void Member::setName(const string &n) { name = n; }
void Member::setEmail(const string &e) { email = e; }

string Member::toCSV() const { return to_string(id) + "," + name + "," + email; }

Member Member::fromCSV(const string &line) {
    stringstream ss(line);
    string id, n, e;
    getline(ss, id, ',');
    getline(ss, n, ',');
    getline(ss, e, ',');
    return Member(stoi(id), trim(n), trim(e));
}

// ------------ Library ------------
Library::Library() { loadAll(); }
Library::~Library() { saveAll(); }

void Library::loadAll() {
    ifstream fb(booksFile);
    if (fb) {
        string line;
        while (getline(fb, line))
            if (!trim(line).empty()) {
                Book b = Book::fromCSV(line);
                books[b.getId()] = b;
                nextBookId = max(nextBookId, b.getId() + 1);
            }
    }

    ifstream fm(membersFile);
    if (fm) {
        string line;
        while (getline(fm, line))
            if (!trim(line).empty()) {
                Member m = Member::fromCSV(line);
                members[m.getId()] = m;
                nextMemberId = max(nextMemberId, m.getId() + 1);
            }
    }
}

void Library::saveAll() {
    ofstream fb(booksFile);
    for (auto &[_, b] : books)
        fb << b.toCSV() << "\n";

    ofstream fm(membersFile);
    for (auto &[_, m] : members)
        fm << m.toCSV() << "\n";
}

// Books
int Library::addBook(const string &t, const string &a, const string &c, int copies) {
    int id = nextBookId++;
    books[id] = Book(id, t, a, c, copies);
    return id;
}

void Library::updateBook(int id, const optional<string> &t, const optional<string> &a,
                         const optional<string> &c, const optional<int> &copies) {
    auto it = books.find(id);
    if (it == books.end()) throw runtime_error("Book not found");
    if (t) it->second.setTitle(*t);
    if (a) it->second.setAuthor(*a);
    if (c) it->second.setCategory(*c);
    if (copies) it->second.setTotalCopies(*copies);
}

void Library::deleteBook(int id) {
    if (!books.count(id)) throw runtime_error("Book not found");
    books.erase(id);
}

vector<Book> Library::listBooks() const {
    vector<Book> v;
    for (auto &[_, b] : books) v.push_back(b);
    sort(v.begin(), v.end(), [](auto &x, auto &y){return x.getId()<y.getId();});
    return v;
}

vector<Book> Library::searchByTitle(const string &q) const {
    vector<Book> r;
    for (auto &[_, b] : books)
        if (lower(b.getTitle()).find(lower(q)) != string::npos)
            r.push_back(b);
    return r;
}

vector<Book> Library::searchByAuthor(const string &q) const {
    vector<Book> r;
    for (auto &[_, b] : books)
        if (lower(b.getAuthor()).find(lower(q)) != string::npos)
            r.push_back(b);
    return r;
}

vector<Book> Library::searchByCategory(const string &q) const {
    vector<Book> r;
    for (auto &[_, b] : books)
        if (lower(b.getCategory()).find(lower(q)) != string::npos)
            r.push_back(b);
    return r;
}

// Members
int Library::addMember(const string &n, const string &e) {
    int id = nextMemberId++;
    members[id] = Member(id, n, e);
    return id;
}

void Library::updateMember(int id, const optional<string>& n, const optional<string>& e) {
    auto it = members.find(id);
    if (it == members.end()) throw runtime_error("Member not found");
    if (n) it->second.setName(*n);
    if (e) it->second.setEmail(*e);
}

void Library::deleteMember(int id) {
    if (!members.count(id)) throw runtime_error("Member not found");
    members.erase(id);
}

vector<Member> Library::listMembers() const {
    vector<Member> v;
    for (auto &[_, m] : members) v.push_back(m);
    sort(v.begin(), v.end(), [](auto &x, auto &y){return x.getId()<y.getId();});
    return v;
}

// Transactions
void Library::issueBook(int memberId, int bookId) {
    if (!members.count(memberId)) throw runtime_error("Invalid member");
    if (!books.count(bookId)) throw runtime_error("Invalid book");
    if (!books[bookId].issueOne()) throw runtime_error("No available copies");
    transactions.push_back({memberId, bookId, "2025-10-23", false});
}

void Library::returnBook(int memberId, int bookId) {
    if (!members.count(memberId)) throw runtime_error("Invalid member");
    if (!books.count(bookId)) throw runtime_error("Invalid book");
    books[bookId].returnOne();
    for (auto &tx : transactions)
        if (tx.memberId == memberId && tx.bookId == bookId && !tx.returned) {
            tx.returned = true;
            return;
        }
}
