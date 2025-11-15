/*
================================================================================
 C++ OOP Subnetting Simulator
================================================================================
 
 This program demonstrates Object-Oriented Programming by simulating a 
 computer network. It allows you to:
 1. Define a main network (e.g., 192.168.0.0/24).
 2. Automatically "subnet" it into smaller networks.
 3. Create virtual devices (PCs).
 4. Assign available IPs from subnets to those devices.
 5. Simulate a "ping" to see if devices are on the same subnet.

 How to Compile:
 g++ -std=c++17 -o subnet_sim subnet_simulator.cpp
 ./subnet_sim
*/

#include <iostream>  // For std::cout, std::cerr
#include <string>    // For std::string
#include <vector>    // For std::vector
#include <map>       // For std::map
#include <memory>    // For std::unique_ptr, std::make_unique, std::move
#include <sstream>   // For std::stringstream
#include <stdexcept> // For std::runtime_error
#include <cstdint>   // For uint32_t (32-bit integer)
#include <cmath>     // For std::ceil, std::log2, std::pow
#include <iomanip>   // For std::setw, std::left

// ===== 1. IPAddress Class =====
// Encapsulates all the logic and bitwise math for a single IPv4 address.

class IPAddress {
private:
    // Stores the IP as a single 32-bit number (e.g., 192.168.1.1 is 3232235777)
    // This makes subnet math *much* easier.
    uint32_t m_address;

    /**
     * @brief Private helper to convert "192.168.1.1" string to 3232235777.
     */
    uint32_t stringToInt(const std::string& ip_string) {
        std::stringstream ss(ip_string);
        std::string segment;
        uint32_t ip_int = 0;

        for (int i = 0; i < 4; ++i) {
            if (!std::getline(ss, segment, '.')) {
                throw std::runtime_error("Invalid IP string format");
            }
            int octet = std::stoi(segment);
            if (octet < 0 || octet > 255) {
                throw std::runtime_error("Invalid IP octet value");
            }
            // Bitwise magic: shift the octet to its correct position
            // 1st octet: shift left 24 bits
            // 2nd octet: shift left 16 bits
            // 3rd octet: shift left 8 bits
            // 4th octet: no shift
            ip_int = (ip_int << 8) | octet;
        }
        return ip_int;
    }

public:
    // Default constructor (0.0.0.0)
    IPAddress() : m_address(0) {}

    // Constructor from a 32-bit integer
    IPAddress(uint32_t ip_int) : m_address(ip_int) {}

    // Main constructor from a string like "192.168.1.1"
    IPAddress(const std::string& ip_string) {
        m_address = stringToInt(ip_string);
    }

    /**
     * @brief Converts the internal 32-bit int back to "192.168.1.1" format.
     */
    std::string toString() const {
        std::stringstream ss;
        ss << ((m_address >> 24) & 0xFF) << "."
           << ((m_address >> 16) & 0xFF) << "."
           << ((m_address >> 8) & 0xFF) << "."
           << (m_address & 0xFF);
        return ss.str();
    }

    /**
     * @brief Returns the raw 32-bit integer value.
     */
    uint32_t toInt() const {
        return m_address;
    }

    /**
     * @brief Performs a bitwise AND with a mask. Used to find a network address.
     */
    IPAddress applyMask(const IPAddress& mask) const {
        return IPAddress(m_address & mask.toInt());
    }

    /**
     * @brief Static utility to create a Subnet Mask from a CIDR number (e.g., 24).
     * /24 -> 255.255.255.0
     */
    static IPAddress getMaskFromCIDR(int cidr) {
        if (cidr < 0 || cidr > 32) {
            throw std::runtime_error("Invalid CIDR value");
        }
        if (cidr == 0) {
            return IPAddress(0U);
        }
        // Creates a 32-bit number with 'cidr' ones from the left
        // e.g., cidr=24: 11111111.11111111.11111111.00000000
        uint32_t mask_int = (0xFFFFFFFF << (32 - cidr));
        return IPAddress(mask_int);
    }

