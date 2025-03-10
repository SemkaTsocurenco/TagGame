#include "../include/main.hpp"



Field::Field(float Count_Cell, float Size_Window){
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
std::pair<int, int> Field::get_cell_index(float MousePosX, float MousePosY){
    if (MousePosX < 0 || MousePosY < 0 || MousePosX > size_of_window || MousePosY > size_of_window){
        return {-1, -1};
    }
    int i = static_cast<int>(MousePosX / size_of_cell);
    int j = static_cast<int>(MousePosY / size_of_cell);
    return {i, j};
}

// Границы ячейки: {x_min, y_min, x_max, y_max}
std::vector<float> Field::get_cell(float MousePosX, float MousePosY){
    if (MousePosX < 1 || MousePosY < 1 || MousePosX > size_of_window || MousePosY > size_of_window){
        return {0, 0, 0, 0};
    }
    float x_min = size_of_cell * static_cast<int>(MousePosX / size_of_cell);
    float x_max = x_min + size_of_cell;
    float y_min = size_of_cell * static_cast<int>(MousePosY / size_of_cell);
    float y_max = y_min + size_of_cell;
    return {x_min, y_min, x_max, y_max};
}

float Field::get_size_cell(){
    return size_of_cell;
}

// Отрисовка базовой сетки
void Field::Draw_field(sf::RenderWindow& window){
    for (int i = 0; i < count_of_cell; i++ ){
        for (int j = 0; j < count_of_cell; j++ ){                    
            window.draw(Cells[i][j]);
        }
    }
}

// Отрисовка объединённых объектов (как в предыдущем примере)
void Field::Draw_Merged_Tags(sf::RenderWindow& window) {
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

                    sf::Text text;
                    text.setPosition({(ci * size_of_cell+ size_of_cell / 2) - 14, (cj * size_of_cell  + size_of_cell / 2) - 14});
                    text.setFillColor(sf::Color::Black);
                    text.setCharacterSize(25);
                    sf::Font f;
                    f.loadFromFile("../res/Kameron.ttf");
                    text.setFont(f);
                    text.setStyle(text.Bold);
                    text.setOutlineThickness(3);
                    text.setOutlineColor(sf::Color::White);
                    text.setString(std::to_string(currentClass));
                    window.draw(rect);
                    window.draw(text);

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



// Функция, возвращающая цвет для номера класса (более 15 вариантов)
sf::Color Field::getColorForClass(int class_value) {
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


// По координатам (индексу) и полю возвращает группу (DFS) для объекта
std::vector<std::pair<int,int>> Field::getGroup(int i, int j) {
    std::vector<std::pair<int,int>> group;
    int currentClass = this->Class_cells[i][j];
    int rows = this->Class_cells.size();
    int cols = this->Class_cells[0].size();
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
               !visited[ni][nj] && this->Class_cells[ni][nj] == currentClass) {
                visited[ni][nj] = true;
                st.push({ni,nj});
            }
        }
    }
    return group;
}