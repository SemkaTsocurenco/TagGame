#include "./include/main.hpp"
#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <SFML/Graphics.hpp> // Предполагается, что SFML используется

// Размер доски
const int BOARD_SIZE = 5;

// Поля: начальное и целевое состояние
std::vector<std::vector<int>> initialField;
std::vector<std::vector<int>> targetField;

// Уникальные классы (объекты)
std::set<int> classes;

// Для каждого класса сохраняем координаты верхнего левого угла целевого положения
std::unordered_map<int, std::pair<int, int>> targetTopLeft;

/**
 * @brief Получить список позиций для заданного объекта.
 *
 * @param state Матрица состояния.
 * @param tag Идентификатор объекта.
 * @return std::vector<std::pair<int,int>> Список координат, где найден объект.
 */
std::vector<std::pair<int, int>> getClassPositions(const std::vector<std::vector<int>>& state, int tag) {
    std::vector<std::pair<int, int>> positions;
    for (size_t i = 0; i < state.size(); i++) {
        for (size_t j = 0; j < state[i].size(); j++) {
            if (state[i][j] == tag) {
                positions.push_back({static_cast<int>(i), static_cast<int>(j)});
            }
        }
    }
    return positions;
}

/**
 * @brief Определить координаты верхнего левого угла для объекта.
 *
 * @param state Матрица состояния.
 * @param tag Идентификатор объекта.
 * @return std::pair<int,int> Координаты в формате (колонка, строка).
 */
std::pair<int, int> getTopLeft(const std::vector<std::vector<int>>& state, int tag) {
    auto positions = getClassPositions(state, tag);
    if (positions.empty()) return {0, 0};
    int minRow = BOARD_SIZE, minCol = BOARD_SIZE;
    for (const auto& pos : positions) {
        if (pos.first < minRow) minRow = pos.first;
        if (pos.second < minCol) minCol = pos.second;
    }
    return {minCol, minRow};
}

/**
 * @brief Эвристическая функция для оценки состояния.
 *
 * Суммируется манхэттенское расстояние между текущими и целевыми позициями (верхние левые углы) для каждого объекта.
 *
 * @param state Текущее состояние.
 * @return int Эвристическая оценка.
 */
int heuristic(const std::vector<std::vector<int>>& state) {
    int total = 0;
    for (auto tag : classes) {
        auto current = getTopLeft(state, tag);
        auto target = targetTopLeft[tag];
        total += std::abs(current.first - target.first) + std::abs(current.second - target.second);
    }
    return total;
}

/**
 * @brief Вывод поля в консоль.
 *
 * @param state Матрица состояния.
 * @param title Заголовок для вывода.
 */
void printField(const std::vector<std::vector<int>>& state, const std::string& title) {
    std::cout << "\n" << title << "\n";
    std::cout << "+-----+-----+-----+-----+-----+\n";
    for (const auto& row : state) {
        std::cout << "|  ";
        for (auto cell : row) {
            if (cell != 0) {
                std::cout << cell << (cell < 10 ? "  |  " : " |  ");
            } else {
                std::cout << "   |  ";
            }
        }
        std::cout << "\n+-----+-----+-----+-----+-----+\n";
    }
}

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

    // targetField = {
    //     {6, 6, 5, 2, 2},
    //     {7, 7, 5, 5, 2},
    //     {1, 1, 1, 3, 3},
    //     {4, 4, 8, 8, 8},
    //     {9, 0, 0, 0, 10}
    // };


    printField(initialField, "Начальное положение:");
    printField(targetField, "Целевое положение:");

    // Сбор уникальных классов из целевого поля
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
    std::cout << "\nУникальные классы ячеек: ";
    for (auto tag : classes) {
        std::cout << tag << "; ";
        targetTopLeft[tag] = getTopLeft(targetField, tag);
    }
    std::cout << "\n\nВерхние левые углы для каждого объекта целевого положения:\n";
    for (auto tag : classes) {
        auto coord = targetTopLeft[tag];
        std::cout << "Класс: " << tag << "  Координаты: { " 
                  << coord.first << ", " << coord.second << " };\n";
    }
    std::cout << "\n\n~~~~~~~~ Поиск решения A*... ~~~~~~~~\n\n";
}

/// ////////////////////////////////////////////////////////////////////////////
/// Реализация A*
/// ////////////////////////////////////////////////////////////////////////////

/**
 * @brief Преобразует состояние в строковое представление для использования в качестве ключа.
 *
 * @param state Матрица состояния.
 * @return std::string Строковое представление.
 */
std::string stateToString(const std::vector<std::vector<int>>& state) {
    std::stringstream ss;
    for (const auto& row : state) {
        for (auto cell : row) {
            ss << cell << ",";
        }
        ss << ";";
    }
    return ss.str();
}

/**
 * @brief Проверяет, можно ли переместить заданный объект в направлении (dr, dc).
 *
 * Для проверки создаётся временная копия состояния, в которой клетки объекта очищаются, после чего проверяется,
 * что все клетки, в которые планируется перемещение, свободны и находятся в пределах доски.
 *
 * @param state Текущее состояние.
 * @param piece Идентификатор объекта.
 * @param dr Изменение строки.
 * @param dc Изменение столбца.
 * @return true Если перемещение возможно.
 * @return false Иначе.
 */
bool canMove(const std::vector<std::vector<int>>& state, int piece, int dr, int dc) {
    auto positions = getClassPositions(state, piece);
    // Создаем временную копию, где клетки текущего объекта считаются свободными
    std::vector<std::vector<int>> temp = state;
    for (auto& pos : positions) {
        temp[pos.first][pos.second] = 0;
    }
    // Проверяем, что для каждой клетки новое положение свободно и в пределах доски
    for (auto& pos : positions) {
        int newR = pos.first + dr;
        int newC = pos.second + dc;
        if (newR < 0 || newR >= BOARD_SIZE || newC < 0 || newC >= BOARD_SIZE)
            return false;
        if (temp[newR][newC] != 0)
            return false;
    }
    return true;
}

