#ifndef DCEL
#define DCEL
#include "PolygonPoint.h"
template<class A, class B> ostream& operator<<(ostream& out, const pair<A, B> &a){ return out<<"("<<a.first<<", "<<a.second<<")";}
#endif
#define ld long double
#define pu push_back

using namespace std;

typedef struct Vertex Vertex;
typedef struct HalfEdge HalfEdge;

typedef struct Face 
{
  HalfEdge *edge;  /* rep->left == this */
  int id;

  void setID(int id) {this->id = id;}
  int getID() {return id;}
} Face;

typedef struct HalfEdge 
{
  HalfEdge *prevEdge;  /* prev->next == this */
  HalfEdge *nextEdge;  /* next->prev == this */
  HalfEdge *twinEdge;  /* twin->twin == this */
  Vertex *origin;     /* twin->next->tail == tail &&
                              prev->twin->tail == tail */
  Face *face;       /* prev->left == left && next->left == left */

  void setTwinEdge(HalfEdge* edge) {twinEdge = edge;}
  
  void setFace(Face* face) {this->face = face;}
  void setPrevEdge(HalfEdge* edge) {prevEdge = edge;}
  void setNextEdge(HalfEdge* edge) {nextEdge = edge;}

  HalfEdge* getPrevEdge() {return prevEdge;}
  HalfEdge* getNextEdge() {return nextEdge;}
  HalfEdge* getTwinEdge() {return twinEdge;}
  Face* getFace() {return face;}

  void setOrigin(Vertex* vertex);
  Vertex* getOrigin();
  bool operator ==(HalfEdge* edge);
  pair<pair<ld,ld> , pair<ld,ld> >  getEdgePair();

} HalfEdge;

typedef struct Vertex {
  Point point;
  HalfEdge *halfEdge;  /* rep->tail == this */
  vector<HalfEdge*> edges;

  public:
  Vertex(Point p): point(p) {}

  void setHalfEdge(HalfEdge* edge) {halfEdge = edge;}

  HalfEdge* getHalfEdge() {return halfEdge;}
  Point getPoint() {return point;}
  pair<ld,ld> getPair() {return point.getPair();}

  void addHalfEdge(HalfEdge* edge) {edges.pu(edge);}

  bool lessPoint(const Point point1, const Point point2) const
  {
    if(point1.getX() - point.getX() >= 0 && point2.getX() - point.getX() < 0) return true;
    if(point1.getX() - point.getX() < 0 && point2.getX() - point.getX() >= 0) return false;
    if(point1.getX() - point.getX() == 0 && point2.getX() - point.getX() == 0)
    {
      if(point1.getY() - point.getY() >= 0  || point2.getY() - point.getY() >= 0) return point1.getY() > point2.getY();
      return point2.getY() > point1.getY();
    }

    ld det = (point1.getX() - point.getX()) * (point2.getY() - point.getY()) - (point2.getX() - point.getX()) * (point1.getY() - point.getY());
    if(det < 0) return true;
    if(det > 0) return false;

    ld d1 = (point1.getX() - point.getX()) * (point1.getX() - point.getX()) + (point1.getY() - point.getY()) * (point1.getY() - point.getY());
    ld d2 = (point2.getX() - point.getX()) * (point2.getX() - point.getX()) + (point2.getY() - point.getY()) * (point2.getY() - point.getY());
    return d1 > d2;
  }



  void sortEdges()
  {
    sort(edges.begin(), edges.end(), [this](HalfEdge* edge1, HalfEdge* edge2){
      bool ans =  this->lessPoint(edge1->getTwinEdge()->getOrigin()->getPoint(),  edge2->getTwinEdge()->getOrigin()->getPoint());
      // cerr << point.getPair() << " " << edge1->getOrigin()->getPoint().getPair() << " " << edge2->getOrigin()->getPoint().getPair() << " " << ans << endl;
      return ans;
    });
  }

  void printEdges()
  {
    cerr << "Original Point: " << point.getPair() << " " << edges.size() << endl;
    for(auto edgePointer : edges)
    {
      cerr << edgePointer->getTwinEdge()->getOrigin()->getPoint().getPair() << endl;
    }
  }

  void assignEdges()
  {
    if(edges.size() == 1) 
    {
      edges[0]->getTwinEdge()->setNextEdge(edges[0]);
      edges[0]->setPrevEdge(edges[0]->getTwinEdge());
    }
    else
    {
      int edgeSize = edges.size();
      for(int i=0;i<edges.size();i++)
      {
        edges[(i-1+edgeSize)%edgeSize]->getTwinEdge()->setNextEdge(edges[i]);
        edges[i]->setPrevEdge(edges[(i-1+edgeSize)%edgeSize]->getTwinEdge());
      }
    }

    setHalfEdge(edges[0]);
  }

  bool operator ==(Vertex* vertex)
  {
    return getPair() == vertex->getPair();
  }


} Vertex;

