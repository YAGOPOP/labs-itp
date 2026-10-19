#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <print>
#include <span>
#include <unordered_map>
#include <utility>
#include <vector>

using std::println;
using std::size_t;
using std::span;
using std::unordered_map;
using std::vector;

using idx_t = uint32_t;

class VertexConnections {
  private:
    vector<idx_t> connections;

  public:
    // VertexConnections(vector<idx_t> connections) : connections(std::move(connections)) {}

    span<const idx_t> get_connections() const { return connections; }

    size_t degree() const {
        return connections.size();
    }

    bool is_even() const { return degree() % 2 == 0; }

    void add_connection(idx_t vertex) {
        connections.push_back(vertex);
    }
};

class Graph {
  private:
    unordered_map<idx_t, VertexConnections> adjacency_list;
  public:

    bool are_all_degrees_even() const {
        for (const auto& [_, vertex] : adjacency_list) {
            if (!vertex.is_even()) {
                return false;
            }
        }

        return true;
    }

    /// Returns true if added, false if already exists
    bool add_vertex(idx_t index) {
        return adjacency_list.try_emplace(index).second;
    }

    /// Returns false if one ov verticies does not exist.
    bool add_connection(idx_t u, idx_t v) {
        auto it_u = adjacency_list.find(u);
        auto it_v = adjacency_list.find(v);

        if (it_u == adjacency_list.end() ||
            it_v == adjacency_list.end()) {
            return false;
        }

        it_u->second.add_connection(v);
        it_v->second.add_connection(u);

        return true;
    }
};

int main() {
    Graph graph;

    graph.add_vertex(1);
    graph.add_vertex(2);
    graph.add_vertex(3);
    graph.add_vertex(4);
    graph.add_vertex(5);

    graph.add_connection(1, 2);
    graph.add_connection(2, 3);
    graph.add_connection(3, 4);
    graph.add_connection(4, 1);

    println("{}", graph.are_all_degrees_even());
}
