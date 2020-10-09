#include "Actor.h"
#include "StudentWorld.h"
#include "GameController.h"
#include <algorithm>
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

////////////////////////////////////////////////////////////////
/*****************   Actor Implementation   *******************/
////////////////////////////////////////////////////////////////

Actor::Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* world)
	: GraphObject(imageID, startX, startY, dir, size, depth)
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

void Actor::setActive(bool status)
{
	m_active = status;
}

void Actor::newLocation(int& x, int& y, int distance, Direction dir)
{
	//edit coordinates to move distance units in direction
	if (dir == left)
		x -= distance;
	else if (dir == right)
		x += distance;
	else if (dir == down)
		y -= distance;
	else if (dir == up)
		y += distance;
}

bool Actor::blocksPath()
{
	return false;
}

bool Actor::isProtester()
{
	return false;
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
	if (!isActive())
		return;

	int move;
	if (getWorld()->getKey(move) == true)
	{
		switch (move)
		{
		case KEY_PRESS_ESCAPE:
			setActive(false);				//abort current level
			return;
		//go left
		case KEY_PRESS_LEFT:
			if (getDirection() == left && getWorld()->canMove(getX()-1, getY()))
				moveTo(getX()-1, getY());												//if is in current direction, then check if allowed to move and use moveTo()
			else
				setDirection(left);														//if is in diff dir than current then turn but do not move
			break;
		//go right
		case KEY_PRESS_RIGHT:
			if (getDirection() == right && getWorld()->canMove(getX()+1, getY()))
				moveTo(getX()+1, getY());
			else
				setDirection(right);
			break;
		//go down
		case KEY_PRESS_DOWN:
			if (getDirection() == down && getWorld()->canMove(getX(), getY()-1))
				moveTo(getX(), getY()-1);
			else
				setDirection(down);
			break;
		//go up
		case KEY_PRESS_UP:
			if (getDirection() == up && getWorld()->canMove(getX(), getY() + 1))
				moveTo(getX(), getY() + 1);
			else
				setDirection(up);
			break;
		case KEY_PRESS_SPACE:
			if (m_water > 0) 
			{
				//Add squirt into world
				getWorld()->playSound(SOUND_PLAYER_SQUIRT);
				m_water--;
				int x = getX();
				int y = getY();
				newLocation(x, y, 1, getDirection());
				getWorld()->addSquirt(x, y);
			}
			break;
		case KEY_PRESS_TAB:
			if (m_gold > 0)
			{
				//Add nugget into world
				m_gold--;
				getWorld()->dropGold(getX(), getY());
			}
			break;
		case 'z':
		case 'Z':
			if (m_sonar > 0)
			{
				//use sonar
				getWorld()->playSound(SOUND_SONAR);
				m_sonar--;
				getWorld()->useSonar(getX(), getY());
			}
		}
	}

	//if overlap with earth -> remove earth
	bool playSound = false;
	for (int k = getY(); k < getY() + 4 && k < 60; k++)
	{
		for (int i = getX(); i < getX() + 4 && i < 64; i++)
		{
			if (getWorld()->removeEarth(i, k))
			{
				//make digging sound
				playSound = true;
			}
		}
	}

	if (playSound)
		getWorld()->playSound(SOUND_DIG);
}

int TunnelMan::getPoints() const
{
	return m_points;
}

void TunnelMan::setSonar(int sonar)
{
	m_sonar += sonar;
}

int TunnelMan::getSonar() const
{
	return m_sonar;
}

void TunnelMan::setWater(int water) 
{ 
	m_water += water; 
}

int TunnelMan::getWater() const
{
	return m_water;
}

void TunnelMan::setGold(int gold) 
{
	m_gold += gold; 
}

int TunnelMan::getGold() const
{
	return m_gold;
}

bool TunnelMan::annoy(int decPoints)
{
	//decrement points
	m_points -= decPoints;

	//if health is gone
	if (m_points <= 0)
	{
		//give up and die!
		getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
		setActive(false);
		return true;
	}
	
	return false;
}


