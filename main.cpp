#include "main.hpp"
#include <stack>
#include <vector>
#include <algorithm>
#include <cmath>

// Функция, возвращающая цвет для номера класса (более 15 вариантов)
sf::Color getColorForClass(int class_value) {
    static const std::vector<sf::Color> colors = {
        sf::Color(255, 0, 0),       // Red
        sf::Color(0, 0, 255),       // Blue
        sf::Color(0, 255, 0),       // Green
        sf::Color(255, 0, 255),     // Magenta
        sf::Color(255, 255, 0),     // Yellow
        sf::Color(0, 255, 255),     // Cyan
        sf::Color(255, 165, 0),     // Orange
        sf::Color(128, 0, 128),     // Purple
        sf::Color(0, 255, 128),     // Lime-ish
        sf::Color(255, 192, 203),   // Pink
        sf::Color(0, 128, 128),     // Teal
        sf::Color(255, 215, 0),     // Gold
        sf::Color(165, 42, 42),     // Brown
        sf::Color(128, 128, 0),     // Olive
        sf::Color(0, 0, 128),       // Navy
        sf::Color(199, 21, 133)     // MediumVioletRed
    };
    int idx = (class_value - 1) % colors.size();
    return colors[idx];
}

class tags {
private:
    struct Tag {
        sf::RectangleShape Shape;
        std::pair<float, float> possition;
        int class_tag;
    };
    std::vector<Tag> Tags;
public:
    int get_sizeTags(){
        return Tags.size();
    }

    // Добавление тэга с нужным цветом
    void add_tag(std::pair<float, float> pos, int class_of_tag, float size_of_cell){
        sf::RectangleShape rect;
        rect.setFillColor(getColorForClass(class_of_tag));
        rect.setPosition({pos.first, pos.second});
        // Немного уменьшаем размер для отступов
        rect.setSize({ size_of_cell - 2, size_of_cell - 2 });
        Tag t;
        t.Shape = rect;
        t.class_tag = class_of_tag;
        t.possition = pos;
        Tags.push_back(t);
    }

    tags(){};

    // Отрисовка отдельных тегов (для отладки)
    void Draw_Tags(sf::RenderWindow& window){
        for (auto t : Tags){
            sf::Text text;
            text.setPosition({t.possition.first, t.possition.second});
            text.setFillColor(sf::Color::Black);
            text.setCharacterSize(20);
            sf::Font f;
            f.loadFromFile("../ShareTechMonoRegular.ttf");
            text.setFont(f);
            text.setString(std::to_string(t.class_tag));
            window.draw(t.Shape);
            window.draw(text);
        }
    }
};

class Field {
public:    
    std::vector<std::vector<sf::RectangleShape>> Cells;
    // Матрица: 0 – пустая, иначе номер класса объекта
    std::vector<std::vector<int>> Class_cells; 

    Field(float Count_Cell, float Size_Window){
        count_of_cell = Count_Cell;
        size_of_cell = Size_Window / Count_Cell;
        size_of_window = Size_Window;
        Cells.resize(Count_Cell);
        Class_cells.resize(Count_Cell);
        for (int i = 0; i < Count_Cell; i++ ){
            Cells[i].resize(Count_Cell);
            Class_cells[i].resize(Count_Cell, 0);
        }
        for (int i = 0; i < Count_Cell; i++ ){
            for (int j = 0; j < Count_Cell; j++ ){
                sf::RectangleShape rect;
                rect.setFillColor(sf::Color::White);
                rect.setOutlineColor(sf::Color::Black);
                rect.setPosition({size_of_cell * i, size_of_cell * j});
                rect.setSize({size_of_cell, size_of_cell});
                rect.setOutlineThickness(2.0f);
                Cells[i][j] = rect;
            }
        }
    }

    // По координатам мыши (относительно окна) возвращает индексы ячейки
    std::pair<int, int> get_cell_index(float MousePosX, float MousePosY){
        if (MousePosX < 0 || MousePosY < 0 || MousePosX > size_of_window || MousePosY > size_of_window){
            return {-1, -1};
        }
        int i = static_cast<int>(MousePosX / size_of_cell);
        int j = static_cast<int>(MousePosY / size_of_cell);
        return {i, j};
    }

