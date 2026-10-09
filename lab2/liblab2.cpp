#include "liblab2.hpp"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <print>
#include <stack>
#include <utility>
#include <vector>

namespace {

using std::pair;
using std::print;
using std::println;
using std::size_t;
using std::span;
using std::stack;
using std::swap;
using std::vector;
using std::chrono::milliseconds;

using borders_t = pair<size_t, size_t>;

size_t lomuto_partition(
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

borders_t lomuto_partition_with_eq(
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

void recursive_quick_sort_core(span<long> array, size_t left, size_t right) {

    if (left >= right) {
        return;
    }
    size_t pivot = lomuto_partition(array, left, right);
    if (pivot > left) {
        recursive_quick_sort_core(array, left, pivot - 1);
    }
    if (pivot < right) {
        recursive_quick_sort_core(array, pivot + 1, right);
    }
}

size_t hoare_partition(span<long> array, size_t left, size_t right) {
    long pivot = array[(left + right) / 2];
    while (true) {
        while (array[left] < pivot) {
            left++;
        }
        while (array[right] > pivot) {
            right--;
        }
        if (left >= right) {
            return right;
        }
        swap(array[left], array[right]);
        left++;
        right--;
    }
}

void quick_sort_hp_core(
    span<long> array,
    const size_t left,
    const size_t right
) {
    const size_t pivot = hoare_partition(array, left, right);
    if (left >= right) {
        return;
    }
    quick_sort_hp_core(array, left, pivot);
    quick_sort_hp_core(array, pivot + 1, right);
}

} // namespace

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

void benchmark_sorting(
    const std::string_view name,
    std::span<const long> array,
    SortFunction sort
) {
    vector<long> vec(array.begin(), array.end());
    milliseconds elapsed = measure_sorting_time(vec, sort);

    assert(std::is_sorted(vec.begin(), vec.end()));

    println("Algorithm: {}; time elapsed: {}", name, elapsed);
}

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

        size_t pivot_idx = lomuto_partition(array, left, right);

        if (pivot_idx > left) {
            borders.push({left, pivot_idx - 1});
        }

        if (pivot_idx < right) {
            borders.push({pivot_idx + 1, right});
        }
    }
}

void recursive_quick_sort(span<long> array) {
    if (array.size() < 2) {
        return;
    }

    recursive_quick_sort_core(array, 0, array.size() - 1);
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

        auto [eq_left, eq_right] = lomuto_partition_with_eq(array, left, right);

        if (eq_left > left) {
            borders.push({left, eq_left - 1});
        }

        if (eq_right < right) {
            borders.push({eq_right + 1, right});
        }
    }
}



void quick_sort_hp(span<long> array) {
    if (array.size() < 2) {
        return;
    }

    quick_sort_hp_core(array, 0, array.size() - 1);
}
