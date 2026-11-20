#include <print>
#include <random>
#include <vector>

import graphs;

using std::println;

int main() {
    std::random_device hw_rng;

    Graph graph= Graph::make_maze(9, 12, hw_rng);

    std::vector<vertex_id_t > path = {};
    std::vector<vertex_id_t > best_path = {};

    graph.maze_dfs(1, 4, path, best_path);

    println("{}", best_path);
}
