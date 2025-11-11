/*
 * ===================================================================
 * | C++ OPERATING SYSTEM PROCESS SIMULATOR                          |
 * ===================================================================
 *
 * This program simulates various CPU scheduling algorithms based on
 * user-provided process details.
 *
 * Features:
 * - Object-Oriented Design (Process, Scheduler, OSSimulator)
 * - Scheduling Algorithms:
 * 1. First-Come-First-Served (FCFS)
 * 2. Shortest-Job-First (SJF) (Non-Preemptive)
 * 3. Priority (Non-Preemptive)
 * 4. Round Robin (Preemptive)
 * - Calculates:
 * - Completion Time
 * - Turnaround Time
 * - Waiting Time
 * - Average Turnaround Time
 * - Average Waiting Time
 * - Displays a text-based Gantt Chart.
 * - Interactive console menu.
 */

#include <iostream>
#include <vector>
#include <string>
#include <memory>       // For std::unique_ptr
#include <algorithm>    // For std::sort, std::min
#include <queue>          // For Round Robin ready queue
#include <iomanip>      // For std::setw, std::setprecision
#include <limits>       // For std::numeric_limits
#include <map>          // For Gantt chart merging

// ===================================================================
// | 1. PROCESS CLASS                                                |
// ===================================================================
// | Represents a single process with all its attributes and metrics.|
// ===================================================================

/**
 * @struct Process
 * @brief Holds all data for a single process, including its
 * attributes (PID, times) and calculated metrics.
 */
struct Process {
    // --- Attributes ---
    int pid;
    int arrivalTime;
    int burstTime;
    int priority;

    // --- State for Preemptive Algorithms ---
    int remainingBurstTime;

    // --- Calculated Metrics ---
    int completionTime;
    int turnaroundTime;
    int waitingTime;

    /**
     * @brief Constructor for a new Process.
     * @param id Process ID
     * @param at Arrival Time
     * @param bt Burst Time
     * @param p Priority (lower number = higher priority)
     */
    Process(int id, int at, int bt, int p = 0)
        : pid(id), arrivalTime(at), burstTime(bt), priority(p),
          remainingBurstTime(bt), completionTime(0), turnaroundTime(0), waitingTime(0) {}

    /**
     * @brief Resets the process's metrics and state for a new simulation.
     */
    void reset() {
        remainingBurstTime = burstTime;
        completionTime = 0;
        turnaroundTime = 0;
        waitingTime = 0;
    }

    /**
     * @brief Calculates turnaround and waiting times after completion.
     */
    void calculateMetrics() {
        turnaroundTime = completionTime - arrivalTime;
        waitingTime = turnaroundTime - burstTime;
    }
};

// ===================================================================
// | 2. SCHEDULER INTERFACE & IMPLEMENTATIONS                        |
// ===================================================================
// | Defines the Scheduler interface and concrete algorithm classes. |
// ===================================================================

/**
 * @struct GanttEntry
 * @brief Represents a single block in the Gantt chart timeline.
 */
struct GanttEntry {
    std::string processName; // "P1", "P2", or "Idle"
    int startTime;
    int endTime;

    GanttEntry(const std::string& name, int start, int end)
        : processName(name), startTime(start), endTime(end) {}
};

/**
 * @class Scheduler
 * @brief Abstract base class (interface) for all scheduling algorithms.
 * This demonstrates the Strategy Design Pattern.
 */
class Scheduler {
public:
    virtual ~Scheduler() = default;

    /**
     * @brief The core scheduling logic.
     * @param processes A vector of processes to be scheduled. Metrics
     * (completionTime, etc.) will be updated in-place.
     * @return A vector of GanttEntry structs representing the timeline.
     */
    virtual std::vector<GanttEntry> schedule(std::vector<Process>& processes) = 0;

    /**
     * @brief Returns the name of the algorithm.
     */
    virtual std::string getName() const = 0;
};


// --- FCFS Implementation ---

/**
 * @class FCFS_Scheduler
 * @brief Implements the First-Come-First-Served scheduling algorithm.
 */
