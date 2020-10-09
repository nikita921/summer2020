#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class TunnelMan;
class Earth;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);
	virtual ~StudentWorld();
	virtual int init();
	virtual int move();
	virtual void cleanUp();

	bool removeEarth(int x, int y);
	bool canMove(int x, int y);

private:
	std::list<Actor*> m_actors;
	TunnelMan* m_player;
	Earth* m_field[60][64];
};

#endif // STUDENTWORLD_H_
