#include <iostream>      // For console I/O (cout, cerr)
#include <vector>        // For the frameManager
#include <unordered_map> // For all the mapping tables
#include <string>        // For error message strings
#include <stdexcept>     // For exceptions (runtime_error, out_of_range)
#include <cstddef>       // For size_t
#include <iomanip>       // For output formatting (setw, left)

// --- 1. Constants ---
// (Replaces Constants.h)

// 4KB page size
constexpr size_t PAGE_SIZE = 4096;
constexpr size_t FRAME_SIZE = PAGE_SIZE;

// 1MB of total physical memory
constexpr size_t TOTAL_PHYSICAL_MEMORY = 1024 * 1024;

// Total number of available frames
constexpr size_t NUM_FRAMES = TOTAL_PHYSICAL_MEMORY / FRAME_SIZE;

// --- 2. PageTable Class ---
// (Replaces PageTable.h and PageTable.cpp)

class PageTable
{
private:
    // Maps a logical Page Number to a physical Frame Number
    std::unordered_map<int, int> pageToFrameMap;

    // Total number of pages in the segment (for bounds checking)
    size_t segmentSizeInPages;

public:
    // Default constructor for map compatibility
    PageTable() : segmentSizeInPages(0) {}

    // Constructor
    explicit PageTable(size_t sizeInPages) : segmentSizeInPages(sizeInPages) {}

    /**
     * Maps a logical page number to a physical frame number.
     * This simulates the OS loading a page into memory.
     */
    void mapPageToFrame(int pageNum, int frameNum)
    {
        if (pageNum < 0 || pageNum >= segmentSizeInPages)
        {
            throw std::out_of_range(
                "Page Fault: Page number " + std::to_string(pageNum) +
                " is out of bounds for this segment (Max: " +
                std::to_string(segmentSizeInPages - 1) + ").");
        }
        pageToFrameMap[pageNum] = frameNum;
    }

    /**
     * Gets the frame number for a given page number.
     * @throws std::runtime_error if page is out of bounds or not in memory.
     */
    int getFrame(int pageNum) const
    {
        if (pageNum < 0 || pageNum >= segmentSizeInPages)
        {
            throw std::out_of_range(
                "Page Fault: Page number " + std::to_string(pageNum) +
                " is out of bounds for this segment (Max: " +
                std::to_string(segmentSizeInPages - 1) + ").");
        }

        if (pageToFrameMap.find(pageNum) == pageToFrameMap.end())
        {
            throw std::runtime_error(
                "Page Fault: Page " + std::to_string(pageNum) +
                " is not currently loaded into physical memory.");
        }

        return pageToFrameMap.at(pageNum);
    }

    /**
     * Gets the total number of pages this segment is allowed to have.
     */
    size_t getTotalPages() const
    {
        return segmentSizeInPages;
    }

    /**
     * Prints all the page-to-frame mappings in this table.
     */
    void printTable() const
    {
        std::cout << std::left << std::setw(10) << "  Page #" << " | "
                  << std::setw(10) << "Frame #" << "\n";
        std::cout << "  ----------+----------\n";
        for (const auto &entry : pageToFrameMap)
        {
            std::cout << "  " << std::setw(10) << entry.first << "| "
                      << std::setw(10) << entry.second << "\n";
        }
    }
};

// --- 3. SegmentTable Class ---
// (Replaces SegmentTable.h and SegmentTable.cpp)

class SegmentTable
{
private:
    // Maps a Segment Number to its corresponding PageTable
    std::unordered_map<int, PageTable> segmentMap;

public:
    /**
     * Adds a new segment to the table.
     * @param segmentNum The ID of the segment (e.g., 0 for code, 1 for stack).
     * @param segmentSizeInBytes The total logical size of the segment.
     */
    void addSegment(int segmentNum, size_t segmentSizeInBytes)
    {
        if (segmentMap.find(segmentNum) != segmentMap.end())
        {
            std::cerr << "Warning: Segment " << segmentNum
                      << " already exists. Overwriting.\n";
        }

        // Calculate number of pages needed (ceiling division)
        size_t numPages = (segmentSizeInBytes + PAGE_SIZE - 1) / PAGE_SIZE;

        segmentMap[segmentNum] = PageTable(numPages);
    }

    /**
     * Gets a reference to the PageTable for a given segment.
     * @throws std::runtime_error if segmentNum is invalid (Segmentation Fault).
     */
    PageTable &getPageTable(int segmentNum)
    {
        if (segmentMap.find(segmentNum) == segmentMap.end())
        {
            throw std::runtime_error(
                "Segmentation Fault: Invalid segment number " +
                std::to_string(segmentNum) + ".");
        }
        return segmentMap.at(segmentNum);
    }

