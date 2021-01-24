#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
#include <string>
#include <stack>
#include <vector>

enum WallPos
{
    TopWall = 0,
    DownWall,
    LeftWall,
    RightWall
};

struct Cell
{
    sf::Vector2i position;
    // false is no wall, true is wall
    bool walls[4] = { true, true, true, true };
};

class Maze
{
public:
    Maze();

    void InitMaze(int w, int h);


    sf::Vector2i Generate(const sf::Vector2i& origin);

    Cell GetCurrentCell(int index);

    bool IsGenerated();

    void BindToVertexArray(sf::VertexArray& vertexArray);

    ~Maze();

public:
    sf::Vector2i size;
private:
    Cell* m_maze;
    Cell m_dummyCell;
    bool m_generated;
};

