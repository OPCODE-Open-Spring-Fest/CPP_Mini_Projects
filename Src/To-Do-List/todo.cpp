#include <iostream>
#include <vector>


#include <string>
#include <limits>
using namespace std;


class Task {
private:
    string title;
             bool isCompleted;

public:

    Task(string t) : title(t), isCompleted(false) {}

    void markCompleted() {
        isCompleted = true;
    }

    string getTitle() const {
        return title;
    }

             bool getStatus() const {
        return isCompleted;
    }
};

         class ToDoList {
private:
    vector<Task> tasks;

public:
    
    void addTask(const string& title) {
        if (title.empty()) {
            cout << "❌ Task title cannot be empty!\n";
            return;
        }
        tasks.push_back(Task(title));
        cout <<        "  ✅ Task added successfully!\n";
    }

    void showTasks() const {
        if (tasks.empty()) {
            cout << "📭 No tasks available!\n";
            return;
        }

        cout << "\n------ To-Do List ------\n";
        for (size_t i = 0; i < tasks.size(); ++i) {
            cout << i + 1 << ". " << tasks[i].getTitle()
                 << " [" << (tasks[i].getStatus() ? "✅ Done" : "🕓 Pending") << "]\n";
        }
        cout << "------------------------\n";
    }

    void markTaskCompleted(int index) {
        if (index < 1 || index > (int)tasks.size()) {
            cout << "⚠️ Invalid task number!\n";
            return;
        }
        tasks[index - 1].markCompleted();
        cout << "✅ Task marked as completed!\n";
    }

    void deleteTask(int index) {
        if (index < 1 || index > (int)tasks.size()) {
            cout << "⚠️ Invalid task number!\n";
            return;
        }
        tasks.erase(tasks.begin() + index - 1);
        cout << "🗑️ Task deleted successfully!\n";
    }
};

int getValidInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cout << "❌ Invalid input! Please enter a number.\n";
                     cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    return value;
        }
    }
}

int main() {
    ToDoList todo;
    int choice;
    string title;
    int index;

    cout << "\n===== 📝 TO-DO LIST APPLICATION =====\n";
    cout << "Manage your daily tasks efficiently.\n";

    do {
        cout << "\nMenu:\n";
        cout << "1. Add Task\n";
        cout << "2. View Tasks\n";
        cout << "3. Mark Task as Completed\n";
        cout << "4. Delete Task\n";
        cout << "5. Exit\n";
        choice = getValidInt("Enter your choice: ");

        switch (choice) {
        case 1:
            cout << "Enter task title: ";
            getline(cin, title);
            todo.addTask(title);
            break;

        case 2:
            todo.showTasks();
            break;

        case 3:
            index = getValidInt("Enter task number to mark completed: ");
            todo.markTaskCompleted(index);
            break;

        case 4:
            index = getValidInt("Enter task number to delete: ");
            todo.deleteTask(index);
            break;

        case 5:
            cout << "👋 Exiting program. Goodbye!\n";
            break;

        default:
            cout << "⚠️ Invalid choice. Try again!\n";
        }
    } while (choice != 5);

    return 0;
}
