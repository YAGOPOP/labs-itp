#include <print>

import graphs;

using std::println;

int main() {
    Graph graph;

    for (vertex_id_t i = 1; i <= 4; i++) {
        graph.add_vertex(i);
    }

    graph.add_arc(1, 2, 1);
    graph.add_arc(2, 3, 1);
    graph.add_arc(3, 4, 1);

    PathMatrix path_matrix = graph.find_all_shortest_paths();

    Graph full_graph = Graph::from_path_matrix(path_matrix);

    if (auto result = full_graph.find_minimum_hamiltonian_cycle(1)) {
        println("Minimum cost: {}", result->first);

        auto route = expand_cycle(path_matrix, result->second);

        println("Actual route:");
        print_path(std::span{route});
    }

}
