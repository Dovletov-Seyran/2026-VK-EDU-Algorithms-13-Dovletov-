#include "SetGraph.h"

#include <cassert>

SetGraph::SetGraph(int verticesCount) : nextSets(verticesCount), prevSets(verticesCount) {}

SetGraph::SetGraph(const IGraph& other)
    : nextSets(other.VerticesCount()), prevSets(other.VerticesCount()) {
    for (int i = 0; i < other.VerticesCount(); i++) {
        std::vector<int> next = other.GetNextVertices(i);
        for (size_t j = 0; j < next.size(); j++) {
            AddEdge(i, next[j]);
        }
    }
}

void SetGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < VerticesCount());
    assert(to >= 0 && to < VerticesCount());
    nextSets[from].insert(to);
    prevSets[to].insert(from);
}

int SetGraph::VerticesCount() const {
    return static_cast<int>(nextSets.size());
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < VerticesCount());
    return std::vector<int>(nextSets[vertex].begin(), nextSets[vertex].end());
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < VerticesCount());
    return std::vector<int>(prevSets[vertex].begin(), prevSets[vertex].end());
}