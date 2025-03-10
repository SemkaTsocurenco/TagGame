#include "../include/main.hpp"


// Левый клик – добавление объектов (как раньше)
void eventProcessor::LeftPress(sf::Event event, sf::RenderWindow& window, int class_element, Field& field){
    sf::Vector2i Mouse_pos = sf::Mouse::getPosition(window);
    auto cell_index = field.get_cell_index(Mouse_pos.x, Mouse_pos.y);
    if(cell_index.first == -1 || cell_index.second == -1)
        return;
    if(field.Class_cells[cell_index.first][cell_index.second] == 0){
        field.Class_cells[cell_index.first][cell_index.second] = class_element;
    }
}


// Левый клик – добавление объектов (как раньше)
void eventProcessor::LeftRealise(sf::Event event, sf::RenderWindow& window, int class_element, Field& field, int& class_number){
    sf::Vector2i Mouse_pos = sf::Mouse::getPosition(window);
    auto cell_index = field.get_cell_index(Mouse_pos.x, Mouse_pos.y);
    if(cell_index.first != -1 && cell_index.second != -1 &&
        field.Class_cells[cell_index.first][cell_index.second] == class_number)
        class_number++;
    std::cout << "class number: " << class_number << "\n";
}