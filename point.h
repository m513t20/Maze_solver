#ifndef _GEOMETRY_H
#define _GEOMETRY_H

#include<iostream>;
#include <cmath>;


using namespace std;

class Point {
	//данные
	

	public: 
		int x, y;
		int fx, fy;

		template <class value_class> Point(value_class ax, value_class ay);
		Point();
		Point(const Point& p);

		Point get_father();

		//функции

		const double calc_distance(const Point& p);

		Point* neighboors();

		friend istream& operator >>(istream& is,Point a);
		friend ostream& operator <<(ostream& os,Point a);
};

template<class value_class>
inline Point::Point(value_class ax, value_class ay)
{
	x = (int)(ax);
	y = (int)(ay);

}




#endif // !_GEOMETRY_H

