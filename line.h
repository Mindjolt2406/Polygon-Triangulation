#ifndef LINE
#define LINE
#include "point.h"
#endif

#define ld long double
#define mp std::make_pair
#define INF 1e9
#define EPS 1e-6

extern ld sweepY;

typedef struct Line
{
  Point u,v;
  ld slope;

  Line() {}
  Line(Point u, Point v): u(u), v(v) 
  {
    computeSlope();
  }
  Line( ld px, ld py, ld qx, ld qy): u(px,py), v(qx,qy) 
  {
    computeSlope();
  }
  Line(const Line &line): u(line.u), v(line.v), slope(line.slope) {}

  void computeSlope()
  {
    if(v.getX() == u.getX()) slope = INF;
    else slope = (v.getY() - u.getY())/((ld)(v.getX() - u.getX()));
  }

  bool isZeroSlope()
  {
    return abs(slope) < EPS;
  }

  void orderLine()
  {
    if(v.getY() > u.getY())
    {
      std::swap(u,v);
    }
    else if(v.getY() == u.getY() && v.getX() > u.getX()) std::swap(u,v);
  }

  Point getU() {return u;}
  Point getV() {return v;}

  // bool isStartPoint(Point p) 
  // {

  // }
  // Problem
  bool on_left(Point p) 
  {
    return p.cross(u,v) > 0;
  }

  bool on_right(Point p) 
  {
    return p.cross(u,v) < 0;
  }

  int getMaxY()
  {
    return (u.getY() > v.getY()) ? u.getY() : v.getY();
  }

  int getMinY()
  {
    return (u.getY() < v.getY()) ? u.getY() : v.getY();
  }

  ld evaluateX(ld y) const
  {
    return ((y - u.getY())/slope) + u.getX();
  }

  ld evaluateY(ld x) const
  {
    return u.getY() + slope * (x - u.getX());
  }

  bool operator<(const Line& line) const
  {
    return (this->evaluateX(sweepY)) < line.evaluateX(sweepY);
  }

  bool operator ==(const Line& line) const
  {
    return (this->u == line.u) && (this->v == line.v);
  }

  std::pair< std::pair<ld,ld>, std::pair<ld,ld> > getPair()
  {
    return mp(mp(u.getX(), u.getY()), mp(v.getX(), v.getY()));
  }

  std::string to_string()
  {
    std::string temp;
    temp += "u: " + u.to_string() + " v: " + v.to_string(); 
    return temp;
  }
} Line;
