class eventProcessor{
    public: 
    // Левый клик – добавление объектов (как раньше)
        void LeftPress(sf::Event event, sf::RenderWindow& window, int class_element, Field& field);
        void LeftRealise(sf::Event event, sf::RenderWindow& window, int class_element, Field& field, int& class_number);
};