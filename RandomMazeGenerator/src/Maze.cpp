#include "Maze.h"
#include "Maze.h"

Cell::Cell()
{
	cell.setSize(sf::Vector2f(30.f, 30.f));
	cell.setPosition(0.f, 0.f);
	cell.setFillColor(sf::Color::Blue);
	cell.setOrigin(sf::Vector2f(15.0f, 15.0f));
	for (int i = 0; i < 4; i++)
		walls[i].setFillColor(sf::Color::Black);
	
	// Upper wall
	walls[WallPos::TopWall].setSize(sf::Vector2f(32.0f, 1.0f));
	walls[WallPos::TopWall].setOrigin(sf::Vector2f(15.0f, 1.0f));
	walls[WallPos::TopWall].setPosition(sf::Vector2f(0.0f, -15.0f));

	// Down Wall
	walls[WallPos::DownWall].setSize(sf::Vector2f(32.0f, 1.0f));
	walls[WallPos::DownWall].setOrigin(sf::Vector2f(15.0f, 0.0f));
	walls[WallPos::DownWall].setPosition(sf::Vector2f(0.0f, +15.0f));

	// Left Wall
	walls[WallPos::LeftWall].setSize(sf::Vector2f(1.0f, 32.0f));
	walls[WallPos::LeftWall].setOrigin(sf::Vector2f(1.0f, 15.0f));
	walls[WallPos::LeftWall].setPosition(sf::Vector2f(-15.0f, 0.0f));

	// Right Wall
	walls[WallPos::RightWall].setSize(sf::Vector2f(1.0f, 32.0f));
	walls[WallPos::RightWall].setOrigin(sf::Vector2f(0.0f, 15.0f));
	walls[WallPos::RightWall].setPosition(sf::Vector2f(+15.0f, 0.0f));
	
	visited = false;
	
}

void Cell::SetPosition(const sf::Vector2f& position)
{
	cell.setPosition(position);

	walls[0].setPosition(sf::Vector2f(position.x + 0.0f, position.y + -15.0f));

	walls[1].setPosition(sf::Vector2f(position.x + 0.0f, position.y + +15.0f));

	walls[2].setPosition(sf::Vector2f(position.x + -15.0f, position.y + 0.0f));

	walls[3].setPosition(sf::Vector2f(position.x + +15.0f, position.y + 0.0f));
}

void Cell::SetPosition(const sf::Vector2i& pos)
{
	sf::Vector2f position;
	position.x = pos.x * 32.0f;
	position.y = pos.y * 32.0f;
	cell.setPosition(position);

	walls[WallPos::TopWall].setPosition(sf::Vector2f(position.x + 0.0f, position.y + -15.0f));

	walls[WallPos::DownWall].setPosition(sf::Vector2f(position.x + 0.0f, position.y + +15.0f));

	walls[WallPos::LeftWall].setPosition(sf::Vector2f(position.x + -15.0f, position.y + 0.0f));

	walls[WallPos::RightWall].setPosition(sf::Vector2f(position.x + +15.0f, position.y + 0.0f));
}

void Cell::UpdateCellWall(int index)
{
	walls[index].setFillColor(sf::Color::White);
}

void Cell::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	target.draw(cell);
	for (int i = 0; i < 4; i++)
	{
		target.draw(walls[i]);
	}
}

//------------------------------------------------------------------//

Maze::Maze(int width, int height)
{
	m_iWidth = width;
	m_iHeight = height;
	m_maze = new Cell[m_iWidth * m_iHeight]{ };
	m_bFinished = false;

	for (int y = 0; y < m_iHeight; y++)
	{
		for (int x = 0; x < m_iWidth; x++)
		{
			int index = y * m_iWidth + x;
			m_maze[index].SetPosition(sf::Vector2i(x, y));
		}
	}
	srand(time(0));
	m_cellsStack.push(sf::Vector2i(0, 0));
}

