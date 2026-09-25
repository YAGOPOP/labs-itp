#include <cassert>
import std;



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
using std::ptrdiff_t;

constexpr array<size_t, 5> arr_sizes =
    {1000, 10000, 100000, 500000, 1000000};

// constexpr array<size_t, 3> arr_sizes = {1000, 10000, 100000};

void print_array(std::span<long> array);

void bubble_sort(std::span<long> array);
void iterative_quick_sort(std::span<long> array);
void iterative_quick_sort_with_eq(std::span<long> array);
void recursive_quick_sort(span<long> array);

using SortFunction = void (*)(std::span<long>);
milliseconds measure_sorting_time(std::span<long> array, SortFunction sort);

void benchmark_sorting(
    const std::string_view name,
    const std::span<long> array,
    SortFunction sort
);

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

        println("---------Benchmarking collection of {} elements---------", arr_size);

        benchmark_sorting("recursive quick sort", sub_vec_prime, recursive_quick_sort);
        benchmark_sorting("iterative quick sort", sub_vec_prime, iterative_quick_sort);
        benchmark_sorting("iterative quick sort with equal", sub_vec_prime, iterative_quick_sort_with_eq);
        // benchmark_sorting("bubble sort", sub_vec_prime, bubble_sort);
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
milliseconds measure_sorting_time(std::span<long> array, SortFunction sort) {
    using namespace std::chrono;
    time_point start = steady_clock::now();
    sort(array);
    time_point end = steady_clock::now();
    return duration_cast<milliseconds>(end - start);
}

void benchmark_sorting(
    const std::string_view name,
    const std::span<long> array,
    SortFunction sort
) {
    vector<long> vec(array.begin(), array.end());
    milliseconds elapsed = measure_sorting_time(vec, sort);

    assert(std::is_sorted(vec.begin(), vec.end()));

    println(
        "Algorithm: {}; time (milliseconds) elapsed: {}",
        name,
        elapsed
    );
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

    while (i <= greater) {
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
