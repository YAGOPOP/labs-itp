#include <SFML/Graphics.hpp>
#include <random>
#include <vector>

import graphs;
import maze_visualization;

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
        "Maze test building"
    );

    sf::RenderWindow window2(
        sf::VideoMode(
            static_cast<unsigned int>(
                static_cast<float>(width) * cell_size + offset * 2.0f
            ),
            static_cast<unsigned int>(
                static_cast<float>(height) * cell_size + offset * 2.0f
            )
        ),
        "Maze test passing"
    );

    auto solution = build_solution(best_path, width, cell_size);
    auto maze = build_maze_walls(graph, width, height, cell_size);

    sf::Transform transform;
    transform.translate(offset, offset);

    while (window.isOpen() && window2.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        while (window2.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window2.close();
            }
        }

        window.clear(sf::Color::White);
        window.draw(maze, transform);
        window.display();

        window2.clear(sf::Color::White);
        window2.draw(maze, transform);
        window2.draw(solution, transform);
        window2.display();
    }
}
