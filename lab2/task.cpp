#include "liblab2.hpp"

#include <array>
#include <cstddef>
#include <limits>
#include <print>
#include <random>
#include <vector>

using std::array;
using std::println;
using std::size_t;
using std::vector;

constexpr array<size_t, 5> arr_sizes = {1000, 10000, 100000, 500000, 1000000};

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

        println(
            "---------Benchmarking collection of {} elements---------", arr_size
        );

        // benchmark_sorting(
        //     "recursive quick sort", sub_vec_prime, recursive_quick_sort
        // );
        // benchmark_sorting(
        //     "iterative quick sort", sub_vec_prime, iterative_quick_sort
        // );
        benchmark_sorting(
            "iterative quick sort with equal",
            sub_vec_prime,
            iterative_quick_sort_with_eq
        );
        benchmark_sorting(
            "recursive quick sort Hoare partition", sub_vec_prime, quick_sort_hp
        );
        benchmark_sorting("bubble sort", sub_vec_prime, bubble_sort);
    }
}
