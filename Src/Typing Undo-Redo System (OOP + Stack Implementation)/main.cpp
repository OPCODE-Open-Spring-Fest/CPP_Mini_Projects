#include <iostream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

/**
 * TextEditor class - Manages typing, undo, and redo operations
 */
class TextEditor {
private:
    vector<string> currentText;  // Stores the current document text
    stack<string> undoStack;     // Stack for undo operations
    stack<string> redoStack;     // Stack for redo operations

public:
    /**
     * Constructor - Initializes the text editor
     */
    TextEditor() {
        cout << "Text Editor initialized!\n";
    }

    /**
     * Type text - Adds new text to the document
     * @param text The text to add
     */
    void typeText(const string& text) {
        if (text.empty()) {
            cout << "Error: Cannot add empty text!\n";
            return;
        }
        
        undoStack.push(text);
        currentText.push_back(text);
        
        // Clear redo stack when new text is typed
        while (!redoStack.empty()) {
            redoStack.pop();
        }
        
        cout << "✓ Text added: \"" << text << "\"\n";
    }

    /**
     * Undo - Removes the last typed text
     */
    void undo() {
        if (undoStack.empty()) {
            cout << "Nothing to undo!\n";
            return;
        }
        
        string lastText = undoStack.top();
        undoStack.pop();
        
        redoStack.push(lastText);
        currentText.pop_back();
        
        cout << "↩ Undone: \"" << lastText << "\"\n";
    }

    /**
     * Redo - Restores the most recently undone text
     */
    void redo() {
        if (redoStack.empty()) {
            cout << "Nothing to redo!\n";
            return;
        }
        
        string textToRestore = redoStack.top();
        redoStack.pop();
        
        undoStack.push(textToRestore);
        currentText.push_back(textToRestore);
        
        cout << "↪ Redone: \"" << textToRestore << "\"\n";
    }

    /**
     * Display the current document text
     */
    void showCurrentText() const {
        cout << "\n\n";
        cout << "📜 CURRENT DOCUMENT:\n";
        cout << "\n";
        
        if (currentText.empty()) {
            cout << "[Empty Document]\n";
        } else {
            for (size_t i = 0; i < currentText.size(); i++) {
                cout << currentText[i];
                if (i < currentText.size() - 1) {
                    cout << " ";
                }
            }
            cout << "\n";
        }
        
        cout << "═══════════════════════════════════════\n\n";
    }

    /**
     * Display statistics about the editor state
     */
    void showStats() const {
        cout << "\n📊 Editor Statistics:\n";
        cout << "   • Words typed: " << currentText.size() << "\n";
        cout << "   • Undo available: " << undoStack.size() << "\n";
        cout << "   • Redo available: " << redoStack.size() << "\n\n";
    }

    /**
     * Destructor
     */
    ~TextEditor() {
        cout << "Text Editor closed. Goodbye!\n";
    }
};

/**
 * Display the menu
 */
void displayMenu() {
    cout << "\n";
    cout << "//    TYPING UNDO/REDO SYSTEM MENU       //\n";
    cout << "\n";
    cout << "  1. ✍️  Type Text                      \n";
    cout << "  2. ↩️  Undo Typing                    \n";
    cout << "  3. ↪️  Redo Typing                    \n";
    cout << "  4. 📜 Show Current Text               \n";
    cout << "  5. 📊 Show Statistics                 \n";
    cout << "  6. 🚪 Exit                            \n";
    cout << "\n";
    cout << "Enter your choice: ";
}

/**
 * Main function - Entry point of the program
 */
int main() {
    TextEditor editor;
    int choice;
    string text;
    
    cout << "\n\n";
    cout << "//  Welcome to Typing Undo/Redo System!  //\n";
    cout << "\n";
    
    do {
        displayMenu();
        cin >> choice;
        cin.ignore(); // Clear the newline character from buffer
        
        switch (choice) {
            case 1:
                cout << "Enter text to type: ";
                getline(cin, text);
                editor.typeText(text);
                break;
                
            case 2:
                editor.undo();
                break;
                
            case 3:
                editor.redo();
                break;
                
            case 4:
                editor.showCurrentText();
                break;
                
            case 5:
                editor.showStats();
                break;
                
            case 6:
                cout << "\n🚪 Exiting program...\n";
                break;
                
            default:
                cout << "❌ Invalid choice! Please select 1-6.\n";
        }
        
    } while (choice != 6);
    
    return 0;
}