////////////////////////////////////////////////////////////////
/***************   Protestor Implementation   *****************/
////////////////////////////////////////////////////////////////

Protester::Protester(StudentWorld* world, int imageID) : Actor(imageID, 60, 60, left, 1, 0, world)
{
	m_leaveOilField = false;
	m_numSquaresToMove = rand() % 53 + 8;
	m_curTick = m_ticksToWait = std::max(0, 3 - (int)getWorld()->getLevel() / 4);
	m_ticksToShout = 0;
	m_ticksToTurn = 200;
 }

Protester::~Protester()
{
}

void Protester::doSomething()
{
	if (!isActive())
		return;

	//rest state
	if (m_curTick != 0)
	{
		m_curTick--;
		return;
	}

	//not rest state, reset current tick
	m_curTick = m_ticksToWait;

	//if leaving the oil field, remove at (60,60)
	if (m_leaveOilField)
	{
		if (getX() == 60 && getY() == 60)
			setActive(false);
	}

	//shout
	if (m_ticksToShout > 0)
		m_ticksToShout--;

	//if it is not leaving and it can shout at this tick then shout
	if (!m_leaveOilField && m_ticksToShout == 0 && getWorld()->isTunnelManNearby(getX(), getY(), 4) && getWorld()->canAnnoyTunnelMan(this))
	{
		getWorld()->playSound(SOUND_PROTESTER_YELL);
		getWorld()->annoyTunnelMan(2);
		m_ticksToShout = 15;
		return;
	}

	//move in current direction
	if (m_numSquaresToMove > 0 && m_ticksToTurn > 0)
	{
		int x = getX();
		int y = getY();
		m_numSquaresToMove--;
		m_ticksToTurn--;
		newLocation(x, y, 1, getDirection());
		if (getWorld()->canMove(x, y) && !getWorld()->checkForEarth(x, y))
			moveTo(x, y);
		else
		{
			m_numSquaresToMove = 0;
		}
			
	}

	//change direction
	if (m_numSquaresToMove <= 0 || m_ticksToTurn <= 0)
	{
		for (;;)
		{
			//pick random direction
			int a = rand() % 4;
			int x = getX();
			int y = getY();
			if (a == 0)
				newLocation(x, y, 1, left);
			else if (a == 1)
				newLocation(x, y, 1, right);
			else if (a == 2)
				newLocation(x, y, 1, down);
			else if (a == 3)
				newLocation(x, y, 1, up);

			//make sure moving in said direction is possible
			if (getWorld()->canMove(x, y) && !getWorld()->checkForEarth(x, y))
			{
				if (a == 0)
					setDirection(left);
				else if (a == 1)
					setDirection(right);
				else if (a == 2)
					setDirection(down);
				else if (a == 3)
					setDirection(up);
				moveTo(x, y);
				m_numSquaresToMove = rand() % 53 + 8;
				break;
			}
		}

		//reset turn ticks
		if (m_ticksToTurn <= 0)
			m_ticksToTurn = 200;
	}

}

bool Protester::leavingField()
{
	return m_leaveOilField;
}

void Protester::setLeavingField(bool leave)
{
	m_leaveOilField = leave;
}

int Protester::getSquares()
{
	return m_numSquaresToMove;
}

void Protester::setSquares(int squares)
{
	m_numSquaresToMove = squares;
}

int Protester::getCurTick()
{
	return m_curTick;
}

void Protester::setCurTick(int tick)
{
	m_curTick = tick;
}

int Protester::getTicksToWait()
{
	return m_ticksToWait;
}

void Protester::setTicksToWait(int amount)
{
	m_ticksToWait = amount;
}

int Protester::getPoints()
{
	return m_points;
}

void Protester::setPoints(int amount)
{
	m_points = amount;
}

bool Protester::isProtester()
{
	return true;
}

