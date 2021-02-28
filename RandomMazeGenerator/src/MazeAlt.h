//#pragma once
//#include "SFML/Graphics.hpp"
//#include <iostream>
//#include <string>
//#include <stack>
//#include <vector>
//
//enum WallPos
//{
//    TopWall = 0,
//    DownWall,
//    LeftWall,
//    RightWall
//};
//
//struct CellAlt
//{
//    sf::Vector2i position;
//    // false is no wall, true is wall
//    bool walls[4] = { true, true, true, true };
//};
//
//class MazeAlt
//{
//public:
//    MazeAlt();
//
//    void InitMaze(int w, int h);
//
//
//    sf::Vector2i GenerateOnSpot(const sf::Vector2i& origin);
//
//    CellAlt GetCurrentCell(int index);
//
//    bool IsGenerated();
//
//    void BindToVertexArray(sf::VertexArray& vertexArray);
//
//    ~MazeAlt();
//
//public:
//    sf::Vector2i size;
//private:
//    CellAlt* m_maze;
//    CellAlt m_dummyCell;
//    bool m_generated;
//};