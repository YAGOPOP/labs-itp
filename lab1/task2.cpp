#include <cstdlib>
#include <ctime>
#include <format>
#include <iostream>
#include <print>
#include <string>
#include <stdexcept>

int get_random_number(int floor_inclusive, int ceiling_inclusive);
int get_input_number(int floor, int ceiling, const std::string &prompt);
int bot_move(const int sticks);
int player_move(int sticks);
bool sticks_game_loop(bool turn);
bool game_loop();

int main() {
    std::time_t t = std::time(nullptr);
    std::srand(static_cast<unsigned int>(t));

    std::println("-------Новая игра-------");
    if (game_loop()) {
        std::print("Противник победил, ");
    } else {
        std::print("Вы победили, ");
    }
    std::println("конец игры.");
}

int get_random_number(int floor, int ceiling) {
    return floor + std::rand() % (ceiling - floor + 1);
}

int get_input_number(int floor, int ceiling, const std::string &prompt) {
    std::string input_number;
    while (true) {
        std::print("{}", prompt);
        if (!std::getline(std::cin, input_number)) {
            std::println("\nВвод завершён.");
            std::exit(0);
        }

        int number;
        try {
            number = std::stoi(input_number);
        } catch (const std::invalid_argument&) {
            std::println("Неверный ввод.");
            continue;
        }

        if (number < floor || number > ceiling) {
            std::println("Число вне допустимого диапазона.");
            continue;
        }
        return number;
    }
}

int bot_move(const int sticks) {
    int cap = 3;
    if (sticks < 3) {
        cap = sticks;
    }

    int move;
    // make perfect move
    switch (sticks % 4) {
    case 0:
        move = 3;
        break;
    case 1:
        // perfect move does not exist
        move = get_random_number(1, cap);
        break;
    case 2:
        move = 1;
        break;
    case 3:
        move = 2;
        break;
    }

    if (sticks < 7) {
        if (get_random_number(0, 1)) {
            move = get_random_number(1, cap);
        }
    }

    return move;
}

int player_move(int sticks) {
    int ceiling = 3;
    if (sticks < 3) {
        ceiling = sticks;
    }
    int move = get_input_number(
        1, ceiling, std::format("Ваш ход [{};{}]: ", 1, ceiling)
    );
    return move;
}

/// returns true if bot won
/// bot_turn true - bot goes first
bool sticks_game_loop(bool bot_turn) {
    int sticks_count = 21;
    while (sticks_count > 0) {
        std::println("Осталось палочек: {}", sticks_count);
        int move;
        if (bot_turn) {
            move = bot_move(sticks_count);
            std::println("Противник взял палочек: {}", move);
        } else {
            move = player_move(sticks_count);
        }
        sticks_count -= move;

        bot_turn = !bot_turn;
    }

    return bot_turn;
}

/// rerurns true if bot won
bool game_loop() {
    int bot_score = 100;
    int player_score = 100;
    bool bot_turn = get_random_number(0, 1);
    while (bot_score > 0 && player_score > 0) {
        std::println(
            "Ваш счёт: {}; счёт противника: {}", player_score, bot_score
        );
        std::println("-----Новый раунд-----");
        if (sticks_game_loop(bot_turn)) {
            bot_score += 15;
            player_score -= 15;
            std::println("Противник победил в раунде.");
        } else {
            player_score += 15;
            bot_score -= 15;
            std::println("Вы победили в раунде.");
        }
        bot_turn = !bot_turn;
    }
    return bot_score > player_score;
}
