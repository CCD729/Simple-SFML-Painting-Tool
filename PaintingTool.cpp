#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include <iostream>

//Brush default properties
const int BrushRadiusDefault = 15;
const sf::Color BrushColorDefault = sf::Color(170, 196, 255);

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
            // Space key creates a screenshot
            if (event.key.code == sf::Keyboard::Space) {
                sf::Texture texture;
                texture.create(window.getSize().x, window.getSize().y);
                texture.update(window);
                if (texture.copyToImage().saveToFile("Mywork.png"))
                {
                    std::cout << "Screenshot saved to Mywork.png" << std::endl;
                }
            }
        }
        //window.clear();
        
        // Set brush position to the mouse cursor
        shape.setPosition(sf::Mouse::getPosition(window).x - brushRadius, sf::Mouse::getPosition(window).y - brushRadius);
        
        //Only draw when the mouse left button is down
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            window.draw(shape);
        }
        window.display();
    }

    return 0;
}