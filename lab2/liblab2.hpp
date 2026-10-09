#pragma once

#ifndef LIBLAB2
#define LIBLAB2

#include <span>
#include <string_view>
#include <chrono>


void bubble_sort(std::span<long> array);
void iterative_quick_sort(std::span<long> array);
void iterative_quick_sort_with_eq(std::span<long> array);
void recursive_quick_sort(std::span<long> array);
void quick_sort_hp(std::span<long> array);

using SortFunction = void (*)(std::span<long>);
void benchmark_sorting(
    const std::string_view name,
    std::span<const long> array,
    SortFunction sort
);

template<typename T = std::chrono::milliseconds>
T measure_sorting_time(std::span<long> array, SortFunction sort) {
    using namespace std::chrono;
    time_point start = steady_clock::now();
    sort(array);
    time_point end = steady_clock::now();
    return duration_cast<T>(end - start);
}
#endif
