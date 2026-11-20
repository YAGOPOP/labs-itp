module;

#include <SFML/Graphics.hpp>
#include <cstddef>
#include <vector>

export module maze_visualization;

import graphs;

export sf::VertexArray build_solution(
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

export sf::VertexArray build_maze_walls(
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
