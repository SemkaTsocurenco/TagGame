#include "../include/main.hpp"



void printField(const std::vector<std::vector<int>>& state, const std::string& title) {
    std::cout << "\n" << title << "\n";
    std::cout << "+-----+-----+-----+-----+-----+\n";
    for (const auto& row : state) {
        std::cout << "|  ";
        for (auto cell : row) {
            if (cell != 0) {
                std::cout << cell << "  |  ";
            } else {
                std::cout << "   |  ";
            }
        }
        std::cout << "\n+-----+-----+-----+-----+-----+\n";
    }
}

void printClasses() {

    std::cout << "\nУникальные классы: ";
    for (auto tag : classes) {
        std::cout << tag << " ";
    }
    std::cout << "\n\nВерхние левые углы целевого положения:\n";
    for (auto tag : classes) {
        auto coord = targetTopLeft[tag];
        std::cout << "Объект " << tag << ": { " << coord.first << ", " << coord.second << " };\n";
    }
    std::cout << "\n~~~~~~~~~~~~~~~~ Поиск решения A*... ~~~~~~~~~~~~~~~~\n\n";
}

void printA_star_path (std::vector<Node*>& path){
    // Выводим последовательность ходов в консоль (пропуская начальное состояние)
    int moveNumber = 1;
    // Заголовок таблицы (ширины можно скорректировать по необходимости)
    std::cout << "+-----------------------------------------------------------+\n";
    std::cout << "| " << std::setw(6) << std::left << "Step"
              << " | " << std::setw(15) << std::left << "Move"
              << " | " << std::setw(8) << std::right << "g"
              << " | " << std::setw(8) << std::right << "h"
              << " | " << std::setw(8) << std::right << "f"
              << " |\n";
    std::cout << "+--------+-----------------+----------+----------+----------+\n";

    for (size_t i = 1; i < path.size(); i++) {
        // Формирование строки "piece - direction"
        std::string move = std::to_string(path[i]->piece) + " - " + path[i]->direction;
        std::cout << "| " << std::setw(6) << std::left << moveNumber
                  << " | " << std::setw(17) << std::left << move
                  << " | " << std::setw(8) << std::right << path[i]->g
                  << " | " << std::setw(8) << std::right << path[i]->h
                  << " | " << std::setw(8) << std::right << path[i]->f
                  << " |\n";
        std::cout << "+--------+-----------------+----------+----------+----------+\n";
        moveNumber++;
    }

    std::cout << "\n~~~~~~~~~~~~~~~~ Решение Завершено ~~~~~~~~~~~~~~~~\n\n";
}
