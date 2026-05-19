#include "MatrixGraph.h"

#include <cassert>

MatrixGraph::MatrixGraph(int verticesCount)
    : adjacencyMatrix(verticesCount, std::vector<bool>(verticesCount, false)) {}

MatrixGraph::MatrixGraph(const IGraph& other)
    : adjacencyMatrix(other.VerticesCount(), std::vector<bool>(other.VerticesCount(), false)) {
    for (int i = 0; i < other.VerticesCount(); i++) {
        std::vector<int> next = other.GetNextVertices(i);
        for (size_t j = 0; j < next.size(); j++) {
            adjacencyMatrix[i][next[j]] = true;
        }
    }
}

void MatrixGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < VerticesCount());
    assert(to >= 0 && to < VerticesCount());
    adjacencyMatrix[from][to] = true;
}

int MatrixGraph::VerticesCount() const {
    return static_cast<int>(adjacencyMatrix.size());
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < VerticesCount());
    std::vector<int> result;
    for (int i = 0; i < VerticesCount(); i++) {
        if (adjacencyMatrix[vertex][i]) {
            result.push_back(i);
        }
    }
    return result;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < VerticesCount());
    std::vector<int> result;
    for (int i = 0; i < VerticesCount(); i++) {
        if (adjacencyMatrix[i][vertex]) {
            result.push_back(i);
        }
    }
    return result;
}