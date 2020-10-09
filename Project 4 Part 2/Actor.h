#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject
{
  public:
    
    //constructor
    Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* world);
    
    //destructor
    virtual ~Actor();

    //pure virtual function
    virtual void doSomething() = 0;

    //returns if actor is alive (true) or dead (false)
    bool isActive();

    //set active status 
    void setActive(bool status);

    //change input coordinates based distance and direction
    void newLocation(int& x, int& y, int distance, Direction dir);

    //returns false on default, manually change if actor blocks path
    virtual bool blocksPath();

    //returns false on default, manually change if is protester
    virtual bool isProtester();

    //returns pointer to student world
    StudentWorld* getWorld();

  private:
    bool m_active;
    StudentWorld* m_world;
};

class Earth : public Actor
{
  public:

    //constructor
    Earth(int startX, int startY, StudentWorld* world);

    //destructor
    virtual ~Earth();

    virtual void doSomething();
};

class TunnelMan : public Actor
{
  public:

    //constructor
    TunnelMan(StudentWorld* world);

    //destructor
    virtual ~TunnelMan();

    //move, remove earth, drop gold, squirt water, escape, or use sonar
    virtual void doSomething();

    //returns hit points left
    int getPoints() const;

    //returns sonars left
    int getSonar() const;

    //adds sonars to count
    void setSonar(int sonar);

    //returns amount of water
    int getWater() const;

    //adds water to count
    void setWater(int water);

    //returns number of gold nuggets
    int getGold() const;

    //adds nuggets to stash
    void setGold(int golds);

    //annoy tunnelman and decrement hit points 
    //return true if lost a life, false otherwise
    bool annoy(int decPoints);

private:
    int m_points;
    int m_water;
    int m_sonar;
    int m_gold;
};

class Protester : public Actor
{
  public:

    //constructor
    Protester(StudentWorld* world, int imageID);

    //destructor
    virtual ~Protester();

    //move, shout at player
    virtual void doSomething();

    //returns if protester is leaving
    bool leavingField();

    //set that protester should leave
    void setLeavingField(bool leave);

    //return number of squares left to travel in curr direction
    int getSquares();

    //set number of squares to travel in curr direction
    void setSquares(int squares);

    //return current tick
    int getCurTick();

    //(re)set current tick
    void setCurTick(int tick);

    //return ticks to wait to do something
    int getTicksToWait();

    //set ticks to wait to do something
    void setTicksToWait(int amount);

    //return number of hit points left
    int getPoints();

    //set number of hit points
    void setPoints(int amount);

    //annoy protester, decrement hit points
    virtual void annoy(int amount);

    //return true bc it is a protester
    virtual bool isProtester();

  private:
    int m_ticksToWait;
    int m_numSquaresToMove;
    bool m_leaveOilField;
    int m_curTick;
    int m_ticksToShout;
    int m_ticksToTurn;
    int m_points;
};

class Regular : public Protester
{
  public:  
    //constructor
    Regular(StudentWorld* world);

};

class Hardcore : public Protester
{
  public:
    //constructor
    Hardcore(StudentWorld* world);
};

class Goodies : public Actor
{
  public:
    
    //constructor
    Goodies(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* world);
    
    //destructor
    virtual ~Goodies();
    virtual int getTick();
    virtual void incTick();
    virtual bool getVisibility();

    //check if tunnelman is nearby and reveal object
    virtual void revealGoodie(Goodies* obj, int rad);

  private:
    int m_tickCounter;
    bool revealed;
};

class Boulders : public Goodies
{
  public:

    //constructor
    Boulders(int startX, int startY, StudentWorld* world);
    
    //destructor
    virtual ~Boulders();

    //check if it should fall, check if it falls on an actor
    virtual void doSomething();

    //returns true because it blocks paths
    virtual bool blocksPath();

  private:
      int m_status;   //1 is stable, 2 is waiting, 3 is falling
};

class Squirt : public Goodies
{
  public:
   
    //constructor
    Squirt(int startX, int startY, Direction dir, StudentWorld* world);
    
    //destructor
    virtual ~Squirt();
    
    //travel four squares, check for actors
    virtual void doSomething();    

  private:
    int m_travel;
};

class Barrel : public Goodies
{
public:

    //constructor
    Barrel(int startX, int startY, StudentWorld* world);
    
    //destructor
    virtual ~Barrel();
    
    //check if it should be visible, and if player picked it up
    virtual void doSomething();
};

class Nugget : public Goodies
{
  public:  

    //constructor
    Nugget(int startX, int startY, StudentWorld* world, bool hasBeenPicked);
    
    //destructor
    virtual ~Nugget();
    
    //check if it should be visible, and if player picked it up
    //check if player dropped it, and protester saw it
    virtual void doSomething();

  private:
    bool pickedByTunnelMan;
};

class Sonar : public Goodies
{
  public:

    //constructor
    Sonar(StudentWorld* world);

    //destructor
    virtual ~Sonar();
    
    //check if it should be visible, and if player picked it up
    virtual void doSomething();

  private:
    int ticksLeft;
};

class WaterPool : public Goodies
{
  public:

    //constructor
    WaterPool(int startX, int startY, StudentWorld* world);

    //destructor
    virtual ~WaterPool();

    //check if it should be visible, and if player picked it up
    virtual void doSomething();

  private:
    int ticks;
};


#endif // ACTOR_H_