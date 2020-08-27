#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include<list>
#include<iostream>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class Socrates;
class Projectile;
class Flame;
class Goodie;
class RHealth;
class FThrower;
class ELife;
class Fungus;
class Bacteria;
class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
	~StudentWorld();
    virtual int init();//initialize the game
    virtual int move();//implement all actions in a game and return status
    virtual void cleanUp();//clean up everything and free the memories
	bool getshot(Projectile* p);//actions that a projectile will take when hitting an actor
	void addFlame();//add flames to the actor list when player pressed enter key
	void addSpray();//add sprays to the actor list when player pressed space key
	void addBacteria(int type,double x, double y);//add bacteria to the actor list when a new bacteria is born either from a pit or a duplication of a full bacteria
	void reducepits();//inform the student world class that a pit has already released all bacteria by decreasing the number of pits that is present by one
	bool gethealth(RHealth* r);//things going on when player gets restore health goodies(return true if got, false if not)
	bool getflame(FThrower* f);//things going on when player gets Flame thrower goodies(return true if got, false if not)
	bool getgoodie(Goodie* g);//return true if player gets a goodie, otherwise false.
	bool getlife(ELife* e);//things going on when player gets extra life goodies(return true if got, false if not)
	bool getfungus(Fungus* fungus);//things going on when player runs into a fungus(return true if did, false if not)
	bool hurtSocrates(Bacteria* b);//used to determine if a bacteria has hurt a Socrates and also implement the effect of damage(decrease player’s hp, play sound)
	bool inrange(Bacteria* b);//used to determine if the player is in 72pixels around a bacteria(for aggressive salmonella use)
	bool inrange2(Bacteria* b);//used to determine if the player is in 256 pixels around a bacteria(for ecoli use)
	bool blockbydirt(double x, double y);//determine if the result of the next movement’s position is going to cause an overlap with a dirt
	bool getFood(Bacteria* b);//determine if a bacteria got food and implement the effect of getting a food(increase the number of food eaten by the bacteria by one and set the food to dead)
	void dirtoplayer(Bacteria* b);//get the direction from the bacterium to the player and set it to the direction of the bacterium
	bool dirtofood(Bacteria* b);//get the direction from the bacterium to the closest food and set it to the direction of the bacterium
	double countDistance(double x1, double y1, double x2, double y2);//count the distance between two x,y pairs(two coordinates) 
private:
	Socrates* m_player;
	list<Actor*> m_actors;
	int m_pits;
	int m_bacteria;
	void dirtoactor(Actor* a,Bacteria* b);//set the direction of bacteria towards the actor specified
	double countDistance(Actor* a1, Actor* a2);// count the distance between two actors
	bool overlap(Actor* a1, Actor* a2);// determine if two actors overlap
	bool overlap(double x1, double y1, double x2, double y2);//determine if two coordinates overlap
	bool moverlap(double x1, double y1, double x2, double y2);//determine if two coordinates are in movement overlap positions according to spec
	void validcoord(list<Actor*> a,double& x,double& y);//check if the coordinate is valid(in the circle, does not overlap with some actors that cannot be overlapped)
	void randompos(double& x, double& y);//generate a random coordinate
};

#endif // STUDENTWORLD_H_
