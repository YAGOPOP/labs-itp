#include <array>
#include <chrono>
#include <cstddef>
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
constexpr array<size_t, 6> arr_sizes = {1000, 10000, 100000, 500000, 1000000, 10000000};

// constexpr array<size_t, 3> arr_sizes = {1000, 10000, 100000};

void print_array(std::span<long> array);

void bubble_sort(std::span<long> array);
void iterative_quick_sort(std::span<long> array);
void iterative_quick_sort_with_eq(std::span<long> array);
void recursive_quick_sort(span<long> array);

using SortFunction = void (*)(std::span<long>);
milliseconds benchmark_sorting(std::span<long> array, SortFunction sort);

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<long> distrib(
        std::numeric_limits<long>::min(), std::numeric_limits<long>::max()
    );

    vector<long> vec_prime(arr_sizes.back());
    for (size_t i = 0; i < arr_sizes.back(); i++) {
        vec_prime[i] = distrib(gen);
    }

    for (size_t arr_size : arr_sizes) {
        vector<long> sub_vec_prime(
            vec_prime.begin(),
            vec_prime.begin() + static_cast<ptrdiff_t>(arr_size)
        );

        vector<long> vec_quick_iter(sub_vec_prime);
        milliseconds duration_quick_iter =
            benchmark_sorting(vec_quick_iter, iterative_quick_sort);
        println(
            "iterative quick sort {} elements, elapsed milis: {}",
            arr_size,
            duration_quick_iter.count()
        );

        vector<long> vec_quick_iter_eq(sub_vec_prime);
        milliseconds duration_quick_iter_eq =
            benchmark_sorting(vec_quick_iter_eq, iterative_quick_sort_with_eq);
        println(
            "iterative quick sort with equal {} elements, elapsed milis: {}",
            arr_size,
            duration_quick_iter_eq.count()
        );

        vector<long> vec_quick_rec(sub_vec_prime);
        milliseconds duration_quick_rec =
            benchmark_sorting(vec_quick_rec, iterative_quick_sort_with_eq);
        println(
            "recursive quick sort {} elements, elapsed milis: {}",
            arr_size,
            duration_quick_rec.count()
        );

        // vector<long> vec_bubble(sub_vec_prime);
        // milliseconds duration_bubble =
        //     benchmark_sorting(vec_bubble, bubble_sort);
        // println(
        //     "bubble sort {} elements, elapsed milis: {}",
        //     arr_size,
        //     duration_bubble.count()
        // );
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

using borders_t = pair<size_t, size_t>;
void iterative_quick_sort(span<long> array) {
    if (array.size() < 2) {
        return;
    }

    stack<borders_t> borders;

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

void recursive_quick_sort_core(span<long> array, size_t left, size_t right) {
    if (left >= right) {
        return;
    }
    size_t pivot = quick_sort_partition(array, left, right);
    if (pivot > left) {
        recursive_quick_sort_core(array, left, pivot - 1);
    }
    if (pivot < right) {
        recursive_quick_sort_core(array, pivot + 1, right);
    }
}

void recursive_quick_sort(span<long> array) {
    if (array.size() < 2) {
        return;
    }

    recursive_quick_sort_core(array, 0, array.size() - 1);
}

borders_t quick_sort_partition_with_eq(
    span<long> array,
    const size_t left,
    const size_t right
) {
    long pivot = array[right];
    size_t lower = left;
    size_t i = left;
    size_t greater = right;

    while (i < greater) {
        if (array[i] < pivot) {
            swap(array[lower], array[i]);
            lower++;
            i++;
        } else if (array[i] > pivot) {
            swap(array[i], array[greater]);
            greater--;
        } else {
            i++;
        }
    }

    return {lower, greater};
}

void iterative_quick_sort_with_eq(std::span<long> array) {
    if (array.size() < 2) {
        return;
    }

    stack<borders_t> borders;

    borders.push(pair<size_t, size_t>(0, array.size() - 1));

    while (!borders.empty()) {
        auto [left, right] = borders.top();
        borders.pop();

        if (left >= right) {
            continue;
        }

        auto [eq_left, eq_right] =
            quick_sort_partition_with_eq(array, left, right);

        if (eq_left > left) {
            borders.push({left, eq_left - 1});
        }

        if (eq_right < right) {
            borders.push({eq_right + 1, right});
        }
    }
}
