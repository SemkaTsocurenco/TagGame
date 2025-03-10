#include "../include/main.hpp"

// Функция, возвращающая цвет для номера класса (более 15 вариантов)


int tags::get_sizeTags(){
    return Tags.size();
}

// Добавление тэга с нужным цветом
void tags::add_tag(std::pair<float, float> pos, int class_of_tag, float size_of_cell){
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


// Отрисовка отдельных тегов (для отладки)
void tags::Draw_Tags(sf::RenderWindow& window){
    for (auto t : Tags){
        sf::Text text;
        text.setPosition({t.possition.first, t.possition.second});
        text.setFillColor(sf::Color::Black);
        text.setCharacterSize(20);
        sf::Font f;
        f.loadFromFile("../res/ShareTechMonoRegular.ttf");
        text.setFont(f);
        text.setString(std::to_string(t.class_tag));
        window.draw(t.Shape);
        window.draw(text);
    }
}



// Функция, возвращающая цвет для номера класса (более 15 вариантов)
sf::Color tags::getColorForClass(int class_value) {
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


