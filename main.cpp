#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
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

// Размеры доски и визуализации
const int BOARD_SIZE = 5;         // размер игрового поля 3x3
const int CELL_SIZE = 100;        // размер клетки в пикселях
const int BOARD_MARGIN = 50;      // отступ поля от края окна

// Поля: начальное и целевое состояние
std::vector<std::vector<int>> initialField;
std::vector<std::vector<int>> targetField;

// Уникальные объекты (метки)
std::set<int> classes;

// Для каждого объекта сохраняем координаты верхнего левого угла целевого положения
std::unordered_map<int, std::pair<int, int>> targetTopLeft;

/**
 * @brief Получить список позиций для заданного объекта.
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
 * Возвращает пару (столбец, строка).
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
 * @brief Эвристическая функция.
 *
 * Суммируется манхэттенское расстояние между текущими и целевыми позициями (верхние левые углы) для каждого объекта.
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
 */
void printField(const std::vector<std::vector<int>>& state, const std::string& title) {
    std::cout << "\n" << title << "\n";
    std::cout << "+-----+-----+-----+\n";
    for (const auto& row : state) {
        std::cout << "|  ";
        for (auto cell : row) {
            if (cell != 0) {
                std::cout << cell << "  |  ";
            } else {
                std::cout << "   |  ";
            }
        }
        std::cout << "\n+-----+-----+-----+\n";
    }
}

/**
 * @brief Инициализация начального и целевого состояний, а также вычисление целевых позиций объектов.
 */
void initialize() {
    // Пример 3x3 поля. Можно раскомментировать другой вариант.
    // initialField = {
    //     {0, 0, 1},
    //     {2, 0, 0},
    //     {2, 2, 0}
    // };

    // targetField = {
    //     {0, 2, 0},
    //     {0, 2, 2},
    //     {0, 0, 1}
    // };


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
    std::cout << "\nУникальные классы: ";
    for (auto tag : classes) {
        std::cout << tag << " ";
        targetTopLeft[tag] = getTopLeft(targetField, tag);
    }
    std::cout << "\n\nВерхние левые углы целевого положения:\n";
    for (auto tag : classes) {
        auto coord = targetTopLeft[tag];
        std::cout << "Объект " << tag << ": { " << coord.first << ", " << coord.second << " };\n";
    }
    std::cout << "\n~~~~~~~~ Поиск решения A*... ~~~~~~~~\n\n";
}

/**
 * @brief Преобразует состояние в строковое представление.
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
 * @brief Проверка возможности перемещения объекта.
 */
