#include "Actor.h"
#include "StudentWorld.h"
#include<cstdlib>
////////////////////////////////////////////
//Actor implementation//////////////////////
////////////////////////////////////////////
Actor::Actor(int id, double startX, double startY, StudentWorld* s, int dir, int depth)
	:GraphObject(id, startX, startY, dir, depth) {
	m_alive = true;
	m_student = s;
	m_hp = 0;
	m_maxhp = 100;
}

StudentWorld* Actor::getstudent() {
	return m_student;
}
bool Actor::damageable()const {
	return false;
}
bool Actor::isEcoli()const {
	return false;
}
bool Actor::isBacteria()const {
	return false;
}
bool Actor::isProjectile()const {
	return false;
}
bool Actor::isFood()const {
	return false;
}
bool Actor::isDirt()const {
	return false;
}
bool Actor::isFungus()const {
	return false;
}
bool Actor::cantoverlap()const {
	return false;
}
bool Actor::couldblock()const {
	return false;
}
bool Actor::hashp()const {
	return false;
}

void Actor::setDead() {
	m_alive = false;
}

bool Actor::alive()const {
	return m_alive;
}

int Actor::gethp()const {
	return m_hp;
}

void Actor::changehp(int a) {
	m_hp += a;
	if (m_hp <= 0)
		setDead();
	if (m_hp > m_maxhp)
		m_hp = m_maxhp;
}
Actor:: ~Actor() {}
//////////////////////////////////////////
//Socrates implementation/////////////////
//////////////////////////////////////////
Socrates::Socrates(StudentWorld* student)
	: Actor(IID_PLAYER, 0, VIEW_RADIUS,student, 0, 0) {
	changehp(100);
	m_spray = 20;
	m_flame = 5;
}

Socrates::~Socrates() {
}

bool Socrates::canoverlap()const {
	return false;
}

bool Socrates::hashp()const {
	return true;
}
int Socrates::getflamenum()const {
	return m_flame;
}
int Socrates::getspray()const {
	return m_spray;
}
void Socrates::increaseflame(int i) {
	m_flame += i;
}

void Socrates::getPositionInThisDirection1(Direction angle, int units, double& dx, double& dy)
{
	const double PI = 4 * atan(1);
	dx = (VIEW_WIDTH / 2 + units * cos(angle * 1.0 / 360 * 2 * PI));
	dy = (VIEW_HEIGHT / 2 + units * sin(angle * 1.0 / 360 * 2 * PI));
}

void Socrates::doSomething() {
	if (!alive())
		return;
	int ch;
	if (getstudent()->getKey(ch)) {
		switch (ch) {
		case KEY_PRESS_LEFT: {
			int tempdir1 = getDirection() + 180 + 5;
			double x1, y1;
			getPositionInThisDirection1(tempdir1, VIEW_RADIUS, x1, y1);
			moveTo(x1, y1);
			setDirection(tempdir1 + 180);
		}
			break;
		case KEY_PRESS_RIGHT: {
			int tempdir2 = getDirection() + 180 - 5;
			double x2, y2;
			getPositionInThisDirection1(tempdir2, VIEW_RADIUS, x2, y2);
			moveTo(x2, y2);
			setDirection(tempdir2 + 180);
		}
			break;
		case KEY_PRESS_ENTER: {
			if (m_flame > 0) {
				getstudent()->addFlame();
				getstudent()->playSound(SOUND_PLAYER_FIRE);
				m_flame--;
			}
			break;
		}
		case KEY_PRESS_SPACE: {
			if (m_spray > 0) {
				getstudent()->addSpray();
				getstudent()->playSound(SOUND_PLAYER_SPRAY);
				m_spray--;
			}
			break;
		}
		default:
			break;
		}
	}
	else {
		if (m_spray < 20)
			m_spray++;
	}
}
//////////////////////////////////////////////////
//Dirt Pile implementation////////////////////////
//////////////////////////////////////////////////
Dirt::Dirt(double x, double y,StudentWorld* s)
	:Actor(IID_DIRT, x, y,s, 0, 1) {
}

Dirt::~Dirt() {}
bool Dirt::isDirt()const {
	return true;
}
bool Dirt::damageable()const {
	return true;
}

bool Dirt::couldblock()const {
	return true;
}

void Dirt::doSomething() {
}
//////////////////////////////////////////////////
//////Food implementation/////////////////////////
//////////////////////////////////////////////////
Food::Food(double x, double y,StudentWorld* s) 
:Actor(IID_FOOD,x,y,s,90,1){
}
Food::~Food() {}
bool Food::damageable()const {
	return true;
}
bool Food::isFood()const {
	return true;
}
bool Food::couldblock()const {
	return true;
}
bool Food::cantoverlap()const {
	return true;
}

