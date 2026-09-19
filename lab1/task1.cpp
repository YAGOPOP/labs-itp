#include <cstdlib>
#include <ctime>
#include <print>
#include <string>

constexpr int FLOOR_INCLUSIVE = 15;
constexpr int CEILING_EXCLUSIVE = 25;
constexpr int AMOUNT = 1800;
constexpr int INTERVALS_AMOUNT = 10;

constexpr int DIAGRAMM_MAX_WIDTH = 50;
constexpr int RANGE_ACCURACY = 0;

constexpr int GENERAL_INTERVAL_SIZE = CEILING_EXCLUSIVE - FLOOR_INCLUSIVE;
constexpr double INTERVAL_STEP =
    static_cast<double>(GENERAL_INTERVAL_SIZE) / INTERVALS_AMOUNT;

double get_random_number(int floor_inclusive, int ceiling_exclusive);

int main() {
    int interval_counters[INTERVALS_AMOUNT] = {0};

    std::time_t t = std::time(nullptr);
    std::srand(static_cast<unsigned int>(t));

    for (int i = 0; i < AMOUNT; i++) {
        double random_number =
            get_random_number(FLOOR_INCLUSIVE, CEILING_EXCLUSIVE);
        double offset = random_number - FLOOR_INCLUSIVE;
        int interval_number = static_cast<int>(offset / INTERVAL_STEP);
        interval_counters[interval_number] += 1;
    }

    int sum = 0;
    for (int j = 0; j < INTERVALS_AMOUNT; j++) {
        int count = interval_counters[j];
        unsigned long line = static_cast<unsigned long>(
            static_cast<double>(count) / AMOUNT * DIAGRAMM_MAX_WIDTH
        );
        std::string s(line, '*');
        double cur_floor = FLOOR_INCLUSIVE + INTERVAL_STEP * j;
        double cur_ceiling = cur_floor + INTERVAL_STEP;
        std::println(
            "{0} [{1:.{4}f};{2:.{4}f}): {3}",
            count,
            cur_floor,
            cur_ceiling,
            s,
            RANGE_ACCURACY
        );
        sum += count;
    }
    std::println("Всего: {}", sum);
    return 0;
}

double get_random_number(int floor_inclusive, int ceiling_exclusive) {
    double r = static_cast<double>(std::rand()) / (RAND_MAX + 1.0);
    return floor_inclusive + r * (ceiling_exclusive - floor_inclusive);
}