class FCFS_Scheduler : public Scheduler {
public:
    std::string getName() const override { return "First-Come-First-Served (FCFS)"; }

    std::vector<GanttEntry> schedule(std::vector<Process>& processes) override {
        // Sort processes by arrival time.
        std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
            return a.arrivalTime < b.arrivalTime;
        });

        std::vector<GanttEntry> ganttChart;
        int currentTime = 0;

        for (auto& p : processes) {
            // Check for CPU idle time
            if (currentTime < p.arrivalTime) {
                ganttChart.emplace_back("Idle", currentTime, p.arrivalTime);
                currentTime = p.arrivalTime;
            }

            // Execute the process
            int startTime = currentTime;
            p.completionTime = currentTime + p.burstTime;
            currentTime = p.completionTime;

            // Calculate metrics
            p.calculateMetrics();

            // Add to Gantt chart
            ganttChart.emplace_back("P" + std::to_string(p.pid), startTime, p.completionTime);
        }
        return ganttChart;
    }
};


// --- SJF (Non-Preemptive) Implementation ---

/**
 * @class SJF_Scheduler
 * @brief Implements the Shortest-Job-First (Non-Preemptive) algorithm.
 */
class SJF_Scheduler : public Scheduler {
public:
    std::string getName() const override { return "Shortest-Job-First (SJF) (Non-Preemptive)"; }

    std::vector<GanttEntry> schedule(std::vector<Process>& processes) override {
        std::vector<GanttEntry> ganttChart;
        int n = processes.size();
        std::vector<bool> isCompleted(n, false);
        int currentTime = 0;
        int completedCount = 0;

        while (completedCount < n) {
            int shortestJobIndex = -1;
            int minBurst = std::numeric_limits<int>::max();

            // Find the shortest job that has arrived and is not completed
            for (int i = 0; i < n; ++i) {
                if (processes[i].arrivalTime <= currentTime && !isCompleted[i]) {
                    if (processes[i].burstTime < minBurst) {
                        minBurst = processes[i].burstTime;
                        shortestJobIndex = i;
                    }
                }
            }

            if (shortestJobIndex == -1) {
                // No process is ready, CPU is idle
                // Find the next process to arrive
                int nextArrival = std::numeric_limits<int>::max();
                for (int i = 0; i < n; ++i) {
                    if (!isCompleted[i]) {
                        nextArrival = std::min(nextArrival, processes[i].arrivalTime);
                    }
                }
                
                // Avoid adding "Idle" if simulation is done
                if (nextArrival != std::numeric_limits<int>::max()) {
                     ganttChart.emplace_back("Idle", currentTime, nextArrival);
                     currentTime = nextArrival;
                }
               
            } else {
                // Execute the selected process
                Process& p = processes[shortestJobIndex];
                int startTime = currentTime;
                p.completionTime = currentTime + p.burstTime;
                currentTime = p.completionTime;

                p.calculateMetrics();
                isCompleted[shortestJobIndex] = true;
                completedCount++;

                ganttChart.emplace_back("P" + std::to_string(p.pid), startTime, p.completionTime);
            }
        }
        return ganttChart;
    }
};


// --- Priority (Non-Preemptive) Implementation ---

/**
 * @class Priority_Scheduler
 * @brief Implements the Priority (Non-Preemptive) algorithm.
 * Assumes lower number = higher priority.
 */
class Priority_Scheduler : public Scheduler {
public:
    std::string getName() const override { return "Priority (Non-Preemptive)"; }

