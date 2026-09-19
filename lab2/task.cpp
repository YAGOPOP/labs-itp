#include <chrono>
#include <cstddef>
#include <limits>
#include <print>
#include <random>
#include <span>
#include <utility>

constexpr std::size_t arr_size = 100000;

void bubble_sort(std::span<long> array);
void print_array(std::span<long> array);

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<long> distrib(
        std::numeric_limits<long>::min(), std::numeric_limits<long>::max()
    );

    std::array<long, arr_size> a;
    for (std::size_t i = 0; i < arr_size; i++) {
        a[i] = distrib(gen);
    }

    auto start = std::chrono::steady_clock::now();
    bubble_sort(a);
    auto end = std::chrono::steady_clock::now();

    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::println("bubble sort {} milis: {}", arr_size, duration.count());
}

void bubble_sort(std::span<long> array) {
    std::size_t arr_len = array.size();
    for (std::size_t i = 0; i < arr_len; i++) {
        for (std::size_t j = 0; j < arr_len - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                std::swap(array[j], array[j + 1]);
            }
        }
    }
}

void print_array(const std::span<long> array) {
    std::size_t last = array.size() - 1;
    for (std::size_t i = 0; i < last; i++) {
        std::print("{} ", array[i]);
    }
    std::println("{}", array[last]);
}
