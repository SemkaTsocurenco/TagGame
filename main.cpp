#include "main.hpp"



class tags{
	private: 
		struct Tag{
			sf::RectangleShape Shape;
			std::pair <float, float> possition;
			int class_tag;
		};
		std::vector<Tag> Tags;
	public:
		int get_sizeTags(){
			return Tags.size();
		};

		void add_tag(std::pair<float, float> pos, int class_of_tag){
			sf::RectangleShape rect;
			rect.setFillColor(sf::Color::Red);
			rect.setPosition ({pos.first, pos.second});
			Tag t;
			t.Shape = rect;
			t.class_tag = class_of_tag;
			t.possition = pos;
			Tags.push_back(t);
		};

		tags(){

		};

		void Draw_Tags(sf::RenderWindow& window){
			for (auto t : Tags){
				sf::Text text;
				text.setPosition({t.possition.first, t.possition.second});
				text.setColor(sf::Color::Black);
				text.setCharacterSize(5.0f);
				text.setString (std::to_string(t.class_tag));
				window.draw(text);
				window.draw(t.Shape);
			}
		};

};

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



void LeftRealeased(sf::Event event){
	
};

void LeftPress(sf::Event event, sf::RenderWindow& window, int class_element, tags t){
	sf::Vector2i Mouse_possition = sf::Mouse::getPosition(window);
	int x_cell = window.getSize().x / Mouse_possition.x;
	int y_cell = window.getSize().y / Mouse_possition.y;
	t.add_tag({x_cell, y_cell}, class_element);
	

}

int main (){

    sf::RenderWindow window(sf::VideoMode(500, 500), "TagGame", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(30); 
	sf::View view(sf::FloatRect(0, 0, 500, 500));
    window.setView(view);
    window.setVerticalSyncEnabled(true); 
	int class_number = 0 ;
	tags t;
	while (window.isOpen())
    {
		sf::Event event;
	
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape){
				window.close();
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
				// std::cout<<"LEFT\n";
				LeftPress(event, window, class_number, t);
			}

			if (event.type == sf::Event::MouseButtonReleased) {
				
				class_number++;
				// std::cout<<class_number<<"\n";

			}			
		}
		


		window.clear(sf::Color::White);
		window.setView(view);
		Field field(5, 500);
		field.Draw_field(window);
		std::cout<<t.get_sizeTags()<<"\n";
		t.Draw_Tags(window);
		window.display();
         
    }
	return 0;
}
