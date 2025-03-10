#include "./include/main.hpp"
#include <stack>
#include <vector>
#include <algorithm>
#include <cmath>




//--------------------------------------------------------
// Структура для хранения состояния перетаскивания
struct DragState {
    bool dragging = false;
    int selectedClass = 0;
    // Группа ячеек, принадлежащих перемещаемому объекту
    std::vector<std::pair<int,int>> group;
    // Опорная ячейка, с которой начат драггинг
    std::pair<int,int> startMouseIndex;
};

DragState dragState;

// По координатам (индексу) и полю возвращает группу (DFS) для объекта
std::vector<std::pair<int,int>> getGroup(int i, int j, Field &field) {
    std::vector<std::pair<int,int>> group;
    int currentClass = field.Class_cells[i][j];
    int rows = field.Class_cells.size();
    int cols = field.Class_cells[0].size();
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::stack<std::pair<int,int>> st;
    st.push({i,j});
    visited[i][j] = true;
    while(!st.empty()){
        auto [ci, cj] = st.top();
        st.pop();
        group.push_back({ci,cj});
        const std::vector<std::pair<int,int>> directions = { {1,0}, {-1,0}, {0,1}, {0,-1} };
        for(auto d: directions){
            int ni = ci + d.first, nj = cj + d.second;
            if(ni >= 0 && ni < rows && nj >= 0 && nj < cols &&
               !visited[ni][nj] && field.Class_cells[ni][nj] == currentClass) {
                visited[ni][nj] = true;
                st.push({ni,nj});
            }
        }
    }
    return group;
}

// Левый клик – добавление объектов (как раньше)
void LeftPress(sf::Event event, sf::RenderWindow& window, int class_element, tags& t, Field& field){
    sf::Vector2i Mouse_pos = sf::Mouse::getPosition(window);
    auto cell_index = field.get_cell_index(Mouse_pos.x, Mouse_pos.y);
    if(cell_index.first == -1 || cell_index.second == -1)
        return;
    if(field.Class_cells[cell_index.first][cell_index.second] == 0){
        std::vector<float> a = field.get_cell(Mouse_pos.x, Mouse_pos.y);
        if(!a.empty()){
            t.add_tag({a[0], a[1]}, class_element, field.get_size_cell());
            field.Class_cells[cell_index.first][cell_index.second] = class_element;
        }
    }
}

// Функция начала перетаскивания (правый клик)
void startDragging(sf::Event event, Field &field, sf::RenderWindow &window) {
    sf::Vector2i Mouse_pos = sf::Mouse::getPosition(window);
    auto cell_index = field.get_cell_index(Mouse_pos.x, Mouse_pos.y);
    if(cell_index.first == -1 || cell_index.second == -1)
        return;
    int cellClass = field.Class_cells[cell_index.first][cell_index.second];
    if(cellClass != 0) {
        dragState.dragging = true;
        dragState.selectedClass = cellClass;
        dragState.group = getGroup(cell_index.first, cell_index.second, field);
        dragState.startMouseIndex = cell_index;
    }
}

// Обработка перетаскивания
void processDragging(Field &field, sf::RenderWindow &window) {
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
void stopDragging() {
    dragState.dragging = false;
    dragState.group.clear();
    dragState.selectedClass = 0;
}

int main (){
    sf::RenderWindow window(sf::VideoMode(500, 500), "TagGame", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(30);
    sf::View view(sf::FloatRect(0, 0, 500, 500));
    window.setView(view);
    int class_number = 1;
    tags t;
    Field field(5, 500);

    while (window.isOpen())
    {
        sf::Event event;
    
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();

            // Начало перетаскивания правой кнопкой
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
                startDragging(event, field, window);
            }
            // Перемещение объекта
            if (event.type == sf::Event::MouseMoved) {
                processDragging(field, window);
            }
            // Завершение перетаскивания
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right) {
                stopDragging();
            }

            // Левый клик для создания объектов
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i Mouse_pos = sf::Mouse::getPosition(window);
                auto cell_index = field.get_cell_index(Mouse_pos.x, Mouse_pos.y);
                if(cell_index.first != -1 && cell_index.second != -1 &&
                   field.Class_cells[cell_index.first][cell_index.second] == class_number)
                    class_number++;
                std::cout << "class number: " << class_number << "\n";
            }        

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                LeftPress(event, window, class_number, t, field);
            }
        }

        window.clear(sf::Color::White);
        field.Draw_field(window);
        field.Draw_Merged_Tags(window);
        window.display();
    }
    return 0;
}
