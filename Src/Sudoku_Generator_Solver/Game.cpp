#include <iostream>
#include <random>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

class SudokuSolver
{
private:
    vector<vector<int>> grid;
    vector<vector<int>> solution;

public:
    SudokuSolver()
    {
        grid.assign(9, vector<int>(9, 0));
        solution.assign(9, vector<int>(9, 0));
    }

    bool isValid(vector<vector<int>> &g, int row, int col, int num)
    {
        // Check row
        for (int j = 0; j < 9; j++)
        {
            if (g[row][j] == num)
                return false;
        }

        // Check column
        for (int i = 0; i < 9; i++)
        {
            if (g[i][col] == num)
                return false;
        }

        // Check 3x3 box
        int boxRow = 3 * (row / 3);
        int boxCol = 3 * (col / 3);
        for (int i = boxRow; i < boxRow + 3; i++)
        {
            for (int j = boxCol; j < boxCol + 3; j++)
            {
                if (g[i][j] == num)
                    return false;
            }
        }

        return true;
    }

    bool solve(vector<vector<int>> &g)
    {
        for (int row = 0; row < 9; row++)
        {
            for (int col = 0; col < 9; col++)
            {
                if (g[row][col] == 0)
                {
                    for (int num = 1; num <= 9; num++)
                    {
                        if (isValid(g, row, col, num))
                        {
                            g[row][col] = num;
                            if (solve(g))
                                return true;
                            g[row][col] = 0;
                        }
                    }
                    return false;
                }
            }
        }
        return true;
    }

    void fillDiagonalBoxes()
    {
        vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        for (int box = 0; box < 3; box++)
        {
            auto rng = std::default_random_engine(std::random_device{}());
            std::shuffle(numbers.begin(), numbers.end(), rng);
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    grid[box * 3 + i][box * 3 + j] = numbers[i * 3 + j];
                }
            }
        }
    }

    int countSolutions(vector<vector<int>> &g, int &count)
    {
        if (count > 1)
            return count;

        for (int row = 0; row < 9; row++)
        {
            for (int col = 0; col < 9; col++)
            {
                if (g[row][col] == 0)
                {
                    for (int num = 1; num <= 9; num++)
                    {
                        if (isValid(g, row, col, num))
                        {
                            g[row][col] = num;
                            countSolutions(g, count);
                            g[row][col] = 0;
                        }
                    }
                    return count;
                }
            }
        }
        count++;
        return count;
    }

    bool hasUniqueSolution(vector<vector<int>> &puzzle)
    {
        vector<vector<int>> testGrid = puzzle;
        int solutionCount = 0;
        countSolutions(testGrid, solutionCount);
        return solutionCount == 1;
    }

    vector<vector<int>> generate(string difficulty = "medium")
    {
        // Number of clues based on difficulty
        int clues;
        if (difficulty == "easy")
            clues = 50;
        else if (difficulty == "medium")
            clues = 40;
        else if (difficulty == "hard")
            clues = 30;
        else if (difficulty == "expert")
            clues = 20;
        else
            clues = 40;

        // Generate complete solution
        grid.assign(9, vector<int>(9, 0));
        fillDiagonalBoxes();
        solve(grid);
        solution = grid;

        // Create puzzle by removing numbers
        vector<vector<int>> puzzle = grid;
        vector<pair<int, int>> cells;
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                cells.push_back({i, j});
            }
        }
        auto rng = std::default_random_engine(std::random_device{}());
        std::shuffle(cells.begin(), cells.end(), rng);

        int removed = 0;
        int targetRemovals = 81 - clues;

        for (auto &cell : cells)
        {
            if (removed >= targetRemovals)
                break;

            int row = cell.first;
            int col = cell.second;
            int backup = puzzle[row][col];
            puzzle[row][col] = 0;

            if (hasUniqueSolution(puzzle))
            {
                removed++;
            }
            else
            {
                puzzle[row][col] = backup;
            }
        }

        return puzzle;
    }

    void printGrid(const vector<vector<int>> &g)
    {
        for (int i = 0; i < 9; i++)
        {
            if (i % 3 == 0 && i != 0)
            {
                cout << "------+-------+------" << endl;
            }
            for (int j = 0; j < 9; j++)
            {
                if (j % 3 == 0 && j != 0)
                {
                    cout << "| ";
                }
                cout << (g[i][j] != 0 ? to_string(g[i][j]) : ".") << " ";
            }
            cout << endl;
        }
    }

    vector<vector<int>> getSolution()
    {
        return solution;
    }
};

int main()
{
    srand(time(0));

    cout << "==================================================" << endl;
    cout << "SUDOKU GENERATOR AND SOLVER (C++)" << endl;
    cout << "==================================================" << endl;

    SudokuSolver solver;
    vector<string> difficulties = {"easy", "medium", "hard", "expert"};

    for (const auto &difficulty : difficulties)
    {
        cout << "\n==================================================" << endl;
        cout << "PUZZLE (" << difficulty << ")" << endl;
        cout << "==================================================" << endl;

        vector<vector<int>> puzzle = solver.generate(difficulty);
        solver.printGrid(puzzle);

        cout << "\n==================================================" << endl;
        cout << "SOLUTION (" << difficulty << ")" << endl;
        cout << "==================================================" << endl;

        solver.printGrid(solver.getSolution());
    }

    cout << "\n==================================================" << endl;
    cout << "Code execution completed!" << endl;
    cout << "==================================================" << endl;

    return 0;
}