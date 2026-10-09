#include "liblab2.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <limits>
#include <print>
#include <random>
#include <span>
#include <vector>

using std::array;
using std::println;
using std::size_t;
using std::vector;

constexpr size_t ELEMENTS_AMOUNT = 50;

constexpr long FLOOR = 12050L;
constexpr long CEILING = 32705L;

/// returns true if array was sorted correctly
bool check_sorting(SortFunction sort, std::span<long> array);

int main() {
    std::random_device rd;
    std::uniform_int_distribution<long> distrib(FLOOR, CEILING);

    vector<long> random_numbers(ELEMENTS_AMOUNT);
    for (size_t i = 0; i < ELEMENTS_AMOUNT; i++) {
        random_numbers[i] = distrib(rd);
    }

    println(
        "\033[31mAlgoritm:\033[0m iterative quick sort with equal; sorted: \033[31m{}\033[0m",
        check_sorting(iterative_quick_sort_with_eq, random_numbers)
    );
    println(
        "\033[31mAlgoritm:\033[0m recursive quick sort Hoare partition; sorted: \033[31m{}\033[0m",
        check_sorting(quick_sort_hp, random_numbers)
    );
    println(
        "\033[31mAlgoritm:\033[0m bubble sort; sorted: \033[31m{}\033[0m",
        check_sorting(bubble_sort, random_numbers)
    );
}

bool check_sorting(SortFunction sort, std::span<long> array) {
    vector<long> vec(array.begin(), array.end());
    print_array(vec);
    sort(vec);
    print_array(vec);
    return std::is_sorted(vec.begin(), vec.end());
}
