#include <bits/stdc++.h>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace std::this_thread;
using namespace std::chrono;

const int SIZE = 10;
int delay_ms = 120;

bool visited[SIZE * SIZE];
bool walls[SIZE * SIZE][2] = {0};
int startPos = 0;
int goalPos = SIZE * SIZE - 1;
int path[SIZE * SIZE] = {0};
int pos = startPos;
int rot = 0;
int pathIndex = 0;

// ------------------------------------------------------------
void drawMazeConsole(bool showPath = false, bool clearScreen = true) {
    if (clearScreen) {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    cout << "\n";
    for (int j = 0; j < SIZE; j++) {
        for (int i = 0; i < SIZE; i++) {
            cout << "+";
            if (walls[i + j * SIZE][1])
                cout << "   ";
            else
                cout << "---";
        }
        cout << "+\n";

        for (int i = 0; i < SIZE; i++) {
            if (walls[i + j * SIZE][0])
                cout << " ";
            else
                cout << "|";

            int position = i + j * SIZE;

            if (position == startPos)
                cout << " S ";
            else if (position == goalPos)
                cout << " G ";
            else if (position == pos && showPath)
                cout << " P ";
            else if (showPath && path[position])
                cout << " . ";
            else
                cout << "   ";
        }
        cout << "|\n";
    }

    for (int i = 0; i < SIZE; i++) cout << "+---";
    cout << "+\n";
    cout.flush();
    sleep_for(milliseconds(delay_ms));
}

// ------------------------------------------------------------
int nextCell(int pos, int size) {
    int options[4], count = 0;
    if (pos >= size && !visited[pos - size]) options[count++] = -size;
    if ((pos + 1) % size != 0 && !visited[pos + 1]) options[count++] = 1;
    if (pos < size * (size - 1) && !visited[pos + size]) options[count++] = size;
    if (pos % size != 0 && !visited[pos - 1]) options[count++] = -1;

    if (count == 0) return 0;
    return options[rand() % count];
}

void connect(int pos1, int pos2) {
    if (pos2 > pos1) {
        if (pos2 == pos1 + 1) walls[pos2][0] = 1;
        else walls[pos2][1] = 1;
    } else {
        if (pos1 == pos2 + 1) walls[pos1][0] = 1;
        else walls[pos1][1] = 1;
    }
}

void randomDFS(int pos, int size) {
    visited[pos] = 1;
    int next = pos + nextCell(pos, size);

    while (next != pos) {
        connect(pos, next);
        pos = next;
        drawMazeConsole(false);
        randomDFS(pos, size);
        next = pos + nextCell(pos, size);
    }
}

// ------------------------------------------------------------
bool move() {
    char dir;
    if (abs(rot) % 4 == 0) dir = 'N';
    else if (abs(rot) % 4 == 1) dir = 'E';
    else if (abs(rot) % 4 == 2) dir = 'S';
    else dir = 'W';

    switch (dir) {
        case 'E':
            if ((pos + 1) % SIZE != 0 && walls[pos + 1][0]) { pos += 1; path[pos] = ++pathIndex; return true; }
            return false;
        case 'S':
            if (pos < SIZE * (SIZE - 1) && walls[pos + SIZE][1]) { pos += SIZE; path[pos] = ++pathIndex; return true; }
            return false;
        case 'W':
            if (pos % SIZE != 0 && walls[pos][0]) { pos -= 1; path[pos] = ++pathIndex; return true; }
            return false;
        case 'N':
            if (pos >= SIZE && walls[pos][1]) { pos -= SIZE; path[pos] = ++pathIndex; return true; }
            return false;
    }
    return false;
}

void solveMazeRightHand() {
    path[0] = 1;
    rot = 1;
    pos = startPos;

    while (pos != goalPos) {
        rot++;
        if (!move()) {
            rot--;
            if (!move()) {
                rot--;
                if (!move()) {
                    rot--;
                    move();
                }
            }
        }
        drawMazeConsole(true);
    }
}

// ------------------------------------------------------------
int main() {
    srand(time(0));

    memset(visited, 0, sizeof(visited));
    cout << "\nGenerating maze...\n";
    randomDFS(0, SIZE);

    cout << "\nMaze generation complete!\n";
    drawMazeConsole(false, false); // show final maze once
    sleep_for(seconds(2));

    cout << "\nSolving maze (Right-Hand Rule)...\n";
    solveMazeRightHand();

    cout << "\nFinal solved maze:\n";
    drawMazeConsole(true, false);

    cout << "\nPress Enter to exit...";
    cin.ignore();
    cin.get();
    return 0;
}