void Protester::annoy(int amount)
{
	//cant be annoyed if leaving
	if (m_leaveOilField == true)
		return;

	//decrement from hit points
	m_points -= amount;

	//if hit points reached zero, leave oil field
	if (m_points <= 0)
	{
		getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
		setLeavingField(true);
		setActive(false);
	}

	//otherwise reset ticks to wait to do something
	else
	{
		getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
		m_ticksToWait = std::max(50, 100 - (int)getWorld()->getLevel() * 10);
	}
}

////////////////////////////////////////////////////////////////
/****************   Regular Implementation   ******************/
////////////////////////////////////////////////////////////////

Regular::Regular(StudentWorld* world) : Protester(world, TID_PROTESTER)
{
	setPoints(5);
}

////////////////////////////////////////////////////////////////
/***************   Hardcore Implementation   ******************/
////////////////////////////////////////////////////////////////
Hardcore::Hardcore(StudentWorld* world) : Protester(world, TID_HARD_CORE_PROTESTER)
{
	setPoints(20);
}

////////////////////////////////////////////////////////////////
/****************   Goodies Implementation   ******************/
////////////////////////////////////////////////////////////////

Goodies::Goodies(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* world)
	: Actor(imageID, startX, startY, dir, size, depth, world)
{
	m_tickCounter = 0;
	revealed = false;
}

Goodies::~Goodies()
{
}

int Goodies::getTick()
{
	return m_tickCounter;
}

void Goodies::incTick()
{
	m_tickCounter++;
}

bool Goodies::getVisibility()
{
	return revealed;
}

void Goodies::revealGoodie(Goodies* obj, int rad)
{
	//if player is nearby, reveal goodie
	if (obj->isActive() && obj->getWorld()->isTunnelManNearby(getX(), getY(), rad))
	{
		setVisible(true);
		revealed = true;
	}
}

////////////////////////////////////////////////////////////////
/****************   Boulder Implementation   ******************/
////////////////////////////////////////////////////////////////

Boulders::Boulders(int startX, int startY, StudentWorld* world) : Goodies(TID_BOULDER, startX, startY, down, 1.0, 1, world)
{
	m_status = 1;
}

Boulders::~Boulders()
{
}

void Boulders::doSomething()
{
	if (!isActive())
		return;

	//stable, check for earth below
	if (m_status == 1)
	{
		if (!getWorld()->checkForEarth(getX(), getY() - 1))
			m_status = 2;
	}

	//waiting, increment ticks
	else if (m_status == 2)
	{
		if (getTick() == 30)
			m_status = 3;
		incTick();
	}

	//falling, check for actors below and ability to move
	else if (m_status == 3)
	{
		if (getWorld()->isTunnelManNearby(getX(), getY(), 3))
		{
			getWorld()->annoyTunnelMan(100);
		}

		if (getWorld()->isProtestorNearby(getX(), getY(), 3))
		{
			getWorld()->killProtester(getX(), getY());
			getWorld()->increaseScore(100);
			getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
		}

		if (getY() - 1 >= 0 && !(getWorld()->checkForEarth(getX(), getY() - 1)))
			moveTo(getX(), getY() - 1);

		else
			setActive(false);
	}
}

bool Boulders::blocksPath()
{ return true;}

////////////////////////////////////////////////////////////////
/*****************   Squirt Implementation   ******************/
////////////////////////////////////////////////////////////////

Squirt::Squirt(int startX, int startY, Direction dir, StudentWorld* world) : Goodies(TID_WATER_SPURT, startX, startY, dir, 1, 1, world), m_travel(3)
{
	m_travel = 3;
}

void Squirt::doSomething()
{
	if (!isActive())
		return;

	//travelled four squares
	if (m_travel == 0)
		setActive(false);

	//if can move, move to new location
	int x = getX();
	int y = getY();
	newLocation(x, y, 1, getDirection());

	//check for protesters
	if (getWorld()->isProtestorNearby(getX(), getY(), 3))
	{
		setActive(false);
		getWorld()->increaseScore(100);
		getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
	}

	//check if it can move
	if (getWorld()->canMove(x, y) && !getWorld()->checkForEarth(x,y)) //TODO HERE
	{
		moveTo(x, y);
		m_travel--;
	}
	
	//if can't move, set dead
	else
		setActive(false);
}

