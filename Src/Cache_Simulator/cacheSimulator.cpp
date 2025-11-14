#include <bits/stdc++.h>
using namespace std;

//  Cache Block
struct CacheBlock
{
    unsigned long long tag;
    bool valid;
    CacheBlock() : tag(0), valid(false) {}
};

// REPLACEMENT POLICY INTERFACE
class ReplacementPolicy
{
public:
    virtual int chooseVictim(int setIndex, const vector<unsigned long long> &future = {}) = 0;
    virtual void onAccess(int setIndex, int lineIndex) = 0;
    virtual void onInsert(int setIndex, int lineIndex) = 0;
    virtual void reset() = 0;
    virtual ~ReplacementPolicy() {}
};

// FIRST IN FIRST OUT (FIFO) ALGORITHM
class FIFOReplacement : public ReplacementPolicy
{
    vector<vector<int>> order;

public:
    FIFOReplacement(int sets, int ways) { order.assign(sets, {}); }
    int chooseVictim(int setIndex, const vector<unsigned long long> & = {})
    {
        int victim = order[setIndex].front();
        order[setIndex].erase(order[setIndex].begin());
        return victim;
    }
    void onAccess(int, int) {}
    void onInsert(int setIndex, int lineIndex) { order[setIndex].push_back(lineIndex); }
    void reset()
    {
        for (auto &v : order)
            v.clear();
    }
};

// LEAST RECENTLY USED (LRU) ALGORITHM
class LRUReplacement : public ReplacementPolicy
{
    vector<vector<int>> recent;

public:
    LRUReplacement(int sets, int ways) { recent.assign(sets, {}); }
    int chooseVictim(int setIndex, const vector<unsigned long long> & = {})
    {
        int victim = recent[setIndex].front();
        recent[setIndex].erase(recent[setIndex].begin());
        return victim;
    }
    void onAccess(int setIndex, int lineIndex)
    {
        auto &r = recent[setIndex];
        r.erase(remove(r.begin(), r.end(), lineIndex), r.end());
        r.push_back(lineIndex);
    }
    void onInsert(int setIndex, int lineIndex) { onAccess(setIndex, lineIndex); }
    void reset()
    {
        for (auto &r : recent)
            r.clear();
    }
};

// LEAST FREQUENTLY USED (LFU) ALGORITHM
class LFUReplacement : public ReplacementPolicy
{
    vector<vector<int>> freq;

public:
    LFUReplacement(int sets, int ways) { freq.assign(sets, vector<int>(ways, 0)); }
    int chooseVictim(int setIndex, const vector<unsigned long long> & = {})
    {
        int victim = 0, minFreq = freq[setIndex][0];
        for (int i = 1; i < (int)freq[setIndex].size(); i++)
            if (freq[setIndex][i] < minFreq)
            {
                minFreq = freq[setIndex][i];
                victim = i;
            }
        return victim;
    }
    void onAccess(int setIndex, int lineIndex) { freq[setIndex][lineIndex]++; }
    void onInsert(int setIndex, int lineIndex) { freq[setIndex][lineIndex] = 1; }
    void reset()
    {
        for (auto &f : freq)
            fill(f.begin(), f.end(), 0);
    }
};

// BELADY (Optimal) ALGORITHM
class BeladyReplacement : public ReplacementPolicy
{
    vector<vector<unsigned long long>> *cacheRefs;
    vector<vector<CacheBlock>> *cacheSets;
    vector<unsigned long long> *trace;
    size_t currentIndex;

public:
    BeladyReplacement(int, int, vector<vector<unsigned long long>> *refs,
                      vector<vector<CacheBlock>> *sets,
                      vector<unsigned long long> *t)
        : cacheRefs(refs), cacheSets(sets), trace(t), currentIndex(0) {}

    void setCurrentIndex(size_t idx) { currentIndex = idx; }

    int chooseVictim(int setIndex, const vector<unsigned long long> & = {}) override
    {
        auto &set = (*cacheSets)[setIndex];
        int victim = -1;
        size_t farthest = 0;

        for (int i = 0; i < (int)set.size(); i++)
        {
            unsigned long long tag = set[i].tag;
            size_t nextUse = SIZE_MAX;

            for (size_t j = currentIndex + 1; j < trace->size(); j++)
            {
                unsigned long long addr = (*trace)[j];
                unsigned long long block = addr / cacheRefs->at(0).size();
                unsigned long long tagFuture = block / cacheRefs->size();
                int setFuture = block % cacheRefs->size();

                if (setFuture == setIndex && tagFuture == tag)
                {
                    nextUse = j;
                    break;
                }
            }
            if (nextUse == SIZE_MAX)
                return i; // never used again
            if (nextUse > farthest)
            {
                farthest = nextUse;
                victim = i;
            }
        }
        return victim == -1 ? 0 : victim;
    }

    void onAccess(int, int) {}
    void onInsert(int, int) {}
    void reset() {}
};