    // --- Operator Overloads for easy comparison and iteration ---
    bool operator==(const IPAddress& other) const {
        return m_address == other.m_address;
    }
    bool operator!=(const IPAddress& other) const {
        return m_address != other.m_address;
    }
    bool operator<(const IPAddress& other) const {
        return m_address < other.m_address;
    }
    bool operator<=(const IPAddress& other) const {
        return m_address <= other.m_address;
    }
    // Overload the ++ operator to easily get the next IP address
    IPAddress& operator++() {
        m_address++;
        return *this;
    }
};


// ===== 2. Subnet Class =====
// Encapsulates the logic of a subnet, including its range of valid IPs.

class Subnet {
private:
    IPAddress m_networkAddress;
    IPAddress m_subnetMask;
    int m_cidr;
    IPAddress m_broadcastAddress;
    IPAddress m_firstHost; // First usable IP
    IPAddress m_lastHost;  // Last usable IP

    /**
     * @brief Private helper to calculate network/host/broadcast addresses.
     */
    void calculateAddresses() {
        // Broadcast is the network address OR'd with the *inverted* mask
        // e.g., Net: 192.168.1.0, InvMask: 0.0.0.255 -> Bcast: 192.168.1.255
        uint32_t broadcast_int = m_networkAddress.toInt() | (~m_subnetMask.toInt());
        m_broadcastAddress = IPAddress(broadcast_int);

        // Host range is from (network + 1) to (broadcast - 1)
        // We handle the edge case of /31 and /32 networks (no usable hosts)
        if (m_cidr >= 31) {
            m_firstHost = IPAddress(0); // No valid hosts
            m_lastHost = IPAddress(0);  // No valid hosts
        } else {
            m_firstHost = IPAddress(m_networkAddress.toInt() + 1);
            m_lastHost = IPAddress(broadcast_int - 1);
        }
    }

public:
    /**
     * @brief Constructor from a CIDR string, e.g., "192.168.1.0/24".
     */
    Subnet(const std::string& cidr_string) {
        size_t slash_pos = cidr_string.find('/');
        if (slash_pos == std::string::npos) {
            throw std::runtime_error("Invalid CIDR string: missing '/'");
        }

        std::string ip_part = cidr_string.substr(0, slash_pos);
        std::string cidr_part = cidr_string.substr(slash_pos + 1);

        m_cidr = std::stoi(cidr_part);
        m_subnetMask = IPAddress::getMaskFromCIDR(m_cidr);
        
        // Ensure we store the *true* network address, not just what was passed
        IPAddress input_ip(ip_part);
        m_networkAddress = input_ip.applyMask(m_subnetMask);

        // Calculate all other addresses
        calculateAddresses();
    }

    // --- Public Getters ---

    IPAddress getNetworkAddress() const { return m_networkAddress; }
    IPAddress getBroadcastAddress() const { return m_broadcastAddress; }
    IPAddress getFirstHostAddress() const { return m_firstHost; }
    IPAddress getLastHostAddress() const { return m_lastHost; }
    std::string getMaskString() const { return m_subnetMask.toString(); }
    int getCIDR() const { return m_cidr; }
    
    int getTotalHosts() const {
        if (m_cidr >= 31) return 0;
        // 2^(32 - cidr) - 2
        return static_cast<int>(std::pow(2, 32 - m_cidr) - 2);
    }

    /**
     * @brief **Key Method:** Checks if a given IP is in this subnet's *usable host range*.
     */
    bool contains(const IPAddress& ip) const {
        if (m_cidr >= 31) return false; // No hosts
        return ip.toInt() >= m_firstHost.toInt() && ip.toInt() <= m_lastHost.toInt();
    }

    // Check if two Subnet objects represent the same network
    bool operator==(const Subnet& other) const {
        return m_networkAddress == other.m_networkAddress && m_cidr == other.m_cidr;
    }
};


// ===== 3. NetworkDevice Class =====
// Represents a single PC. It "has an" IP and "belongs to" a Subnet.