    std::vector<GanttEntry> schedule(std::vector<Process>& processes) override {
        std::vector<GanttEntry> ganttChart;
        int n = processes.size();
        std::vector<bool> isCompleted(n, false);
        int currentTime = 0;
        int completedCount = 0;

        while (completedCount < n) {
            int highestPriorityIndex = -1;
            int minPriority = std::numeric_limits<int>::max();

            // Find the highest priority job that has arrived and is not completed
            for (int i = 0; i < n; ++i) {
                if (processes[i].arrivalTime <= currentTime && !isCompleted[i]) {
                    if (processes[i].priority < minPriority) {
                        minPriority = processes[i].priority;
                        highestPriorityIndex = i;
                    }
                }
            }

            if (highestPriorityIndex == -1) {
                // No process is ready, CPU is idle
                int nextArrival = std::numeric_limits<int>::max();
                for (int i = 0; i < n; ++i) {
                    if (!isCompleted[i]) {
                        nextArrival = std::min(nextArrival, processes[i].arrivalTime);
                    }
                }

                if (nextArrival != std::numeric_limits<int>::max()) {
                    ganttChart.emplace_back("Idle", currentTime, nextArrival);
                    currentTime = nextArrival;
                }
            } else {
                // Execute the selected process
                Process& p = processes[highestPriorityIndex];
                int startTime = currentTime;
                p.completionTime = currentTime + p.burstTime;
                currentTime = p.completionTime;

                p.calculateMetrics();
                isCompleted[highestPriorityIndex] = true;
                completedCount++;

                ganttChart.emplace_back("P" + std::to_string(p.pid), startTime, p.completionTime);
            }
        }
        return ganttChart;
    }
};


// --- Round Robin Implementation ---

/**
 * @class RoundRobin_Scheduler
 * @brief Implements the Round Robin (Preemptive) algorithm.
 */
class RoundRobin_Scheduler : public Scheduler {
private:
    int timeQuantum;

public:
    RoundRobin_Scheduler(int tq) : timeQuantum(tq) {}
    std::string getName() const override { return "Round Robin (TQ=" + std::to_string(timeQuantum) + ")"; }

    std::vector<GanttEntry> schedule(std::vector<Process>& processes) override {
        std::vector<GanttEntry> ganttChart;
        int n = processes.size();
        
        // Sort processes by arrival time to manage new arrivals
        std::vector<int> processIndices(n);
        for(int i=0; i<n; ++i) processIndices[i] = i;
        std::sort(processIndices.begin(), processIndices.end(), [&](int a, int b){
            return processes[a].arrivalTime < processes[b].arrivalTime;
        });

        std::queue<int> readyQueue; // Queue of process indices
        int currentTime = 0;
        int completedCount = 0;
        int arrivalIndex = 0; // Tracks next process to arrive

        while (completedCount < n) {
            // 1. Add newly arrived processes to the ready queue
            while (arrivalIndex < n && processes[processIndices[arrivalIndex]].arrivalTime <= currentTime) {
                readyQueue.push(processIndices[arrivalIndex]);
                arrivalIndex++;
            }

            if (readyQueue.empty()) {
                // 2. If queue is empty, CPU is idle
                if (arrivalIndex < n) {
                    // Fast-forward to the next arrival
                    int nextArrivalTime = processes[processIndices[arrivalIndex]].arrivalTime;
                    if (currentTime < nextArrivalTime) {
                         ganttChart.emplace_back("Idle", currentTime, nextArrivalTime);
                         currentTime = nextArrivalTime;
                    }
                } else {
                    // Should be done, but as a safeguard
                    break;
                }
            } else {
                // 3. Process from the ready queue
                int currentProcessIndex = readyQueue.front();
                readyQueue.pop();
                Process& p = processes[currentProcessIndex];

                int startTime = currentTime;
                int execTime = std::min(timeQuantum, p.remainingBurstTime);

                p.remainingBurstTime -= execTime;
                currentTime += execTime;

                ganttChart.emplace_back("P" + std::to_string(p.pid), startTime, currentTime);

                // 4. IMPORTANT: Add processes that arrived *during* this time slice
                while (arrivalIndex < n && processes[processIndices[arrivalIndex]].arrivalTime <= currentTime) {
                    readyQueue.push(processIndices[arrivalIndex]);
                    arrivalIndex++;
                }

                // 5. Check if process is finished or needs to be re-queued
                if (p.remainingBurstTime == 0) {
                    p.completionTime = currentTime;
                    p.calculateMetrics();
                    completedCount++;
                } else {
                    // Add it back to the end of the queue
                    readyQueue.push(currentProcessIndex);
                }
            }
        }
        return ganttChart;
    }
};


