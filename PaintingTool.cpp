#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

//Brush default properties
const int BrushRadiusDefault = 15;
const sf::Color BrushColorDefault = sf::Color::White;

int main()
{
    //Variables for brush
    int brushRadius = BrushRadiusDefault;
    sf::Color brushColor = BrushColorDefault;

    //Create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "My Little Drawing Tool");

    // Create brush
    sf::CircleShape shape(brushRadius);
    shape.setFillColor(brushColor);

    // Game loop
    while (window.isOpen())
    {
        //Window close handler
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        //window.clear();
        
        // Set brush position to the mouse cursor
        shape.setPosition(sf::Mouse::getPosition(window).x - brushRadius, sf::Mouse::getPosition(window).y - brushRadius);
        window.draw(shape);
        window.display();
    }

    return 0;
}