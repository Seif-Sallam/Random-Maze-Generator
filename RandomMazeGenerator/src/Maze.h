#pragma once
#include "SFML/Graphics.hpp"
#include <stack>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

enum WallPos
{
	TopWall = 0,
	DownWall,
	LeftWall,
	RightWall
};

struct Cell : public sf::Drawable
{
	struct CellConfig
	{
		bool walls[4]{ false };
	};
	sf::RectangleShape cell;
	sf::RectangleShape walls[4];
	bool visited;
	CellConfig config;
	Cell();
	void SetPosition(const sf::Vector2f& position);	
	void SetPosition(const sf::Vector2i& position);
	void UpdateCellWall(int wall);
	void draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default) const override;
};

class Maze
{
public:
	Maze(int width, int height);

	bool GenerateStep();
	
	void Draw(sf::RenderWindow* window, const sf::Vector2f& worldStart, const sf::Vector2f& worldEnd);
	~Maze();
private:
	bool m_bFinished;
	Cell* m_maze;
	int m_iWidth;
	int m_iHeight;
	std::stack<sf::Vector2i> m_cellsStack;
	sf::Vector2i stackTop;
};

