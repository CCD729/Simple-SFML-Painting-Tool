#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include <iostream>
#include <vector>

//Brush default properties
const float BrushRadiusDefault = 15.f;
const sf::Color BrushColorDefault = sf::Color(170, 196, 255);
int main()
{
    //Variables for brush
    float brushRadius = BrushRadiusDefault;
    sf::Color brushColor = BrushColorDefault;

    //Create the window
    //sf::ContextSettings settings;
    //settings.antialiasingLevel = 8;
    //sf::RenderWindow window(sf::VideoMode(800, 600), "My Little Drawing Tool", sf::Style::Default, settings);
    sf::RenderWindow window(sf::VideoMode(800, 600), "My Little Drawing Tool");
    window.setFramerateLimit(60);

    // Create brush
    sf::CircleShape shape(brushRadius);
    shape.setFillColor(brushColor);
    //Create stack for drawing steps
    std::vector<std::vector<sf::Shape*>> shapeStack;
    std::vector<sf::Shape*> currentDraw;

    //Boolean variables
    bool saveRelease = true;
    bool undoRelease = true;
    bool mouseLeftDown = false;

    // Game loop
    while (window.isOpen())
    {
        //Window close handler
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            //Save current draw trail
            else if (event.type == sf::Event::MouseMoved && mouseLeftDown){
                if (mouseLeftDown) {
                    shape.setFillColor(brushColor);
                }
                shape.setPosition(sf::Mouse::getPosition(window).x - brushRadius, sf::Mouse::getPosition(window).y - brushRadius);
                currentDraw.push_back(new sf::CircleShape(shape));
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.key.code == sf::Mouse::Left) {
                    mouseLeftDown = true;
                    shape.setPosition(sf::Mouse::getPosition(window).x - brushRadius, sf::Mouse::getPosition(window).y - brushRadius);
                    currentDraw.push_back(new sf::CircleShape(shape));
                }
            }
            else if (event.type == sf::Event::MouseButtonReleased) {
                if (event.key.code == sf::Mouse::Left) {
                    mouseLeftDown = false;
                }
                shapeStack.push_back(currentDraw);
                currentDraw.clear();
            }
            //save & undo boolean reset
            else if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::S)
                {
                    saveRelease = true;
                }
                else if (event.key.code == sf::Keyboard::Z) {
                    undoRelease = true;
                }
            }
        }
        // Ctrl+S saves a screenshot
        if (saveRelease && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            saveRelease = false;
            sf::Texture texture;
            texture.create(window.getSize().x, window.getSize().y);
            texture.update(window);
            if (texture.copyToImage().saveToFile("Mywork.png")) {
                std::cout << "Screenshot saved to Mywork.png" << std::endl;
            }
        }
        // Ctrl+Z undos a previous step
        if (undoRelease && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
            undoRelease = false;
            if (!shapeStack.empty()) {
                shapeStack.pop_back();
            }
        }
        window.clear(sf::Color::Transparent);
        // Draw completed shapes
        for (unsigned int i = 0; i < shapeStack.size(); i++) {
            for(unsigned int j = 0; j<shapeStack[i].size(); j++){
                window.draw(*(shapeStack[i][j]));
            }
        }
        //temp draw currentDraw
        for (unsigned int i = 0; i < currentDraw.size(); i++) {
            window.draw(*currentDraw[i]);
        }
        window.display();
    }
    return 0;
}