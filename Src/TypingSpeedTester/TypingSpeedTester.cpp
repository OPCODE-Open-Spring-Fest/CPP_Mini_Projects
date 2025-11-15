#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace std::chrono;

string getRandomSnippet() {
    vector<string> snippets = {
        "for(int i = 0; i < n; ++i) cout << arr[i] << ' ';",
        "if(a > b) swap(a, b);",
        "while(left <= right) mid = (left + right) / 2;",
        "vector<int> nums = {1, 2, 3, 4, 5};",
        "cout << \"Hello, World!\" << endl;"
    };
    srand((unsigned) time(0));
    return snippets[rand() % snippets.size()];
}

int countMistakes(const string& original, const string& typed) {
    int mistakes = 0;
    int len=max(original.size(), typed.size());
    for (int i = 0; i < len; ++i) {
        if (i>=original.size()||i>=typed.size()||original[i]!=typed[i])
            mistakes++;
    }
    return mistakes;
}

void showResults(const string& snippet, const string& typed, double duration) {
    int totalChars=snippet.size();
    int mistakes=countMistakes(snippet, typed);
    double accuracy=((double)(totalChars - mistakes) / totalChars) * 100.0;

    double minutes = duration/60.0;
    double wpm = (typed.size()/5.0)/minutes;

    cout << "\n\n========== RESULTS ==========\n";
    cout << "Time Taken: " << fixed << setprecision(2) << duration << " seconds\n";
    cout << "Mistakes: " << mistakes << "\n";
    cout << "Accuracy: " << fixed << setprecision(2) << accuracy << "%\n";
    cout << "Typing Speed: " << fixed << setprecision(2) << wpm << " WPM\n";
    cout << "=============================\n\n";
}

int main() {
    cout << "==============================\n";
    cout << "    Code Typing Speed Tester\n";
    cout << "==============================\n\n";

    string snippet = getRandomSnippet();
    cout << "Type the following code snippet exactly as shown:\n\n";
    cout << "-> " << snippet << "\n\n";
    cout << "Press ENTER when you’re done typing.\n\n";

    cout << "Start typing below:\n> ";
    auto start = high_resolution_clock::now();

    string typed;
    getline(cin, typed);

    auto end = high_resolution_clock::now();
    duration<double> diff = end - start;

    showResults(snippet, typed, diff.count());

    cout << "Thank you for using the Code Typing Speed Tester!\n";
    return 0;
}
