#ifndef EDGE
#define EDGE
#include "PolygonPoint.h"
#endif

typedef struct Edge
{
  PolygonPoint* u,*v;
  Edge *prevEdgePointer, *nextEdgePointer;

  Edge() {}
  Edge(PolygonPoint* u, PolygonPoint*v): u(u), v(v) {}

  Edge* getNextEdgePointer() {return nextEdgePointer;}
  Edge* getPrevEdgePointer() {return prevEdgePointer;}

  void setNextEdgePointer(Edge* edge)
  {
    nextEdgePointer = edge;
  }

  void setPrevEdgePointer(Edge* edge)
  {
    prevEdgePointer = edge;
  }

} Edge;