class NetworkDevice {
private:
    std::string m_hostname;
    std::unique_ptr<IPAddress> m_ip; // Device *owns* its IP configuration
    Subnet* m_subnet;                // Raw pointer; device *belongs to* a subnet, but doesn't own it.

public:
    NetworkDevice(const std::string& name) : m_hostname(name), m_ip(nullptr), m_subnet(nullptr) {}

    /**
     * @brief "Plugs" the device into a subnet with a specific IP.
     * Takes ownership of the IPAddress object.
     */
    void assign(std::unique_ptr<IPAddress> ip, Subnet* subnet) {
        m_ip = std::move(ip); // std::move transfers ownership
        m_subnet = subnet;
        std::cout << "  [Assign] Device " << m_hostname 
                  << " assigned IP " << m_ip->toString() 
                  << " in subnet " << m_subnet->getNetworkAddress().toString() 
                  << "/" << m_subnet->getCIDR() << "\n";
    }

    // --- Public Getters ---
    const IPAddress* getIP() const { return m_ip.get(); }
    const Subnet* getSubnet() const { return m_subnet; }
    std::string getName() const { return m_hostname; }

    /**
     * @brief The core simulation logic!
     * Checks if a target device is on the same subnet.
     */
    void ping(const NetworkDevice& target) const {
        std::cout << "\n[PING] From: " << this->m_hostname 
                  << " (" << (m_ip ? m_ip->toString() : "UNASSIGNED") << ")"
                  << "\n  -> To:   " << target.getName() 
                  << " (" << (target.getIP() ? target.getIP()->toString() : "UNASSIGNED") << ")\n";

        if (this->m_subnet == nullptr || target.getSubnet() == nullptr) {
            std::cout << "[RESULT] 🔴 FAILED. One or more devices are not connected to a subnet.\n";
            return;
        }

        // The main logic: do the subnet pointers point to the *same object*?
        if (this->m_subnet == target.getSubnet()) {
            std::cout << "[RESULT] 🟢 SUCCESS! Devices are on the same local subnet (" 
                      << this->m_subnet->getNetworkAddress().toString() << ").\n";
        } else {
            std::cout << "[RESULT] 🔴 FAILED. Devices are on different subnets."
                      << "\n         (Source: " << this->m_subnet->getNetworkAddress().toString()
                      << " vs Dest: " << target.getSubnet()->getNetworkAddress().toString() << ")"
                      << "\n         (Requires a router to communicate.)\n";
        }
    }
};


// ===== 4. Network Class =====
// The main manager class. It "owns" all the subnets and devices using smart pointers.

class Network {
private:
    // These vectors and maps *own* all the objects.
    std::vector<std::unique_ptr<Subnet>> m_subnets;
    std::map<std::string, std::unique_ptr<NetworkDevice>> m_devices; // Find device by name
    
    // Tracks which IPs are already in use.
    // We store the IP as an int for fast map lookups.
    std::map<uint32_t, NetworkDevice*> m_ip_allocations;

public:
    Network() {}

    /**
     * @brief Automatically carves a main network into smaller subnets.
     * @param main_network_cidr e.g., "192.168.1.0/24"
     * @param hosts_per_subnet The *minimum* number of hosts required per subnet.
     * @return true on success, false on failure.
     */
    bool designSubnets(const std::string& main_network_cidr, int min_hosts_per_subnet) {
        try {
            Subnet main_net(main_network_cidr);
            
            // 1. Find the smallest CIDR that can fit the required hosts.
            // We need 'min_hosts_per_subnet + 2' total addresses (for network and broadcast).
            // new_cidr = 32 - ceil(log2(hosts + 2))
            int host_bits = static_cast<int>(std::ceil(std::log2(min_hosts_per_subnet + 2)));
            int new_cidr = 32 - host_bits;

            if (new_cidr < main_net.getCIDR()) {
                std::cerr << "Error: Cannot create subnets larger than the main network.\n";
                return false;
            }

            // 2. Calculate how many subnets of this size we can make.
            int subnet_bits = new_cidr - main_net.getCIDR();
            int num_subnets = static_cast<int>(std::pow(2, subnet_bits));
            uint32_t subnet_size = static_cast<uint32_t>(std::pow(2, host_bits));

            std::cout << "[Network] Main net " << main_net.getNetworkAddress().toString() << "/" << main_net.getCIDR()
                      << " will be split into " << num_subnets << "x /" << new_cidr 
                      << " subnets (size: " << subnet_size << " IPs each).\n";

            // 3. Create all the new subnets.
            for (int i = 0; i < num_subnets; ++i) {
                uint32_t new_net_addr_int = main_net.getNetworkAddress().toInt() + (i * subnet_size);
                IPAddress new_net_ip(new_net_addr_int);
                
                std::string new_cidr_string = new_net_ip.toString() + "/" + std::to_string(new_cidr);
                m_subnets.push_back(std::make_unique<Subnet>(new_cidr_string));
            }
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error in designSubnets: " << e.what() << '\n';
            return false;
        }
    }

