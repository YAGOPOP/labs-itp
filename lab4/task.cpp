#include <map>
#include <print>

import graphs;

int main() {
    Graph graph;

    for (vertex_id_t i = 1; i <= 8; i++) {
        graph.add_vertex(i);
    }

    graph.add_arc(1, 2, 3);
    graph.add_arc(1, 5, 6);
    graph.add_arc(1, 6, 7);
    graph.add_arc(1, 8, 2);

    graph.add_arc(2, 4, 1);
    graph.add_arc(2, 7, 8);
    graph.add_arc(2, 8, 3);

    graph.add_arc(3, 7, 3);
    graph.add_arc(3, 4, 2);

    graph.add_arc(4, 5, 4);
    graph.add_arc(4, 6, 5);

    graph.add_arc(6, 7, 4);
    graph.add_arc(6, 8, 6);

    graph.add_arc(7, 8, 5);

    // надо добраться до каждой вершины и обратно из вершины `1`
    // вывести все пути и найти оптимальные

    std::optional<std::map<vertex_id_t, ShortestPath>> a =
        graph.find_shortest_pathes_dijkstra(1);

    if (!a.has_value()) {
        return 1;
    }

    std::println("{}", a.value().at(3).weight);
    std::println("{}", a.value().at(3).path);
}
