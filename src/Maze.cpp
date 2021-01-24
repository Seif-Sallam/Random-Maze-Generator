#include "Maze.h"
Maze::Maze()
    : m_maze(nullptr), m_generated(false)
{
    m_dummyCell.position = sf::Vector2i(-1, -1);
}

void Maze::InitMaze(int w, int h)
{
    m_maze = new Cell[w * h];
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            m_maze[y * w + x].position = sf::Vector2i(x, y);
        }
    }
    size = sf::Vector2i(w, h);
    srand(time(0));
}

sf::Vector2i Maze::Generate(const sf::Vector2i& origin)
{
    if (!m_generated) {
        // The Stack of the "positions" of cells 
        std::stack<sf::Vector2i> cellsStack;

        auto FoundInVector = [&](std::vector<sf::Vector2i> cells, sf::Vector2i c) {
            for (int i = 0; i < cells.size(); i++)
                if (cells[i] == c)
                    return true;
            return false;
        };

        std::vector<sf::Vector2i> visitedCells;
        visitedCells.reserve(size.x * size.y);
        // Starting with the stack with the origin and marking it as visited
        cellsStack.push(origin);
        visitedCells.emplace_back(origin);

        while (!cellsStack.empty())
        {
            sf::Vector2i currentCell = cellsStack.top();

            // Finding the top, down, right, left elemnts positions in the a4rray
            sf::Vector2i top, down, right, left;
            top = sf::Vector2i(currentCell.x, currentCell.y - 1);
            down = sf::Vector2i(currentCell.x, currentCell.y + 1);
            right = sf::Vector2i(currentCell.x + 1, currentCell.y);
            left = sf::Vector2i(currentCell.x - 1, currentCell.y);

            //  if there is a possibility to find one unvisited place in the 4 directions
            std::vector<int> possibleWalls;
            // The indexes of all the cells
            int currentIndex = currentCell.y * size.y + currentCell.x;

            if (top.y > -1) {
                int topIndex = top.y * size.y + top.x;
                if (!FoundInVector(visitedCells, top))
                    possibleWalls.push_back(WallPos::TopWall);
            }
            if (down.y < size.y) {
                int downIndex = down.y * size.y + down.x;
                if (!FoundInVector(visitedCells, down))
                    possibleWalls.push_back(WallPos::DownWall);
            }
            if (right.x < size.x) {
                int rightIndex = right.y * size.y + right.x;
                if (!FoundInVector(visitedCells, right))
                    possibleWalls.push_back(WallPos::RightWall);
            }
            if (left.x > -1) {
                int leftIndex = left.y * size.y + left.x;
                if (!FoundInVector(visitedCells, left))
                    possibleWalls.push_back(WallPos::LeftWall);
            }
            // Mark the cell as visited if it was not already visited
            if (!FoundInVector(visitedCells, currentCell))
                visitedCells.emplace_back(currentCell);

            if (possibleWalls.size() == 0)  // We are in a dead end
            {
                cellsStack.pop();
                continue;
            }
            else // We have an option
            {
                int random = rand() % possibleWalls.size();
                int number = possibleWalls[random];
                // Since top is 0, down is 1, left is 2, and right is 3
                switch (number)
                {
                case 0:
                    cellsStack.push(top);
                    m_maze[currentCell.y * size.x + currentCell.x].walls[WallPos::TopWall] = false;
                    m_maze[top.y * size.x + top.x].walls[WallPos::DownWall] = false;
                    break;
                case 1:
                    cellsStack.push(down);
                    m_maze[currentCell.y * size.x + currentCell.x].walls[WallPos::DownWall] = false;
                    m_maze[down.y * size.x + down.x].walls[WallPos::TopWall] = false;
                    break;
                case 2:
                    cellsStack.push(left);
                    m_maze[currentCell.y * size.x + currentCell.x].walls[WallPos::LeftWall] = false;
                    m_maze[left.y * size.x + left.x].walls[WallPos::RightWall] = false;
                    break;
                case 3:
                    cellsStack.push(right);
                    m_maze[currentCell.y * size.x + currentCell.x].walls[WallPos::RightWall] = false;
                    m_maze[right.y * size.x + right.x].walls[WallPos::LeftWall] = false;
                    break;
                }
            }
            possibleWalls.clear();
        }
        int lastVisited = 0;
        auto pos = visitedCells[lastVisited];
        //last move was right
        if (visitedCells[lastVisited].x - visitedCells[lastVisited + 1].x == 1)
        {
            m_maze[pos.y * size.x + pos.x].walls[WallPos::RightWall] = false;
        }
        else if (visitedCells[lastVisited].x - visitedCells[lastVisited + 1].x == -1)
        {
            m_maze[pos.y * size.x + pos.x].walls[WallPos::LeftWall] = false;
        }
        else if (visitedCells[lastVisited].y - visitedCells[lastVisited + 1].y == 1)
        {
            m_maze[pos.y * size.x + pos.x].walls[WallPos::DownWall] = false;
        }
        else if (visitedCells[lastVisited].y - visitedCells[lastVisited + 1].y == -1)
        {
            m_maze[pos.y * size.x + pos.x].walls[WallPos::TopWall] = false;
        }
        sf::Vector2i finalPoint = visitedCells[visitedCells.size() - 1];
        visitedCells.clear();
        m_generated = true;
        return finalPoint;
    }

    return sf::Vector2i(-1, -1);

}

