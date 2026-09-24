#include <array>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <limits>
#include <print>
#include <random>
#include <span>
#include <stack>
#include <utility>
#include <vector>

using std::array;
using std::pair;
using std::print;
using std::println;
using std::size_t;
using std::span;
using std::stack;
using std::swap;
using std::vector;
using std::chrono::milliseconds;
// constexpr array<size_t, 5> arr_sizes = {1000, 10000, 100000, 500000,
// 1000000};

constexpr array<size_t, 3> arr_sizes = {1000, 10000, 100000};

void print_array(std::span<long> array);
void bubble_sort(std::span<long> array);
size_t quick_sort_partition(
    span<long> array,
    const size_t left,
    const size_t right
);
void iterative_quick_sort(std::span<long> array);

using SortFunction = void (*)(std::span<long>);
milliseconds benchmark_sorting(std::span<long> array, SortFunction sort);

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<long> distrib(
        std::numeric_limits<long>::min(), std::numeric_limits<long>::max()
    );

    for (size_t arr_size : arr_sizes) {
        vector<long> vec_prime(arr_size);
        for (size_t i = 0; i < arr_size; i++) {
            vec_prime[i] = distrib(gen);
        }
        vector<long> vec_quick(vec_prime);
        milliseconds duration_quick =
            benchmark_sorting(vec_quick, iterative_quick_sort);
        println(
            "quick sort {} elements, elapsed milis: {}",
            arr_size,
            duration_quick.count()
        );

        vector<long> vec_bubble(vec_prime);
        milliseconds duration_bubble =
            benchmark_sorting(vec_bubble, bubble_sort);
        println(
            "bubble sort {} elements, elapsed milis: {}",
            arr_size,
            duration_bubble.count()
        );
    }
}

void bubble_sort(std::span<long> array) {
    size_t arr_len = array.size();
    for (size_t i = 0; i < arr_len; i++) {
        for (size_t j = 0; j < arr_len - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                swap(array[j], array[j + 1]);
            }
        }
    }
}

void print_array(const std::span<long> array) {
    size_t last = array.size() - 1;
    print("[");
    for (size_t i = 0; i < last; i++) {
        print("{} ", array[i]);
    }
    println("{}]", array[last]);
}

using SortFunction = void (*)(std::span<long>);
milliseconds benchmark_sorting(std::span<long> array, SortFunction sort) {
    using namespace std::chrono;
    time_point start = steady_clock::now();
    sort(array);
    time_point end = steady_clock::now();
    return duration_cast<milliseconds>(end - start);
}

void iterative_quick_sort(std::span<long> array) {
    if (array.size() < 2) {
        return;
    }

    stack<pair<size_t, size_t>> borders;

    borders.push(pair<size_t, size_t>(0, array.size() - 1));

    while (!borders.empty()) {
        auto [left, right] = borders.top();
        borders.pop();

        if (left >= right) {
            continue;
        }

        size_t pivot_idx = quick_sort_partition(array, left, right);

        if (pivot_idx > left) {
            borders.push({left, pivot_idx - 1});
        }

        if (pivot_idx < right) {
            borders.push({pivot_idx + 1, right});
        }
    }
}

size_t quick_sort_partition(
    span<long> array,
    const size_t left,
    const size_t right
) {
    long pivot = array[right];
    size_t i = left;
    for (size_t j = left; j < right; j++) {
        if (array[j] < pivot) {
            swap(array[i], array[j]);
            i++;
        }
    }
    swap(array[i], array[right]);
    return i;
}
