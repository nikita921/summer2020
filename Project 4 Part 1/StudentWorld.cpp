#include "StudentWorld.h"
#include "Actor.h"
#include <string>
using namespace std;

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(std::string assetDir) : GameWorld(assetDir), m_field{}
{
	m_player = nullptr;
}

StudentWorld::~StudentWorld()
{
    //cleanUp code
	//delete actors
	list<Actor*> ::iterator it = m_actors.begin();

	while (it != m_actors.end())
	{
		it = m_actors.erase(it);
	}

	//delete earth
	for (int i = 0; i < 60; i++)
	{
		for (int k = 0; k < 64; k++)
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
	list<Actor*>::iterator it = m_actors.begin();
	
	while (it != m_actors.end())
	{
		it = m_actors.erase(it);
	}

	//delete earth
	for (int i = 0; i < 60; i++)
	{
		for (int k = 0; k < 64; k++)
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

bool StudentWorld::removeEarth(int col, int row)
{
	for (int y = 0; y < 60; y++)
	{
		for (int x = 0; x < 64; x++)
		{
			if (m_field[y][x] == nullptr)
				continue;

			//if earth exists at coordinates then delete
			else if (m_field[y][x]->isActive() && m_field[y][x]->getX() == col && m_field[y][x]->getY() == row)
			{
				m_field[y][x]->setVisible(false);
				m_field[y][x]->setDead();
				return true;
			}
		}
	}

	return false;
}

int StudentWorld::init()
{
	//add earth
	for (int y = 0; y < 60; y++) 
	{
		for (int x = 0; x < 64; x++) 
		{
			//if bottom four rows then fill
			if (y < 4)
				m_field[y][x] = new Earth(x, y, this);

			//fill left and right of the tunnel
			else if (x < 30 || x > 33)
				m_field[y][x] = new Earth(x, y, this);
		}
	}

	//add player
	m_player = new TunnelMan(this);
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	m_player->doSomething();

	// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	

	if (isGameOver())
		return GWSTATUS_PLAYER_DIED;

	return GWSTATUS_CONTINUE_GAME;
}

bool StudentWorld::canMove(int x, int y)
{
	if (x < 0 || x > 60 || y < 0 || y > 60)
		return false;

	//check if there is a boulder

	return true;
}