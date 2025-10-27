#include <bits/stdc++.h>
using namespace std;

const int INF = numeric_limits<int>::max();

class Graph
{
private:
    int V; // Number of vertices
    bool is_directed;

    vector<list<pair<int, int>>> adj;
    vector<int> in_degree;

    void DFSUtil(int v, vector<bool> &visited) const
    {
        visited[v] = true;
        cout << v << " ";

        for (const auto &neighbor : adj[v])
        {
            if (!visited[neighbor.first])
            {
                DFSUtil(neighbor.first, visited);
            }
        }
    }

    bool isCyclicUndirectedUtil(int v, vector<bool> &visited, int parent) const
    {
        visited[v] = true;

        for (const auto &neighbor : adj[v])
        {
            int neighbor_vertex = neighbor.first;

            if (!visited[neighbor_vertex])
            {
                if (isCyclicUndirectedUtil(neighbor_vertex, visited, v))
                    return true;
            }
            else if (neighbor_vertex != parent)
            {
                return true;
            }
        }
        return false;
    }

    bool isCyclicDirectedUtil(int v, vector<bool> &visited, vector<bool> &recStack) const
    {
        visited[v] = true;
        recStack[v] = true;

        for (const auto &neighbor : adj[v])
        {
            int neighbor_vertex = neighbor.first;
            if (!visited[neighbor_vertex])
            {
                if (isCyclicDirectedUtil(neighbor_vertex, visited, recStack))
                    return true;
            }
            else if (recStack[neighbor_vertex])
            {
                return true;
            }
        }
        recStack[v] = false;
        return false;
    }

    vector<int> dijkstra(int src) const
    {
        if (src < 0 || src >= V)
        {
            throw out_of_range("Source vertex for Dijkstra is out of range.");
        }
        priority_queue<pair<int, int>,
                       vector<pair<int, int>>,
                       greater<pair<int, int>>>
            pq;

        vector<int> dist(V, INF);
        pq.push({0, src});
        dist[src] = 0;

        while (!pq.empty())
        {
            int u = pq.top().second;
            int d = pq.top().first;
            pq.pop();

            if (d > dist[u])
                continue;

            for (const auto &neighbor : adj[u])
            {
                int v = neighbor.first;
                int weight = neighbor.second;

                if (dist[u] != INF && dist[u] + weight < dist[v])
                {
                    dist[v] = dist[u] + weight;
                    pq.push({dist[v], v});
                }
            }
        }
        return dist;
    }

public:
    Graph(int vertices, bool directed = false) : V(vertices), is_directed(directed)
    {
        if (vertices <= 0)
        {
            throw invalid_argument("Number of vertices must be positive.");
        }
        adj.resize(V);
        in_degree.resize(V, 0);
    }

    void addEdge(int u, int v, int weight = 1)
    {
        if (u < 0 || u >= V || v < 0 || v >= V)
        {
            throw out_of_range("Vertex index out of range. Vertices must be from 0 to " + to_string(V - 1) + ".");
        }
        adj[u].push_back({v, weight});
        if (is_directed)
        {
            in_degree[v]++;
        }
        else
        {
            adj[v].push_back({u, weight});
        }
    }

    void printGraph() const
    {
        cout << "\n--- Graph Adjacency List (V=" << V << ") ---" << endl;
        for (int i = 0; i < V; ++i)
        {
            cout << "Vertex " << i << ":";
            for (const auto &neighbor : adj[i])
            {
                cout << " -> (" << neighbor.first << ", w:" << neighbor.second << ")";
            }
            cout << endl;
        }
    }

    int getNumVertices() const { return V; }

    int getInDegree(int v) const
    {
        if (v < 0 || v >= V)
            throw out_of_range("Vertex index out of range.");
        if (is_directed)
        {
            return in_degree[v];
        }
        else
        {
            return adj[v].size();
        }
    }

    int getOutDegree(int v) const
    {
        if (v < 0 || v >= V)
            throw out_of_range("Vertex index out of range.");
        return adj[v].size();
    }

