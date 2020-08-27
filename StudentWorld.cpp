#include "StudentWorld.h"
#include "GameConstants.h"
#include"Actor.h"
#include <string>
#include<cmath>
#include <cstdlib>
#include<sstream>
#include<iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}
StudentWorld::~StudentWorld() {
	cleanUp();
}

int StudentWorld::init()
{
	m_player = new Socrates(this);
	int L = getLevel();
	m_pits = L;
	m_bacteria =0;
	for (int i = 0; i < L; i++) {
		double x, y;
		validcoord(m_actors, x, y);
		Pit* pit = new Pit(x, y, this);
		m_actors.push_back(pit);
	}
	int numofFood = min(5 * L, 25);
	int numofDirt = max(180 - 20 * L, 20);
	for (int i = 0; i < numofFood; i++) {
		double x, y;
		validcoord(m_actors, x, y);
		Food* f = new Food(x, y,this);
		m_actors.push_back(f);
	}
	for (int i = 0; i < numofDirt; i++) {
		double x, y;
		validcoord(m_actors, x, y);
		Dirt* d = new Dirt(x, y,this);
		m_actors.push_back(d);
	}
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	m_player->doSomething();
	list<Actor*>::iterator iter = m_actors.begin();
	while (iter != m_actors.end()) {
		if (!(*iter)->alive()) {
			Actor* temp = *iter;
			iter = m_actors.erase(iter);
			delete temp;
		}
		else {
			(*iter)->doSomething();
			iter++;
		}
		if (!m_player->alive()) {
			decLives();
			playSound(SOUND_PLAYER_DIE);
			return GWSTATUS_PLAYER_DIED;
		}
	}
	int L = getLevel();
	int chancegoodie = max(510 - L * 5, 250);
	int chancefungus= max(510 - L * 5, 200);
	int b = randInt(0, chancefungus);
	if (b == 0) {
		double x, y;
		randompos(x, y);
		Fungus* fungus = new Fungus(x, y, this, L);
		m_actors.push_back(fungus);
	}
	int a = randInt(0, chancegoodie);
	if (a == 0) {
		int b = randInt(1, 10);
		if (b == 1) {
			double x, y;
			randompos(x, y);
			ELife* e = new ELife(x, y, this, L);
			m_actors.push_back(e);
		}
		if (b > 1&&b<5) {
			double x, y;
			randompos(x, y);
			FThrower* f = new FThrower(x, y, this, L);
			m_actors.push_back(f);
		}
		else {
			double x, y;
			randompos(x, y);
			RHealth* r = new RHealth(x, y, this, L);
			m_actors.push_back(r);
		}
	}
	ostringstream oss;
	oss.fill('0');
	if (getScore() < 0) {
		oss << "Score: -" << setw(5) << -getScore() << "  Level: " << getLevel() << "  Lives: " << getLives() << "  Health: " << m_player->gethp() << "  Sprays: " << m_player->getspray() << "  Flames: " << m_player->getflamenum();
	}
	else {
		oss << "Score: " << setw(6) << getScore() << "  Level: " << getLevel() << "  Lives: " << getLives() << "  Health: " << m_player->gethp() << "  Sprays: " << m_player->getspray() << "  Flames: " << m_player->getflamenum();
	}
	string ass = oss.str();
	setGameStatText(ass);
	if (m_bacteria == 0&&m_pits==0)
		return GWSTATUS_FINISHED_LEVEL;
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	delete m_player;
	list<Actor*>::iterator iter = m_actors.begin();
		while (iter != m_actors.end()) {
			Actor* temp = *iter;
			iter = m_actors.erase(iter);
			delete temp;
		}
}

