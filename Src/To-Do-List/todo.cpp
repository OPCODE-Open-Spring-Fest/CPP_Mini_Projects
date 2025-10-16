#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Class representing a single Task
class Task {
private:
    string title;
    bool isCompleted;

public:
    // Constructor
    Task(string t) {
        title = t;
        isCompleted = false;
    }

    // Mark task as completed
    void markCompleted() {
        isCompleted = true;
    }

    // Get task title
    string getTitle() const {
        return title;
    }

    // Check completion status
    bool getStatus() const {
        return isCompleted;
    }
};

// Class representing the To-Do List
class ToDoList {
private:
    vector<Task> tasks;

public:
    // Add a new task
    void addTask(const string& title) {
        tasks.push_back(Task(title));
        cout << "Task added successfully!\n";
    }

    // Display all tasks
    void showTasks() const {
        if (tasks.empty()) {
            cout << "No tasks available!\n";
            return;
        }

        cout << "\n------ To-Do List ------\n";
        for (size_t i = 0; i < tasks.size(); ++i) {
            cout << i + 1 << ". " << tasks[i].getTitle()
                 << " [" << (tasks[i].getStatus() ? "Done" : "Pending") << "]\n";
        }
        cout << "------------------------\n";
    }

    // Mark a task as completed
    void markTaskCompleted(int index) {
        if (index < 1 || index > (int)tasks.size()) {
            cout << "Invalid task number!\n";
            return;
        }
        tasks[index - 1].markCompleted();
        cout << "Task marked as completed!\n";
    }

    // Delete a task
    void deleteTask(int index) {
        if (index < 1 || index > (int)tasks.size()) {
            cout << "Invalid task number!\n";
            return;
        }
        tasks.erase(tasks.begin() + index - 1);
        cout << "Task deleted successfully!\n";
    }
};

// Main program
int main() {
    ToDoList todo;
    int choice;
    string title;
    int index;

    cout << "\n===== TO-DO LIST APPLICATION =====\n";
    cout << "Manage your daily tasks efficiently.\n";

    do {
        cout << "\nMenu:\n";
        cout << "1. Add Task\n";
        cout << "2. View Tasks\n";
        cout << "3. Mark Task as Completed\n";
        cout << "4. Delete Task\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // To handle newline

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
            cout << "Enter task number to mark completed: ";
            cin >> index;
            todo.markTaskCompleted(index);
            break;

        case 4:
            cout << "Enter task number to delete: ";
            cin >> index;
            todo.deleteTask(index);
            break;

        case 5:
            cout << "Exiting program. Goodbye!\n";
            break;

        default:
            cout << "Invalid choice. Try again!\n";
        }
    } while (choice != 5);

    return 0;
}


