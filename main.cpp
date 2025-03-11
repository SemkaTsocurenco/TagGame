
#include "./include/main.hpp"



int main() {
    initialize();

    // Запускаем алгоритм A* и получаем путь (решение)
    std::vector<Node*> solutionPath = a_star_solution();
    if (solutionPath.empty()) {
        std::cout << "Решение не найдено.\n";
        return 0;
    }

    // Визуализируем найденное решение
    visualizeSolution(solutionPath);

    return 0;
}