void HalfEdge::setOrigin(Vertex* vertex) {origin = vertex;}

Vertex* HalfEdge::getOrigin() {return origin;}

bool HalfEdge::operator ==(HalfEdge* edge)
{
  return (edge->origin == origin && edge->twinEdge->origin == twinEdge->origin);
}

pair<pair<ld,ld> , pair<ld,ld> >  HalfEdge::getEdgePair()
{
  return mp(getOrigin()->getPair(), getTwinEdge()->getOrigin()->getPair());
}

void convertShapeToPolygon(vector<vector<Point> > &shape, vector<vector<PolygonPoint*> > &polygons)
{
  for(auto points : shape)
  {
    vector<PolygonPoint*> polygon;
    for(auto point : points)
    {
      PolygonPoint* pointer = new PolygonPoint(point);
      polygon.push_back(pointer);
    }

    int numPoints = polygon.size();
    for(int i=0;i<numPoints;i++)
    {
      polygon[i]->setNextPointer(polygon[(i+1) % numPoints]);
      polygon[i]->setPrevPointer(polygon[(i-1+numPoints)%numPoints]);
    }
    polygons.pu(polygon);
  }
}

void assignLinesToPolygons(vector<Line> &lines, vector<vector<PolygonPoint*> > &polygons)
{
  map<pair<ld,ld>, Vertex* > vertexMap;
  vector<vector<Point> > shape;

  for(auto line : lines)
  {
    auto pairU = line.getU().getPair();
    auto pairV = line.getV().getPair();

    if(!vertexMap.count(pairU)) 
    {
      Point tempPoint = line.getU();
      vertexMap[pairU] = new Vertex(tempPoint);
    }
    if(!vertexMap.count(pairV))
    {
      Point tempPoint = line.getV();
      vertexMap[pairV] = new Vertex(tempPoint);
    }

    HalfEdge* edgeU = new HalfEdge();
    HalfEdge* edgeV = new HalfEdge();

    edgeU->setOrigin(vertexMap[pairU]);
    edgeV->setOrigin(vertexMap[pairV]);
    edgeU->setTwinEdge(edgeV);
    edgeV->setTwinEdge(edgeU);

    vertexMap[pairU]->addHalfEdge(edgeU);
    vertexMap[pairV]->addHalfEdge(edgeV);
  }

  for(auto it : vertexMap) (it.second)->sortEdges();
  for(auto it : vertexMap) (it.second)->assignEdges();
  

  set<pair<pair<ld,ld> , pair<ld,ld> > > visitedEdges;
  int faceID = 0;
  for(auto it : vertexMap)
  {
    for(auto edge : (it.second)->edges)
    {
      // cerr << "Original Edge: " << edge->getEdgePair() << endl;
      HalfEdge* tempEdge = edge;
      vector<Point> points;
      while(!visitedEdges.count(edge->getEdgePair()))
      {
        // cerr << "Edge: " << edge->getEdgePair() << endl;
        visitedEdges.insert(edge->getEdgePair());
        points.pu(edge->getOrigin()->point);
        edge = edge->getNextEdge();
      }
      if(points.size())
      {
        cerr << "Original Edge: " << edge->getEdgePair() << endl;
        cerr << endl;
        for(auto it : points) cerr << it.getPair() << " ";
        cerr << endl;
        cerr << endl;
        shape.pu(points);
      }
    }
  }

  convertShapeToPolygon(shape,polygons);
} 