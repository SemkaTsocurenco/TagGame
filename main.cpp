#include "main.hpp"

int main (){

    sf::RenderWindow window(sf::VideoMode(500, 500), "TagGame", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(30); 
	sf::View view(sf::FloatRect(0, 0, 500, 500));
    window.setView(view);
    window.setVerticalSyncEnabled(true); 

	  while (window.isOpen())
    {
		window.clear(sf::Color::White);
		window.setView(view);
		window.display();
         
    }
	return 0;
}