    double getDensity() const
    {
        long long E = 0;
        for (int i = 0; i < V; ++i)
        {
            E += adj[i].size();
        }

        if (V <= 1)
            return 0.0;

        if (is_directed)
        {
            long long max_E = (long long)V * (V - 1);
            return (double)E / max_E;
        }
        else
        {
            E /= 2;
            long long max_E = ((long long)V * (V - 1)) / 2;
            return (double)E / max_E;
        }
    }

    int getShortestPath(int src, int dest) const
    {
        if (src < 0 || src >= V || dest < 0 || dest >= V)
        {
            throw out_of_range("Vertex index out of range.");
        }
        vector<int> dist = dijkstra(src);
        return dist[dest];
    }

    bool isReachable(int src, int dest) const
    {
        return getShortestPath(src, dest) != INF;
    }

    bool isCyclic() const
    {
        vector<bool> visited(V, false);

        if (is_directed)
        {
            vector<bool> recStack(V, false);
            for (int i = 0; i < V; ++i)
            {
                if (!visited[i])
                {
                    if (isCyclicDirectedUtil(i, visited, recStack))
                        return true;
                }
            }
        }
        else
        {
            for (int i = 0; i < V; ++i)
            {
                if (!visited[i])
                {
                    if (isCyclicUndirectedUtil(i, visited, -1))
                        return true;
                }
            }
        }
        return false;
    }

    int getConnectedComponents() const
    {
        vector<bool> visited(V, false);
        int count = 0;
        for (int i = 0; i < V; ++i)
        {
            if (!visited[i])
            {
                stack<int> s;
                s.push(i);
                visited[i] = true;
                count++;

                while (!s.empty())
                {
                    int u = s.top();
                    s.pop();
                    for (const auto &neighbor : adj[u])
                    {
                        if (!visited[neighbor.first])
                        {
                            visited[neighbor.first] = true;
                            s.push(neighbor.first);
                        }
                    }
                }
            }
        }
        return count;
    }

    bool isConnected() const
    {
        if (is_directed)
        {
            cerr << "\n(Note: 'isConnected' is for undirected graphs. Checking for weak connectivity.)" << endl;
            return getConnectedComponents() == 1;
        }
        return getConnectedComponents() == 1;
    }

    int getEccentricity(int v) const
    {
        vector<int> dists = dijkstra(v);
        int max_dist = 0;

        for (int i = 0; i < V; ++i)
        {
            if (i == v)
                continue;

            if (dists[i] == INF)
            {
                return INF;
            }
            if (dists[i] > max_dist)
            {
                max_dist = dists[i];
            }
        }
        return max_dist;
    }

    int getDiameter() const
    {
        int diameter = 0;
        for (int i = 0; i < V; ++i)
        {
            int ecc = getEccentricity(i);
            if (ecc == INF)
            {
                return INF;
            }
            if (ecc > diameter)
            {
                diameter = ecc;
            }
        }
        return diameter;
    }

    int getRadius() const
    {
        int radius = INF;
        for (int i = 0; i < V; ++i)
        {
            int ecc = getEccentricity(i);
            if (ecc < radius)
            {
                radius = ecc;
            }
        }
        return radius;
    }
};

// --- Helper Functions for User Input ---

/**
 * @brief Safely gets an integer from the user.
 */