    /**
     * Gets a const reference for non-mutating operations.
     * @throws std::runtime_error if segmentNum is invalid (Segmentation Fault).
     */
    const PageTable &getPageTable(int segmentNum) const
    {
        if (segmentMap.find(segmentNum) == segmentMap.end())
        {
            throw std::runtime_error(
                "Segmentation Fault: Invalid segment number " +
                std::to_string(segmentNum) + ".");
        }
        return segmentMap.at(segmentNum);
    }

    /**
     * Prints all segments and their page tables.
     */
    void printTable() const
    {
        std::cout << "--- Segment Table ---\n";
        for (const auto &entry : segmentMap)
        {
            std::cout << "Segment " << entry.first
                      << " (Total Pages: " << entry.second.getTotalPages() << ")\n";
            entry.second.printTable();
            std::cout << "\n";
        }
    }
};

// --- 4. Process Class ---
// (Replaces Process.h and Process.cpp)

class Process
{
private:
    int pid;
    SegmentTable segmentTable;

public:
    // Default constructor for map compatibility
    Process() : pid(-1) {}

    explicit Process(int processId) : pid(processId) {}

    int getPid() const
    {
        return pid;
    }

    SegmentTable &getSegmentTable()
    {
        return segmentTable;
    }

    const SegmentTable &getSegmentTable() const
    {
        return segmentTable;
    }
};

// --- 5. MemoryManagementSystem Class ---
// (Replaces MemoryManagementSystem.h and MemoryManagementSystem.cpp)

class MemoryManagementSystem
{
private:
    // Simulates physical memory frames. true = used, false = free
    std::vector<bool> frameManager;

    // Map of all active processes
    std::unordered_map<int, Process> processMap;

    /**
     * Finds the first available free frame in physical memory.
     * @return The frame number.
     * @throws std::runtime_error if no free frames are available.
     */
    int allocateFrame()
    {
        for (size_t i = 0; i < frameManager.size(); ++i)
        {
            if (!frameManager[i])
            {
                frameManager[i] = true; // Mark as used
                return static_cast<int>(i);
            }
        }
        throw std::runtime_error("Out of Physical Memory: No free frames available.");
    }

public:
    MemoryManagementSystem()
    {
        // Initialize all frames as free
        frameManager.resize(NUM_FRAMES, false);
    }

    /**
     * Creates a new process and adds it to the system.
     */
    void createProcess(int pid)
    {
        if (processMap.find(pid) != processMap.end())
        {
            std::cerr << "Warning: Process " << pid << " already exists.\n";
            return;
        }
        processMap[pid] = Process(pid);
        std::cout << "System: Created Process " << pid << "\n";
    }

    /**
     * Adds a new segment to a specified process.
     */
    void addSegment(int pid, int segmentNum, size_t segmentSizeInBytes)
    {
        if (processMap.find(pid) == processMap.end())
        {
            throw std::runtime_error("Error: Process " + std::to_string(pid) + " not found.");
        }
        processMap.at(pid).getSegmentTable().addSegment(segmentNum, segmentSizeInBytes);
        std::cout << "System: Added Segment " << segmentNum << " (size: "
                  << segmentSizeInBytes << " bytes) to Process " << pid << "\n";
    }

    /**
     * Simulates loading a page from disk into a physical frame.
     * This allocates a frame and updates the process's page table.
     */
    void loadPageToMemory(int pid, int segmentNum, int pageNum)
    {
        try
        {
            // 1. Allocate a physical frame
            int newFrame = allocateFrame();

            // 2. Get the process's page table for this segment
            PageTable &pageTable = processMap.at(pid)
                                       .getSegmentTable()
                                       .getPageTable(segmentNum);

            // 3. Map the page to the frame
            pageTable.mapPageToFrame(pageNum, newFrame);

            std::cout << "System: Loaded Page " << pageNum << " (Segment "
                      << segmentNum << ", PID " << pid
                      << ") into Frame " << newFrame << "\n";
        }
        catch (const std::exception &e)
        {
            std::cerr << "System Error during page load: " << e.what() << "\n";
            // Note: In a real OS, if allocation failed, we'd swap out a page.
        }
    }

