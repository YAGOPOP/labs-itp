module;

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <map>
#include <optional>
#include <print>
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
using weight_t = double;

export struct ShortestPath {
    weight_t weight = 0.0;
    vector<vertex_id_t> path = {};
};

export using PathMatrix =
    std::map<vertex_id_t, std::map<vertex_id_t, ShortestPath>>;

export class Graph {
  private:
    edge_id_t next_edge_id = 0;

    struct Arc {
        vertex_id_t vertex;
        edge_id_t edge_id;
        weight_t weight;
    };

    unordered_map<vertex_id_t, vector<Arc>> adjacency_list;

    bool is_bridge(
        vertex_id_t u,
        const Arc &candidate,
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

    std::optional<Arc> choose_next_edge(
        vertex_id_t u,
        const unordered_set<edge_id_t> &used_edges
    ) const {
        std::optional<Arc> candidate;
        size_t count = 0;

        for (const Arc &connection : adjacency_list.at(u)) {
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

        for (auto &[v, _, _] : adjacency_list.at(u)) {
            if (!std::ranges::contains(path, v)) {
                if (hamiltonian_dfs(v, path)) {
                    return true;
                }
            }
        }

        path.pop_back();
        return false;
    }

    void hamiltonian_dfs(
        vertex_id_t u,
        vector<vertex_id_t> &path,
        weight_t current_weight,
        weight_t &best_weight,
        vector<vertex_id_t> &best_cycle
    ) const {
        if (current_weight >= best_weight) {
            return;
        }

        path.push_back(u);

        if (path.size() == adjacency_list.size()) {
            for (const Arc &edge : adjacency_list.at(u)) {
                if (edge.vertex != path.front()) {
                    continue;
                }

                weight_t total_weight = current_weight + edge.weight;

                if (total_weight < best_weight) {
                    best_weight = total_weight;

                    best_cycle = path;
                    best_cycle.push_back(path.front());
                }
            }
        }

        for (const Arc &edge : adjacency_list.at(u)) {
            if (std::ranges::contains(path, edge.vertex)) {
                continue;
            }

            hamiltonian_dfs(
                edge.vertex,
                path,
                current_weight + edge.weight,
                best_weight,
                best_cycle
            );
        }

        path.pop_back();
        return;
    }

    std::pair<unordered_set<vertex_id_t>, std::map<vertex_id_t, ShortestPath>>
    initialize_dijkstra(vertex_id_t start) const {
        unordered_set<vertex_id_t> visited;

        std::map<vertex_id_t, ShortestPath> shortest_paths;
        const auto inf = std::numeric_limits<weight_t>::infinity();
        for (const auto &[id, _] : adjacency_list) {
            shortest_paths.try_emplace(id, ShortestPath{inf, {}});
        }
        shortest_paths.at(start) = ShortestPath{0.0, {start}};

        return {visited, shortest_paths};
    }

    std::optional<vertex_id_t> find_current_vertex_dijkstra(
        const unordered_set<vertex_id_t> &visited,
        const std::map<vertex_id_t, ShortestPath> &shortest_paths
    ) const {
        std::optional<vertex_id_t> current = std::nullopt;

        weight_t min_not_visited_weight =
            std::numeric_limits<weight_t>::infinity();

        for (auto const &[id, cur_sp] : shortest_paths) {
            if (min_not_visited_weight > cur_sp.weight &&
                !visited.contains(id)) {
                current = id;
                min_not_visited_weight = cur_sp.weight;
            }
        }

        return current;
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

    bool add_arc(vertex_id_t u, vertex_id_t v, weight_t weight) {
        auto it_u = adjacency_list.find(u);
        auto it_v = adjacency_list.find(v);

        if (it_u == adjacency_list.end() || it_v == adjacency_list.end()) {
            return false;
        }

        it_u->second.push_back({v, next_edge_id, weight});
        it_v->second.push_back({u, next_edge_id, weight});
        next_edge_id++;

        return true;
    }

    bool add_arc(vertex_id_t u, vertex_id_t v) {
        return this->add_arc(u, v, 0);
    }

    vector<vertex_id_t> get_vertex_connections(vertex_id_t vertex_id) const {
        vector<vertex_id_t> neighbors;
        for (auto &[id, _, _] : adjacency_list.at(vertex_id)) {
            neighbors.push_back(id);
        }
        return neighbors;
    }

    bool are_neighbours(vertex_id_t u, vertex_id_t v) const {
        for (auto &[potential_v, _, _] : adjacency_list.at(u)) {
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

            for (auto [neighbor, _, _] : adjacency_list.at(v)) {
                if (visited.insert(neighbor).second) {
                    vertex_stack.push(neighbor);
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

    std::optional<std::map<vertex_id_t, ShortestPath>>
    find_shortest_paths_dijkstra(vertex_id_t start) const {
        if (!adjacency_list.contains(start)) {
            return std::nullopt;
        }

        auto [visited, shortest_paths] = initialize_dijkstra(start);

        while (true) {
            std::optional<vertex_id_t> current_opt =
                find_current_vertex_dijkstra(visited, shortest_paths);
            if (!current_opt) {
                break;
            }
            vertex_id_t current = *current_opt;

            for (auto const &[ne_id, _, w] : adjacency_list.at(current)) {
                if (visited.contains(ne_id)) {
                    continue;
                }

                auto &next = shortest_paths.at(ne_id);
                const auto &cur_path = shortest_paths.at(current);

                weight_t candidate_weight = cur_path.weight + w;
                if (next.weight > candidate_weight) {
                    next.weight = candidate_weight;

                    next.path = cur_path.path;
                    next.path.push_back(ne_id);
                }
            }

            visited.insert(current);

            if (visited.size() == adjacency_list.size()) {
                break;
            }
        }

        return shortest_paths;
    }

    PathMatrix find_all_shortest_paths() {
        PathMatrix all_paths;
        for (const auto &[start_vertex, _] : adjacency_list) {
            auto paths = find_shortest_paths_dijkstra(start_vertex);
            if (!paths) {
                continue;
            }
            all_paths.try_emplace(start_vertex, std::move(*paths));
        }

        return all_paths;
    }

    static Graph from_path_matrix(const PathMatrix &path_matrix) {
        Graph full_graph;

        vector<vertex_id_t> vertices;
        for (const auto &[vertex, _] : path_matrix) {
            full_graph.add_vertex(vertex);
            vertices.push_back(vertex);
        }

        for (size_t i = 0; i < vertices.size(); ++i) {
            for (size_t j = i + 1; j < vertices.size(); ++j) {
                const auto &shortest_path =
                    path_matrix.at(vertices[i]).at(vertices[j]);

                if (vertices[i] == vertices[j]) {
                    continue;
                }

                if (!std::isfinite(shortest_path.weight)) {
                    continue;
                }

                full_graph.add_arc(
                    vertices[i], vertices[j], shortest_path.weight
                );
            }
        }

        return full_graph;
    }

    std::optional<std::pair<weight_t, vector<vertex_id_t>>>
    find_minimum_hamiltonian_cycle(vertex_id_t start) const {
        if (!adjacency_list.contains(start)) {
            return std::nullopt;
        }

        weight_t best_weight = std::numeric_limits<weight_t>::infinity();

        vector<vertex_id_t> path;
        vector<vertex_id_t> best_cycle;

        hamiltonian_dfs(start, path, 0.0, best_weight, best_cycle);

        if (best_cycle.empty()) {
            return std::nullopt;
        }

        return std::pair{best_weight, std::move(best_cycle)};
    }
};

export template <typename T> void print_array(const std::span<T> array) {
    for (auto v : array) {
        std::print("{} ", v);
    }
    std::println("");
}

export template <typename T> void print_path(const std::span<T> array) {
    if (array.empty()) {
        return;
    }

    // std::print("{}", array[0]);
    for (std::size_t i = 0; i < array.size() - 1; i++) {
        std::print("{} -> ", array[i]);
    }
    std::println("{}", array.back());
}

export vector<vertex_id_t> expand_cycle(
    const PathMatrix &path_matrix,
    const vector<vertex_id_t> &cycle
) {
    vector<vertex_id_t> route;

    for (size_t i = 0; i + 1 < cycle.size(); ++i) {
        const auto &segment = path_matrix.at(cycle[i]).at(cycle[i + 1]).path;

        if (segment.empty()) {
            throw std::logic_error("Path is empty");
        }

        auto begin = segment.begin();

        if (!route.empty()) {
            ++begin;
        }

        route.insert(route.end(), begin, segment.end());
    }

    return route;
}
