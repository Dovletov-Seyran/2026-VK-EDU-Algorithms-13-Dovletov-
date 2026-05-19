#include "ListGraph.h"

#include <cassert>

ListGraph::ListGraph(int verticesCount)
    : adjacencyLists(verticesCount), prevAdjacencyLists(verticesCount) {}

ListGraph::ListGraph(const IGraph& other)
    : adjacencyLists(other.VerticesCount()), prevAdjacencyLists(other.VerticesCount()) {
    for (int i = 0; i < other.VerticesCount(); i++) {
        std::vector<int> next = other.GetNextVertices(i);
        for (size_t j = 0; j < next.size(); j++) {
            AddEdge(i, next[j]);
        }
    }
}

void ListGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < VerticesCount());
    assert(to >= 0 && to < VerticesCount());
    adjacencyLists[from].push_back(to);
    prevAdjacencyLists[to].push_back(from);
}

int ListGraph::VerticesCount() const {
    return static_cast<int>(adjacencyLists.size());
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < VerticesCount());
    return adjacencyLists[vertex];
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < VerticesCount());
    return prevAdjacencyLists[vertex];
}