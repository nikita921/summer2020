#include "Actor.h"
#include "StudentWorld.h"
#include "GameController.h"
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

////////////////////////////////////////////////////////////////
/*****************   Actor Implementation   *******************/
////////////////////////////////////////////////////////////////

Actor::Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* world) : GraphObject(imageID,
	startX, startY, dir, size, depth)
{
	setVisible(true);
	m_active = true;
	m_world = world;
}

Actor::~Actor()
{}

bool Actor::isActive()
{
	return m_active;
}

void Actor::setDead()
{
	m_active = false;
}

StudentWorld* Actor::getWorld()
{
	return m_world;
}

////////////////////////////////////////////////////////////////
/*****************   Earth Implementation   *******************/
////////////////////////////////////////////////////////////////

Earth::Earth(int startX, int startY, StudentWorld* world) : Actor(TID_EARTH, startX, startY, right, 0.25, 3, world)
{}

Earth::~Earth()
{}

void Earth::doSomething()
{
	return;
}
////////////////////////////////////////////////////////////////
/***************   TunnelMan Implementation   *****************/
////////////////////////////////////////////////////////////////

TunnelMan::TunnelMan(StudentWorld* world) : Actor(TID_PLAYER, 30, 60, right, 1, 0, world)
{
	m_points = 10;
	m_water = 5;
	m_sonar = 1;
	m_gold = 0;
}

TunnelMan::~TunnelMan()
{}

void TunnelMan::doSomething()
{
	//check if dead


	//if overlap with earth -> remove earth
	for (int k = getY(); k < getY() + 4 && k < 60; k++) 
	{
		for (int i = getX(); i < getX() + 4 && i < 64; i++) 
		{
			if (getWorld()->removeEarth(i, k))
				//make digging sound
				GameController::getInstance().playSound(SOUND_DIG);
		}
	}
	
		
	//get user input, if key pressed, perform action
		//escape means abort current level
			//tunnelman no longer active
			//replay level or end game if lives are over

		//spacebar means squirt if water exists
			//reduce water count
			//create new squirt

		//arrow key
	int move;
	if (getWorld()->getKey(move) == true)
	{
		switch (move)
		{
		case KEY_PRESS_LEFT:
			if (getDirection() == left && getWorld()->canMove(getX()-1, getY()))
				moveTo(getX()-1, getY());												//if is in current direction, then check if allowed to move and use moveTo()
			else
				setDirection(left);														//if is in diff dir than current then turn but do not move
			break;
		case KEY_PRESS_RIGHT:
			if (getDirection() == right && getWorld()->canMove(getX()+1, getY()))
				moveTo(getX()+1, getY());
			else
				setDirection(right);
			break;
		case KEY_PRESS_DOWN:
			if (getDirection() == down && getWorld()->canMove(getX(), getY()-1))
				moveTo(getX(), getY()-1);
			else
				setDirection(down);
			break;
		case KEY_PRESS_UP:
			if (getDirection() == up && getWorld()->canMove(getX(), getY() + 1))
				moveTo(getX(), getY() + 1);
			
			else
				setDirection(up);
			break;
		}
	}

	//if key not pressed
}

void TunnelMan::setPoints(int points)
{
	m_points = points;
}

int TunnelMan::getPoints() const
{
	return m_points;
}

void TunnelMan::setSonar(int sonar)
{
	m_sonar = sonar;
}

int TunnelMan::getSonar() const
{
	return m_sonar;
}

void TunnelMan::setWater(int water) 
{ 
	m_water = water; 
}

int TunnelMan::getWater() const
{
	return m_water;
}

void TunnelMan::setGold(int gold) 
{
	m_gold = gold; 
}

int TunnelMan::getGold() const
{
	return m_gold;
}
