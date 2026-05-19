#ifndef SET_GRAPH_H
#define SET_GRAPH_H

#include <set>

#include "IGraph.h"

class SetGraph : public IGraph {
public:
    explicit SetGraph(int verticesCount);
    SetGraph(const IGraph& other);

    void AddEdge(int from, int to) override;
    int VerticesCount() const override;
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::set<int>> nextSets;
    std::vector<std::set<int>> prevSets;
};

#endif