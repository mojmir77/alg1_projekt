#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <set>

class Graph {
private:
    std::set<std::pair<int, int>> edges; //vstupní hrany
    std::set<int> vertices; // vstupní vrcholy
    std::set<std::pair<int, int>> allEdges; // hrany pro celkový DAG
    std::vector<int> topologicalOrder; //topologický seznam vrcholů
public:
    bool readInput(std::string fileName);
    void print();
    bool kahnSort();
    int getExpectedEdges();
    void DAGCompletion(std::vector<int> tOrder);
    bool fileOutput();

};

bool Graph::readInput(std::string fileName) {
    std::ifstream iFile(fileName);
    if (not iFile.is_open()) {std::cerr << "Error opening file: " << fileName << std::endl; return false;}
    std::string sA;
    std::string sB;
    int A;
    int B;
    while (iFile >> sA >> sB) {
        A = stoi(sA);
        B = stoi(sB);
        vertices.insert(A);
        vertices.insert(B);
        edges.insert(std::make_pair(A, B));
    }
    iFile.close();
    return true;
}
void Graph::print() {
    if (kahnSort()){std::cout << "Graph is DAG" << std::endl;} else {std::cout << "Graph is not DAG" << std::endl;}
    std::cout << "Number of vertices: " << vertices.size() << std::endl;
    std::cout << "Number of edges: " << edges.size() << std::endl;
    std::cout << "Expected number of edges: " << getExpectedEdges() << std::endl;
    if (topologicalOrder.size() == vertices.size()) {
        std::cout << "Number of vertices of complete DAG: " << topologicalOrder.size() << std::endl;
        DAGCompletion(topologicalOrder);
        std::cout << "Number of edges of complete DAG: " << allEdges.size() << std::endl;
    }

}

bool Graph::kahnSort() {
    topologicalOrder.clear();
    // 1. Spočítání vstupních hran pro každý vrchol
    std::map<int, int> inDegree;
    std::map<int, std::vector<int>> adj;

    for (const auto &v : vertices) {
        inDegree[v] = 0;
    }
    for (const auto &edge : edges) {
        adj[edge.first].push_back(edge.second);
        inDegree[edge.second]++;
    }

    // 2. Přidá do fronty vrcholy s 0 vstupujícími hranami
    std::queue<int> q;
    for (const auto &[v, deg] : inDegree) {
        if (deg == 0) q.push(v);
    }

    // 3. Tvoření topologického seznamu
    while (!q.empty()) {
        topologicalOrder.push_back(q.front());
        for (int neighbor : adj[q.front()]) {
            inDegree[neighbor]--;
            if (inDegree[neighbor] == 0) {
                q.push(neighbor);
            }
        }
        q.pop();
    }

    // Pokud se nerovnají graf není DAG
    return topologicalOrder.size() == vertices.size();
}

int Graph::getExpectedEdges() {
    return (vertices.size() * (vertices.size() - 1))/2;
}

void Graph::DAGCompletion(std::vector<int> tOrder) {
    for (size_t i = 0; i < tOrder.size(); i++) {
        for (size_t j = i + 1; j < tOrder.size(); j++) {
            allEdges.insert({tOrder[i], tOrder[j]});
        }
    }
}
bool Graph::fileOutput() {
    std::string fileName = "output.txt";
    std::ofstream oFile("output.txt");
    if (not oFile.is_open()) {std::cerr << "Error opening file: " << fileName << std::endl; return false;}

    std::string sA;
    std::string sB;
    for (auto &edge : allEdges) {
        sA = std::to_string(edge.first);
        sB = std::to_string(edge.second);
        oFile << sA << " " << sB << std::endl;
    }
    oFile.close();
    return true;
}
int main(int argc, char* argv[]) {
    if (argc < 3){ std::cerr << "Not enough arguments" << std::endl; return 1;}
    Graph *g = new Graph();
    if (g->readInput(argv[1])) {
        g->print();
        g->fileOutput();
    }
    free(g);
    return 0;
}