#include <cmath>;
#include <iostream>;
#include "point.h"


using namespace std;



Point::Point()
{
}

Point::Point(const Point& p)
{
	x = p.x;
	y = p.y;
	fx = p.fx;
	fy = p.fy;
}

Point Point::get_father()
{
	return Point(fx,fy);
}

//Point
const double 
Point::calc_distance(const Point& p) {
	double dx = x - p.x;
	double dy = y - p.y;
	return sqrt(dx * dx + dy * dy);

}

Point* Point::neighboors()
{
	Point* nbs = new Point[4];
	nbs[0] = Point(x + 1, y);
	nbs[0].fx = x;
	nbs[0].fy = y;
	nbs[1] = Point(x, y + 1);
	nbs[1].fx = x;
	nbs[1].fy = y;
	nbs[2] = Point(x - 1, y);
	nbs[2].fx = x;
	nbs[2].fy = y;
	nbs[3] = Point(x, y - 1);
	nbs[3].fx = x;
	nbs[3].fy = y;

	return nbs;
}



istream& operator>>(istream& is, Point a)
{
	return is >> a.x >> a.y;
}

ostream& operator<<(ostream& os, Point a)
{
	return os << "(" << a.x << ", " << a.y << ")";
}