bool canMove(const std::vector<std::vector<int>>& state, int piece, int dr, int dc) {
    auto positions = getClassPositions(state, piece);
    std::vector<std::vector<int>> temp = state;
    for (auto& pos : positions) {
        temp[pos.first][pos.second] = 0;
    }
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
 * @brief Применяет перемещение объекта и возвращает новое состояние.
 */
std::vector<std::vector<int>> applyMove(const std::vector<std::vector<int>>& state, int piece, int dr, int dc) {
    auto positions = getClassPositions(state, piece);
    std::vector<std::vector<int>> newState = state;
    // Убираем объект с исходной позиции
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
 * @brief Структура узла для алгоритма A*.
 */
struct Node {
    std::vector<std::vector<int>> state;
    int g; // стоимость пути до этого состояния
    int h; // эвристическая оценка от этого состояния до цели
    int f; // сумма g + h
    int piece;            // объект, перемещение которого привело к этому состоянию
    std::string direction; // направление движения (например, "↑", "↓", "←", "→")
    Node* parent;         // указатель на родительский узел

    Node(const std::vector<std::vector<int>>& state, int g, int h, int piece = 0,
         const std::string& direction = "", Node* parent = nullptr)
        : state(state), g(g), h(h), f(g + h), piece(piece), direction(direction), parent(parent) {}
};

/**
 * @brief Компаратор для приоритетной очереди (узел с меньшим f имеет больший приоритет).
 */
struct CompareNode {
    bool operator()(const Node* a, const Node* b) const {
        return a->f > b->f;
    }
};

/**
 * @brief Генерация соседних состояний для текущего узла.
 */
std::vector<Node*> getNeighbors(Node* current) {
    std::vector<Node*> neighbors;
    // Возможные направления: (dr, dc) и символ направления
    std::vector<std::pair<std::pair<int, int>, std::string>> directions = {
        {{-1, 0}, "up"},
        {{1, 0}, "down"},
        {{0, -1}, "left"},
        {{0, 1}, "right"}
    };
    // Для каждого объекта из глобального набора
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
 * @brief Функция A*, возвращающая путь (последовательность узлов) от начального до целевого состояния.
 */
std::vector<Node*> a_star_solution() {
    Node* start = new Node(initialField, 0, heuristic(initialField));
    std::priority_queue<Node*, std::vector<Node*>, CompareNode> openSet;
    openSet.push(start);
    std::unordered_set<std::string> closedSet;
    Node* solution = nullptr;

    while (!openSet.empty()) {
        Node* current = openSet.top();
        openSet.pop();

        if (current->state == targetField) {
            solution = current;
            break;
        }

        std::string stateKey = stateToString(current->state);
        if (closedSet.find(stateKey) != closedSet.end())
            continue;
        closedSet.insert(stateKey);

        auto neighbors = getNeighbors(current);
        for (auto neighbor : neighbors) {
            std::string neighborKey = stateToString(neighbor->state);
            if (closedSet.find(neighborKey) == closedSet.end()) {
                openSet.push(neighbor);
            } else {
                delete neighbor;
            }
        }
    }

    std::vector<Node*> path;
    if (solution == nullptr) {
        std::cout << "Решение не найдено.\n";
        return path;
    }

    // Восстанавливаем путь от решения до начального состояния
    for (Node* node = solution; node != nullptr; node = node->parent) {
        path.push_back(node);
    }
    std::reverse(path.begin(), path.end());

    // Выводим последовательность ходов в консоль (пропуская начальное состояние)
    int moveNumber = 1;
    for (size_t i = 1; i < path.size(); i++) {
        std::cout << "Ход " << moveNumber << ": перемещение объекта " 
                  << path[i]->piece << " - " << path[i]->direction 
                  << " (g=" << path[i]->g << ", h=" << path[i]->h 
                  << ", f=" << path[i]->f << ")\n";
        moveNumber++;
    }
    return path;
}

/**
 * @brief Получить строку с подробным разбором эвристики для данного состояния.
 */
std::string getHeuristicBreakdown(const std::vector<std::vector<int>>& state) {
    std::stringstream ss;
    ss << "Heuristic (summ of Manhattan distances):\n";
    for (int tag : classes) {
        auto current = getTopLeft(state, tag);
        auto target = targetTopLeft[tag];
        int dist = std::abs(current.first - target.first) + std::abs(current.second - target.second);
        ss << "  Object " << tag << ": now state (" << current.first << ", " << current.second 
           << "), target (" << target.first << ", " << target.second << ") => " 
           << dist << "\n";
    }
    return ss.str();
}

/**
 * @brief Функция визуализации решения с использованием SFML.
 *
 * Для каждого узла (состояния) из найденного пути поочерёдно отрисовывается игровое поле с
 * дополнительной информацией: номер хода, направление движения, значения g, h, f и подробный разбор эвристики.
 */
void visualizeSolution(const std::vector<Node*>& path) {
    // Создаём окно
    sf::RenderWindow window(sf::VideoMode(BOARD_MARGIN * 2 + CELL_SIZE * BOARD_SIZE + 300, BOARD_MARGIN * 2 + CELL_SIZE * BOARD_SIZE), "A* Visualization");
    window.setFramerateLimit(60);

    // Загружаем шрифт (файл arial.ttf должен быть в рабочей директории)
    sf::Font font;
    if (!font.loadFromFile("../Nunito.ttf")) {
        std::cerr << "Не удалось загрузить шрифт arial.ttf\n";
        return;
    }

    // Переменная для номера текущего шага
    size_t currentStep = 0;

    // Пауза между шагами (в секундах)
    const float pauseTime = 2.0f;
    sf::Clock stepClock;
    stepClock.restart();

    while (window.isOpen()) {
        // Обработка событий окна (закрытие, нажатие клавиш)
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            // При нажатии пробела переходим к следующему шагу
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                if (currentStep < path.size() - 1) {
                    currentStep++;
                    stepClock.restart();
                }
            }
        }

        // Если прошло заданное время, автоматический переход к следующему шагу
        if (stepClock.getElapsedTime().asSeconds() >= pauseTime) {
            if (currentStep < path.size() - 1) {
                currentStep++;
                stepClock.restart();
            }
        }

        window.clear(sf::Color::White);

        // Отрисовка игрового поля
        Node* node = path[currentStep];
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
                cell.setPosition(BOARD_MARGIN + j * CELL_SIZE, BOARD_MARGIN + i * CELL_SIZE);
                cell.setFillColor(sf::Color(220, 220, 220));
                cell.setOutlineThickness(2);
                cell.setOutlineColor(sf::Color::Black);
                window.draw(cell);

                // Если в клетке есть число, отрисовываем его
                if (node->state[i][j] != 0) {
                    sf::Text text;
                    text.setFont(font);
                    text.setString(std::to_string(node->state[i][j]));
                    text.setCharacterSize(24);
                    text.setFillColor(sf::Color::Black);
                    // Центрирование текста
                    sf::FloatRect textRect = text.getLocalBounds();
                    text.setOrigin(textRect.left + textRect.width / 2.0f,
                                   textRect.top  + textRect.height / 2.0f);
                    text.setPosition(BOARD_MARGIN + j * CELL_SIZE + CELL_SIZE / 2,
                                     BOARD_MARGIN + i * CELL_SIZE + CELL_SIZE / 2);
                    window.draw(text);
                }
            }
        }

        // Подготовка и отрисовка информационной панели справа
        sf::Text infoText;
        infoText.setFont(font);
        infoText.setCharacterSize(16);
        infoText.setFillColor(sf::Color::Black);
        std::stringstream infoSS;
        infoSS << "Step " << currentStep << " of " << path.size()-1 << "\n";
        if (currentStep == 0)
            infoSS << "Initail state\n";
        else {
            infoSS << "Move Obj: " << node->piece 
                   << " (" << node->direction << ")\n";
        }
        infoSS << "g = " << node->g << "\n";
        infoSS << "h = " << node->h << "\n";
        infoSS << "f = " << node->f << "\n\n";
        infoSS << getHeuristicBreakdown(node->state);
        infoText.setString(infoSS.str());
        infoText.setPosition(BOARD_MARGIN + CELL_SIZE * BOARD_SIZE + 20, BOARD_MARGIN);
        window.draw(infoText);

        window.display();
    }
}

/**
 * @brief Главная функция.
 */
int main() {
    initialize();

    // Запускаем алгоритм A* и получаем путь (решение)
    std::vector<Node*> solutionPath = a_star_solution();
    if (solutionPath.empty()) {
        std::cout << "Решение не найдено.\n";
        return 0;
    }

    // Визуализируем найденное решение
    std::cout << "\nНажимайте пробел или ждите " << "2 сек. для перехода к следующему шагу визуализации.\n";
    visualizeSolution(solutionPath);

    return 0;
}
