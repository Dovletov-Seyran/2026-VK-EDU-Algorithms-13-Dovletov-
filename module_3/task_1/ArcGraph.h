#ifndef ARC_GRAPH_H
#define ARC_GRAPH_H

#include <utility>

#include "IGraph.h"

class ArcGraph : public IGraph {
public:
    explicit ArcGraph(int verticesCount);
    ArcGraph(const IGraph& other);

    void AddEdge(int from, int to) override;
    int VerticesCount() const override;
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    int vertexCount;
    std::vector<std::pair<int, int>> edges;
};

#endif