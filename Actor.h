#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include<string>
using namespace std;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;
class Actor:public GraphObject {
public:
	Actor(int id, double startX, double startY,StudentWorld* s,int dir = 0, int depth = 0);
	void setDead();//set the status of the actor to dead
	StudentWorld* getstudent();
	virtual ~Actor();//virtual destructor since Actor is a base class
	virtual bool damageable()const;
	virtual bool isProjectile()const;
	virtual bool isFood()const;
	virtual bool isDirt()const;
	virtual bool isFungus()const;
	virtual bool isBacteria()const;
	virtual bool isEcoli()const;
	virtual bool cantoverlap()const;
	virtual bool couldblock()const;
	virtual bool hashp()const;//all these virtual functions return false by default and are later modified in subclasses
	int gethp()const;//return hitpoints of currect actor
	void changehp(int a);//change hitpoints of current actor
    bool alive() const;//return whether the actor is alive
	virtual void doSomething()=0;//pure virtual doSomething to be modified in subclasses
private:
	bool m_alive;
	int m_hp;
	int m_maxhp;
	StudentWorld* m_student;
};

class Socrates :public Actor {
public:
	Socrates(StudentWorld* student);
	~Socrates();
	virtual bool canoverlap()const;
	virtual bool hashp() const;
	void getPositionInThisDirection1(Direction angle, int units, double& dx, double& dy);//to get the position aroung the rim with direction respect to the center
	virtual void doSomething();//get keys and implement corresponding actions, if no keys get, then replenish sprays to 20
	int getflamenum()const;//return the number of flames left
	int getspray()const;//return the number of sprays left
	void increaseflame(int i);// increase the number of flames left

private:
	int m_flame;
	int m_spray;
};

class Dirt :public Actor {
public:
	Dirt(double x,double y,StudentWorld* s);
	~Dirt();
	virtual bool damageable()const;
	virtual bool isDirt()const;
	virtual bool couldblock()const;//used to identify actor property
	virtual void doSomething();//do nothing
};

class Food :public Actor {
public:
	Food(double x, double y,StudentWorld* s);
	~Food();
	virtual bool damageable()const;
	virtual bool isFood()const;
	virtual bool couldblock()const;//could block movements of bacteria
	virtual void doSomething();// do nothing
	virtual bool cantoverlap()const;//cannot be overlapped with pits and food and dirt
};

class Projectile :public Actor {
public:
	Projectile(int id, double x, double y, int d,StudentWorld* s,int dir);
	virtual~Projectile();
	virtual bool isProjectile()const;
	virtual int getDamage() const=0; //get the damage of the projectile, pure virtual because base class could have different damages
	virtual void doSomething();//if the projectile is dissipated, return immediately, if it hits something, do the damage and dissipate, and return immediately, if none of the above happened, move in its current direction in the specified distance, if it moved a total distance more than it should, it dissipates.
private:
	int m_distance;
};
class Flame :public Projectile {
public:
	Flame(double x, double y, int dir, StudentWorld* s);
	virtual int getDamage()const;
};
class Spray :public Projectile {
public:
	Spray(double x, double y, int dir, StudentWorld* s);
	virtual int getDamage()const;
};
class Goodie :public Actor {
public:
	Goodie(int id, double x, double y, StudentWorld* s,int L);
	virtual ~Goodie();
	virtual bool getSomething()=0;
	virtual void doSomething();//if dissipated, return immediately, if not, then see if player gets the goodie, tell studentworld to play sound and do corresponding action to the player and dissipate the goodie. If nothing happened, reduce the tick left for the goodie before it dissipates, if time left not greater than 0, dissipate itself.
	virtual bool damageable()const;
private:
	int m_lifespan;
};
class RHealth :public Goodie {
public:
	RHealth(double x, double y, StudentWorld* s, int L);
	virtual bool getSomething();//tell student world to check and implement the player getting a restore health goodie
};
class FThrower :public Goodie {
public:
	FThrower(double x, double y, StudentWorld* s, int L);
	virtual bool getSomething();//tell student world to check and implement the player getting a flame thrower goodie
};
class ELife :public Goodie {
public:
	ELife(double x, double y, StudentWorld* s, int L);
	virtual bool getSomething();//tell student world to check and implement the player getting an extra life goodie
};
class Fungus :public Goodie {
public:
	Fungus(double x, double y, StudentWorld* s, int L);
	virtual bool getSomething();//tell student world to check and implement the player running into a fungus
	virtual bool isFungus()const;
};
class Pit :public Actor {
public:
	Pit(double x, double y, StudentWorld* s);
	virtual void doSomething();//if it contains no more bacteria, then dissipate and inform the student world class that a pit dissipated. Otherwise add bacteria into the studentworld actor list according to the spec and decrease the number of corresponding bacteria held by one.
	virtual bool cantoverlap()const;//it connot be overlapped while constructed in studentworld class's init funciton
private:
	int m_rs;
	int m_as;
	int m_ec;
};
class Bacteria :public Actor {
public:
	Bacteria(int id, double x, double y, StudentWorld* s);
	virtual~Bacteria();
	virtual void doSomething();
	virtual bool damageable()const;//return true because could be damaged by projectile
	virtual bool hashp()const;
	virtual bool isBacteria()const;
	virtual int getdamage()const=0;//get damage of the bacteria pure virtual because different bacteria have different damages
	virtual bool aggressive()const;
	int getnumoffood()const;//return the number of food that the bacteria had ate
	void eatfood();//increase the number of food the bacteria had ate by one
	void resetfood();//set the number of food the bacteria had ate to 0
	int getmoveplan()const;//get the move plan distance of bacteria
	void decmoveplan();//decrease the move plan distance of the bacteria
	void resetplan();//set the move plan distance to 10 
	void trymove();//try to move getpixel() amount of pixels in current direction, if failed, then set direction to a new direction
	virtual int getpixel()const;//return the pixels the bacteria should move during each tick, if it could. It returns 3 by default which is the number of pixels salmonellas move.
private:
	int m_food;
	int m_displan;
};
class ReSal :public Bacteria {
public:
	ReSal(double x, double y, StudentWorld* s);
	virtual int getdamage()const;
};
class AgSal :public Bacteria {
public:
	AgSal(double x, double y, StudentWorld* s);
	virtual int getdamage()const;
	virtual bool aggressive()const;
};
class Ecoli :public Bacteria {
public:
	Ecoli(double x, double y, StudentWorld* s);
	virtual int getdamage()const;
	virtual bool isEcoli()const;
	virtual int getpixel()const;//return 2
};
#endif // ACTOR_H_
