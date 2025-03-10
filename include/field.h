

class Field {
public:    
    std::vector<std::vector<sf::RectangleShape>> Cells;
    std::vector<std::vector<int>> Class_cells; 



    Field(float Count_Cell, float Size_Window);

    // По координатам мыши (относительно окна) возвращает индексы ячейки
    std::pair<int, int> get_cell_index(float MousePosX, float MousePosY);

    // Границы ячейки: {x_min, y_min, x_max, y_max}
    std::vector<float> get_cell(float MousePosX, float MousePosY);

    float get_size_cell();

    // Отрисовка базовой сетки
    void Draw_field(sf::RenderWindow& window);

    // Отрисовка объединённых объектов (как в предыдущем примере)
    void Draw_Merged_Tags(sf::RenderWindow& window);

private: 
    int count_of_cell;
    float size_of_cell;
    float size_of_window;
    sf::Color getColorForClass(int class_value);

};