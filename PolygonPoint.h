#ifndef POLYGONPOINT
#define POLYGONPOINT
#include "line.h"
#endif
using namespace std;

typedef struct PolygonPoint
{
  int id = -1;
  ld x,y;
  // Line nextLine;
  // Line prevLine;
  PolygonPoint *nextPoint, *prevPoint;
  Point point;
  bool isMerge = false, isSplit = false, isEnd = false, isStart = false, isRegular = false;
  Line leftEdge;

  PolygonPoint()
  {
    Point p(0,0);
    x = 0, y = 0, id = 0;
    point = p;
  }

  PolygonPoint(ld x, ld y, int id) : x(x), y(y), point(x,y), id(id){}
  PolygonPoint(const PolygonPoint &p): nextPoint(p.nextPoint), prevPoint(p.prevPoint), isMerge(p.isMerge), isSplit(p.isSplit), isEnd(p.isEnd), isStart(p.isStart), isRegular(p.isRegular), leftEdge(p.leftEdge), x(p.x), y(p.y), point(p.x,p.y), id(p.id){}

  // void equate(PolygonPoint &point)
  // {
  //   x = point.getX(), y = point.getY(), nextPoint = point.getNextPointer(), prevPoint = point.getPrevPointer(), isMerge = point.isMerge, isStart = point.isStart, isRegular = point.isRegular, 
  // }


  PolygonPoint* getPrevPointer() {return prevPoint;}
  PolygonPoint* getNextPointer() {return nextPoint;}
  PolygonPoint getPrevPoint() {return *prevPoint;}
  PolygonPoint getNextPoint() {return *nextPoint;}

  void setNextPointer(PolygonPoint* pointer) {this->nextPoint = pointer;}
  void setPrevPointer(PolygonPoint* pointer) {this->prevPoint = pointer;}

  bool isStartPoint() {return isStart;}
  bool isEndPoint() {return isEnd;}
  bool isRegularPoint() {return isRegular;}
  bool isSplitPoint() {return isSplit;}
  bool isMergePoint() {return isMerge;}

  ld getX()
  {
    return x;
  }

  ld getY()
  {
    return y;
  }

  Point getPoint() const {return point;}

  Line getPrevLine()
  {
    return Line(getPrevPoint().getPoint(), this->getPoint());
  }

  Line getNextLine()
  {
    return Line(this->getPoint(), getNextPoint().getPoint());
  }

  void setID(int id) {this->id = id;}
  int getID() {return id;}

  void setType()
  {
    // t(x,y,nextPoint->getY(), prevPoint->getY());
    if(nextPoint->getY() <= y && prevPoint->getY() <= y) // Start vertex or a split vertex
    {
      Line l(prevPoint->getPoint(),this->getPoint());
      if(l.on_right(nextPoint->getPoint())) // reflex
      {
        isSplit = true;
      }
      else isStart = true;
    }
    else if(nextPoint->getY() >= y && prevPoint->getY() >= y) // End or merge vertex
    {
        Line l(prevPoint->getPoint(),this->getPoint());
        if(l.on_right(nextPoint->getPoint())) // reflex
        {
          isMerge = true;
        }
        else isEnd = true;
    }
    else isRegular = true;
  }

  // If true, the interior of the polygon lies towards the left of this point. 
  bool isRegularFacingLeft()
  {
    if(isRegular)
    {
      return prevPoint->getY() < nextPoint->getY();
    }
    else return false;
  }

  void setLeftEdge(Line line)
  {
    leftEdge = line;
  }

  Line getLeftEdge()
  {
    return leftEdge;
  }

  void updateLeftEdge(set<Line> &setLines, map<PolygonPoint, Line> &inverseHelper)
  {
    Line queryLine(getPoint(), getPoint());
    auto it = setLines.upper_bound(queryLine);
    if(it == setLines.begin()) 
    {
      cerr << "Error in computing the left vertex of point " << to_string() << endl;
      cerr << "------------------------" << endl;
      cerr << "Status of the setLines: " << endl;
      for(auto it : setLines) cerr << it.to_string() << endl;
      cerr << "------------------------" << endl;
    }
    else
    {
      it--;
      leftEdge = *it;
      inverseHelper[*this] = *it;
    }
  }

  bool operator < (const PolygonPoint p) const
  {
    return point < p.getPoint();
  }

  bool operator !=(const PolygonPoint p) const
  {
    return !(x == p.x && y == p.y);
  }

  std::pair<ld,ld> getPair()
  {
    return point.getPair();
  }

  std::string getStatus()
  {
    if(isRegular) return "Regular";
    else if(isSplit) return "Split  ";
    else if(isMerge) return "Merge  ";
    else if(isStart) return "Start  ";
    else return "End    ";
  }

  std::string to_string()
  {
    std::string s;
    s += point.to_string() + " Next: " + nextPoint->getPoint().to_string() + " Prev: " + prevPoint->getPoint().to_string() + " Status: " + getStatus() + " Id: " + std::to_string(getID()) + ((std::to_string(getID()).size()-1) ? "" : " ");
    return s;
  }

} PolygonPoint;
