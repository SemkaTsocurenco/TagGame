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


class DragProc {
    public:
        DragState dragState;

        // Функция начала перетаскивания (правый клик)
        void startDragging(sf::Event event, Field &field, sf::RenderWindow &window) ;

        // Обработка перетаскивания
        void processDragging(Field &field, sf::RenderWindow &window) ;

        // Завершение перетаскивания
        void stopDragging() ;

};