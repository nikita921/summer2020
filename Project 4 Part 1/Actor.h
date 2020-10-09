#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject
{
  public:
      Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* world);
      virtual ~Actor();
      virtual void doSomething() = 0;
      virtual bool isActive();
      virtual void setDead();
      StudentWorld* getWorld();

  private:
      bool m_active;
      StudentWorld* m_world;
};

class Earth : public Actor
{
  public:
      Earth(int startX, int startY, StudentWorld* world);
      virtual ~Earth();
      virtual void doSomething();
};

class TunnelMan : public Actor
{
  public:
      TunnelMan(StudentWorld* world);
      virtual ~TunnelMan();
      virtual void doSomething();
      int getPoints() const;
      void setPoints(int points);
      int getSonar() const;
      void setSonar(int sonar);
      int getWater() const;
      void setWater(int water);
      int getGold() const;
      void setGold(int golds);

private:
    int m_points;
    int m_water;
    int m_sonar;
    int m_gold;
};



#endif // ACTOR_H_