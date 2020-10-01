#include<bits/stdc++.h>
#include "PolygonPoint.h"
// g++ -std=c++17 -Wl,-stack_size -Wl,0x10000000 main.cpp
#define mt make_tuple
#define pu push_back
#define MOD 1000000007
#define EPS 1e-6
#define ll long long int
#define ld long double
#define fi first
#define se second
#define all(v) v.begin(),v.end()
#define pr(v) { for(int i=0;i<v.size();i++) { v[i]==INF? cout<<"INF " : cout<<v[i]<<" "; } cout<<endl;}
#define __ {ios::sync_with_stdio(false);cin.tie(NULL);cout.tie(NULL);}

using namespace std;
template<class A, class B> ostream& operator<<(ostream& out, const pair<A, B> &a){ return out<<"("<<a.first<<", "<<a.second<<")";}
template <int> ostream& operator<<(ostream& os, const vector<int>& v) { os << "["; for (int i = 0; i < v.size(); ++i) { if(v[i]!=INF) os << v[i]; else os << "INF";if (i != v.size() - 1) os << ", "; } os << "]\n"; return os; } 
template <typename T> ostream& operator<<(ostream& os, const vector<int>& v) { os << "["; for (int i = 0; i < v.size(); ++i) { os << v[i]; ;if (i != v.size() - 1) os << ", "; } os << "]\n"; return os; } 


ld sweepY = 0;
int pointID = 0;
vector<Line> diagonals;
vector<pair<PolygonPoint*, PolygonPoint*> > diagonalPoints;
vector<PolygonPoint*> leftCurrPoint, rightCurrPoint;

void drawDiagonal(PolygonPoint *point1, PolygonPoint *point2)
{
  // diagonals.pu(Line(point1.getPoint(), point2.getPoint()));
  diagonalPoints.pu(mp(point1, point2));
}

void addDiagonal(PolygonPoint *point1, PolygonPoint *point2, vector<PolygonPoint*> &allPoints)
{

  PolygonPoint *copy1 = new PolygonPoint(*point1);
  PolygonPoint *copy2 = new PolygonPoint(*point2);

  copy1->setPrevPointer(copy2);
  copy1->setNextPointer(point1->getNextPointer());

  copy2->setNextPointer(copy1);
  copy2->setPrevPointer(point2->getPrevPointer());

  point1->setNextPointer(point2);
  point2->setPrevPointer(point1);

  copy1->getNextPointer()->setPrevPointer(copy1);
  copy2->getPrevPointer()->setNextPointer(copy2);

  copy1->setID(pointID++);
  copy2->setID(pointID++);
  allPoints.pu(copy1);
  allPoints.pu(copy2);
}

struct pointComp
{
  bool operator()(const PolygonPoint * point1, const PolygonPoint* point2)
  {
    return *point1 < *point2;
  }
};

