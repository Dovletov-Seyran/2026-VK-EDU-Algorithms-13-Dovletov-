#include <cassert>
#include <climits>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>

class WeightedGraph {
public:
    explicit WeightedGraph(int verticesCount) : adjacencyLists(verticesCount) {}

    void AddEdge(int from, int to, int weight) {
        assert(from >= 0 && from < VerticesCount());
        assert(to >= 0 && to < VerticesCount());
        adjacencyLists[from].push_back(std::make_pair(to, weight));
        adjacencyLists[to].push_back(std::make_pair(from, weight));
    }

    int VerticesCount() const {
        return static_cast<int>(adjacencyLists.size());
    }

    std::vector<std::pair<int, int>> GetNextVertices(int vertex) const {
        assert(vertex >= 0 && vertex < VerticesCount());
        return adjacencyLists[vertex];
    }

private:
    std::vector<std::vector<std::pair<int, int>>> adjacencyLists;
};

int dijkstra(const WeightedGraph& graph, int from, int to) {
    int verticesCount = graph.VerticesCount();
    std::vector<int> dist(verticesCount, INT_MAX);
    dist[from] = 0;

    std::priority_queue<std::pair<int, int>,
                        std::vector<std::pair<int, int>>,
                        std::greater<std::pair<int, int>>>
        pq;
    pq.push(std::make_pair(0, from));

    while (!pq.empty()) {
        std::pair<int, int> top = pq.top();
        pq.pop();
        int currentDist = top.first;
        int current = top.second;

        if (currentDist > dist[current]) {
            continue;
        }

        std::vector<std::pair<int, int>> neighbors = graph.GetNextVertices(current);
        for (size_t i = 0; i < neighbors.size(); i++) {
            int next = neighbors[i].first;
            int weight = neighbors[i].second;
            if (dist[current] + weight < dist[next]) {
                dist[next] = dist[current] + weight;
                pq.push(std::make_pair(dist[next], next));
            }
        }
    }

    return dist[to];
}

int main() {
    int n = 0;
    int m = 0;
    std::cin >> n >> m;

    WeightedGraph graph(n);
    for (int i = 0; i < m; i++) {
        int from = 0;
        int to = 0;
        int weight = 0;
        std::cin >> from >> to >> weight;
        graph.AddEdge(from, to, weight);
    }

    int u = 0;
    int w = 0;
    std::cin >> u >> w;

    std::cout << dijkstra(graph, u, w) << std::endl;

    return 0;
}