    /**
     * The core translation logic.
     * Converts a logical address (segment, page, offset) to a physical address.
     * @return The physical memory address.
     * @throws std::runtime_error for any translation faults.
     */
    long long translateAddress(int pid, int segmentNum, int pageNum, int offset)
    {
        std::cout << "Translating (PID " << pid << ", Seg " << segmentNum
                  << ", Page " << pageNum << ", Offset " << offset << "): ";

        // 1. Validate Offset
        if (offset < 0 || offset >= PAGE_SIZE)
        {
            throw std::runtime_error(
                "Address Fault: Offset " + std::to_string(offset) +
                " is larger than PAGE_SIZE (" + std::to_string(PAGE_SIZE) + ").");
        }

        // 2. Find Process
        if (processMap.find(pid) == processMap.end())
        {
            throw std::runtime_error("Address Fault: Process " + std::to_string(pid) + " not found.");
        }
        Process &process = processMap.at(pid);

        // 3. Find Segment Table -> Get Page Table
        //    (This throws Segmentation Fault if segmentNum is invalid)
        PageTable &pageTable = process.getSegmentTable().getPageTable(segmentNum);

        // 4. Find Page Table -> Get Frame
        //    (This throws Page Fault if pageNum is invalid or not loaded)
        int frameNum = pageTable.getFrame(pageNum);

        // 5. Calculate Physical Address
        //    PhysicalAddress = (FrameNumber * FrameSize) + Offset
        long long physicalAddress = (static_cast<long long>(frameNum) * FRAME_SIZE) + offset;

        return physicalAddress;
    }

    /**
     * Prints the complete memory map for a given process.
     */
    void printProcessMemoryMap(int pid) const
    {
        if (processMap.find(pid) == processMap.end())
        {
            std::cerr << "Error: Process " << pid << " not found.\n";
            return;
        }
        std::cout << "\n=============================================\n";
        std::cout << "MEMORY MAP FOR PROCESS " << pid << "\n";
        std::cout << "=============================================\n";
        processMap.at(pid).getSegmentTable().printTable();
        std::cout << "---------------------------------------------\n";
    }
};

// --- 6. Main Function ---
// (Replaces main.cpp)

/**
 * @brief Helper function to safely test address translations.
 */
void tryTranslation(MemoryManagementSystem &mmu, int pid, int seg, int page, int offset)
{
    try
    {
        long long pa = mmu.translateAddress(pid, seg, page, offset);
        std::cout << "-> SUCCESS: Physical Address = " << pa << "\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "-> FAILED: " << e.what() << "\n";
    }
}

int main()
{
    std::cout << "=== Segmentation + Paging MMU Simulator (Single File) ===\n";
    std::cout << "Page Size: " << PAGE_SIZE << " bytes\n";
    std::cout << "Total Physical Memory: " << TOTAL_PHYSICAL_MEMORY << " bytes\n";
    std::cout << "Total Frames: " << NUM_FRAMES << "\n\n";

    MemoryManagementSystem mmu;

    // 1. Create a process
    mmu.createProcess(101);

    // 2. Add segments to the process
    // Segment 0: Code (10000 bytes -> needs 3 pages)
    mmu.addSegment(101, 0, 10000);
    // Segment 1: Stack (8000 bytes -> needs 2 pages)
    mmu.addSegment(101, 1, 8000);
    // Segment 2: Heap (5000 bytes -> needs 2 pages)
    mmu.addSegment(101, 2, 5000);

    std::cout << "\n";

    // 3. Simulate the OS loading pages into memory (e.g., on-demand)
    mmu.loadPageToMemory(101, 0, 0); // Load Code Page 0
    mmu.loadPageToMemory(101, 0, 1); // Load Code Page 1
    mmu.loadPageToMemory(101, 1, 0); // Load Stack Page 0
    mmu.loadPageToMemory(101, 2, 1); // Load Heap Page 1

    // 4. Print the memory map for the process
    mmu.printProcessMemoryMap(101);

    // 5. Run address translation tests
    std::cout << "=== Address Translation Tests ===\n";

    // --- Test 1: Valid Address ---
    // Accessing Segment 0 (Code), Page 1, Offset 1234
    tryTranslation(mmu, 101, 0, 1, 1234);

    // --- Test 2: Valid Address ---
    // Accessing Segment 1 (Stack), Page 0, Offset 50
    tryTranslation(mmu, 101, 1, 0, 50);

    // --- Test 3: Page Fault (Page not loaded) ---
    // Accessing Segment 0 (Code), Page 2 (valid page, but not loaded)
    tryTranslation(mmu, 101, 0, 2, 100);

    // --- Test 4: Page Fault (Page out of bounds) ---
    // Accessing Segment 1 (Stack), Page 2 (Segment 1 only has pages 0, 1)
    tryTranslation(mmu, 101, 1, 2, 200);

    // --- Test 5: Segmentation Fault (Invalid segment) ---
    // Accessing Segment 5 (does not exist)
    tryTranslation(mmu, 101, 5, 0, 300);

    // --- Test 6: Offset Fault (Offset > PAGE_SIZE) ---
    // Accessing Segment 0, Page 1, Offset 9999
    tryTranslation(mmu, 101, 0, 1, 9999);

    // --- Test 7: Invalid PID ---
    tryTranslation(mmu, 999, 0, 0, 10);

    return 0;
}