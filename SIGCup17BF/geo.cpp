#include "geo.h"
namespace geo{
	Point proj(Point p1, Point p2, Point q){
		return p1 + ((p2 - p1) * ((p2 - p1) % (q - p1) / (p2 - p1).len2()));
	}
}