bool Maze::GenerateStep()
{
	if (!m_bFinished)
	{
		sf::Vector2i currentCell = m_cellsStack.top();
		sf::Vector2i top, down, right, left;
		top = sf::Vector2i(currentCell.x, currentCell.y - 1);
		down = sf::Vector2i(currentCell.x, currentCell.y + 1);
		right = sf::Vector2i(currentCell.x + 1, currentCell.y);
		left = sf::Vector2i(currentCell.x - 1, currentCell.y);
		//  if there is a possibility to find one unvisited place in the 4 directions
		std::vector<int> possibleWalls;
		// The indexes of all the cells
		int currentIndex = currentCell.y * m_iWidth + currentCell.x;
		if (top.y > -1)
		{
			int topIndex = currentIndex - m_iWidth;
			if (!m_maze[topIndex].visited)
				possibleWalls.push_back(WallPos::TopWall);
		}
		if (down.y < m_iHeight)
		{
			int downIndex = currentIndex + m_iWidth;
			if (!m_maze[downIndex].visited)
				possibleWalls.push_back(WallPos::DownWall);
		}
		if (left.x > -1)
		{
			int leftIndex = currentIndex - 1;
			if (!m_maze[leftIndex].visited)
				possibleWalls.push_back(WallPos::LeftWall);
		}
		if (right.x < m_iWidth)
		{
			int rightIndex = currentIndex + 1;
			if (!m_maze[rightIndex].visited)
				possibleWalls.push_back(WallPos::RightWall);
		}
		if (!m_maze[currentIndex].visited) {
			m_maze[currentIndex].visited = true;
			m_maze[currentIndex].cell.setFillColor(sf::Color::White);
		}

		if (possibleWalls.size() == 0)
		{
			m_cellsStack.pop();
		}
		else
		{
			m_maze[currentIndex].cell.setFillColor(sf::Color::White);
			int random = rand() % possibleWalls.size();
			int number = possibleWalls[random];
			switch (number) {
			case WallPos::TopWall:
				m_cellsStack.push(top);
				m_maze[currentIndex].UpdateCellWall(WallPos::TopWall);
				m_maze[currentIndex - m_iWidth].UpdateCellWall(WallPos::DownWall);
				m_maze[currentIndex - m_iWidth].cell.setFillColor(sf::Color::Green);
				break;
			case WallPos::DownWall:
				m_cellsStack.push(down);
				m_maze[currentIndex].UpdateCellWall(WallPos::DownWall);
				m_maze[currentIndex + m_iWidth].UpdateCellWall(WallPos::TopWall);
				m_maze[currentIndex + m_iWidth].cell.setFillColor(sf::Color::Green);
				break;
			case WallPos::LeftWall:
				m_cellsStack.push(left);
				m_maze[currentIndex].UpdateCellWall(WallPos::LeftWall);
				m_maze[currentIndex - 1].UpdateCellWall(WallPos::RightWall);
				m_maze[currentIndex - 1].cell.setFillColor(sf::Color::Green);
				break;
			case WallPos::RightWall:
				m_cellsStack.push(right);
				m_maze[currentIndex].UpdateCellWall(WallPos::RightWall);
				m_maze[currentIndex + 1].UpdateCellWall(WallPos::LeftWall);
				m_maze[currentIndex + 1].cell.setFillColor(sf::Color::Green);
				break;
			}
		}
		possibleWalls.clear();
		
		if (m_cellsStack.empty())
			m_bFinished = true;

	}
	return m_bFinished;
}

void Maze::Draw(sf::RenderWindow* window, const sf::Vector2f& worldStart, const sf::Vector2f& worldEnd)
{
	for (int y = 0; y < m_iHeight; y++)
	{
		for (int x = 0; x < m_iWidth; x++)
		{
			int index = y * m_iWidth + x;
			window->draw(m_maze[index]);

		}
	}

}

Maze::~Maze()
{
}
