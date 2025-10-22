#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <limits>

using namespace std;

class Subject {
public:
    string name;
    double marks;
    double credits;

    Subject(string n = "", double m = 0, double c = 1)
        : name(n), marks(m), credits(c) {}
};

class Calculator {
public:
    static string getLetterGrade(double marks) {
        if (marks >= 90) return "A+";
        if (marks >= 85) return "A";
        if (marks >= 80) return "A-";
        if (marks >= 75) return "B+";
        if (marks >= 70) return "B";
        if (marks >= 65) return "C+";
        if (marks >= 60) return "C";
        if (marks >= 50) return "D";
        return "F";
    }

    static double gradePoint4(double marks) {
        if (marks >= 90) return 4.0;
        if (marks >= 85) return 3.7;
        if (marks >= 80) return 3.5;
        if (marks >= 75) return 3.3;
        if (marks >= 70) return 3.0;
        if (marks >= 65) return 2.5;
        if (marks >= 60) return 2.0;
        if (marks >= 50) return 1.0;
        return 0.0;
    }

    static double gradePoint10(double marks) {
        if (marks >= 90) return 10;
        if (marks >= 80) return 9;
        if (marks >= 70) return 8;
        if (marks >= 60) return 7;
        if (marks >= 50) return 6;
        if (marks >= 40) return 5;
        return 0;
    }
};

class Semester {
private:
    vector<Subject> subjects;

public:
    void inputSubjects() {
        int n;
        cout << "Enter number of subjects: ";
        cin >> n;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        for (int i = 0; i < n; i++) {
            string name;
            double marks, credits;

            cout << "\nSubject " << i + 1 << " name: ";
            getline(cin, name);

            cout << "Marks (0–100): ";
            cin >> marks;
            while (marks < 0 || marks > 100) {
                cout << "Invalid marks. Enter again (0–100): ";
                cin >> marks;
            }

            cout << "Credits (default 1): ";
            if (!(cin >> credits)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                credits = 1;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            subjects.emplace_back(name, marks, credits);
        }
    }

    void displaySummary() {
        double totalWeighted4 = 0, totalWeighted10 = 0, totalCredits = 0;

        cout << "\n-----------------------------------------------\n";
        cout << left << setw(12) << "Subject"
             << setw(8) << "Marks"
             << setw(8) << "Grade"
             << setw(10) << "4.0 GPA"
             << setw(10) << "10-Point" << "\n";
        cout << "-----------------------------------------------\n";

        for (auto &sub : subjects) {
            string grade = Calculator::getLetterGrade(sub.marks);
            double gp4 = Calculator::gradePoint4(sub.marks);
            double gp10 = Calculator::gradePoint10(sub.marks);

            cout << left << setw(12) << sub.name
                 << setw(8) << sub.marks
                 << setw(8) << grade
                 << setw(10) << gp4
                 << setw(10) << gp10 << "\n";

            totalWeighted4 += gp4 * sub.credits;
            totalWeighted10 += gp10 * sub.credits;
            totalCredits += sub.credits;
        }

        cout << "-----------------------------------------------\n";
        cout << " Semester GPA (4.0 scale): " 
             << fixed << setprecision(2) << (totalWeighted4 / totalCredits) << "\n";
        cout << " Semester GPA (10-point):  " 
             << fixed << setprecision(2) << (totalWeighted10 / totalCredits) << "\n";
        cout << "-----------------------------------------------\n";
    }
};

int main() {
    cout << "=== C++ Semester Grade Calculator ===\n";
    Semester sem;
    sem.inputSubjects();
    sem.displaySummary();
    cout << "\nThank you for using the calculator!\n";
    return 0;
}
