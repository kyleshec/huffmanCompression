#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>

#include "res/scripts/managers/AssetManager.h"
#include "res/scripts/managers/UIElementManager.h"
#include "res/scripts/ui/Button.h"
#include "res/scripts/ui/super/TextBased.h"
#include "res/scripts/ui/ProgressBar.h"

using namespace std;
using sf::Event;
using sf::Mouse;
using std::rand;

int main() {

    sf::RenderWindow window(sf::VideoMode(1200, 600), "Media Compression by iCompression ");
    window.setFramerateLimit(60); //Set the fps limit to 60 fps.
    AssetManager textureManager("../"); //Make a texture manager with the right prefix for CLion.
    UIElementManager uiElementManager(&window);

    AssetManager::loadAll();

    ProgressBar* progress = new ProgressBar("Progress Bar Test", Vector2f(200, 20), Vector2f(300,200));

    Button button("Dr. Disco", AssetManager::getTexture("test.png"), Vector2f(0, 0), [&button, &progress]()->void{
        button.getSprite().setColor(sf::Color(rand()%256, rand()%256, rand()%256));
        cout << "Progress bar ( " << &(*progress) << ") percentage went from " << progress->getPercentage();
        progress->setPercentage(progress->getPercentage() + .05);
        cout << " to " << progress->getPercentage() << endl;
    });

    Button button2("Press button test", AssetManager::getTexture("test.png"), Vector2f(256, 256), [&button2]()->void{
        button2.getSprite().setTexture(AssetManager::getTexture("testPressed.png"));
    });

    Button button3("Missing texture test", AssetManager::getTexture("test2.png"), Vector2f(128, 0), [&button3]()->void{
        button3.getSprite().setTexture(AssetManager::getTexture("tested.png"));
    });

    TextBased testText("testing text", Vector2f(40, 150), AssetManager::getFont(DEFAULT_FONT), "Hello World!");

    UIElementManager::addElement(&button);
    UIElementManager::addElement(&button2);
    UIElementManager::addElement(&button3);
    UIElementManager::addElement(&testText);

    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            switch(event.type){
                case(Event::Closed):{
                    window.close();
                    break;
                }

                case(Event::MouseButtonPressed):{
                    if (event.mouseButton.button == Mouse::Left){
                        UIElementManager::activateElement(Mouse::getPosition(window));
                    }
                    break;
                }
            }
        }

        window.clear(sf::Color::White);
        UIElementManager::drawAll();

        for (RectangleShape* i : progress->getShapes()){
            window.draw(*i);
        }

        window.display();
    }

    return 0;
}