bool StudentWorld::getshot(Projectile* p) {
	list<Actor*>::iterator iter = m_actors.begin();
	while (iter != m_actors.end()) {
		if ((*iter)->damageable()&& (*iter)-> alive()&& overlap(p, (*iter))) {
			if ((*iter)->isBacteria()) {
				increaseScore(100);
				(*iter)->changehp(-p->getDamage());
				if (!(*iter)->alive()) {
					if ((*iter)->isEcoli())
						playSound(SOUND_ECOLI_DIE);
					else
						playSound(SOUND_SALMONELLA_DIE);
					int turntofood = randInt(0, 1);
					if (turntofood == 0) {
						Food* body = new Food((*iter)->getX(), (*iter)->getY(), this);
						m_actors.push_back(body);
					}
					m_bacteria--;
				}
				else {
					if ((*iter)->isEcoli())
						playSound(SOUND_ECOLI_HURT);
					else
						playSound(SOUND_SALMONELLA_HURT);
				}
			}	
			else
				(*iter)->setDead();
			p->setDead();
			return true;
			break;
		}
		iter++;
	}
	return false;
}
void StudentWorld::addFlame() {
	double x, y;
	int dir = m_player->getDirection();
	for (int i = 0; i < 16; i++) {
		m_player->getPositionInThisDirection(dir, 2*SPRITE_RADIUS, x, y);
		Flame* f = new Flame(x, y, dir, this);
		m_actors.push_back(f);
		dir += 22;
	}
}
void StudentWorld::addSpray() {
	double x, y;
	int dir = m_player->getDirection();
	m_player->getPositionInThisDirection(dir, 2 * SPRITE_RADIUS, x, y);
	Spray* s = new Spray(x, y, dir, this);
	m_actors.push_back(s);
}
void StudentWorld::reducepits() {
	m_pits--;
}
void StudentWorld::addBacteria(int type, double x, double y) {
	playSound(SOUND_BACTERIUM_BORN);
		switch(type) {
		case 1: {
			ReSal* resal = new ReSal(x, y, this);
			m_actors.push_front(resal);
			break;
		}
		case 2: {
			AgSal* agsal = new AgSal(x, y, this);
			m_actors.push_front(agsal);
			break;
		}
		case 3: {
			Ecoli* ecoli = new Ecoli(x, y, this);
			m_actors.push_front(ecoli);
			break;
		}
		default:
			break;
		}
		m_bacteria++;
}
bool StudentWorld::gethealth(RHealth* r) {
	if (overlap(r, m_player)) {
		increaseScore(250);
		m_player->changehp(100);
		r->setDead();
		return true;
	}
	return false;
}
bool StudentWorld::getflame(FThrower* f) {
	if (overlap(f, m_player)) {
		increaseScore(300);
		m_player->increaseflame(5);
		f->setDead();
		return true;
	}
	return false;
}
bool StudentWorld::getgoodie(Goodie* g) {
	if (overlap(g, m_player)) {
		if (!g->isFungus())
			playSound(SOUND_GOT_GOODIE);
		else
			playSound(SOUND_PLAYER_HURT);
		return true;
	}
	return false;
}
bool StudentWorld::getlife(ELife* e) {
	if (overlap(e, m_player)) {
		increaseScore(500);
		incLives();
		e->setDead();
		return true;
	}
	return false;
}
bool StudentWorld::getfungus(Fungus* fungus) {
	if (overlap(fungus, m_player)) {
		increaseScore(-50);
		m_player->changehp(-20);
		fungus->setDead();
		return true;
	}
	return false;
}
bool StudentWorld::hurtSocrates(Bacteria* b) {
	if (overlap(m_player, b)) {
		m_player->changehp(-b->getdamage());
		playSound(SOUND_PLAYER_HURT);
		return true;
	}
	return false;
}
bool StudentWorld::inrange(Bacteria* b) {
	if (countDistance(m_player, b) <= 72)
		return true;
	return false;
}
bool StudentWorld::inrange2(Bacteria* b) {
	if (countDistance(m_player, b) <= VIEW_WIDTH)
		return true;
	return false;
}
bool StudentWorld::blockbydirt(double x, double y) {
	list<Actor*>::iterator iter = m_actors.begin();
	while (iter != m_actors.end()) {
		if ((*iter)->isDirt() && moverlap(x, y, (*iter)->getX(), (*iter)->getY()))
			return true;
		iter++;
	}
	return false;
}
bool StudentWorld::getFood(Bacteria* b) {
	list<Actor*>::iterator iter = m_actors.begin();
	while (iter != m_actors.end()) {
		if ((*iter)->isFood() && overlap((*iter), b)) {
			b->eatfood();
			(*iter)->setDead();
			return true;
		}	
		iter++;
	}
	return false;
}
void StudentWorld::dirtoplayer(Bacteria* b) {
	dirtoactor(m_player, b);
}
void StudentWorld::dirtoactor(Actor* a, Bacteria* b) {
	double dx = a->getX() - b->getX();
	double dy = a->getY() - b->getY();
	const double PI = 4 * atan(1);
	if (dx != 0) {
		int angle = 360 * atan(dy / dx) / 2 / PI;
		if (dx > 0)
			b->setDirection(angle);
		else if (dx < 0)
			b->setDirection(angle + 180);
	}
	else {
		if (dy > 0)
			b->setDirection(90);
		else if (dy < 0)
			b->setDirection(270);
		else
			return;
	}
}
bool StudentWorld::dirtofood(Bacteria* b) {
	list<Actor*>::iterator iter = m_actors.begin();
	Actor* f=nullptr;
	while (iter != m_actors.end()) {
		if ((*iter)->isFood()) {
			if (f == nullptr|| countDistance(f, b) > countDistance((*iter), b))
				f = (*iter);
		}
		iter++;
	}
	if (f == nullptr)
		return false;
	else {
		dirtoactor(f, b);
		return true;
	}
}
double StudentWorld::countDistance(double x1, double y1, double x2, double y2) {
	return sqrt(pow(x1-x2,2) + pow(y1 - y2, 2));
}
double StudentWorld::countDistance(Actor* a1, Actor* a2) {
	return countDistance(a1->getX(), a1->getY(), a2->getX(), a2->getY());
}
bool StudentWorld::overlap(Actor* a1, Actor* a2) {
	double dis = countDistance(a1->getX(), a1->getY(), a2->getX(), a2->getY());
	return (dis <= 2 * SPRITE_RADIUS);
}
bool StudentWorld::overlap(double x1, double y1, double x2, double y2) {
	double dis = countDistance(x1, y1, x2, y2);
	return (dis <= 2 * SPRITE_RADIUS);
}
bool StudentWorld::moverlap(double x1, double y1, double x2, double y2) {
	double dis = countDistance(x1, y1, x2, y2);
	return (dis <= SPRITE_RADIUS);
}

void StudentWorld::validcoord(list<Actor*> a,double& x, double& y) {
	while (true) {
		bool valid = true;
		x = randInt(0, VIEW_WIDTH - 1);
		y = randInt(0, VIEW_HEIGHT - 1);
		if (countDistance(x,y,VIEW_WIDTH/2,VIEW_HEIGHT/2) > 120)
			valid = false;
		for (list<Actor*>::iterator iter = a.begin(); iter != a.end(); iter++) {
			if (((*iter)->cantoverlap()) && overlap(x, y, (*iter)->getX(), (*iter)->getY()))
				valid = false;
		}
		if (valid)
			break;
	}	
 }
void StudentWorld::randompos(double& x, double& y) {
	int angle = randInt(0, 360);
	const double PI = 4 * atan(1);
	x = (VIEW_WIDTH / 2 + VIEW_RADIUS * cos(angle * 1.0 / 360 * 2 * PI));
	y = (VIEW_HEIGHT / 2 + VIEW_RADIUS * sin(angle * 1.0 / 360 * 2 * PI));
}

