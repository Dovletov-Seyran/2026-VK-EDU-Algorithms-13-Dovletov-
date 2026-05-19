// Задача 1. «Представление графа» (5 баллов)
#include <cassert>
#include <iostream>

#include "ArcGraph.h"
#include "ListGraph.h"
#include "MatrixGraph.h"
#include "SetGraph.h"

void printGraph(const IGraph& graph) {
    for (int i = 0; i < graph.VerticesCount(); i++) {
        std::vector<int> next = graph.GetNextVertices(i);
        std::cout << i << ": ";
        for (size_t j = 0; j < next.size(); j++) {
            std::cout << next[j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    ListGraph listGraph(5);
    listGraph.AddEdge(0, 1);
    listGraph.AddEdge(0, 2);
    listGraph.AddEdge(1, 3);
    listGraph.AddEdge(2, 3);
    listGraph.AddEdge(3, 4);

    std::cout << "ListGraph:" << std::endl;
    printGraph(listGraph);

    MatrixGraph matrixGraph(listGraph);
    std::cout << "MatrixGraph (from ListGraph):" << std::endl;
    printGraph(matrixGraph);

    SetGraph setGraph(matrixGraph);
    std::cout << "SetGraph (from MatrixGraph):" << std::endl;
    printGraph(setGraph);

    ArcGraph arcGraph(setGraph);
    std::cout << "ArcGraph (from SetGraph):" << std::endl;
    printGraph(arcGraph);

    // Обратная конверсия.
    ListGraph listGraph2(arcGraph);
    std::cout << "ListGraph (from ArcGraph):" << std::endl;
    printGraph(listGraph2);

    return 0;
}