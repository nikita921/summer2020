#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <queue>
#include <algorithm>	// for min and max functions 
#include <iostream>		// defines the overloads of the << // operator
#include <sstream>		// defines the type std::ostringstream
#include <iomanip>		// defines the manipulator setw
#include <cmath>		// for math operations in distance formula
using namespace std;


// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::Coordinates::Coordinates(int x, int y) : x(x), y(y) { }

StudentWorld::StudentWorld(std::string assetDir) : GameWorld(assetDir)
{
	m_player = nullptr;
	Earth* m_field = {};
	m_nProtestors = 0;
	m_nBarrelsLeft = 0;
}

StudentWorld::~StudentWorld()
{
	//delete actors
	while (!m_actors.empty())
	{
		delete m_actors.back();
		m_actors.pop_back();
	}	

	//delete earth
	for (int i = 0; i < MAX_ROWS; i++)
	{
		for (int k = 0; k < MAX_COLS; k++)
		{
			if (m_field[i][k] != nullptr)
			{
				delete m_field[i][k];
				m_field[i][k] = nullptr;
			}
		}
	}

	//delete tunnelman
	delete m_player;
}

void StudentWorld::cleanUp()
{
	//delete actors
	while (!m_actors.empty())
	{
		delete m_actors.back();
		m_actors.pop_back();
	}

	//delete earth
	for (int i = 0; i < MAX_ROWS; i++)
	{
		for (int k = 0; k < MAX_COLS; k++)
		{
			if (m_field[i][k] != nullptr)
			{
				delete m_field[i][k];
				m_field[i][k] = nullptr;
			}
		}
	}

	//delete tunnelman
	delete m_player;

}

int StudentWorld::init()
{
	//calculate number of barrels, boulder, and gold for each level
	m_ticks = 0;
	m_nBarrelsLeft = min(2 + (int)getLevel(), 21);
	int nBoulders = min((int)getLevel() / 2 + 2, 9);
	int nGold = max(5 - (int)getLevel() / 2, 2);

	int counter = 0;

	//add earth
	for (int y = 0; y < MAX_ROWS; y++)
	{
		for (int x = 0; x < MAX_COLS; x++)
		{
			//if bottom four rows then fill
			if (y < 4)
				m_field[y][x] = new Earth(x, y, this);

			//fill left and right of the tunnel
			else if (x < 30 || x > 33)
				m_field[y][x] = new Earth(x, y, this);
		}
	}

	Coordinates c(0, 0);

	//add boulders
	while (nBoulders > 0)
	{
		c = getRandomLocation(0, 60, 20, 56);
		m_actors.push_back(new Boulders(c.x, c.y, this));
		for (int k = c.y; k < c.y + 4 && k < 60; k++)
		{
			for (int i = c.x; i < c.x + 4 && i < 64; i++)
			{
				removeEarth(i, k);
			}
		}
		nBoulders--;
	}

	//add gold nuggets
	while (nGold > 0)
	{
		c = getRandomLocation(0, 60, 0, 56);
		m_actors.push_back(new Nugget(c.x, c.y, this, false));
		nGold--;
	}

	//add barrels of oil
	while (counter < m_nBarrelsLeft)
	{
		c = getRandomLocation(0, 60, 0, 56);
		m_actors.push_back(new Barrel(c.x, c.y, this));
		counter++;
	}

	//add player
	m_player = new TunnelMan(this);
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	updateStatusText();

	//add a protestor? how many?
	int t = max(25, 200 - (int)getLevel());
	int p = min(15, 2 + ((int)getLevel() * 3)/2);
	if (m_ticks % t == 0 && m_nProtestors < p)
	{
		m_actors.push_back(new Regular(this));

		//increment number of protesters
		m_nProtestors++;
	}

	m_ticks++;
	//add waterpool or sonar?
	int G = getLevel() * 25 + 300;
	
	// 1 in G chance of adding
	if (rand() % G == 1)
	{
		//1 in 5 chance of adding sonar
		if(rand() % 5 == 1)
			m_actors.push_back(new Sonar(this));

		//4 in 5 chance of adding waterpool
		else
		{
			//generate random position where earth doesn't exist
			int x = rand() % 60;
			int y = rand() % 56;
			while (checkForEarth(x, y))
			{
				x = rand() % 60;
				y = rand() % 56;
			}
			m_actors.push_back(new WaterPool(x, y, this));
		}
			
	}

	//call each actor's doSomething function
	m_player->doSomething();
	for (int i = 0; i < m_actors.size(); i++)
	{
		m_actors[i]->doSomething();

		if (!m_actors[i]->isActive())
			m_actors[i]->setVisible(false);

		if (!m_player->isActive())
		{
			decLives();
			return GWSTATUS_PLAYER_DIED;
		}
	}

	//if player is dead, decrement lives
	if (!m_player->isActive())
	{
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}
		
	//if picked up all oil, end level
	if (m_nBarrelsLeft <= 0)
	{
		playSound(SOUND_FINISHED_LEVEL);
		return GWSTATUS_FINISHED_LEVEL;
	}
		
	return GWSTATUS_CONTINUE_GAME;
}

bool StudentWorld::canMove(int x, int y) const
{
	//check if out of bounds
	if (x < 0 || x > 60 || y < 0 || y > 60)
		return false;

	//check if there is a boulder
	for (int i = 0; i < m_actors.size(); i++)
	{
		if (distanceFormula(x, y, m_actors[i]->getX(), m_actors[i]->getY()) < 4 
			&& m_actors[i]->blocksPath() && m_actors[i]->isActive())
			return false;
	}

	return true;
}

