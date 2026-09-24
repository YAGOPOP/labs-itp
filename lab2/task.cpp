#include <array>
#include <chrono>
#include <cstddef>
#include <limits>
#include <print>
#include <random>
#include <span>
#include <utility>
#include <vector>

// constexpr std::array<std::size_t, 5> arr_sizes = {
//     1000, 10000, 100000, 500000, 1000000};
constexpr std::array<std::size_t, 3> arr_sizes = {
    1000, 10000, 100000};

void bubble_sort(std::span<long> array);
void print_array(std::span<long> array);

using SortFunction = void (*)(std::span<long>);
std::chrono::milliseconds
benchmark_sorting(std::span<long> array, SortFunction sort);

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<long> distrib(
        std::numeric_limits<long>::min(), std::numeric_limits<long>::max()
    );

    for (std::size_t arr_size : arr_sizes) {
        std::vector<long> vec(arr_size);
        for (std::size_t i = 0; i < arr_size; i++) {
            vec[i] = distrib(gen);
        }

        std::chrono::milliseconds duration = benchmark_sorting(vec, bubble_sort);
        std::println(
            "bubble sort {} elements, elapsed milis: {}",
            arr_size,
            duration.count()
        );
    }
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

using SortFunction = void (*)(std::span<long>);
std::chrono::milliseconds
benchmark_sorting(std::span<long> array, SortFunction sort) {
    using namespace std::chrono;
    time_point start = steady_clock::now();
    sort(array);
    time_point end = steady_clock::now();
    return duration_cast<milliseconds>(end - start);
}
