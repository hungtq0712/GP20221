#ifndef COMPARISONTIMESPACE_H
#define COMPARISONTIMESPACE_H
#include <iostream>
using namespace std;
typedef struct Objective{
	string name;
	double earliness;
	double tardiness;
	double alpha;
	double beta;
	double gamma;
} Objective;
class ComparisonTimeSpace{
    public:
        ComparisonTimeSpace(int x, int y, double c, Objective *o, double dt){}
        double getObjectiveValue(){}
        int getFirstIndex(){}
    int getSecondIndex(){}
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


#endif // COMPARISONTIMESPACE_H
