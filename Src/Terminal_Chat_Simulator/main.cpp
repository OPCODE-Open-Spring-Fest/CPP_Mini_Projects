/*
 * -----------------------------------------------------------------
 * C++ TERMINAL CHAT SIMULATOR (v2: User Switching)
 * -----------------------------------------------------------------
 * This program simulates a multi-user chat room in the terminal.
 * It uses multithreading to demonstrate concurrency concepts
 * without any networking.
 *
 * Concepts Demonstrated:
 * - Object-Oriented Programming (OOP)
 * - Multithreading (std::thread)
 * - Synchronization (std::mutex, std::lock_guard, std::unique_lock)
 * - Producer-Consumer Model
 * - Broadcasting (std::condition_variable)
 *
 * How to compile (C++11 or newer):
 * g++ -o chat_simulator chat_simulator.cpp -std=c++11 -pthread
 * or
 * clang++ -o chat_simulator chat_simulator.cpp -std=c++11 -pthread
 *
 * How to run:
 * ./chat_simulator
 * -----------------------------------------------------------------
 */

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>       // For std::this_thread::sleep_for
#include <memory>       // For std::unique_ptr
#include <atomic>       // For std::atomic<bool>

// Forward declarations for cross-class references
class User;
class ChatRoom;

// -----------------------------------------------------------------
// CLASS: Message
// Represents a single chat message with a sender and content.
// -----------------------------------------------------------------
class Message {
public:
    std::string sender;
    std::string content;

    Message(const std::string& s, const std::string& c) : sender(s), content(c) {}
};

// -----------------------------------------------------------------
// CLASS: ChatRoom
// The centralized hub. It manages the shared message log and
// handles the synchronization and broadcasting.
// -----------------------------------------------------------------
class ChatRoom {
private:
    // The shared message log. This is the "shared message queue"
    // described in the overview. A vector is used as a log
    // so messages aren't "consumed" (popped) when read.
    std::vector<Message> messages;

    // Mutex to protect access to the shared 'messages' vector
    std::mutex mtx;

    // Condition variable to signal new messages
    std::condition_variable cv;

public:
    /**
     * @brief Adds a new message to the log (Producer).
     * This function is called by any User's 'send' method.
     */
    void addMessage(const std::string& sender, const std::string& content) {
        {
            // Lock the mutex before modifying the shared vector
            std::lock_guard<std::mutex> lock(mtx);
            messages.push_back(Message(sender, content));
        } // Lock is released here

        // Notify all waiting threads that a new message is available
        cv.notify_all();
    }

    /**
     * @brief A special signal to wake up all threads.
     * Used during shutdown to unblock waiting threads.
     */
    void poke() {
        cv.notify_all();
    }

    /**
     * @brief Listens for new messages (Consumer side).
     * This is the core of the consumer/receiver logic.
     * Each User thread calls this function in a loop.
     *
     * @param user The User who is listening.
     * @param lastReadIndex The last message index read by this specific user.
     * Passed by reference to be updated.
     */
    void listen(User* user, size_t& lastReadIndex); // Implementation after User class
};

// -----------------------------------------------------------------
// CLASS: User
// Represents a virtual chat user. Each User instance runs
// its own receiver thread.
// -----------------------------------------------------------------
class User {
private:
    std::string name;
    ChatRoom* chatRoom;          // Pointer to the central room
    size_t lastReadIndex;      // This user's personal "read cursor"
    std::thread receiverThread;
    std::atomic<bool> running; // Controls the receiver thread's lifecycle

    /**
     * @brief The function that runs on this User's private thread.
     * It continuously calls chatRoom->listen() to wait for and
     * print new messages.
     */
    void receiveLoop() {
        lastReadIndex = 0; // Start reading from the beginning
        while (running) {
            // This call will block until new messages are available
            // or until the 'running' flag is set to false.
            chatRoom->listen(this, lastReadIndex);
        }
    }

public:
    User(const std::string& n, ChatRoom* room)
        : name(n), chatRoom(room), lastReadIndex(0), running(true) {
        // Start the receiver thread as soon as the User is created
        receiverThread = std::thread(&User::receiveLoop, this);
    }

    ~User() {
        // Ensure the thread is stopped upon destruction
        stop();
    }

    /**
     * @brief Stops the receiver thread and joins it.
     */
    void stop() {
        // Set 'running' to false. Use exchange to ensure it only runs once.
        bool alreadyStopped = !running.exchange(false);
        if (alreadyStopped) return;

        // Wake up the thread from its cv.wait()
        chatRoom->poke();

        // Wait for the thread to finish execution
        if (receiverThread.joinable()) {
            receiverThread.join();
        }
    }

    /**
     * @brief Sends a message *from* this user *to* the chat room.
     */
    void send(const std::string& content) {
        chatRoom->addMessage(name, content);
    }

    // --- Getters ---
    const std::string& getName() const { return name; }
    bool isRunning() const { return running; }
};