    // Границы ячейки: {x_min, y_min, x_max, y_max}
    std::vector<float> get_cell(float MousePosX, float MousePosY){
        if (MousePosX < 1 || MousePosY < 1 || MousePosX > size_of_window || MousePosY > size_of_window){
            return {0, 0, 0, 0};
        }
        float x_min = size_of_cell * static_cast<int>(MousePosX / size_of_cell);
        float x_max = x_min + size_of_cell;
        float y_min = size_of_cell * static_cast<int>(MousePosY / size_of_cell);
        float y_max = y_min + size_of_cell;
        return {x_min, y_min, x_max, y_max};
    }

    float get_size_cell(){
        return size_of_cell;
    }

    // Отрисовка базовой сетки
    void Draw_field(sf::RenderWindow& window){
        for (int i = 0; i < count_of_cell; i++ ){
            for (int j = 0; j < count_of_cell; j++ ){                    
                window.draw(Cells[i][j]);
            }
        }
    }

    // Отрисовка объединённых объектов (как в предыдущем примере)
    void Draw_Merged_Tags(sf::RenderWindow& window) {
        int rows = Class_cells.size();
        if (rows == 0) return;
        int cols = Class_cells[0].size();
        std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
        const float outlineThickness = 2.0f;
        for (int i = 0; i < rows; i++){
            for (int j = 0; j < cols; j++){
                if (!visited[i][j] && Class_cells[i][j] != 0) {
                    int currentClass = Class_cells[i][j];
                    std::vector<std::pair<int,int>> group;
                    // DFS для сбора группы (4-связность)
                    std::stack<std::pair<int,int>> st;
                    st.push({i,j});
                    visited[i][j] = true;
                    while(!st.empty()){
                        auto [ci, cj] = st.top();
                        st.pop();
                        group.push_back({ci,cj});
                        const std::vector<std::pair<int,int>> directions = { {1,0}, {-1,0}, {0,1}, {0,-1} };
                        for(auto d: directions) {
                            int ni = ci + d.first, nj = cj + d.second;
                            if(ni >= 0 && ni < rows && nj >= 0 && nj < cols &&
                               !visited[ni][nj] && Class_cells[ni][nj] == currentClass) {
                                visited[ni][nj] = true;
                                st.push({ni, nj});
                            }
                        }
                    }
                    // Заливаем каждую ячейку группы (без обводки)
                    sf::Color fillColor = getColorForClass(currentClass);
                    for (auto cell : group) {
                        int ci = cell.first, cj = cell.second;
                        sf::RectangleShape rect;
                        rect.setPosition(ci * size_of_cell, cj * size_of_cell);
                        rect.setSize({size_of_cell, size_of_cell});
                        rect.setFillColor(fillColor);
                        rect.setOutlineThickness(0);
                        window.draw(rect);
                    }
                    // Отрисовываем внешнюю обводку для ячеек группы
                    for (auto cell : group) {
                        int ci = cell.first, cj = cell.second;
                        float x = ci * size_of_cell;
                        float y = cj * size_of_cell;
                        // Верхняя сторона
                        if (cj == 0 || Class_cells[ci][cj - 1] != currentClass) {
                            sf::RectangleShape edge(sf::Vector2f(size_of_cell, outlineThickness));
                            edge.setPosition(x, y);
                            edge.setFillColor(sf::Color::Black);
                            window.draw(edge);
                        }
                        // Нижняя сторона
                        if (cj == cols - 1 || Class_cells[ci][cj + 1] != currentClass) {
                            sf::RectangleShape edge(sf::Vector2f(size_of_cell, outlineThickness));
                            edge.setPosition(x, y + size_of_cell - outlineThickness);
                            edge.setFillColor(sf::Color::Black);
                            window.draw(edge);
                        }
                        // Левая сторона
                        if (ci == 0 || Class_cells[ci - 1][cj] != currentClass) {
                            sf::RectangleShape edge(sf::Vector2f(outlineThickness, size_of_cell));
                            edge.setPosition(x, y);
                            edge.setFillColor(sf::Color::Black);
                            window.draw(edge);
                        }
                        // Правая сторона
                        if (ci == rows - 1 || Class_cells[ci + 1][cj] != currentClass) {
                            sf::RectangleShape edge(sf::Vector2f(outlineThickness, size_of_cell));
                            edge.setPosition(x + size_of_cell - outlineThickness, y);
                            edge.setFillColor(sf::Color::Black);
                            window.draw(edge);
                        }
                    }
                }
            }
        }
    }

private: 
    int count_of_cell;
    float size_of_cell;
    float size_of_window;
};

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