// ===================================================================
// | 3. OS SIMULATOR CLASS                                           |
// ===================================================================
// | Manages the simulation, holds processes, and prints results.    |
// ===================================================================

/**
 * @class OSSimulator
 * @brief Main class to manage the simulation state, processes,
 * and scheduler, and to display results.
 */
class OSSimulator {
private:
    std::vector<Process> processList;         // Working copy for simulation
    std::vector<Process> originalProcessList; // Master copy
    std::unique_ptr<Scheduler> scheduler;
    std::vector<GanttEntry> ganttChart;

    double avgWaitingTime;
    double avgTurnaroundTime;

    /**
     * @brief Merges consecutive Gantt chart entries for the same process.
     * e.g., [P1, 0-2], [P1, 2-4] becomes [P1, 0-4]
     */
    std::vector<GanttEntry> mergeGanttChart(const std::vector<GanttEntry>& rawChart) {
        if (rawChart.empty()) return {};

        std::vector<GanttEntry> merged;
        merged.push_back(rawChart.front());

        for (size_t i = 1; i < rawChart.size(); ++i) {
            if (rawChart[i].processName == merged.back().processName) {
                // Merge with previous entry
                merged.back().endTime = rawChart[i].endTime;
            } else {
                // Add as a new entry
                merged.push_back(rawChart[i]);
            }
        }
        return merged;
    }

    /**
     * @brief Prints the Gantt chart to the console.
     */
    void printGanttChart() {
        if (ganttChart.empty()) return;

        std::cout << "\n--- Gantt Chart ---" << std::endl;
        
        std::vector<GanttEntry> mergedChart = mergeGanttChart(ganttChart);

        // Top line
        std::cout << "+";
        for (const auto& entry : mergedChart) {
            std::cout << std::string(entry.processName.length() + 2, '-') << "+";
        }
        std::cout << "\n|";

        // Process names
        for (const auto& entry : mergedChart) {
            std::cout << " " << entry.processName << " |";
        }
        std::cout << "\n+";

        // Bottom line
        for (const auto& entry : mergedChart) {
            std::cout << std::string(entry.processName.length() + 2, '-') << "+";
        }
        std::cout << "\n";

        // Timestamps
        std::cout << mergedChart.front().startTime;
        for (const auto& entry : mergedChart) {
            std::cout << std::string(entry.processName.length() + 2 - std::to_string(entry.endTime).length(), ' ')
                      << entry.endTime;
        }
        std::cout << "\n" << std::endl;
    }

    /**
     * @brief Prints the table of calculated metrics for all processes.
     */
    void printMetricsTable() {
        if (processList.empty()) return;

        // Sort by PID for a clean final table
        std::sort(processList.begin(), processList.end(), [](const Process& a, const Process& b){
            return a.pid < b.pid;
        });

        std::cout << "--- Process Metrics ---" << std::endl;
        std::cout << "+-----+----------+------------+----------+-----------+-----------------+----------------+\n";
        std::cout << "| PID | Arrival  | Burst Time | Priority | Completion | Turnaround Time | Waiting Time |\n";
        std::cout << "+-----+----------+------------+----------+-----------+-----------------+----------------+\n";

        std::cout << std::fixed << std::setprecision(2);
        for (const auto& p : processList) {
            std::cout << "| " << std::setw(3) << p.pid << " | "
                      << std::setw(8) << p.arrivalTime << " | "
                      << std::setw(10) << p.burstTime << " | "
                      << std::setw(8) << p.priority << " | "
                      << std::setw(9) << p.completionTime << " | "
                      << std::setw(15) << p.turnaroundTime << " | "
                      << std::setw(14) << p.waitingTime << " |\n";
        }
        std::cout << "+-----+----------+------------+----------+-----------+-----------------+----------------+\n";
    }

