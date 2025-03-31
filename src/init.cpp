#include "../include/main.hpp"



std::vector<std::vector<int>> initialField;
std::vector<std::vector<int>> targetField;
std::set<int> classes;
std::set<int> classes_target;
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
        {0, 0, 0, 2, 2},
        {0, 0, 0, 5, 2},
        {0, 0, 0, 5, 5},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}
    };







    printField(initialField, "Начальное положение:");
    printField(targetField, "Целевое положение:");

    // Сбор уникальных объектов из целевого поля
    for (const auto& row : initialField) {
        for (auto cell : row) {
            if (cell != 0) {
                classes.insert(cell);
            }
        }
    }
    for (const auto& row : targetField) {
        for (auto cell : row) {
            if (cell != 0) {
                classes_target.insert(cell);
            }
        }
    }



    // Вычисление и вывод координат верхних левых углов для каждого объекта в целевом состоянии
    // Вместо обхода всех классов используем только классы из targetField
    for (int tag : classes_target) {
        targetTopLeft[tag] = getTopLeft(targetField, tag);
    }


    printClasses();
}

