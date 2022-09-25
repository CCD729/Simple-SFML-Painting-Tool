#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include <iostream>
#include <vector>

//SFML Color to HSV Tool Function by Tenry
//https://en.sfml-dev.org/forums/index.php?topic=7313.0
sf::Color hsv(int hue, float sat, float val);

//My little tool class to handle HSV color
class HSVColor {
public:
    int hue;        
    float sat;
    float val;
    HSVColor(int h, float s, float v) {
        hue = h;
        sat = s;
        val = v;
    }
};
//Brush default properties
const float BrushSizeDefault = 15.f;
const sf::Color BrushColorDefault = sf::Color(170, 196, 255);
const HSVColor HSVColorDefault = HSVColor(222, .33f, 1.f);

int main()
{
    //Variables for brush
    float brushSize = BrushSizeDefault;
    sf::Color brushColor = BrushColorDefault;
    HSVColor brushHSVColor = HSVColorDefault;

    //Create the window
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(800, 600), "My Little Drawing Tool", sf::Style::Default, settings);
    window.setFramerateLimit(60);
    //Hidethe cursor
    window.setMouseCursorVisible(false);

    // Create brush
    enum BrushType {
        CIRCLE,
        SQUARE
    };
    sf::CircleShape circle(brushSize);
    sf::RectangleShape square(sf::Vector2f(brushSize * 2.f, brushSize * 2.f));
    circle.setFillColor(brushColor);
    square.setFillColor(brushColor);
    sf::Shape* currentBrush = &circle;
    BrushType brushType = CIRCLE;
    

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
        sf::Event event;
        while (window.pollEvent(event))
        {
            //Window close handler
            if (event.type == sf::Event::Closed)
                window.close();

            //Brush shape, size & color change handler
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left) {
                    brushHSVColor.hue = (brushHSVColor.hue > 0) ? (brushHSVColor.hue - 3) : 360;
                }
                else if (event.key.code == sf::Keyboard::Right) {
                    brushHSVColor.hue = (brushHSVColor.hue <360) ? (brushHSVColor.hue + 3) : 0;
                }
                else if (event.key.code == sf::Keyboard::Comma) {
                    brushHSVColor.sat = (brushHSVColor.sat > 0.f) ? (brushHSVColor.sat - 0.03f) : 0.f;
                }
                else if (event.key.code == sf::Keyboard::Period) {
                    brushHSVColor.sat = (brushHSVColor.sat < 1.f) ? (brushHSVColor.sat + 0.03f) : 1.f;
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    brushHSVColor.val = (brushHSVColor.val > 0.f) ? (brushHSVColor.val - 0.03f) : 0.f;
                }
                else if (event.key.code == sf::Keyboard::Up) {
                    brushHSVColor.val = (brushHSVColor.val < 1.f) ? (brushHSVColor.val + 0.03f) : 1.f;
                }
                else if (event.key.code == sf::Keyboard::Hyphen) {
                    brushSize = (brushSize > 7.f) ? (brushSize - 4.f) : 3.f;
                    square.setSize(sf::Vector2f(brushSize * 2.f, brushSize * 2.f));
                    circle.setRadius(brushSize);
                    (*currentBrush).setPosition(sf::Mouse::getPosition(window).x - brushSize, sf::Mouse::getPosition(window).y - brushSize);
                }
                else if (event.key.code == sf::Keyboard::Equal) {
                    brushSize = (brushSize < 47.f) ? (brushSize + 4.f) : 50.f;
                    square.setSize(sf::Vector2f(brushSize * 2.f, brushSize * 2.f));
                    circle.setRadius(brushSize);
                    (*currentBrush).setPosition(sf::Mouse::getPosition(window).x - brushSize, sf::Mouse::getPosition(window).y - brushSize);
                }
                else if (event.key.code == sf::Keyboard::Space) {
                    if (brushType == CIRCLE) {
                        square.setFillColor(brushColor);
                        square.setSize(sf::Vector2f(brushSize*2.f, brushSize*2.f));
                        currentBrush = &square;
                        brushType = SQUARE;
                    }
                    else if (brushType == SQUARE) {
                        circle.setFillColor(brushColor);
                        circle.setRadius(brushSize);
                        currentBrush = &circle;
                        brushType = CIRCLE;
                    }
                }
                brushColor = hsv(brushHSVColor.hue, brushHSVColor.sat, brushHSVColor.val);
            }

            //Save current draw trail
            else if (event.type == sf::Event::MouseMoved && mouseLeftDown) {
                if (mouseLeftDown) {
                    (*currentBrush).setFillColor(brushColor);
                }
                (*currentBrush).setPosition(sf::Mouse::getPosition(window).x - brushSize, sf::Mouse::getPosition(window).y - brushSize);
                if (brushType == CIRCLE) {
                    currentDraw.push_back(new sf::CircleShape(*((sf::CircleShape*)currentBrush)));
                }
                else if (brushType == SQUARE) {
                    currentDraw.push_back(new sf::RectangleShape(*((sf::RectangleShape*)currentBrush)));
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.key.code == sf::Mouse::Left) {
                    mouseLeftDown = true;
                    (*currentBrush).setPosition(sf::Mouse::getPosition(window).x - brushSize, sf::Mouse::getPosition(window).y - brushSize);
                    if (brushType == CIRCLE) {
                        currentDraw.push_back(new sf::CircleShape(*((sf::CircleShape*)currentBrush)));
                    }
                    else if (brushType == SQUARE) {
                        currentDraw.push_back(new sf::RectangleShape(*((sf::RectangleShape*)currentBrush)));
                    }
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
        //Display brush as cursor
        (*currentBrush).setFillColor(brushColor);
        (*currentBrush).setPosition(sf::Mouse::getPosition(window).x - brushSize, sf::Mouse::getPosition(window).y - brushSize);
        window.draw(*currentBrush);
        window.display();
    }
    return 0;
}

//Implementation of HSV Tool Function by Tenry
sf::Color hsv(int hue, float sat, float val)
{
    hue %= 360;
    while (hue < 0) hue += 360;

    if (sat < 0.f) sat = 0.f;
    if (sat > 1.f) sat = 1.f;

    if (val < 0.f) val = 0.f;
    if (val > 1.f) val = 1.f;

    int h = hue / 60;
    float f = float(hue) / 60 - h;
    float p = val * (1.f - sat);
    float q = val * (1.f - sat * f);
    float t = val * (1.f - sat * (1 - f));

    switch (h)
    {
    default:
    case 0:
    case 6: return sf::Color(val * 255, t * 255, p * 255);
    case 1: return sf::Color(q * 255, val * 255, p * 255);
    case 2: return sf::Color(p * 255, val * 255, t * 255);
    case 3: return sf::Color(p * 255, q * 255, val * 255);
    case 4: return sf::Color(t * 255, p * 255, val * 255);
    case 5: return sf::Color(val * 255, p * 255, q * 255);
    }
}