    /**
     * @brief Calculates the average metrics after a simulation.
     */
    void calculateAverages() {
        if (processList.empty()) {
            avgWaitingTime = 0;
            avgTurnaroundTime = 0;
            return;
        }

        double totalWaiting = 0;
        double totalTurnaround = 0;
        for (const auto& p : processList) {
            totalWaiting += p.waitingTime;
            totalTurnaround += p.turnaroundTime;
        }

        avgWaitingTime = totalWaiting / processList.size();
        avgTurnaroundTime = totalTurnaround / processList.size();

        std::cout << "\n--- Average Metrics ---" << std::endl;
        std::cout << "Average Waiting Time:     " << avgWaitingTime << std::endl;
        std::cout << "Average Turnaround Time:  " << avgTurnaroundTime << std::endl;
    }

public:
    OSSimulator() : avgWaitingTime(0), avgTurnaroundTime(0) {}

    /**
     * @brief Adds a new process to the master list.
     */
    void addProcess(int pid, int at, int bt, int priority = 0) {
        originalProcessList.emplace_back(pid, at, bt, priority);
        std::cout << "Process P" << pid << " added." << std::endl;
    }

    /**
     * @brief Sets the scheduling algorithm to be used.
     * @param s A unique_ptr to a Scheduler object.
     */
    void setScheduler(std::unique_ptr<Scheduler> s) {
        scheduler = std::move(s);
        std::cout << "Scheduler set to: " << scheduler->getName() << std::endl;
    }

    /**
     * @brief Resets the simulation state.
     */
    void reset() {
        processList = originalProcessList;
        for (auto& p : processList) {
            p.reset();
        }
        ganttChart.clear();
        avgWaitingTime = 0;
        avgTurnaroundTime = 0;
    }

    /**
     * @brief Runs the simulation using the currently set scheduler.
     */
    void run() {
        if (!scheduler) {
            std::cout << "Error: No scheduler set. Please select a scheduler first." << std::endl;
            return;
        }
        if (originalProcessList.empty()) {
            std::cout << "Error: No processes to schedule. Please add processes first." << std::endl;
            return;
        }

        // Reset state to original
        reset();

        std::cout << "\nRunning simulation with " << scheduler->getName() << "..." << std::endl;

        // Run the scheduling algorithm
        ganttChart = scheduler->schedule(processList);

        // Calculate average metrics
        calculateAverages();

        std::cout << "Simulation complete." << std::endl;

        // Display the full results
        displayResults();
    }

    /**
     * @brief Displays the Gantt chart and metrics table.
     */
    void displayResults() {
        if (ganttChart.empty()) {
            std::cout << "No results to display. Run the simulation first." << std::endl;
            return;
        }
        printMetricsTable();
        printGanttChart();
    }
    
    /**
     * @brief Lists all processes currently added to the simulator.
     */
    void listProcesses() {
        if (originalProcessList.empty()) {
            std::cout << "No processes have been added." << std::endl;
            return;
        }
        std::cout << "\n--- Current Processes ---" << std::endl;
        std::cout << "+-----+----------+------------+----------+\n";
        std::cout << "| PID | Arrival  | Burst Time | Priority |\n";
        std::cout << "+-----+----------+------------+----------+\n";
        for (const auto& p : originalProcessList) {
             std::cout << "| " << std::setw(3) << p.pid << " | "
                      << std::setw(8) << p.arrivalTime << " | "
                      << std::setw(10) << p.burstTime << " | "
                      << std::setw(8) << p.priority << " |\n";
        }
        std::cout << "+-----+----------+------------+----------+\n";
    }

    bool hasScheduler() const { return scheduler != nullptr; }
    bool hasProcesses() const { return !originalProcessList.empty(); }
};


// ===================================================================
// | 4. MAIN DRIVER PROGRAM                                          |
// ===================================================================
// | Provides the interactive user menu to control the simulator.    |
// ===================================================================