    /**
     * @brief Creates a new device and adds it to the network.
     * @return A raw pointer to the new device (the Network class still owns it).
     */
    NetworkDevice* createDevice(const std::string& name) {
        if (m_devices.count(name)) {
            std::cerr << "Error: Device with name " << name << " already exists.\n";
            return nullptr;
        }
        auto device = std::make_unique<NetworkDevice>(name);
        NetworkDevice* ptr = device.get(); // Get raw pointer before moving
        m_devices[name] = std::move(device);
        std::cout << "[Network] Created device: " << name << "\n";
        return ptr;
    }

    /**
     * @brief Finds a device by its name.
     */
    NetworkDevice* getDevice(const std::string& name) const {
        auto it = m_devices.find(name);
        if (it != m_devices.end()) {
            return it->second.get();
        }
        return nullptr;
    }
    
    /**
     * @brief Finds a subnet by its network IP.
     */
    Subnet* getSubnet(const std::string& subnet_network_ip) const {
        try {
            IPAddress search_ip(subnet_network_ip);
            for(const auto& subnet : m_subnets) {
                if (subnet->getNetworkAddress() == search_ip) {
                    return subnet.get();
                }
            }
        } catch(...) {}
        return nullptr;
    }

    /**
     * @brief Assigns the *next available* IP from a subnet to a device.
     */
    bool assignIPToDevice(const std::string& device_name, const std::string& subnet_network_ip) {
        NetworkDevice* device = getDevice(device_name);
        if (!device) {
            std::cerr << "Error: Device " << device_name << " not found.\n";
            return false;
        }
        
        Subnet* subnet = getSubnet(subnet_network_ip);
        if (!subnet) {
            std::cerr << "Error: Subnet " << subnet_network_ip << " not found.\n";
            return false;
        }
        
        // Find the first available IP in the subnet's host range
        for (IPAddress ip = subnet->getFirstHostAddress(); ip <= subnet->getLastHostAddress(); ++ip) {
            
            // Check if this IP is free
            if (m_ip_allocations.find(ip.toInt()) == m_ip_allocations.end()) {
                // IP is free! Assign it.
                auto ip_obj = std::make_unique<IPAddress>(ip);
                m_ip_allocations[ip.toInt()] = device; // Mark as allocated
                device->assign(std::move(ip_obj), subnet);
                return true;
            }
        }
        
        std::cerr << "Error: No available IPs in subnet " << subnet_network_ip << ".\n";
        return false;
    }

    /**
     * @brief Kicks off a ping simulation between two devices.
     */
    void simulatePing(const std::string& from_device, const std::string& to_device) const {
        NetworkDevice* src = getDevice(from_device);
        NetworkDevice* dst = getDevice(to_device);

        if (!src || !dst) {
            std::cerr << "Error: Cannot ping. One or both devices not found.\n";
            return;
        }
        src->ping(*dst);
    }
    
