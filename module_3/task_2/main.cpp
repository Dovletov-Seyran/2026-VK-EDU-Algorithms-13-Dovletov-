#include <cassert>
#include <iostream>
#include <queue>
#include <vector>

class Graph {
public:
    explicit Graph(int verticesCount) : adjacencyLists(verticesCount) {}

    void AddEdge(int from, int to) {
        assert(from >= 0 && from < VerticesCount());
        assert(to >= 0 && to < VerticesCount());
        adjacencyLists[from].push_back(to);
        adjacencyLists[to].push_back(from);
    }

    int VerticesCount() const {
        return static_cast<int>(adjacencyLists.size());
    }

    std::vector<int> GetNextVertices(int vertex) const {
        assert(vertex >= 0 && vertex < VerticesCount());
        return adjacencyLists[vertex];
    }

private:
    std::vector<std::vector<int>> adjacencyLists;
};

int countShortestPaths(const Graph& graph, int from, int to) {
    int verticesCount = graph.VerticesCount();

    std::vector<int> dist(verticesCount, -1);
    std::vector<int> pathsCount(verticesCount, 0);

    dist[from] = 0;
    pathsCount[from] = 1;

    std::queue<int> bfsQueue;
    bfsQueue.push(from);

    while (!bfsQueue.empty()) {
        int current = bfsQueue.front();
        bfsQueue.pop();

        std::vector<int> neighbors = graph.GetNextVertices(current);
        for (size_t i = 0; i < neighbors.size(); i++) {
            int next = neighbors[i];
            if (dist[next] == -1) {
                dist[next] = dist[current] + 1;
                pathsCount[next] = pathsCount[current];
                bfsQueue.push(next);
            } else if (dist[next] == dist[current] + 1) {
                pathsCount[next] += pathsCount[current];
            }
        }
    }

    return pathsCount[to];
}

int main() {
    int v = 0;
    int n = 0;
    std::cin >> v >> n;

    Graph graph(v);
    for (int i = 0; i < n; i++) {
        int from = 0;
        int to = 0;
        std::cin >> from >> to;
        graph.AddEdge(from, to);
    }

    int u = 0;
    int w = 0;
    std::cin >> u >> w;

    std::cout << countShortestPaths(graph, u, w) << std::endl;

    return 0;
}