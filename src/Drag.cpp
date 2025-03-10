
#include "../include/main.hpp"

// Функция начала перетаскивания (правый клик)
void DragProc::startDragging(sf::Event event, Field &field, sf::RenderWindow &window) {
    sf::Vector2i Mouse_pos = sf::Mouse::getPosition(window);
    auto cell_index = field.get_cell_index(Mouse_pos.x, Mouse_pos.y);
    if(cell_index.first == -1 || cell_index.second == -1)
        return;
    int cellClass = field.Class_cells[cell_index.first][cell_index.second];
    if(cellClass != 0) {
        dragState.dragging = true;
        dragState.selectedClass = cellClass;
        dragState.group = field.getGroup(cell_index.first, cell_index.second);
        dragState.startMouseIndex = cell_index;
    }
}

// Обработка перетаскивания
void DragProc::processDragging(Field &field, sf::RenderWindow &window) {
    if(!dragState.dragging)
        return;
    // Получаем позицию мыши относительно окна
    sf::Vector2i currentPos = sf::Mouse::getPosition(window);
    auto currentIndex = field.get_cell_index(currentPos.x, currentPos.y);
    if(currentIndex.first == -1 || currentIndex.second == -1)
        return;
    // Вычисляем смещение по клеткам относительно опорной точки
    int dx = currentIndex.first - dragState.startMouseIndex.first;
    int dy = currentIndex.second - dragState.startMouseIndex.second;
    // Ограничим перемещение по одной оси: выбираем ось с большим абсолютным смещением
    if (std::abs(dx) > std::abs(dy)) {
        dy = 0;
    } else {
        dx = 0;
    }
    if(dx == 0 && dy == 0)
        return;
    int rows = field.Class_cells.size();
    int cols = field.Class_cells[0].size();
    // Создаём временную копию поля и освобождаем клетки объекта
    auto temp = field.Class_cells;
    for(auto cell : dragState.group) {
        temp[cell.first][cell.second] = 0;
    }
    // Проверяем, можно ли переместить группу на (dx, dy)
    bool canMove = true;
    for(auto cell : dragState.group) {
        int ni = cell.first + dx;
        int nj = cell.second + dy;
        if(ni < 0 || ni >= rows || nj < 0 || nj >= cols) {
            canMove = false;
            break;
        }
        if(temp[ni][nj] != 0) {
            canMove = false;
            break;
        }
    }
    if(canMove) {
        // Очищаем старые позиции
        for(auto cell : dragState.group) {
            field.Class_cells[cell.first][cell.second] = 0;
        }
        // Обновляем координаты группы
        for(auto &cell : dragState.group) {
            cell.first += dx;
            cell.second += dy;
        }
        // Записываем новые позиции объекта
        for(auto cell : dragState.group) {
            field.Class_cells[cell.first][cell.second] = dragState.selectedClass;
        }
        // Обновляем опорную точку
        dragState.startMouseIndex.first += dx;
        dragState.startMouseIndex.second += dy;
    }
}

// Завершение перетаскивания
void DragProc::stopDragging() {
    dragState.dragging = false;
    dragState.group.clear();
    dragState.selectedClass = 0;
}