void printInverseHelper(map<PolygonPoint, Line> &inverseHelper)
{
  // Prints inverseHelper
  for(auto it : inverseHelper)
  {
    PolygonPoint point = it.first;
    Line line = it.second;
    cerr << point.getPoint().to_string() << " " << line.to_string() << endl;
  }
}
int main()
{
  __;
  int numPoints;
  cin >> numPoints;
  vector<PolygonPoint*> points, allPoints;

  map<int, int> freqY;

  for(int i=0;i<numPoints;i++)
  {
    ld x,y;
    cin >> x >> y;

    // freqY[y]++;
    // if(freqY[y] > 1) y += EPS*(freqY[y]-1);

    PolygonPoint *p = new PolygonPoint(x,y,pointID++);
    points.push_back(p);
  }

  for(int i=0;i<numPoints;i++)
  {
    points[i]->setNextPointer(points[(i+1) % numPoints]);
    points[i]->setPrevPointer(points[(i-1+numPoints)%numPoints]);
  }

  for(int i=0;i<numPoints;i++)
  {
    points[i]->setType();
    allPoints.pu(points[i]);
    // PolygonPoint point = points[i];
    // cout << point.to_string() << endl;
  }

  // cout << endl << endl;

  sort(points.begin(),points.end(),[](PolygonPoint* a, PolygonPoint* b) { return *a < *b; });
  reverse(points.begin(), points.end());

  set<Line> setLines;
  map<PolygonPoint, Line> inverseHelper;

  for(auto pointer : points)
  {
    PolygonPoint point = *pointer;
    // Remove lines where this point is the lower point
    // Now, if there is a line which goes down
    // Start and end add lines before and then do
    // Merge and Split, check before
    // polygon on the left of regular, then add after. Otherwise add before. 

    // Updating the sweep line 
    sweepY = point.getY();

    // cout << "Processing point: " << point.to_string() << endl;

    if(point.getStatus() == "Start  ")
    {
      Line tempLine = point.getNextLine();
      setLines.insert(tempLine);
      inverseHelper[point] = point.getNextLine();
      point.setLeftEdge(point.getNextLine());
    }
    else if(point.getStatus() == "End    ")
    {
      // t(point.to_string(), (point.getPrevPoint()).to_string());

      setLines.erase(point.getPrevLine());
      // No point, but for sake of completion
      inverseHelper[point] = point.getPrevLine();
      point.setLeftEdge(point.getPrevLine());
    }
    else if(point.getStatus() == "Split  ")
    {
      point.updateLeftEdge(setLines, inverseHelper);
      setLines.insert(point.getNextLine());
    }
    else if(point.getStatus() == "Merge  ")
    {
      setLines.erase(point.getPrevLine());
      point.updateLeftEdge(setLines, inverseHelper);
    }
    else
    {
      if(point.isRegularFacingLeft()) 
      {
        point.updateLeftEdge(setLines, inverseHelper);
      }
      else
      {
        setLines.erase(point.getPrevLine());
        inverseHelper[point] = point.getNextLine();
        point.setLeftEdge(point.getNextLine());
        setLines.insert(point.getNextLine());
      }
    }
  }

  // printInverseHelper(inverseHelper);

  map<pair<pair<ld,ld>, pair<ld,ld> > , PolygonPoint*> helper;
  for(auto pointer : points)
  {
    PolygonPoint point = *pointer;
    // cerr << "Processing point: " << point.to_string() << endl;
    // Updating the sweep line
    sweepY = point.getY();

    if(point.isStartPoint())
    {
      Line leftLine = point.getNextLine();
      setLines.insert(leftLine);
      helper[leftLine.getPair()] = pointer;
    }
    else if(point.isEndPoint())
    {
      Line prevLine = point.getPrevLine();
      if(helper[prevLine.getPair()]->isMergePoint())
      {
        // Draw Diagonal from point to helper[prevLine.getPair()]
        drawDiagonal(helper[prevLine.getPair()], pointer);
      }
      setLines.erase(point.getPrevLine());
    }
    else if(point.isSplitPoint())
    {
      Line leftLine = inverseHelper[point];
      // Draw Diagonal from point to helper[leftLine.getPair()]
      drawDiagonal(helper[leftLine.getPair()], pointer);
      helper[leftLine.getPair()] = pointer;
      Line nextLine = point.getNextLine();
      setLines.insert(nextLine);
      helper[nextLine.getPair()] = pointer;
    }
    else if(point.isMergePoint())
    {
      Line prevLine = point.getPrevLine();
      if(helper[prevLine.getPair()]->isMergePoint())
      {
        // Draw Diagonal from point to helper[prevLine.getPair()]
        drawDiagonal(helper[prevLine.getPair()], pointer);
      }
      setLines.erase(prevLine);
      Line leftLine = inverseHelper[point];
      if(helper[leftLine.getPair()]->isMergePoint())
      {
        // Draw a diagonal from pont to helper[leftLine.getPair()]
        drawDiagonal(helper[leftLine.getPair()], pointer);
      }
      helper[leftLine.getPair()] = pointer;
    }
    else
    {
      if(point.isRegularFacingLeft())
      {
        Line leftLine = point.getLeftEdge();
        if(helper[leftLine.getPair()]->isMergePoint())
        {
          // Draw a diagonal between helper[leftLine.getPair()] and point
          drawDiagonal(helper[leftLine.getPair()], pointer);
        }
        helper[leftLine.getPair()] = pointer;
      }
      else
      {
        Line prevLine = point.getPrevLine();
        if(helper[prevLine.getPair()]->isMergePoint())
        {
          // Draw a diagonal between helper[prevLine] and point
          drawDiagonal(helper[prevLine.getPair()], pointer);
        }
        setLines.erase(prevLine);
        Line nextLine = point.getNextLine();
        helper[nextLine.getPair()] = pointer;
        setLines.insert(nextLine);
      }
    }

    // Debugging the helper function
    // cout << "SweepY: " << sweepY << " setLines.size(): " << setLines.size() << endl;
    // for(auto it : helper)
    // {
    //   cout << it.fi << " " << it.second.getPair() << endl;
    // }
  }

  // for(auto line : diagonals)
  // {
  //   cout << line.to_string() << endl;
  // }

  // Print helper lines
  // for(auto it : helper)
  // {
  //   pair<pair<ld,ld> , pair<ld,ld> >  line = it.first;
  //   PolygonPoint point = it.second;
  //   t(line, point.getPair());
  // }

  vector<Line> sides;
  for(int i=0;i<numPoints;i++) sides.push_back(points[i]->getNextLine());

  cout << diagonalPoints.size() + numPoints << endl;
  for(auto line : sides)
  {
    auto pair = line.getPair();
    cout << pair.fi.fi << " " << pair.fi.se << " " << pair.se.fi << " " << pair.se.se << endl;
  }

  for(auto line : diagonalPoints)
  {
    auto pair1 = (line.fi)->getPair(), pair2 = (line.se)->getPair();
    cout << pair1.fi << " " << pair1.se << " " << pair2.fi << " " << pair2.se << endl;
  }


  t(allPoints.size());
  for(auto pair : diagonalPoints)
  {
    PolygonPoint *point1 = pair.fi, *point2 = pair.se;
    addDiagonal(point1, point2, allPoints);
  }

  t(allPoints.size());

  for(auto pointer : allPoints)
  {
    cerr << pointer->getPair() << endl;
  }

  // for(auto pointer : allPoints) t(pointer->getID());
  // _;
  // After monotone partiotining
  vector<Line> finalShape;
  vector<bool> visited(allPoints.size());
  for(int i=0;i<allPoints.size();i++)
  {
    if(!visited[i])
    {
      PolygonPoint* origPointer = allPoints[i];
      PolygonPoint* pointer = origPointer;
      t(i, pointer->getPair());
      while(!visited[pointer->getID()])
      {
        finalShape.push_back(pointer->getNextLine());
        visited[pointer->getID()] = true;
        pointer = pointer->getNextPointer();
        if(pointer->getNextPointer() == NULL) t("true");
        t(pointer->getPair());
      }
    }
  }

  // cout << endl;
  // cout << "Final monotone partitioned polygon" << endl;
  // for(auto line : finalShape)
  // {
  //   cout << line.to_string() << endl;
  // }

  return 0;
}