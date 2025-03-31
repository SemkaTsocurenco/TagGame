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
#include <iomanip>


///////////////////////////////////////////////////////////////////////////////////////
//
//                              Инициализация глобальных переменных      
//
///////////////////////////////////////////////////////////////////////////////////////



// Размеры доски и визуализации
const int BOARD_SIZE = 5;         // размер игрового поля 5x5
const int CELL_SIZE = 100;        // размер клетки в пикселях
const int BOARD_MARGIN = 50;      // отступ поля от края окна

// Поля: начальное и целевое состояние
extern std::vector<std::vector<int>> initialField;
extern std::vector<std::vector<int>> targetField;

// Уникальные объекты (метки)
extern std::set<int> classes;
extern std::set<int> classes_target;

// Для каждого объекта сохраняем координаты верхнего левого угла целевого положения
extern std::unordered_map<int, std::pair<int, int>> targetTopLeft;

/**
 * @brief Инициализация начального и целевого состояний, а также вычисление целевых позиций объектов.
 */
void initialize();


///////////////////////////////////////////////////////////////////////////////////////
//
//                              Вспомогательные функции работы с доской      
//
///////////////////////////////////////////////////////////////////////////////////////

// Функция для получения цвета фишки по её номеру
sf::Color getPieceColor(int piece) ;

/**
 * @brief Получить список позиций для заданного объекта.
 */
std::vector<std::pair<int, int>> getClassPositions(const std::vector<std::vector<int>>& state, int tag) ;

/**
 * @brief Определить координаты верхнего левого угла для объекта.
 *
 * Возвращает пару (столбец, строка).
 */
std::pair<int, int> getTopLeft(const std::vector<std::vector<int>>& state, int tag);


/**
 * @brief Преобразует состояние в строковое представление.
 */
std::string stateToString(const std::vector<std::vector<int>>& state);

/**
 * @brief Проверка возможности перемещения объекта.
 */
bool canMove(const std::vector<std::vector<int>>& state, int piece, int dr, int dc);

/**
 * @brief Применяет перемещение объекта и возвращает новое состояние.
 */
std::vector<std::vector<int>> applyMove(const std::vector<std::vector<int>>& state, int piece, int dr, int dc);


///////////////////////////////////////////////////////////////////////////////////////
//
//                              Алгоритм A*      
//
///////////////////////////////////////////////////////////////////////////////////////


/**
 * @brief Структура узла для алгоритма A*.
 */
struct Node {
    std::vector<std::vector<int>> state;
    int g; // стоимость пути до этого состояния
    int h; // эвристическая оценка от этого состояния до цели
    int f; // сумма g + h
    int piece;            // объект, перемещение которого привело к этому состоянию
    std::string direction; // направление движения (например, "up", "down", "left", "right")
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
 * @brief Эвристическая функция.
 *
 * Суммируется манхэттенское расстояние между текущими и целевыми позициями (верхние левые углы) для каждого объекта.
 */
int heuristic(const std::vector<std::vector<int>>& state);

/**
 * @brief Генерация соседних состояний для текущего узла.
 */
std::vector<Node*> getNeighbors(Node* current);

/**
 * @brief Функция A*, возвращающая путь (последовательность узлов) от начального до целевого состояния.
 */
std::vector<Node*> a_star_solution();

///////////////////////////////////////////////////////////////////////////////////////
//
//                              Вывод в консоль      
//
///////////////////////////////////////////////////////////////////////////////////////


/**
 * @brief Вывод поля в консоль.
 */
void printField(const std::vector<std::vector<int>>& state, const std::string& title);
void printClasses(); 
void printA_star_path (std::vector<Node*>& path);


///////////////////////////////////////////////////////////////////////////////////////
//
//                              SFML Визуализация     
//
///////////////////////////////////////////////////////////////////////////////////////


/**
 * @brief Функция визуализации решения с использованием SFML.
 *
 * Для каждого узла (состояния) из найденного пути поочерёдно отрисовывается игровое поле с
 * дополнительной информацией: номер хода, направление движения, значения g, h, f и подробный разбор эвристики.
 * Визуализация изменена: вместо простых числовых значений фишки отрисовываются как цветные круги с эффектом объёма.
 */
void visualizeSolution(const std::vector<Node*>& path);