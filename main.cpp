#include "./include/main.hpp"
#include <stack>
#include <vector>
#include <algorithm>
#include <cmath>







int main (){
    eventProcessor EP;
    DragProc DRAG;
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
                DRAG.startDragging(event, field, window);
            }
            // Перемещение объекта
            if (event.type == sf::Event::MouseMoved) {
                DRAG.processDragging(field, window);
            }
            // Завершение перетаскивания
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right) {
                DRAG.stopDragging();
            }

            // Левый клик для создания объектов
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                EP.LeftRealise(event, window, class_number, t, field, class_number);
            }        

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                EP.LeftPress(event, window, class_number, t, field);
            }
        }

        window.clear(sf::Color::White);
        field.Draw_field(window);
        field.Draw_Merged_Tags(window);
        window.display();
    }
    return 0;
}
