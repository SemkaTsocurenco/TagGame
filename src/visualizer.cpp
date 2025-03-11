#include "../include/main.hpp"





void visualizeSolution(const std::vector<Node*>& path) {
    // Создаём окно
    sf::RenderWindow window(sf::VideoMode(BOARD_MARGIN * 2 + CELL_SIZE * BOARD_SIZE , BOARD_MARGIN * 2 + CELL_SIZE * BOARD_SIZE), "A* Visualization");
    window.setFramerateLimit(60);

    // Загружаем шрифт (файл Nunito.ttf должен быть в рабочей директории)
    sf::Font font;
    if (!font.loadFromFile("../Nunito.ttf")) {
        std::cerr << "Не удалось загрузить шрифт Nunito.ttf\n";
        return;
    }

    // Переменная для номера текущего шага
    size_t currentStep = 0;

    // Пауза между шагами (в секундах)
    const float pauseTime = 0.5f;
    sf::Clock stepClock;
    stepClock.restart();

    while (window.isOpen()) {
        // Обработка событий окна (закрытие, нажатие клавиш)
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            // При нажатии пробела переходим к следующему шагу
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                if (currentStep < path.size() - 1) {
                    currentStep++;
                    stepClock.restart();
                }
            }
        }

        // Если прошло заданное время, автоматический переход к следующему шагу
        if (stepClock.getElapsedTime().asSeconds() >= pauseTime) {
            if (currentStep < path.size() - 1) {
                currentStep++;
                stepClock.restart();
            }
        }

        window.clear(sf::Color::White);

        // Отрисовка игрового поля
        Node* node = path[currentStep];
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
                cell.setPosition(BOARD_MARGIN + j * CELL_SIZE, BOARD_MARGIN + i * CELL_SIZE);
                cell.setFillColor(sf::Color(220, 220, 220));
                cell.setOutlineThickness(2);
                cell.setOutlineColor(sf::Color::Black);
                window.draw(cell);

                // Если в клетке есть фишка (число не равно 0), отрисовываем её как круг с эффектом объёма
                if (node->state[i][j] != 0) {
                    int piece = node->state[i][j];
                    sf::Color pieceColor = getPieceColor(piece);
                    // Вычисление центра клетки
                    float centerX = BOARD_MARGIN + j * CELL_SIZE + CELL_SIZE / 2;
                    float centerY = BOARD_MARGIN + i * CELL_SIZE + CELL_SIZE / 2;
                    // Создание отбрасываемой тени (смещение на 4 пикселя вправо и вниз)
                    sf::CircleShape shadowCircle(35);
                    shadowCircle.setFillColor(sf::Color(
                        static_cast<sf::Uint8>(pieceColor.r * 0.7),
                        static_cast<sf::Uint8>(pieceColor.g * 0.7),
                        static_cast<sf::Uint8>(pieceColor.b * 0.7)
                    ));
                    shadowCircle.setPosition(centerX - 35 + 4, centerY - 35 + 4);
                    window.draw(shadowCircle);
                    // Основной круг фишки
                    sf::CircleShape chipCircle(35);
                    chipCircle.setFillColor(pieceColor);
                    chipCircle.setOutlineThickness(3);
                    chipCircle.setOutlineColor(sf::Color::Black);
                    chipCircle.setPosition(centerX - 35, centerY - 35);
                    window.draw(chipCircle);
                    // Отрисовка номера фишки по центру
                    sf::Text pieceText;
                    pieceText.setFont(font);
                    pieceText.setString(std::to_string(piece));
                    pieceText.setCharacterSize(24);
                    pieceText.setFillColor(sf::Color::Black);
                    pieceText.setOutlineThickness(5);
                    pieceText.setOutlineColor(sf::Color::White);
                    sf::FloatRect textRect = pieceText.getLocalBounds();
                    pieceText.setOrigin(textRect.left + textRect.width / 2.0f,
                                        textRect.top  + textRect.height / 2.0f);
                    pieceText.setPosition(centerX, centerY);
                    window.draw(pieceText);
                }
            }
        }

        // Подготовка и отрисовка информационной панели справа
        sf::Text infoText;
        infoText.setFont(font);
        infoText.setCharacterSize(16);
        infoText.setFillColor(sf::Color::Black);
        std::stringstream infoSS;
        infoSS << "Step " << currentStep << " of " << path.size()-1 ;
        infoSS << "   |   cost = " << node->g ;
        infoSS << "   |   heuristic = " << node->h ;
        infoSS << "   |   total cost = " << node->f ;
        infoText.setString(infoSS.str());
        infoText.setPosition(BOARD_MARGIN , BOARD_MARGIN/2 - 5);
        window.draw(infoText);
        std::stringstream infoSS1;
        infoSS1 << "Press  \"SPASE\" to sikp to the next step";
        infoText.setString(infoSS1.str());
        infoText.setPosition(BOARD_MARGIN , BOARD_MARGIN + 5 + CELL_SIZE * BOARD_SIZE);
        window.draw(infoText);

        window.display();
    }
}