// -----------------------------------------------------------------
// Implementation of ChatRoom::listen
// (Must be defined *after* the User class is fully defined)
// -----------------------------------------------------------------
void ChatRoom::listen(User* user, size_t& lastReadIndex) {
    // Acquire a unique lock, which can be locked and unlocked.
    // std::unique_lock is required for std::condition_variable::wait
    std::unique_lock<std::mutex> lock(mtx);

    // Wait blocks the current thread until the predicate is true.
    // The predicate checks for two conditions:
    // 1. Are there new messages? (lastReadIndex < messages.size())
    // 2. Is the user thread supposed to shut down? (!user->isRunning())
    // This atomic check prevents a "lost wakeup" on shutdown.
    cv.wait(lock, [this, &lastReadIndex, user] {
        return lastReadIndex < messages.size() || !user->isRunning();
    });

    // If we woke up because we're shutting down, exit immediately.
    if (!user->isRunning()) {
        return;
    }

    // 
    // Otherwise, we woke up for new messages.
    // Print all new messages since our last read index.
    while (lastReadIndex < messages.size()) {
        const Message& msg = messages[lastReadIndex];

        // Don't print messages that this user sent themselves
        if (msg.sender != user->getName()) {
            // This std::cout simulates this user's private terminal window
            std::cout << "[" << user->getName() << "'s Terminal] "
                << msg.sender << ": " << msg.content << std::endl;
        }
        lastReadIndex++; // Update this user's read cursor
    }
    // The unique_lock is automatically released when it goes out of scope
}

// -----------------------------------------------------------------
// CLASS: ChatSimulator
// Controls the simulation, creates the users, and handles
// the main application lifecycle.
// -----------------------------------------------------------------
class ChatSimulator {
private:
    ChatRoom chatRoom;
    std::vector<std::unique_ptr<User>> users;
    int currentUserIndex; // Track the currently controlled user

public:
    ChatSimulator() : currentUserIndex(0) {} // Default to User1 (index 0)

    /**
     * @brief Prints a list of all available users and their indices.
     */
    void listUsers() {
        std::cout << "--- Available Users ---" << std::endl;
        for (size_t i = 0; i < users.size(); ++i) {
            std::cout << "  Index " << (i + 1) << ": " << users[i]->getName() << std::endl;
        }
        std::cout << "-----------------------" << std::endl;
    }

    /**
     * @brief Attempts to switch the active user.
     * @param input The raw command (e.g., "/switch 2")
     */
    void switchUser(const std::string& input) {
        try {
            // Extract the number part (e.g., "2" from "/switch 2")
            // The index is 1-based for the user, so we subtract 1.
            int newIndex = std::stoi(input.substr(8)) - 1;

            if (newIndex >= 0 && newIndex < users.size()) {
                currentUserIndex = newIndex;
                std::cout << "--- Switched control to "
                          << users[currentUserIndex]->getName() << " ---" << std::endl;
            } else {
                std::cout << "--- Error: Invalid user index. ---" << std::endl;
                listUsers();
            }
        } catch (const std::exception& e) {
            std::cout << "--- Error: Invalid command format. Use /switch <number> ---" << std::endl;
        }
    }

    void run() {
        std::cout << "🚀 Starting chat simulation..." << std::endl;

        // Create the users. Their constructors automatically start their threads.
        users.push_back(std::make_unique<User>("User1", &chatRoom));
        users.push_back(std::make_unique<User>("User2", &chatRoom));
        users.push_back(std::make_unique<User>("User3", &chatRoom));

        std::cout << "Virtual users are now listening in separate threads." << std::endl;
        std::cout << "---------------------------------------------------" << std::endl;

        // Give threads a moment to start and wait
        std::this_thread::sleep_for(std::chrono::milliseconds(250));

        // --- SIMULATED CHAT ---
        users[0]->send("Hello everyone! This is User1.");
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        users[1]->send("Hi User1! This is User2. Good to be here.");
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        users[2]->send("Hey all, User3 checking in.");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        // --- MAIN INTERACTION LOOP ---
        std::cout << "\n--- Interactive Mode ---" << std::endl;
        std::cout << "Commands:" << std::endl;
        std::cout << "  /list          - Show all users" << std::endl;
        std::cout << "  /switch <num>  - Switch to user (e.g., /switch 2)" << std::endl;
        std::cout << "  exit           - Quit the simulation" << std::endl;
        std::cout << "------------------------" << std::endl;

        std::string input;
        while (true) {
            // Update prompt to show the current user
            std::cout << "[Your Turn (" << users[currentUserIndex]->getName()
                      << ")]: ";
            std::getline(std::cin, input);

            if (input == "exit") {
                break;
            } else if (input == "/list") {
                listUsers();
            } else if (input.rfind("/switch ", 0) == 0) { // Check for "/switch "
                switchUser(input);
            } else if (!input.empty()) {
                // Send the message as the currently selected user
                users[currentUserIndex]->send(input);

                // Simple "AI" response from User2 if it gets a question
                // from someone *other* than itself.
                if (currentUserIndex != 1 && // Not sent by User2
                    input.find('?') != std::string::npos &&
                    users.size() > 1) {
                    
                    // Launch a detached thread for an automated reply
                    std::thread([&]() {
                        std::this_thread::sleep_for(std::chrono::milliseconds(750));
                        users[1]->send("That's a great question! I'm not sure.");
                    }).detach();
                }
            }
        }

        std::cout << "\nShutting down simulation..." << std::endl;
        users.clear(); // Calls destructors, which stop() and join() threads
        std::cout << "All threads joined. Simulation complete." << std::endl;
    }
};

// -----------------------------------------------------------------
// MAIN FUNCTION
// -----------------------------------------------------------------
int main() {
    ChatSimulator simulator;
    simulator.run();
    return 0;
}