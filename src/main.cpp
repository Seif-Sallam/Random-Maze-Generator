#include <SFML/Graphics.hpp>
#include "Maze.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
    sf::View currentView;
    float speed = 150.f;

    currentView.setSize(sf::Vector2f(window.getSize()));
    sf::Vector2i origin = { 5,10 };
    sf::VertexArray mazeWalls;


    Maze maze;
    maze.InitMaze(30, 30);
    sf::Vector2i finalPoint = maze.Generate(origin);
    maze.BindToVertexArray(mazeWalls);

    
    sf::Vector2f offset = { 50.0f,50.f };
    sf::RectangleShape rect;
    sf::CircleShape startingCircle(8);
    sf::CircleShape endingCircle(8);

    rect.setFillColor(sf::Color::Transparent);
    rect.setSize(sf::Vector2f(32.0f, 32.0f));
    rect.setOutlineColor(sf::Color::Green);
    rect.setOutlineThickness(2.f);

    startingCircle.setFillColor(sf::Color::Green);
    startingCircle.setOrigin(4.f, 4.f);

    endingCircle.setFillColor(sf::Color::Red);
    endingCircle.setOrigin(4.f, 4.f);

    rect.setPosition(sf::Vector2f(origin) * 32.0f + offset);
    startingCircle.setPosition(sf::Vector2f(origin) * 32.0f + offset + sf::Vector2f(16.0f, 16.0f));
    endingCircle.setPosition(sf::Vector2f(finalPoint) * 32.0f + offset + sf::Vector2f(16.0f, 16.0f));


    currentView.setCenter(sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f) + offset / 2.0f);
   
    sf::Clock clock;

    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        window.setTitle("Maze Generator FPS: " + std::to_string(1.f / deltaTime));

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::Resized)
                currentView.setSize(sf::Vector2f(window.getSize()));
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S)
                speed += 50.f;
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::W)
                speed -= 50.f;
        }

        sf::Vector2f direction = {};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            direction = { -1 , 0 };
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            direction = { 1, 0 };
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            direction = { 0 , 1 };
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            direction = { 0 , -1 };

        currentView.move(direction * speed * deltaTime);

        window.setView(currentView);
        window.clear();
        
        window.draw(rect);
        window.draw(startingCircle);
        window.draw(endingCircle);

        window.draw(mazeWalls);
        window.display();
    }

    return 0;
}