Squirt::~Squirt()
{
}

////////////////////////////////////////////////////////////////
/*****************   Nugget Implementation   ******************/
////////////////////////////////////////////////////////////////

Nugget::Nugget(int startX, int startY, StudentWorld* world, bool hasBeenPicked) : Goodies(TID_GOLD, startX, startY, right, 1.0, 2, world)
{
	pickedByTunnelMan = hasBeenPicked;
	if(pickedByTunnelMan)
		setVisible(true);
	else
		setVisible(false);
}

Nugget::~Nugget()
{
}

void Nugget::doSomething()
{
	if (!isActive())
		return;

	//check if it should be revealed
	if (pickedByTunnelMan == false && getVisibility() == false)
	{
		revealGoodie(this, 4);
		return;
	}

	//allow tunnelman to pick it up
	if (getVisibility() && pickedByTunnelMan == false && getWorld()->isTunnelManNearby(getX(), getY(), 3))
	{
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->gotGold();
		getWorld()->increaseScore(10);
		setActive(false);
	}
	
	//if dropped gold then check it's lifetime has elapsed or if protester found it
	if (pickedByTunnelMan && getTick() < 100)
	{
		incTick();
		if (getWorld()->isProtestorNearby(getX(), getY(), 3))
		{
			setActive(false);
			getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
			getWorld()->increaseScore(25);
		}
	}

	else if (pickedByTunnelMan && getTick() == 100)
		setActive(false);
}

////////////////////////////////////////////////////////////////
/*****************   Barrel Implementation   ******************/
////////////////////////////////////////////////////////////////

Barrel::Barrel(int startX, int startY, StudentWorld* world) : Goodies(TID_BARREL, startX, startY, right, 1.0, 2, world)
{
	setVisible(false);
}

Barrel::~Barrel()
{
}

void Barrel::doSomething()
{
	if (!isActive())
		return;

	//check if it should be revealed
	if (getVisibility() == false)
	{
		revealGoodie(this, 3);
		return;
	}

	//allow tunnelman to pick it up
	else if (getVisibility() && getWorld()->isTunnelManNearby(getX(), getY(), 3))
	{
		getWorld()->playSound(SOUND_FOUND_OIL);
		getWorld()->gotOil();
		getWorld()->increaseScore(1000);
		setActive(false);
	}
}

////////////////////////////////////////////////////////////////
/*****************    Sonar Implementation   ******************/
////////////////////////////////////////////////////////////////

Sonar::Sonar(StudentWorld* world) : Goodies(TID_SONAR, 0, 60, right, 1.0, 2, world)
{
	ticksLeft = std::max(100, (300 - (10 * (int)getWorld()->getLevel())));
}

Sonar::~Sonar()
{
}

void Sonar::doSomething()
{
	if (!isActive())
		return;

	//increment tick and check if it should disappear
	incTick();
	if (getTick() >= ticksLeft)
	{
		setActive(false);
		return;
	}

	//allow tunnelman to pick up
	else if (getWorld()->isTunnelManNearby(getX(), getY(), 3))
	{
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->gotSonar();
		getWorld()->increaseScore(75);
		setActive(false);
	}
}

////////////////////////////////////////////////////////////////
/***************    WaterPool Implementation   ****************/
////////////////////////////////////////////////////////////////

WaterPool::WaterPool(int startX, int startY, StudentWorld* world) : Goodies(TID_WATER_POOL, startX, startY, right, 1.0, 2, world)
{
	ticks = std::max(100, (300 - (10 * (int)getWorld()->getLevel())));
}

WaterPool::~WaterPool()
{
}

void WaterPool::doSomething()
{
	if (!isActive())
		return;

	//increment tick and check if it should disappear
	incTick();
	if (getTick() >= ticks)
	{
		setActive(false);
		return;
	}

	//allow tunnelman to pick up
	else if (getWorld()->isTunnelManNearby(getX(), getY(), 3))
	{
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->gotWater();
		getWorld()->increaseScore(100);
		setActive(false);
	}
}

