#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

class TrieNode
{
public:
    map<char, TrieNode *> children;
    bool isEnd;

    TrieNode() : isEnd(false) {}
};

class Trie
{
private:
    TrieNode *root;

    void getAllWordsFromNode(TrieNode *node, string prefix, vector<string> &result) const
    {
        if (!node)
            return;
        if (node->isEnd)
            result.push_back(prefix);
        for (auto &p : node->children)
            getAllWordsFromNode(p.second, prefix + p.first, result);
    }

    bool deleteHelper(TrieNode *node, const string &word, int depth)
    {
        if (!node)
            return false;

        if (depth == word.size())
        {
            if (!node->isEnd)
                return false;
            node->isEnd = false;
            return node->children.empty();
        }

        char c = word[depth];

        if (node->children.find(c) == node->children.end())
            return false;

        if (deleteHelper(node->children[c], word, depth + 1))
        {
            delete node->children[c];
            node->children.erase(c);
            return !node->isEnd && node->children.empty();
        }
        return false;
    }

    void freeMemory(TrieNode *node)
    {
        for (auto &p : node->children)
            freeMemory(p.second);
        delete node;
    }

public:
    Trie() { root = new TrieNode(); }

    ~Trie() { freeMemory(root); }

    void insert(const string &word)
    {
        TrieNode *current = root;
        for (char c : word)
        {
            if (current->children.find(c) == current->children.end())
                current->children[c] = new TrieNode();
            current = current->children[c];
        }
        current->isEnd = true;
    }

    bool search(const string &word) const
    {
        TrieNode *current = root;
        for (char c : word)
        {
            if (current->children.find(c) == current->children.end())
                return false;
            current = current->children[c];
        }
        return current->isEnd;
    }

    vector<string> autoSuggest(const string &prefix) const
    {
        vector<string> result;
        TrieNode *current = root;

        for (char c : prefix)
        {
            if (current->children.find(c) == current->children.end())
                return result;
            current = current->children[c];
        }

        getAllWordsFromNode(current, prefix, result);
        return result;
    }

    void displayAll() const
    {
        vector<string> words;
        getAllWordsFromNode(root, "", words);
        for (auto &w : words)
            cout << w << "\n";
    }

    void deleteWord(const string &word)
    {
        deleteHelper(root, word, 0);
    }
};

int main()
{
    Trie dictionary;
    int choice;
    string word, prefix;

    while (true)
    {
        cout << "\n===== Dictionary Application =====\n";
        cout << "1. Insert Word\n";
        cout << "2. Auto Suggest\n";
        cout << "3. Delete Word\n";
        cout << "4. Display All Words\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";

        cin >> choice;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        if (choice == 5)
        {
            cout << "Exiting program..." << endl;
            break;
        }

        switch (choice)
        {
        case 1:
            cout << "Enter word to insert: ";
            cin >> word;
            dictionary.insert(word);
            cout << "Inserted successfully!\n";
            break;

        case 2:
            cout << "Enter prefix: ";
            cin >> prefix;
            {
                vector<string> suggestions = dictionary.autoSuggest(prefix);
                if (suggestions.empty())
                    cout << "No suggestions found.\n";
                else
                    for (auto &s : suggestions)
                        cout << s << "\n";
            }
            break;

        case 3:
            cout << "Enter word to delete: ";
            cin >> word;
            dictionary.deleteWord(word);
            cout << "Deleted (if existed).\n";
            break;

        case 4:
            cout << "All words stored:\n";
            dictionary.displayAll();
            break;

        default:
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}