/**
 * @brief Utility function to safely read an integer from console.
 * @param prompt The message to display to the user.
 * @param nonNegative Enforces the input to be >= 0.
 * @return The validated integer input.
 */
int getIntInput(const std::string& prompt, bool nonNegative = true) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            if (!nonNegative || value >= 0) {
                break; // Valid input
            } else {
                std::cout << "Input must be non-negative. Please try again.\n";
            }
        } else {
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear(); // Clear error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard buffer
        }
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard trailing newline
    return value;
}

/**
 * @brief Displays the main menu options.
 */
void printMenu() {
    std::cout << "\n=======================================";
    std::cout << "\n| C++ OS Process Simulator Menu       |";
    std::cout << "\n=======================================";
    std::cout << "\n| 1. Add Process                    |";
    std::cout << "\n| 2. List Processes                 |";
    std::cout << "\n| 3. Select Scheduler               |";
    std::cout << "\n| 4. Run Simulation                 |";
    std::cout << "\n| 5. Exit                           |";
    std::cout << "\n=======================================";
    std::cout << "\nEnter your choice (1-5): ";
}

/**
 * @brief Handles the "Add Process" menu action.
 */
void addNewProcess(OSSimulator& sim) {
    std::cout << "\n--- Add New Process ---" << std::endl;
    int pid = getIntInput("Enter Process ID (PID): ");
    int at = getIntInput("Enter Arrival Time: ");
    int bt = getIntInput("Enter Burst Time (must be > 0): ");
    while (bt <= 0) {
        std::cout << "Burst Time must be positive.\n";
        bt = getIntInput("Enter Burst Time (must be > 0): ");
    }
    int priority = getIntInput("Enter Priority (lower number = higher priority): ");
    sim.addProcess(pid, at, bt, priority);
}

/**
 * @brief Handles the "Select Scheduler" menu action.
 */
void selectScheduler(OSSimulator& sim) {
    std::cout << "\n--- Select Scheduler ---" << std::endl;
    std::cout << "1. First-Come-First-Served (FCFS)\n";
    std::cout << "2. Shortest-Job-First (SJF) (Non-Preemptive)\n";
    std::cout << "3. Priority (Non-Preemptive)\n";
    std::cout << "4. Round Robin (RR)\n";
    int choice = getIntInput("Enter choice (1-4): ");

    switch (choice) {
        case 1:
            sim.setScheduler(std::make_unique<FCFS_Scheduler>());
            break;
        case 2:
            sim.setScheduler(std::make_unique<SJF_Scheduler>());
            break;
        case 3:
            sim.setScheduler(std::make_unique<Priority_Scheduler>());
            break;
        case 4: {
            int tq = getIntInput("Enter Time Quantum for Round Robin (must be > 0): ");
            while (tq <= 0) {
                std::cout << "Time Quantum must be positive.\n";
                tq = getIntInput("Enter Time Quantum for Round Robin (must be > 0): ");
            }
            sim.setScheduler(std::make_unique<RoundRobin_Scheduler>(tq));
            break;
        }
        default:
            std::cout << "Invalid choice. Returning to main menu.\n";
            break;
    }
}

/**
 * @brief Main function: the entry point of the program.
 */
int main() {
    OSSimulator simulator;

    // --- Pre-populate with sample data ---
    // Comment out or remove this section for a blank start
    std::cout << "Loading sample data...\n";
    simulator.addProcess(1, 0, 8, 3);
    simulator.addProcess(2, 1, 4, 1);
    simulator.addProcess(3, 2, 9, 4);
    simulator.addProcess(4, 3, 5, 2);
    // -------------------------------------

    while (true) {
        printMenu();
        int choice;
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard newline

        switch (choice) {
            case 1:
                addNewProcess(simulator);
                break;
            case 2:
                simulator.listProcesses();
                break;
            case 3:
                selectScheduler(simulator);
                break;
            case 4:
                simulator.run();
                break;
            case 5:
                std::cout << "Exiting simulator. Goodbye!" << std::endl;
                return 0;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                break;
        }
    }

    return 0;
}