void Food::doSomething() {}
/////////////////////////////////////////
/////Projectile implementation///////////
/////////////////////////////////////////
Projectile::Projectile(int id, double x, double y,int d, StudentWorld* s,int dir) 
:Actor(id,x,y,s,dir,1){
	m_distance = d;
}
Projectile::~Projectile(){}
bool Projectile::isProjectile()const {
	return true;
}
void Projectile::doSomething() {
	if (!alive())
		return;
	if (getstudent()->getshot(this))
		return;
	moveAngle(getDirection(), SPRITE_RADIUS * 2);
	m_distance -= SPRITE_RADIUS * 2;
	if (m_distance <= 0)
		setDead();
}
/////////////////////////////////////////
///////Flame implementation//////////////
/////////////////////////////////////////
Flame::Flame(double x, double y, int dir, StudentWorld* s) 
:Projectile(IID_FLAME,x,y,32,s,dir){
}
int Flame::getDamage()const {
	return 5;
}
/////////////////////////////////////////
///////Spray implementation//////////////
/////////////////////////////////////////
Spray::Spray(double x, double y, int dir, StudentWorld* s)
	:Projectile(IID_SPRAY, x, y, 112, s, dir) {
}
int Spray::getDamage()const {
	return 2;
}
/////////////////////////////////////////
///////Goodie implementation/////////////
/////////////////////////////////////////
Goodie::Goodie(int id, double x, double y, StudentWorld* s,int L) 
:Actor(id,x,y,s,0,1){
	m_lifespan = max(rand()%(300 - 10 * L), 50);
}
Goodie:: ~Goodie() {}
bool Goodie::damageable()const {
	return true;
}
void Goodie::doSomething() {
	if (!alive())
		return;
	getstudent()->getgoodie(this);
	if (getSomething())
		return;
	if (m_lifespan > 0)
		m_lifespan--;
	else
		setDead();
}
/////////////////////////////////////////
///////RHealth implementation/////////////
/////////////////////////////////////////
RHealth::RHealth(double x, double y, StudentWorld* s, int L) 
:Goodie(IID_RESTORE_HEALTH_GOODIE,x,y,s,L){
}
bool RHealth::getSomething(){
	if (getstudent()->gethealth(this))
		return true;
	return false;
}
/////////////////////////////////////////
///////FThrower implementation///////////
/////////////////////////////////////////
FThrower::FThrower(double x, double y, StudentWorld* s, int L)
	:Goodie(IID_FLAME_THROWER_GOODIE, x, y, s, L) {
}
bool FThrower::getSomething() {
	if (getstudent()->getflame(this))
		return true;
	return false;
}
/////////////////////////////////////////
///////ELife implementation//////////////
/////////////////////////////////////////
ELife::ELife(double x, double y, StudentWorld* s, int L)
	:Goodie(IID_EXTRA_LIFE_GOODIE, x, y, s, L) {
}
bool ELife::getSomething() {
	if (getstudent()->getlife(this))
		return true;
	return false;
}
/////////////////////////////////////////
///////Fungus implementation/////////////
/////////////////////////////////////////
Fungus::Fungus(double x, double y, StudentWorld* s, int L) 
:Goodie(IID_FUNGUS,x,y,s,L){
}
bool Fungus::getSomething() {
	if (getstudent()->getfungus(this))
		return true;
	return false;
}
bool Fungus::isFungus()const {
	return true;
}
/////////////////////////////////////////
///////Pit implementation///////////////
/////////////////////////////////////////
Pit::Pit(double x, double y, StudentWorld* s) 
	:Actor(IID_PIT,x,y,s,0,1){
	m_rs = 5;
	m_as = 3;
	m_ec = 2;
}
bool Pit::cantoverlap()const {
	return true;
}
void Pit::doSomething() {
	if (m_rs == 0 && m_as == 0 && m_ec == 0) {
		getstudent()->reducepits();
		setDead();
		return;
	}
	int a = randInt(1, 50);
	if (a == 1) {
		while (true) {
			int b = randInt(1, 3);
			bool c = false;
			if (b == 1 && m_rs != 0) {
				getstudent()->addBacteria(1,getX(), getY());
				m_rs--;
				c = true;
			}
			if (b == 2 && m_as != 0) {
				getstudent()->addBacteria(2, getX(), getY());
				m_as--;
				c = true;
			}
			if (b == 3 && m_ec != 0) {
				getstudent()->addBacteria(3, getX(), getY());
				m_ec--;
				c = true;
			}
			if (c)
				break;
		}
	}
}
/////////////////////////////////////////
///////Bacteria implementation///////////
/////////////////////////////////////////
Bacteria::Bacteria(int id, double x, double y, StudentWorld* s) 
	:Actor(id,x,y,s,90,0){
	m_displan = 0;
	m_food = 0;
}
Bacteria::~Bacteria() {}
bool Bacteria::damageable()const {
	return true;
}
bool Bacteria::isBacteria()const {
	return true;
}
bool Bacteria::hashp()const {
	return true;
}
bool Bacteria::aggressive()const {
	return false;
}
void Bacteria::eatfood() {
	m_food++;
}
void Bacteria::resetfood() {
	m_food = 0;
}
int Bacteria::getnumoffood()const {
	return m_food;
}
int Bacteria::getmoveplan()const {
	return m_displan;
}
void Bacteria::decmoveplan() {
	m_displan--;
}
void Bacteria::resetplan() {
	m_displan = 10;
}
int Bacteria::getpixel()const {
	return 3;
}
void Bacteria::doSomething() {
	if (!alive())
		return;
	bool playerinrange = false;
	bool causedamage = false;
	bool divided = false;
	if (aggressive()) {
		if (getstudent()->inrange(this)) {
			playerinrange = true;
			getstudent()->dirtoplayer(this);
			double x, y;
			getPositionInThisDirection(getDirection(), getpixel(), x, y);
			if (!getstudent()->blockbydirt(x, y))
				moveAngle(getDirection(), getpixel());
		}
	}
	if (getstudent()->hurtSocrates(this))
		causedamage = true;
	if (!causedamage&&getnumoffood()==3) {
		int type;
		if (aggressive())
			type = 2;
		else if (isEcoli())
			type = 3;
		else
			type = 1;
		double x, y;
		if (getX() < VIEW_WIDTH / 2)
			x = getX() + SPRITE_WIDTH / 2;
		else if (getX() == VIEW_WIDTH / 2)
			x = getX();
		else
			x = getX() - SPRITE_WIDTH / 2;
		if (getY() < VIEW_HEIGHT / 2)
			y = getY() + SPRITE_WIDTH / 2;
		else if (getY() == VIEW_HEIGHT / 2)
			y = getY();
		else
			y = getY() - SPRITE_WIDTH / 2;
		getstudent()->addBacteria(type, x, y);
		resetfood();
		divided = true;
	}
	if (!causedamage && !divided) {
		getstudent()->getFood(this);
	}
	if (!playerinrange) {
		if (!isEcoli()) {
			if (getmoveplan() > 0) {
				decmoveplan();
				trymove();
			}
			else {
				if (getstudent()->dirtofood(this)) {
					trymove();
					return;
				}
				else {
					setDirection(randInt(0, 359));
					resetplan();
					return;
				}
			}
		}
		else {
			if (getstudent()->inrange2(this)) {
				getstudent()->dirtoplayer(this);
				for (int i = 0; i < 10; i++) {
					double x, y;
					getPositionInThisDirection(getDirection(), getpixel(), x, y);
					if (getstudent()->blockbydirt(x, y))
						setDirection(getDirection() + 10);
					else {
						moveAngle(getDirection(), getpixel());
						return;
					}
				}
			}
		}
	}
}
void Bacteria::trymove() {
	double x, y;
	getPositionInThisDirection(getDirection(), getpixel(), x, y);
	if (getstudent()->countDistance(x, y, VIEW_WIDTH / 2, VIEW_HEIGHT / 2) < VIEW_RADIUS && !getstudent()->blockbydirt(x, y))
		moveAngle(getDirection(), getpixel());
	else {
		setDirection(randInt(0, 359));
		resetplan();
	}
}
/////////////////////////////////////////
//////Regular Sal implementation/////////
/////////////////////////////////////////
ReSal::ReSal(double x, double y, StudentWorld* s)
:Bacteria(IID_SALMONELLA,x,y,s){
	changehp(4);
}
int ReSal::getdamage()const {
	return 1;
}
/////////////////////////////////////////
/////Aggressive Sal implementation///////
/////////////////////////////////////////
AgSal::AgSal(double x, double y, StudentWorld* s)
	: Bacteria(IID_SALMONELLA, x, y, s) {
	changehp(10);
}
int AgSal::getdamage()const {
	return 2;
}
bool AgSal::aggressive()const {
	return true;
}
/////////////////////////////////////////
/////E-coil implementation///////////////
/////////////////////////////////////////
Ecoli::Ecoli(double x, double y, StudentWorld* s)
	: Bacteria(IID_ECOLI, x, y, s) {
	changehp(5);
}
bool Ecoli::isEcoli()const {
	return true;
}
int Ecoli::getdamage()const {
	return 4;
}
int Ecoli::getpixel()const {
	return 2;
}
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
