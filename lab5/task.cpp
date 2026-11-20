#include <print>
#include <vector>

import graphs;

using std::println;

int main() {
    Graph graph;

    graph.populate_with_verticies(6);

    graph.add_arc(1, 2);
    graph.add_arc(2, 3);
    graph.add_arc(3, 6);
    graph.add_arc(6, 4);
    graph.add_arc(3, 4);
    graph.add_arc(3, 5);
    graph.add_arc(1, 4);
    graph.add_arc(1, 3);
    graph.add_arc(3, 4);

    std::vector<vertex_id_t > path = {};
    std::vector<vertex_id_t > best_path = {};

    graph.maze_dfs(1, 4, path, best_path);

    println("{}", best_path);
    println("{}", path);
}