/**
 * @brief Применяет перемещение объекта в состоянии и возвращает новое состояние.
 *
 * @param state Исходное состояние.
 * @param piece Идентификатор объекта.
 * @param dr Изменение строки.
 * @param dc Изменение столбца.
 * @return std::vector<std::vector<int>> Новое состояние после перемещения.
 */
std::vector<std::vector<int>> applyMove(const std::vector<std::vector<int>>& state, int piece, int dr, int dc) {
    auto positions = getClassPositions(state, piece);
    std::vector<std::vector<int>> newState = state;
    // Убираем объект из исходного положения
    for (auto& pos : positions) {
        newState[pos.first][pos.second] = 0;
    }
    // Записываем объект в новом положении
    for (auto& pos : positions) {
        int newR = pos.first + dr;
        int newC = pos.second + dc;
        newState[newR][newC] = piece;
    }
    return newState;
}

/**
 * @brief Структура узла для поиска A*.
 */
struct Node {
    std::vector<std::vector<int>> state;
    int g; // стоимость пути до текущего состояния
    int h; // эвристическая оценка до цели
    int f; // f = g + h
    int piece;            // объект, перемещение которого привело в это состояние
    std::string direction; // направление перемещения ("up", "down", "left", "right")
    Node* parent;         // указатель на родительский узел

    Node(const std::vector<std::vector<int>>& state, int g, int h, int piece = 0,
         const std::string& direction = "", Node* parent = nullptr)
        : state(state), g(g), h(h), f(g + h), piece(piece), direction(direction), parent(parent) {}
};

/**
 * @brief Компаратор для приоритетной очереди (минимальное f имеет больший приоритет).
 */
struct CompareNode {
    bool operator()(const Node* a, const Node* b) const {
        return a->f > b->f;
    }
};

/**
 * @brief Генерация соседних состояний для текущего узла.
 *
 * Для каждого объекта (фишки) перебираются 4 направления перемещения, если перемещение возможно.
 *
 * @param current Текущий узел.
 * @return std::vector<Node*> Список соседних узлов.
 */
std::vector<Node*> getNeighbors(Node* current) {
    std::vector<Node*> neighbors;
    // Возможные направления перемещения: (dr, dc) и соответствующее имя направления.
    std::vector<std::pair<std::pair<int, int>, std::string>> directions = {
        {{-1, 0}, "up"},
        {{1, 0}, "down"},
        {{0, -1}, "left"},
        {{0, 1}, "right"}
    };
    // Перебираем все объекты (фишки), присутствующие в глобальном наборе
    for (int piece : classes) {
        for (auto& dir : directions) {
            int dr = dir.first.first;
            int dc = dir.first.second;
            if (canMove(current->state, piece, dr, dc)) {
                auto newState = applyMove(current->state, piece, dr, dc);
                Node* neighbor = new Node(newState, current->g + 1, heuristic(newState), piece, dir.second, current);
                neighbors.push_back(neighbor);
            }
        }
    }
    return neighbors;
}

/**
 * @brief Функция поиска решения с помощью алгоритма A*.
 *
 * При нахождении решения восстанавливается путь от начального до целевого состояния, и для каждого хода выводится
 * сообщение в консоль в формате:
 * "Ход N: переместить фишку X direction"
 */
void a_star() {
    Node* start = new Node(initialField, 0, heuristic(initialField));
    std::priority_queue<Node*, std::vector<Node*>, CompareNode> openSet;
    openSet.push(start);
    std::unordered_set<std::string> closedSet;

    Node* solution = nullptr;

    while (!openSet.empty()) {
        Node* current = openSet.top();
        openSet.pop();
        // Если достигнуто целевое состояние, завершаем поиск
        if (current->state == targetField) {
            solution = current;
            break;
        }
        std::string stateKey = stateToString(current->state);
        if (closedSet.find(stateKey) != closedSet.end())
            continue;
        closedSet.insert(stateKey);

        // Генерируем соседние состояния
        auto neighbors = getNeighbors(current);
        for (auto neighbor : neighbors) {
            std::string neighborKey = stateToString(neighbor->state);
            if (closedSet.find(neighborKey) == closedSet.end()) {
                openSet.push(neighbor);
            } else {
                delete neighbor; // освобождаем память, если состояние уже посещалось
            }
        }
    }

    if (solution == nullptr) {
        std::cout << "Решение не найдено.\n";
        return;
    }

    // Восстанавливаем путь от начального состояния до цели
    std::vector<Node*> path;
    for (Node* node = solution; node != nullptr; node = node->parent) {
        path.push_back(node);
    }
    std::reverse(path.begin(), path.end());

    // Выводим последовательность ходов (пропускаем первый узел, так как он соответствует начальному состоянию)
    int moveNumber = 1;
    for (size_t i = 1; i < path.size(); i++) {
        std::cout << "Ход " << moveNumber << ": переместить фишку " 
                  << path[i]->piece << " " << path[i]->direction << "\n";
        moveNumber++;
    }
    // Освобождаем память (простейший способ – можно улучшить, используя умные указатели)
    // Здесь можно пройтись по всем созданным узлам, если требуется.
}

int main() {
    initialize();

    // Запускаем поиск решения алгоритмом A*
    a_star();

    // Создаем окно с использованием SFML
    sf::RenderWindow window(sf::VideoMode(500, 500), "TagGame", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(120);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::Black);
        // Здесь можно отрисовывать объекты игрового поля
        window.display();
    }
    return 0;
}
