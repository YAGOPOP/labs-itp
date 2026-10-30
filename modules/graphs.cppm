module;

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

export module graphs;

using std::size_t;
using std::stack;
using std::unordered_map;
using std::unordered_set;
using std::vector;

export using vertex_id_t = uint32_t;
export using edge_id_t = uint32_t;

export class Graph {
  private:
    edge_id_t next_edge_id = 0;

    struct Connection {
        vertex_id_t vertex;
        edge_id_t edge_id;
    };

    unordered_map<vertex_id_t, vector<Connection>> adjacency_list;

    bool is_bridge(
        vertex_id_t u,
        const Connection &candidate,
        const unordered_set<edge_id_t> &used_edges
    ) const {
        if (u == candidate.vertex) {
            return false;
        }

        unordered_set<vertex_id_t> visited;
        stack<vertex_id_t> vertex_stack;

        visited.insert(u);
        vertex_stack.push(u);

        while (!vertex_stack.empty()) {
            auto current = vertex_stack.top();
            vertex_stack.pop();

            for (const auto &connection : adjacency_list.at(current)) {
                if (connection.edge_id == candidate.edge_id ||
                    used_edges.contains(connection.edge_id)) {
                    continue;
                }

                if (connection.vertex == candidate.vertex) {
                    return false;
                }

                if (visited.insert(connection.vertex).second) {
                    vertex_stack.push(connection.vertex);
                }
            }
        }

        return true;
    }

    std::optional<Connection> choose_next_edge(
        vertex_id_t u,
        const unordered_set<edge_id_t> &used_edges
    ) const {
        std::optional<Connection> candidate;
        size_t count = 0;

        for (const Connection &connection : adjacency_list.at(u)) {
            if (used_edges.contains(connection.edge_id)) {
                continue;
            }

            ++count;
            candidate = connection;

            if (!is_bridge(u, connection, used_edges)) {
                return connection;
            }
        }

        if (count == 1) {
            return candidate;
        }

        return std::nullopt;
    }

    bool hamiltonian_dfs(vertex_id_t u, vector<vertex_id_t> &path) const {
        path.push_back(u);

        if (path.size() == adjacency_list.size()) {
            if (are_neighbours(u, path.front())) {
                return true;
            }
        }

        for (auto &[v, _] : adjacency_list.at(u)) {
            if (!std::ranges::contains(path, v)) {
                if (hamiltonian_dfs(v, path)) {
                    return true;
                }
            }
        }

        path.pop_back();
        return false;
    }

  public:
    bool are_all_degrees_even() const {
        for (const auto &[_, connections] : adjacency_list) {
            if (connections.size() % 2 != 0) {
                return false;
            }
        }

        return true;
    }

    bool add_vertex(vertex_id_t index) {
        return adjacency_list.try_emplace(index).second;
    }

    bool add_connection(vertex_id_t u, vertex_id_t v) {
        auto it_u = adjacency_list.find(u);
        auto it_v = adjacency_list.find(v);

        if (it_u == adjacency_list.end() || it_v == adjacency_list.end()) {
            return false;
        }

        it_u->second.push_back({v, next_edge_id});
        it_v->second.push_back({u, next_edge_id});
        next_edge_id++;

        return true;
    }

    vector<vertex_id_t> get_vertex_connections(vertex_id_t vertex_id) const {
        vector<vertex_id_t> neighbors;
        for (auto &[id, _] : adjacency_list.at(vertex_id)) {
            neighbors.push_back(id);
        }
        return neighbors;
    }

    bool are_neighbours(vertex_id_t u, vertex_id_t v) const {
        for (auto &[potential_v, _] : adjacency_list.at(u)) {
            if (potential_v == v) {
                return true;
            }
        }

        return false;
    }

    bool is_connected_ignoring_isolated() const {
        unordered_set<vertex_id_t> visited;
        stack<vertex_id_t> vertex_stack;

        for (const auto &[id, vertex] : adjacency_list) {
            if (!vertex.empty()) {
                visited.insert(id);
                vertex_stack.push(id);
                break;
            }
        }

        while (!vertex_stack.empty()) {
            vertex_id_t v = vertex_stack.top();
            vertex_stack.pop();

            for (auto [neighbor, _] :
                 adjacency_list.at(v)) {
                if (visited.insert(neighbor)
                        .second) {
                    vertex_stack.push(
                        neighbor
                    );
                }
            }
        }

        for (const auto &[id, vertex] : adjacency_list) {
            if (vertex.size() > 0 && !visited.contains(id)) {
                return false;
            }
        }

        return true;
    }

    bool has_eulerian_cycle() const {
        return are_all_degrees_even() && is_connected_ignoring_isolated();
    }

    std::optional<vector<vertex_id_t>> find_eulerian_cycle() const {
        if (!has_eulerian_cycle()) {
            return std::nullopt;
        }

        std::optional<vertex_id_t> start;
        size_t edge_count = 0;

        for (const auto &[id, connections] : adjacency_list) {
            edge_count += connections.size();

            if (!start && !connections.empty()) {
                start = id;
            }
        }

        edge_count /= 2;

        if (!start) {
            return vector<vertex_id_t>{};
        }

        unordered_set<edge_id_t> used_edges;
        vector<vertex_id_t> cycle;

        vertex_id_t current = *start;
        cycle.push_back(current);

        while (true) {
            auto next = choose_next_edge(current, used_edges);

            if (!next) {
                break;
            }

            used_edges.insert(next->edge_id);

            current = next->vertex;
            cycle.push_back(current);
        }

        if (used_edges.size() != edge_count || cycle.back() != *start) {
            return std::nullopt;
        }

        return cycle;
    }

    std::optional<vector<vertex_id_t>> find_hamiltonian_cycle() const {
        if (adjacency_list.empty()) {
            return std::nullopt;
        }

        vertex_id_t start = adjacency_list.begin()->first;
        vector<vertex_id_t> path;

        if (hamiltonian_dfs(start, path)) {
            path.push_back(path.front());
            return path;
        }

        return std::nullopt;
    }
};
