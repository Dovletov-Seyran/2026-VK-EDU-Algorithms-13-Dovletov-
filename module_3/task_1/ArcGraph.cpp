#include "ArcGraph.h"

#include <cassert>

ArcGraph::ArcGraph(int verticesCount) : vertexCount(verticesCount) {}

ArcGraph::ArcGraph(const IGraph& other) : vertexCount(other.VerticesCount()) {
    for (int i = 0; i < other.VerticesCount(); i++) {
        std::vector<int> next = other.GetNextVertices(i);
        for (size_t j = 0; j < next.size(); j++) {
            edges.push_back(std::make_pair(i, next[j]));
        }
    }
}

void ArcGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < vertexCount);
    assert(to >= 0 && to < vertexCount);
    edges.push_back(std::make_pair(from, to));
}

int ArcGraph::VerticesCount() const {
    return vertexCount;
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < vertexCount);
    std::vector<int> result;
    for (size_t i = 0; i < edges.size(); i++) {
        if (edges[i].first == vertex) {
            result.push_back(edges[i].second);
        }
    }
    return result;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < vertexCount);

    std::vector<int> result;
    for (size_t i = 0; i < edges.size(); i++) {
        if (edges[i].second == vertex) {
            result.push_back(edges[i].first);
        }
    }
    return result;
}