Cell Maze::GetCurrentCell(int index)
{
    if (index > -1 && index < (size.x * size.y))
        return m_maze[index];
    else
        return m_dummyCell;
}

bool Maze::IsGenerated()
{
    return m_generated;
}

void Maze::BindToVertexArray(sf::VertexArray& vertexArray)
{
    if (!this->IsGenerated()) {
        std::cout << "The maze is not generated\n";
        return;
    }
    sf::Vector2f sizeOfCell = { 32.f, 32.f };
    sf::Vector2f offset = { 50.0f, 50.0f };
    std::vector<std::pair<sf::Vector2f, sf::Vector2f>> drawnLines;
    drawnLines.reserve(400);
    vertexArray.clear();
    vertexArray.setPrimitiveType(sf::Lines);

    //struct Line {
    //    sf::Vector2f p1, p2;
    //};

    //std::vector<Line> lines;
    //// Joining the common lines
    //for (int x = 0; x < this->size.x; x++)
    //{
    //    for (int y = 0; y < this->size.y; y++)
    //    {
    //        int currentCellIndex = y * this->size.y + x;
    //        Cell currentCell = this->GetCurrentCell(currentCellIndex);

    //    }
    //}

    for (int x = 0; x < this->size.x; x++)
    {
        for (int y = 0; y < this->size.y; y++)
        {
            int currentCellIndex = y * this->size.x + x;
            Cell currentCell = this->GetCurrentCell(currentCellIndex);
            if (currentCell.position == sf::Vector2i(-1, -1))
            {
                std::cout << "Couldn't render the cell in iteration (x: " << x << " , y: " << y << " )\n";
                continue;
            }
            // The points that will be darwn in screen position
            sf::Vector2f points[4];
            // Top left
            points[0] = sf::Vector2f(currentCell.position.x * sizeOfCell.x, currentCell.position.y * sizeOfCell.y) + offset;
            // Top Right
            points[1] = sf::Vector2f((currentCell.position.x + 1) * sizeOfCell.x, currentCell.position.y * sizeOfCell.y) + offset;
            // Bottom Left
            points[2] = sf::Vector2f(currentCell.position.x * sizeOfCell.x, (currentCell.position.y + 1) * sizeOfCell.y) + offset;
            // Bottom Right
            points[3] = sf::Vector2f((currentCell.position.x + 1) * sizeOfCell.x, (currentCell.position.y + 1) * sizeOfCell.y) + offset;


            auto FindPair = [&](std::vector<std::pair<sf::Vector2f, sf::Vector2f>> v, std::pair<sf::Vector2f, sf::Vector2f> p)
            {
                for (int i = 0; i < v.size(); i++)
                {
                    if (v[i].first == p.first && v[i].second == p.second)
                    {
                        return true;
                    }
                }
                return false;
            };

            // Checking the walls
            if (currentCell.walls[WallPos::TopWall])
            {
                auto topWall = std::make_pair(points[0], points[1]);
                if (!FindPair(drawnLines, topWall)) // It was not FOUND 
                {
                    vertexArray.append(topWall.first);
                    vertexArray[vertexArray.getVertexCount() - 1].color = sf::Color::Blue;
                    vertexArray.append(topWall.second);
                    vertexArray[vertexArray.getVertexCount() - 1].color = sf::Color::Blue;
                    drawnLines.emplace_back(topWall);
                }
            }
            if (currentCell.walls[WallPos::DownWall])
            {
                auto downWall = std::make_pair(points[2], points[3]);
                if (!FindPair(drawnLines, downWall)) // It was not FOUND 
                {
                    vertexArray.append(downWall.first);
                    vertexArray[vertexArray.getVertexCount() - 1].color = sf::Color::Blue;
                    vertexArray.append(downWall.second);
                    vertexArray[vertexArray.getVertexCount() - 1].color = sf::Color::Blue;
                    drawnLines.emplace_back(downWall);
                }
            }
            if (currentCell.walls[WallPos::LeftWall])
            {
                auto leftWall = std::make_pair(points[0], points[2]);
                if (!FindPair(drawnLines, leftWall)) // It was not FOUND 
                {
                    vertexArray.append(leftWall.first);
                    vertexArray[vertexArray.getVertexCount() - 1].color = sf::Color::Blue;
                    vertexArray.append(leftWall.second);
                    vertexArray[vertexArray.getVertexCount() - 1].color = sf::Color::Blue;
                    drawnLines.emplace_back(leftWall);
                }
            }
            if (currentCell.walls[WallPos::RightWall])
            {
                auto rightWall = std::make_pair(points[1], points[3]);
                if (!FindPair(drawnLines, rightWall)) // It was not FOUND 
                {
                    vertexArray.append(rightWall.first);
                    vertexArray[vertexArray.getVertexCount() - 1].color = sf::Color::Blue;
                    vertexArray.append(rightWall.second);
                    vertexArray[vertexArray.getVertexCount() - 1].color = sf::Color::Blue;
                    drawnLines.emplace_back(rightWall);
                }
            }
        }
    }
    drawnLines.clear();
}

Maze::~Maze()
{
    delete[] m_maze;
    m_generated = false;
}
