#include <print>

using std::println;
using std::print;

import graphs;


int main() {
    Graph graph;

    for (vertex_id_t v = 1; v <= 8; v++) {
        graph.add_vertex(v);
    }

    graph.add_connection(1, 2);
    graph.add_connection(1, 8);

    graph.add_connection(2, 3);
    graph.add_connection(2, 8);
    graph.add_connection(2, 6);

    graph.add_connection(3, 7);

    graph.add_connection(4, 8);
    graph.add_connection(4, 5);

    graph.add_connection(5, 6);

    graph.add_connection(6, 7);

    println("Has eulerian cycle: {}", graph.has_eulerian_cycle());
    graph.add_connection(6, 8); // makes graph eulerian
    println("Added edge 6-8 to make graph eulerian");

    if (auto eulerian_cycle = graph.find_eulerian_cycle()) {
        for (vertex_id_t a : *eulerian_cycle) {
            print("{} ", a);
        }
        println();
    } else {
        println("no eulerian cycle found");
    }

    if (auto cycle = graph.find_hamiltonian_cycle()) {
        for (auto v : *cycle) {
            print("{} ", v);
        }
        println();
    } else {
        println("no hamiltonian cycle found");
    }
}
