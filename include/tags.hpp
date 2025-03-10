class tags {
private:
    struct Tag {
        sf::RectangleShape Shape;
        std::pair<float, float> possition;
        int class_tag;
    };
    std::vector<Tag> Tags;

    sf::Color getColorForClass(int class_value);
public:


    int get_sizeTags();

    // Добавление тэга с нужным цветом
    void add_tag(std::pair<float, float> pos, int class_of_tag, float size_of_cell);

    tags(){};

    // Отрисовка отдельных тегов (для отладки)
    void Draw_Tags(sf::RenderWindow& window);

};

