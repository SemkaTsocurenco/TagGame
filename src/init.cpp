#include "../include/main.hpp"



std::vector<std::vector<int>> initialField;
std::vector<std::vector<int>> targetField;
std::set<int> classes;
std::unordered_map<int, std::pair<int, int>> targetTopLeft;
/**
 * @brief Инициализация начального и целевого состояний, а также вычисление целевых позиций объектов.
 */
void initialize() {
    initialField = {
        {1, 1, 1, 2, 2},
        {3, 3, 4, 4, 2},
        {5, 6, 6, 7, 7},
        {5, 5, 8, 8, 8},
        {9, 0, 0, 0, 10}
    };

    targetField = {
        {3, 3, 1, 1, 1},
        {9, 4, 4, 6, 6},
        {5, 10, 2, 2, 0},
        {5, 5, 0, 2, 0},
        {8, 8, 8, 7, 7}
    };

    // // targetField = {
    // //     {1, 1, 1, 2, 2},
    // //     {4, 4, 0, 5, 2},
    // //     {3, 3, 0, 5, 5},
    // //     {6, 6, 10, 9, 0},
    // //     {8, 8, 8, 7, 7}
    // // };

    // targetField = {
    //     {4, 4, 5, 2, 2},
    //     {3, 3, 5, 5, 2},
    //     {1, 1, 1, 0, 0},
    //     {6, 6, 10, 9, 0},
    //     {8, 8, 8, 7, 7}
    // };


    printField(initialField, "Начальное положение:");
    printField(targetField, "Целевое положение:");

    // Сбор уникальных объектов из целевого поля
    for (const auto& row : targetField) {
        for (auto cell : row) {
            if (cell != 0) {
                classes.insert(cell);
            }
        }
    }

    // Проверка соответствия объектов между начальными и целевыми положениями
    std::set<int> initialClasses;
    for (const auto& row : initialField) {
        for (auto cell : row) {
            if (cell != 0) {
                initialClasses.insert(cell);
            }
        }
    }
    if (initialClasses != classes) {
        std::cout << "Целевая позиция не может быть достигнута из-за несоответствия объектов!\n";
        return;
    }

    // Вычисление и вывод координат верхних левых углов для каждого объекта в целевом состоянии
    for (auto tag : classes) {
        targetTopLeft[tag] = getTopLeft(targetField, tag);
    }

    printClasses();
}

