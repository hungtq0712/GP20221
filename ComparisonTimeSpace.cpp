#include "ComparisonTimeSpace.h"

class ComparisonTimeSpace{
public:
	ComparisonTimeSpace(int x, int y, double c, Objective *o, double dt)
	{
		this->x = x; this->y = y;
		this->dt = dt;
		this->e = o->earliness; this->t = o->tardiness; this->c = c;
		this->a = o->alpha; this->b = o->beta; this->g = o->gamma;
	}
	double getObjectiveValue(){
		double earlinessValue = max(0, this->e - this->x*this->dt);
		double tardinessValue  = max(0, this->x*this->dt - this->t);
		return this->a*earlinessValue  + this->b*tardinessValue + this->g*this->c;
	}
	int getFirstIndex(){
		return x;
	}
int getSecondIndex(){
		return y;
	}
private:
	int x, y;
	double e, t, c;
	double dt;
	double a, b, g;
};

struct cmp{
        bool operator() (ComparisonTimeSpace *a,ComparisonTimeSpace *b) {
        	double left = a->getObjectiveValue( );
	double right = b->getObjectiveValue( );
	return left < right;
    }

};
