#include "main.hpp"



class Field{
	public:	
		std::vector<std::vector<sf::RectangleShape>> Cells;
		std::vector<std::vector<int>> Class_cells;

		Field(float Count_Cell, float Size_Window){
			count_of_cell = Count_Cell;
			size_of_cell = Size_Window/Count_Cell;
			size_of_window = Size_Window;
			Cells.resize(size_of_cell);
			for (int i = 0; i < Count_Cell; i++ ){
				Cells[i].resize(size_of_cell);
			}

			for (int i = 0; i < Count_Cell; i++ ){
				for (int j = 0; j < Count_Cell; j++ ){
					sf::RectangleShape rect;
					rect.setFillColor(sf::Color::White);
					rect.setOutlineColor(sf::Color::Black);
					rect.setPosition({size_of_cell * i , size_of_cell * j});
					rect.setSize({size_of_cell, size_of_cell});
					rect.setOutlineThickness(2.0f);
					Cells[i][j] = rect;
				}
			}
		}

		void Draw_field(sf::RenderWindow& window){
			for (int i = 0; i < count_of_cell; i++ ){
				for (int j = 0; j < count_of_cell; j++ ){
					window.draw(Cells[i][j]);
				}
			}
		}

	private: 
		int count_of_cell;
		float size_of_cell;
		float size_of_window;

};

int main (){

    sf::RenderWindow window(sf::VideoMode(500, 500), "TagGame", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(30); 
	sf::View view(sf::FloatRect(0, 0, 500, 500));
    window.setView(view);
    window.setVerticalSyncEnabled(true); 

	  while (window.isOpen())
    {
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape){
				window.close();
			}
		}

		window.clear(sf::Color::White);
		window.setView(view);
		Field field(10, 500);
		field.Draw_field(window);
		window.display();
         
    }
	return 0;
}