int getIntInput(const string &prompt)
{
    int value;
    while (true)
    {
        cout << prompt;
        if (cin >> value)
        {
            break; // Valid integer received
        }
        else
        {
            cout << "Invalid input. Please enter an integer.\n";
            cin.clear(); // Clear the error flag
            // Discard the invalid input
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    return value;
}

/**
 * @brief Safely gets a character choice from the user.
 */
char getCharInput(const string &prompt, char opt1, char opt2)
{
    char value;
    while (true)
    {
        cout << prompt;
        cin >> value;
        value = tolower(value); // Convert to lowercase

        if (value == opt1 || value == opt2)
        {
            // Clear the rest of the input buffer
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break; // Valid choice
        }
        else
        {
            cout << "Invalid input. Please enter '" << opt1 << "' or '" << opt2 << "'.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    return value;
}

/**
 * @brief Formats and prints a property.
 */
void printProperty(const string &name, int value)
{
    cout << "  - " << name << ": ";
    if (value == INF)
    {
        cout << "Infinite" << endl;
    }
    else
    {
        cout << value << endl;
    }
}

/**
 * @brief Main interactive menu for running queries on the graph.
 */
void runQueries(const Graph &g)
{
    while (true)
    {
        cout << "\n--- Analysis Queries ---" << endl;
        cout << "1. Get Shortest Path (A to B)" << endl;
        cout << "2. Get Eccentricity (of a node)" << endl;
        cout << "3. Check Reachability (A to B)" << endl;
        cout << "4. (Back to main menu)" << endl;

        int choice = getIntInput("Enter your choice: ");
        int src, dest, v;

        switch (choice)
        {
        case 1:
            src = getIntInput("  Enter source vertex: ");
            dest = getIntInput("  Enter destination vertex: ");
            printProperty("  Shortest Path", g.getShortestPath(src, dest));
            break;
        case 2:
            v = getIntInput("  Enter vertex: ");
            printProperty("  Eccentricity", g.getEccentricity(v));
            break;
        case 3:
            src = getIntInput("  Enter source vertex: ");
            dest = getIntInput("  Enter destination vertex: ");
            cout << "  - Is " << dest << " reachable from " << src << "? "
                 << (g.isReachable(src, dest) ? "Yes" : "No") << endl;
            break;
        case 4:
            return; // Exit query loop
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}

/**
 * @brief Main function to drive the interactive graph analyzer.
 */
int main()
{
    cout << "===== C++ Graph Analyzer =====" << endl;

    while (true)
    { // Main loop to analyze new graphs
        try
        {
            // --- 1. Graph Setup ---
            int vertices = getIntInput("\nEnter number of vertices: ");
            char directed_input = getCharInput("Is the graph directed? (y/n): ", 'y', 'n');
            bool is_directed = (directed_input == 'y');

            Graph g(vertices, is_directed);
            cout << "Graph created with " << vertices << " vertices (0 to "
                 << vertices - 1 << ")." << endl;

            // --- 2. Add Edges ---
            int num_edges = getIntInput("Enter number of edges to add: ");
            for (int i = 0; i < num_edges; ++i)
            {
                cout << "\nEdge " << (i + 1) << "/" << num_edges << ":" << endl;
                int u = getIntInput("  Enter source vertex (u): ");
                int v = getIntInput("  Enter destination vertex (v): ");
                int w = getIntInput("  Enter weight (e.g., 1): ");
                g.addEdge(u, v, w);
            }

            g.printGraph();

            // --- 3. Run Full Analysis ---
            cout << "\n--- Full Graph Analysis ---" << endl;
            cout << "  - Graph Density: " << g.getDensity() << endl;
            cout << "  - Contains Cycle? " << (g.isCyclic() ? "Yes" : "No") << endl;

            if (is_directed)
            {
                cout << "  - (Note: Connectivity check is for undirected graphs)" << endl;
                cout << "  - Weakly Connected Components: " << g.getConnectedComponents() << endl;
            }
            else
            {
                cout << "  - Is Connected? " << (g.isConnected() ? "Yes" : "No") << endl;
                cout << "  - Connected Components: " << g.getConnectedComponents() << endl;
            }

            printProperty("Graph Diameter", g.getDiameter());
            printProperty("Graph Radius", g.getRadius());

            cout << "\n  --- Vertex Degrees ---" << endl;
            for (int i = 0; i < g.getNumVertices(); ++i)
            {
                cout << "    Vertex " << i << ": ";
                if (is_directed)
                {
                    cout << "In-Degree=" << g.getInDegree(i)
                         << ", Out-Degree=" << g.getOutDegree(i) << endl;
                }
                else
                {
                    cout << "Degree=" << g.getOutDegree(i) << endl;
                }
            }

            // --- 4. Run Specific Queries ---
            runQueries(g);
        }
        catch (const exception &e)
        {
            cerr << "\nAn error occurred: " << e.what() << endl;
            // Clear input buffer in case of an error during input
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        // --- 5. Run Again? ---
        char again = getCharInput("\nAnalyze another graph? (y/n): ", 'y', 'n');
        if (again == 'n')
        {
            break; // Exit main loop
        }
    }

    cout << "\nGoodbye!" << endl;
    return 0;
}