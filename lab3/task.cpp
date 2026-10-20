#include <cstddef>
#include <cstdint>
#include <optional>
#include <print>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using std::println;
using std::size_t;
using std::stack;
using std::unordered_map;
using std::unordered_set;
using std::vector;

using vertex_id_t = uint32_t;
using edge_id_t = uint32_t;

//  todo добавить ориентированный граф
class Graph {
  private:
    edge_id_t edge_id = 0;

    struct Connection {
        vertex_id_t vertex;
        edge_id_t edge_id;
    };

    unordered_map<vertex_id_t, vector<Connection>> adjacency_list;

    bool is_bridge(
        vertex_id_t u,
        const Connection &candidate,
        const unordered_set<edge_id_t> &seen_edges
    ) const {
        vertex_id_t v = candidate.vertex;

        if (u == v) {
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
                    seen_edges.contains(connection.edge_id)) {
                    continue;
                }

                if (connection.vertex == v) {
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
        const unordered_set<edge_id_t> &seen_edges
    ) const {
        std::optional<Connection> candidate;
        size_t count = 0;

        for (const Connection &connection : adjacency_list.at(u)) {
            if (seen_edges.contains(connection.edge_id)) {
                continue;
            }

            ++count;
            candidate = connection;

            if (!is_bridge(u, connection, seen_edges)) {
                return connection;
            }
        }

        if (count == 1) {
            return candidate;
        }

        return std::nullopt;
    }

  public:
    bool are_all_degrees_even() const {
        for (const auto &[_, vertex] : adjacency_list) {
            if (vertex.size() % 2 != 0) {
                return false;
            }
        }

        return true;
    }

    /// Returns true if added, false if already exists
    bool add_vertex(vertex_id_t index) {
        return adjacency_list.try_emplace(index).second;
    }

    /// Returns false if one ov verticies does not exist.
    bool add_connection(vertex_id_t u, vertex_id_t v) {
        auto it_u = adjacency_list.find(u);
        auto it_v = adjacency_list.find(v);

        if (it_u == adjacency_list.end() || it_v == adjacency_list.end()) {
            return false;
        }

        it_u->second.push_back({v, edge_id});
        it_v->second.push_back({u, edge_id});
        edge_id++;

        return true;
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

        if (vertex_stack.empty()) {
            return true;
        }

        while (!vertex_stack.empty()) { // пока есть необработанные вершины
            vertex_id_t v = vertex_stack.top(); // берём одну из них
            vertex_stack.pop();

            for (auto [neighbor, _] :
                 adjacency_list.at(v)) { // и для каждого из её соседей
                if (visited.insert(neighbor)
                        .second) { // если его ещё не посещали
                    vertex_stack.push(
                        neighbor
                    ); // добавляем его в необработанные вершины
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

    // std::optional<vector<vertex_id_t>> find_eulerian_cycle() const {
    //     if (!has_eulerian_cycle()) {
    //         return std::nullopt;
    //     }

    //     vector<vertex_id_t> eulerian_cycle;

    //     unordered_set<edge_id_t> seen_edges;
    //     auto [current_vertex, initial_connections] = *adjacency_list.begin();

    //     auto next = choose_next_edge(current_vertex, seen_edges);

    //     if (next) {
    //         seen_edges.insert(next->edge_id);
    //         current_vertex = next->vertex;
    //     }

    //     return std::nullopt;
    // }

    std::optional<vector<vertex_id_t>> find_eulerian_cycle() const {
        if (!has_eulerian_cycle()) {
            return std::nullopt;
        }

        // Находим стартовую вершину ненулевой степени
        std::optional<vertex_id_t> start;
        size_t edge_count = 0;

        for (const auto& [id, connections] : adjacency_list) {
            edge_count += connections.size();

            if (!start && !connections.empty()) {
                start = id;
            }
        }

        // Каждое ребро хранится дважды
        edge_count /= 2;

        // Граф без рёбер
        if (!start) {
            return vector<vertex_id_t>{};
        }

        unordered_set<edge_id_t> seen_edges;
        vector<vertex_id_t> cycle;

        vertex_id_t current = *start;
        cycle.push_back(current);

        while (true) {
            auto next = choose_next_edge(current, seen_edges);

            if (!next) {
                break;
            }

            // Помечаем ребро пройденным
            seen_edges.insert(next->edge_id);

            // Переходим к следующей вершине
            current = next->vertex;
            cycle.push_back(current);
        }

        // Проверяем, что использованы все рёбра
        // и маршрут замкнулся
        if (seen_edges.size() != edge_count ||
            cycle.back() != *start) {
            return std::nullopt;
        }

        return cycle;
    }
};

int main() {
    Graph graph;

    graph.add_vertex(1);
    graph.add_vertex(2);
    graph.add_vertex(3);
    graph.add_vertex(4);
    // graph.add_vertex(5);

    graph.add_connection(1, 2);
    graph.add_connection(2, 3);
    graph.add_connection(3, 4);
    graph.add_connection(4, 1);
    // graph.add_connection(4, 5);

    println("{}", graph.has_eulerian_cycle());
    for (auto a : graph.find_eulerian_cycle().value()) {
        println("{}", a);
    }
}