    /**
     * @brief Prints a report of all subnets and their assigned devices.
     */
    void printLayout() const {
        std::cout << "\n" << std::string(60, '=') << "\n";
        std::cout << " 🖥️  NETWORK LAYOUT REPORT\n";
        std::cout << std::string(60, '=') << "\n";
        
        for (const auto& subnet : m_subnets) {
            std::cout << "Subnet:  " << std::setw(15) << std::left << subnet->getNetworkAddress().toString() 
                      << " /" << subnet->getCIDR() 
                      << " (" << subnet->getTotalHosts() << " hosts)\n";
            std::cout << "  Mask:    " << subnet->getMaskString() << "\n";
            std::cout << "  Range:   " << subnet->getFirstHostAddress().toString()
                      << " -> " << subnet->getLastHostAddress().toString() << "\n";
            std::cout << "  Devices:\n";
            
            bool found_device = false;
            for(const auto& pair : m_devices) {
                NetworkDevice* dev = pair.second.get();
                if(dev->getSubnet() == subnet.get()) {
                    std::cout << "    - " << std::setw(15) << std::left << dev->getName()
                              << " (" << dev->getIP()->toString() << ")\n";
                    found_device = true;
                }
            }
            if (!found_device) {
                std::cout << "    (No devices assigned)\n";
            }
            std::cout << "------------------------------------------------------------\n";
        }
    }
};


// ===== 5. Main Function (Simulation) =====
// This is where we set up and run the simulation.

int main() {
    std::cout << "========================================\n";
    std::cout << " Virtual Subnet Simulator Initializing \n";
    std::cout << "========================================\n";

    Network myOffice;

    // 1. Carve up the main network "192.168.1.0/24"
    // We need at least 60 hosts per subnet (e.g., for Sales and Accounting).
    // The simulator will calculate that /26 (62 hosts) is the correct size.
    std::cout << "\n--- 1. Designing Network ---\n";
    myOffice.designSubnets("192.168.1.0/24", 60);
    // This will create 4 subnets:
    // - 192.168.1.0/26
    // - 192.168.1.64/26
    // - 192.168.1.128/26
    // - 192.168.1.192/26

    // 2. Create virtual devices
    std::cout << "\n--- 2. Creating Devices ---\n";
    myOffice.createDevice("PC-Accounting-01"); // Belongs in 1st subnet
    myOffice.createDevice("PC-Accounting-02"); // Belongs in 1st subnet
    myOffice.createDevice("PC-Sales-01");      // Belongs in 2nd subnet
    myOffice.createDevice("PC-Sales-02");      // Belongs in 2nd subnet
    myOffice.createDevice("PC-HR-01");         // Belongs in 3rd subnet

    // 3. Assign devices to subnets
    // The system automatically picks the *next available* IP in that subnet.
    std::cout << "\n--- 3. Assigning IPs ---\n";
    myOffice.assignIPToDevice("PC-Accounting-01", "192.168.1.0"); // Gets 192.168.1.1
    myOffice.assignIPToDevice("PC-Accounting-02", "192.168.1.0"); // Gets 192.168.1.2
    
    myOffice.assignIPToDevice("PC-Sales-01", "192.168.1.64");  // Gets 192.168.1.65
    myOffice.assignIPToDevice("PC-Sales-02", "192.168.1.64");  // Gets 192.168.1.66
    
    myOffice.assignIPToDevice("PC-HR-01", "192.168.1.128"); // Gets 192.168.1.129

    // 4. Print the final network map
    myOffice.printLayout();

    // 5. Run the ping simulations
    std::cout << "\n--- 4. Running Simulations ---\n";
    
    // Test 1: Two devices on the SAME subnet (Sales <-> Sales)
    // This should SUCCEED.
    myOffice.simulatePing("PC-Sales-01", "PC-Sales-02");

    // Test 2: Two devices on DIFFERENT subnets (Accounting <-> Sales)
    // This should FAIL.
    myOffice.simulatePing("PC-Accounting-01", "PC-Sales-01");
    
    // Test 3: Another pair on the SAME subnet (Accounting <-> Accounting)
    // This should SUCCEED.
    myOffice.simulatePing("PC-Accounting-02", "PC-Accounting-01");
    
    // Test 4: Another pair on DIFFERENT subnets (Sales <-> HR)
    // This should FAIL.
    myOffice.simulatePing("PC-Sales-01", "PC-HR-01");

    std::cout << "\n========================================\n";
    std::cout << "         Simulation Complete \n";
    std::cout << "========================================\n";

    return 0;
}