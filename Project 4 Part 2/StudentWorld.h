#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
#include <queue>

const int MAX_ROWS = 60;
const int MAX_COLS = 64;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class TunnelMan;
class Earth;
class Squirt;
class Protester;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);
	//destructor
	virtual ~StudentWorld();

	//set up game field
	virtual int init();

	//call each actor to do something and check player status
	//add new actors if necessary
	virtual int move();

	//destroy dynamically allocated memory between levels
	virtual void cleanUp();

	//checks for boulders and out of bounds when moving
	//true if can move, false otherwise
	bool canMove(int x, int y) const;

	//checks for earth at given location
	//true if there is earth, false otherwise
	bool checkForEarth(int x, int y) const;

	//true if tunnelman is within radius, false otherwise
	bool isTunnelManNearby(int x, int y, int rad);

	//true if protester is within radius, false otherwise
	bool isProtestorNearby(int x, int y, int rad);

	//sets protester at location dead
	void killProtester(int x, int y);

	//checks if protestor is facing correct direction to shout
	bool canAnnoyTunnelMan(Actor* p);

	//sets gold to only be picked up by protester
	void dropGold(int x, int y);

	//checks bounds then adds squirt to world
	void addSquirt(int x, int y);

	//updates player's number of gold
	void gotGold();

	//decrement number of barrels left
	void gotOil();

	//increase player's amount of water
	void gotWater();

	//increase player's number of sonars
	void gotSonar();

	//make objects within sonar's vicinity visible
	void useSonar(int x, int y);

	//check if protester is facing correct direction to annoy
	void annoyTunnelMan(int amount);

	//remove overlapping earth with the given image
	bool removeEarth(int x, int y);


private:
	struct Coordinates 
	{
		Coordinates(int x, int y);
		int x;
		int y;
	};

	//generate location for boulders, barrels, and gold
	Coordinates getRandomLocation(int xLowerBound, int xUpperBound, int yLowerBound, int yUpperBound);
	
	//create formatted string to display
	void updateStatusText();

	//calculate distance between two points
	double distanceFormula(int x1, int y1, int x2, int y2) const;
	
	std::vector<Actor*> m_actors;
	TunnelMan* m_player;
	Earth* m_field[60][64];
	int m_nProtestors;
	int m_nBarrelsLeft;
	int m_ticks;
};

#endif // STUDENTWORLD_H_
