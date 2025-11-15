#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;
void printFrames(const vector<int>& frames) {
    cout << "Frames: ";
    for (int f : frames) {
        if (f == -1) cout << "- ";
        else cout << f << " ";
    }
    cout << endl;
}
void simulateFIFO(int framesCount, const vector<int>& pages) {
    vector<int> frames(framesCount, -1);
    int pointer = 0, hits = 0, misses = 0;

    cout << "\n===== FIFO Page Replacement =====\n";

    for (int p : pages) {
        bool hit = false;
        for (int f : frames) {
            if (f == p) { hit = true; break; }
        }

        if (hit) {
            hits++;
            cout << "Page " << p << ": HIT  | ";
        } else {
            misses++;
            frames[pointer] = p;
            pointer = (pointer + 1) % framesCount;
            cout << "Page " << p << ": MISS | ";
        }

        printFrames(frames);
    }

    cout << "\nTotal Hits  : " << hits;
    cout << "\nTotal Misses: " << misses;
    cout << "\nHit Ratio   : " << fixed << setprecision(2) 
         << (double)hits / pages.size() << endl;
}
void simulateLRU(int framesCount, const vector<int>& pages) {
    vector<int> frames(framesCount, -1);
    vector<int> lastUsed(framesCount, -1);
    int time = 0, hits = 0, misses = 0;

    cout << "\n===== LRU Page Replacement =====\n";

    for (int p : pages) {
        time++;
        bool hit = false;
        for (int i = 0; i < framesCount; i++) {
            if (frames[i] == p) {
                hit = true;
                lastUsed[i] = time;
                break;
            }
        }

        if (hit) {
            hits++;
            cout << "Page " << p << ": HIT  | ";
        } else {
            misses++;

            int replaceIndex = -1;
            for (int i = 0; i < framesCount; i++) {
                if (frames[i] == -1) {
                    replaceIndex = i;
                    break;
                }
            }
            if (replaceIndex == -1) {
                int minTime = lastUsed[0];
                replaceIndex = 0;

                for (int i = 1; i < framesCount; i++) {
                    if (lastUsed[i] < minTime) {
                        minTime = lastUsed[i];
                        replaceIndex = i;
                    }
                }
            }

            frames[replaceIndex] = p;
            lastUsed[replaceIndex] = time;
            cout << "Page " << p << ": MISS | ";
        }

        printFrames(frames);
    }

    cout << "\nTotal Hits  : " << hits;
    cout << "\nTotal Misses: " << misses;
    cout << "\nHit Ratio   : " << fixed << setprecision(2) 
         << (double)hits / pages.size() << endl;
}
void simulateOptimal(int framesCount, const vector<int>& pages) {
    vector<int> frames(framesCount, -1);
    int hits = 0, misses = 0;

    cout << "\n===== Optimal Page Replacement =====\n";

    for (int i = 0; i < (int)pages.size(); i++) {
        int p = pages[i];
        bool hit = false;
        for (int f : frames) {
            if (f == p) { hit = true; break; }
        }

        if (hit) {
            hits++;
            cout << "Page " << p << ": HIT  | ";
        } else {
            misses++;
            int replaceIdx = -1;
            for (int j = 0; j < framesCount; j++) {
                if (frames[j] == -1) {
                    replaceIdx = j;
                    break;
                }
            }
            if (replaceIdx == -1) {
                int farthest = -1;
                for (int j = 0; j < framesCount; j++) {
                    int nextUse = -1;
                    for (int k = i + 1; k < (int)pages.size(); k++) {
                        if (pages[k] == frames[j]) {
                            nextUse = k;
                            break;
                        }
                    }
                    if (nextUse == -1) {
                        replaceIdx = j;
                        break;
                    }

                    if (nextUse > farthest) {
                        farthest = nextUse;
                        replaceIdx = j;
                    }
                }
            }

            frames[replaceIdx] = p;
            cout << "Page " << p << ": MISS | ";
        }

        printFrames(frames);
    }

    cout << "\nTotal Hits  : " << hits;
    cout << "\nTotal Misses: " << misses;
    cout << "\nHit Ratio   : " << fixed << setprecision(2) 
         << (double)hits / pages.size() << endl;
}

int main() {
    int framesCount, n, choice;
    cout << "Enter number of frames: ";
    cin >> framesCount;

    cout << "Enter number of page requests: ";
    cin >> n;

    vector<int> pages(n);
    cout << "Enter the page reference string:\n";
    for (int i = 0; i < n; i++) cin >> pages[i];

    cout << "\nChoose Algorithm:\n";
    cout << "1. FIFO\n2. LRU\n3. Optimal\n";
    cout << "Enter choice: ";
    cin >> choice;

    switch (choice) {
        case 1: simulateFIFO(framesCount, pages); break;
        case 2: simulateLRU(framesCount, pages); break;
        case 3: simulateOptimal(framesCount, pages); break;
        default: cout << "Invalid choice!\n";
    }

    return 0;
}
