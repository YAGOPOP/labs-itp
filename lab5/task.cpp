#include <SFML/Graphics.hpp>
#include <random>
#include <vector>

import graphs;

sf::VertexArray build_maze_walls(
    const Graph &graph,
    size_t width,
    size_t height,
    float cell_size
);

sf::VertexArray build_solution(
    const std::vector<vertex_id_t> &path,
    size_t width,
    float cell_size
);

int main() {
    constexpr size_t width = 30;
    constexpr size_t height = 30;
    constexpr float cell_size = 20.0f;
    constexpr float offset = 3.0f;

    std::random_device rng;

    Graph graph = Graph::make_maze(width, height, rng);

    std::vector<vertex_id_t> path;
    std::vector<vertex_id_t> best_path;

    const vertex_id_t start = 0;
    const vertex_id_t finish = static_cast<vertex_id_t>(width * height - 1);

    graph.maze_dfs(start, finish, path, best_path);

    sf::RenderWindow window(
        sf::VideoMode(
            static_cast<unsigned int>(
                static_cast<float>(width) * cell_size + offset * 2.0f
            ),
            static_cast<unsigned int>(
                static_cast<float>(height) * cell_size + offset * 2.0f
            )
        ),
        "Maze"
    );

    auto solution = build_solution(best_path, width, cell_size);
    auto maze = build_maze_walls(graph, width, height, cell_size);

    sf::Transform transform;
    transform.translate(offset, offset);

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color::White);
        window.draw(solution, transform);
        window.draw(maze, transform);
        window.display();
    }
}

sf::VertexArray build_maze_walls(
    const Graph &graph,
    size_t width,
    size_t height,
    float cell_size
) {
    sf::VertexArray walls(sf::Lines);

    auto add_wall = [&](float x1, float y1, float x2, float y2) {
        walls.append(sf::Vertex{{x1, y1}, sf::Color::Black});
        walls.append(sf::Vertex{{x2, y2}, sf::Color::Black});
    };

    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            const vertex_id_t current = static_cast<vertex_id_t>(y * width + x);

            // Левая и верхняя границы рисуются только по краям.
            if (x == 0) {
                add_wall(
                    static_cast<float>(x) * cell_size,
                    static_cast<float>(y) * cell_size,
                    static_cast<float>(x) * cell_size,
                    static_cast<float>(y + 1) * cell_size
                );
            }

            if (y == 0) {
                add_wall(
                    static_cast<float>(x) * cell_size,
                    static_cast<float>(y) * cell_size,
                    static_cast<float>(x + 1) * cell_size,
                    static_cast<float>(y) * cell_size
                );
            }

            // Правая граница или внутренняя стена справа.
            if (x + 1 == width ||
                !graph.are_neighbours(
                    current, static_cast<vertex_id_t>(current + 1)
                )) {
                add_wall(
                    static_cast<float>(x + 1) * cell_size,
                    static_cast<float>(y) * cell_size,
                    static_cast<float>(x + 1) * cell_size,
                    static_cast<float>(y + 1) * cell_size
                );
            }

            // Нижняя граница или внутренняя стена снизу.
            if (y + 1 == height ||
                !graph.are_neighbours(
                    current, static_cast<vertex_id_t>(current + width)
                )) {
                add_wall(
                    static_cast<float>(x) * cell_size,
                    static_cast<float>(y + 1) * cell_size,
                    static_cast<float>(x + 1) * cell_size,
                    static_cast<float>(y + 1) * cell_size
                );
            }
        }
    }
    return walls;
}

sf::VertexArray build_solution(
    const std::vector<vertex_id_t> &path,
    size_t width,
    float cell_size
) {
    sf::VertexArray solution(sf::LineStrip);

    for (vertex_id_t vertex : path) {
        const size_t x = vertex % width;
        const size_t y = vertex / width;

        solution.append(
            sf::Vertex{
                {(static_cast<float>(x) + 0.5f) * cell_size,
                 (static_cast<float>(y) + 0.5f) * cell_size},
                sf::Color::Red
            }
        );
    }
    return solution;
}