//CACHE CLASS
class Cache
{
private:
    int cacheSize, blockSize, associativity, numSets;
    vector<vector<CacheBlock>> sets;
    ReplacementPolicy *policy;
    unsigned long long hits, misses, accesses;
    string policyName;

public:
    Cache(int c, int b, int a, string policyType, vector<unsigned long long> *trace = nullptr)
        : cacheSize(c), blockSize(b), associativity(a),
          hits(0), misses(0), accesses(0), policyName(policyType)
    {
        numSets = cacheSize / (blockSize * associativity);
        sets.resize(numSets, vector<CacheBlock>(associativity));

        for (auto &c : policyType)
            c = toupper(c);
        if (policyType == "FIFO")
            policy = new FIFOReplacement(numSets, associativity);
        else if (policyType == "LRU")
            policy = new LRUReplacement(numSets, associativity);
        else if (policyType == "LFU")
            policy = new LFUReplacement(numSets, associativity);
        else
            policy = new BeladyReplacement(numSets, associativity, nullptr, &sets, trace);
    }

    pair<int, unsigned long long> decode(unsigned long long address)
    {
        unsigned long long blockNumber = address / blockSize;
        int setIndex = blockNumber % numSets;
        unsigned long long tag = blockNumber / numSets;
        return {setIndex, tag};
    }

    bool access(unsigned long long address, size_t index, vector<unsigned long long> *trace = nullptr)
    {
        accesses++;
        auto decoded = decode(address);
        int setIndex = decoded.first;
        unsigned long long tag = decoded.second;

        auto &set = sets[setIndex];

        // Check for hit
        for (int i = 0; i < associativity; i++)
        {
            if (set[i].valid && set[i].tag == tag)
            {
                hits++;
                policy->onAccess(setIndex, i);
                return true;
            }
        }

        // Miss
        misses++;
        int victim = -1;
        for (int i = 0; i < associativity; i++)
            if (!set[i].valid)
            {
                victim = i;
                break;
            }

        if (victim == -1)
        {
            if (policyName == "BELADY")
            {
                ((BeladyReplacement *)policy)->setCurrentIndex(index);
            }
            victim = policy->chooseVictim(setIndex);
        }

        set[victim].valid = true;
        set[victim].tag = tag;
        policy->onInsert(setIndex, victim);
        return false;
    }

    void printCacheState()
    {
        for (int s = 0; s < numSets; s++)
        {
            cout << "Set " << setw(2) << s << ": ";
            for (int w = 0; w < associativity; w++)
                cout << (sets[s][w].valid ? "[T" + to_string(sets[s][w].tag) + "] " : "[ ] ");
            cout << "\n";
        }
    }

    void showStats()
    {
        double missRate = (double)misses / max(accesses, 1ULL);
        double hitRate = 1.0 - missRate;
        double AMAT = 1 + missRate * 100; // let us assume miss penalty 100 cycles

        cout << fixed << setprecision(2);
        cout << "\nAccesses: " << accesses
             << " | Hits: " << hits
             << " | Misses: " << misses << endl;
        cout << "Hit Rate: " << hitRate * 100 << "%  Miss Rate: " << missRate * 100 << "%\n";
        cout << "Average Memory Access Time (AMAT): " << AMAT << " cycles\n";
    }
};

// Simulator starts from here
class CacheSimulator
{
private:
    int cacheSize, blockSize, associativity;
    string policyName;
    vector<unsigned long long> trace;

public:
    bool loadFromFile(string filename)
    {
        ifstream fin(filename);
        if (!fin.is_open())
        {
            cerr << "Error: Could not open " << filename << endl;
            return false;
        }
        string token;
        while (fin >> token)
        {
            if (token == "CACHE_SIZE")
                fin >> cacheSize;
            else if (token == "BLOCK_SIZE")
                fin >> blockSize;
            else if (token == "ASSOCIATIVITY")
                fin >> associativity;
            else if (token == "POLICY")
                fin >> policyName;
            else if (token == "ACCESSES")
            {
                unsigned long long addr;
                while (fin >> addr)
                    trace.push_back(addr);
            }
        }
        fin.close();
        return true;
    }

    void run()
    {
        Cache cache(cacheSize, blockSize, associativity, policyName, &trace);
        cout << "\nCACHE CONFIGURATION:\n";
        cout << "Cache Size: " << cacheSize << "B, Block: " << blockSize
             << "B, Assoc: " << associativity
             << "-way, Policy: " << policyName << "\n\n";

        for (size_t i = 0; i < trace.size(); i++)
        {
            bool hit = cache.access(trace[i], i, &trace);
            cout << "Access " << setw(2) << i + 1
                 << " | Addr: " << setw(6) << trace[i]
                 << " | " << (hit ? "HIT" : "MISS") << "\n";
            cache.printCacheState();
            cout << "---------------------------------------------\n";
        }

        cache.showStats();
        cout << "\nSimulation Complete.\n";
    }
};

// DRIVER CODE
int main()
{
    CacheSimulator sim;
    if (sim.loadFromFile("cacheData.txt"))
        sim.run();
    return 0;
}
