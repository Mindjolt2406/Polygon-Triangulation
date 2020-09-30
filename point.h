#define ld long double
#define t1(x)                cerr<<#x<<" : "<<x<<endl
#define t2(x, y)             cerr<<#x<<" : "<<x<<" "<<#y<<" : "<<y<<endl
#define t3(x, y, z)          cerr<<#x<<" : " <<x<<" "<<#y<<" : "<<y<<" "<<#z<<" : "<<z<<endl
#define t4(a,b,c,d)          cerr<<#a<<" : "<<a<<" "<<#b<<" : "<<b<<" "<<#c<<" : "<<c<<" "<<#d<<" : "<<d<<endl
#define t5(a,b,c,d,e)          cerr<<#a<<" : "<<a<<" "<<#b<<" : "<<b<<" "<<#c<<" : "<<c<<" "<<#d<<" : "<<d<<" "<<#e<<" : "<<e<<endl
#define t6(a,b,c,d,e,f)          cerr<<#a<<" : "<<a<<" "<<#b<<" : "<<b<<" "<<#c<<" : "<<c<<" "<<#d<<" : "<<d<<" "<<#e<<" : "<<e<<" "<<#f<<" : "<<f<<endl
#define GET_MACRO(_1,_2,_3,_4,_5,_6,NAME,...) NAME
#define t(...) GET_MACRO(__VA_ARGS__,t6,t5, t4, t3, t2, t1)(__VA_ARGS__)
#define _ cerr<<"here"<<endl;

typedef struct Point
{
  ld x,y;

  Point(): x(0), y(0) {}
  Point(ld x, ld y): x(x), y(y) {}
  Point(const Point &p): x(p.getX()), y(p.getY()) {}

  ld length() const
  {
    return sqrtl(x*x +  y*y);
  }

  ld length2() const
  {
    return x*x + y*y;
  }

  ld getX() const {return x;}
  ld getY() const {return y;}

  ld cross(Point lhs, Point rhs) 
  {
    return (lhs-*this)*(rhs-*this);
  }

  Point operator + (Point rhs)
  {
    return Point(x+rhs.x, y+rhs.y);
  }

  Point operator - (Point rhs)
  {
    return Point(x-rhs.x, y-rhs.y);
  }

  ld operator * (Point rhs)
  {
    return x*rhs.y - y*rhs.x;
  }

  bool operator == (Point p)
  {
    return (p.getX() == x && p.getY() == y);
  }

  bool operator == (const Point &p) const
  {
    return (p.getX() == x && p.getY() == y);
  }

  bool operator < (const Point p) const
  {
    if(y < p.getY()) return true;
    if(p.getY() == y) return x < p.getX();
    return false;
  }

  std::pair<ld,ld> getPair()
  {
    return std::make_pair(x,y);
  }

  std::string to_string()
  {
    return "( " + std::to_string(x) + ", " + std::to_string(y) + " )";
  }

} Point;