bool StudentWorld::checkForEarth(int x, int y) const
{
	for (int k = y; k < y + 4 && k < MAX_ROWS; k++)
	{
		for (int i = x; i < x + 4 && i < MAX_COLS; i++)
		{
			//if earth exists and is active, return true
			if (m_field[k][i] != nullptr && m_field[k][i]->isActive())
				return true;
		}
	}

	return false;
}

bool StudentWorld::isTunnelManNearby(int x, int y, int rad)
{
	//check if distance formula is less than given radius
	return (distanceFormula(m_player->getX(), m_player->getY(), x, y) <= rad);
}

bool StudentWorld::isProtestorNearby(int x, int y, int rad)
{
	for (int i = 0; i < m_actors.size(); i++)
	{
		//plug protester coordinates into distance formula
		if (m_actors[i]->isProtester() && m_actors[i]->isActive() &&
			distanceFormula(m_actors[i]->getX(), m_actors[i]->getY(), x, y) <= rad)
			return true;
	}
	return false;
}

bool StudentWorld::removeEarth(int col, int row)
{
	if (m_field[row][col] != nullptr && m_field[row][col]->isActive())
	{
		m_field[row][col]->setVisible(false);
		m_field[row][col]->setActive(false);
		return true;
	}

	return false;
}

double StudentWorld::distanceFormula(int x1, int y1, int x2, int y2) const
{
	//implement distance formula
	int x = x2 - x1;
	int y = y2 - y1;
	double distance = pow(x, 2) + pow(y, 2);
	distance = sqrt(distance);
	return distance;
}

void StudentWorld::addSquirt(int x, int y)
{
	//check boulders and earth before adding
	if (canMove(x, y) && !checkForEarth(x, y))
		m_actors.push_back(new Squirt(x, y, m_player->getDirection(), this));
}

void StudentWorld::dropGold(int x, int y)
{
	//add gold nugget
	m_actors.push_back(new Nugget(x, y, this, true));
}

void StudentWorld::killProtester(int x, int y)
{
	//find protester and set dead
	for (int i = 0; i < m_actors.size(); i++)
	{
		if (m_actors[i]->isProtester() && m_actors[i]->isActive())
			m_actors[i]->setActive(false);	
	}
}

bool StudentWorld::canAnnoyTunnelMan(Actor* p)
{
	int x = m_player->getX();
	int y = m_player->getY();
	int a = p->getX();
	int b = p->getY();
	p->newLocation(a, b, 1, p->getDirection());

	//if direction is to the right 
	if (a-p->getX() == 1 && p->getX() < x)
		return true;

	//if direction is to the left
	if (p->getX() - a == 1 && p->getX() > x)
		return true;

	//if direction is down
	if (p->getY() - b == 1 && p->getY() > y)
		return true;

	//if direction is up
	if (b - p->getY() == 1 && p->getY() < y)
		return true;

	return false;
}

void StudentWorld::gotGold()
{
	//increase number of nuggets
	m_player->setGold(1);
}

void StudentWorld::gotOil()
{
	//decrease number of barrels lefy
	m_nBarrelsLeft--;
}

void StudentWorld::gotWater()
{
	//increase water stash
	m_player->setWater(5);
}

void StudentWorld::gotSonar()
{
	//increase sonar stash
	m_player->setSonar(1);
}

void StudentWorld::useSonar(int x, int y)
{
	//find everything in vicinity and set visible
	for (int i = 0; i < m_actors.size(); i++)
	{
		if (m_actors[i]->isActive() && (distanceFormula(x, y, m_actors[i]->getX(), m_actors[i]->getY()) < 12))
			m_actors[i]->setVisible(true);
	}
}

void StudentWorld::annoyTunnelMan(int amount)
{
	//decrement players health
	m_player->annoy(amount);
}

StudentWorld::Coordinates StudentWorld::getRandomLocation(int xLowerBound, int xUpperBound, int yLowerBound, int yUpperBound)
{
	//find range
	int xRange = xUpperBound - xLowerBound + 1;
	int yRange = yUpperBound - yLowerBound + 1;

	//find random coordinates
	int x = rand() % xRange + xLowerBound;

	while(x > 26 && x < 34)
		x = rand() % xRange + xLowerBound;

	int y = rand() % yRange + yLowerBound;

	for (int i = 0; i < m_actors.size(); i++)
	{
		//make sure there are no objects within 6 units of each other
		while ((distanceFormula(x, y, m_actors[i]->getX(), m_actors[i]->getY()) < 6))
		{
			while (x > 26 && x < 34)
				x = rand() % xRange + xLowerBound;
			y = rand() % yRange + yLowerBound;
		}
	}

	return StudentWorld::Coordinates(x,y);
}

void StudentWorld::updateStatusText()
{
	ostringstream oss;
	oss << "Lvl: ";
	oss.fill(' ');
	oss << setw(2) << getLevel() << " ";
	oss << " Lives: ";
	oss << setw(1) << getLives() << " ";
	oss << " Hlth: ";
	oss << setw(3) << (m_player->getPoints())*10 << "% ";
	oss << " Wtr: ";
	oss << setw(2) << m_player->getWater();
	oss << " Gld: ";
	oss << setw(2) << m_player->getGold();
	oss << " Oil Left: ";
	oss << setw(2) << m_nBarrelsLeft;
	oss << " Sonar: ";
	oss << setw(2) << m_player->getSonar();
	oss << " Scr: ";
	oss.fill('0');
	oss << setw(6) << getScore();
	string s = oss.str();
	setGameStatText(s);
}




