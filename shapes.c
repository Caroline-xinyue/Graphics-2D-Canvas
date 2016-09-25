// shapes.c
#include "shapes.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

Point* make_point(double x, double y, color_type color, int isFilled) {
  Point* new;
  if ( (new = (Point*) malloc( sizeof(Point) ) )!= NULL) {
    new -> x = x;
    new -> y = y;
    new -> color = color;
    new -> isFilled = isFilled;
  }
  else {
    printf("Out of memory!\n");
    exit(0);
  }
  return new;
}

Line* make_line(Point* p1, Point* p2) {
  Line* new;
  if ( (new = (Line*) malloc( sizeof(Line) ) )!= NULL) {
    new -> point1 = p1;
    new -> point2 = p2;
  }
  else {
    printf("Out of memory!\n");
    exit(0);
  }
  return new;
}

Triangle* make_triangle(Point* p1, Point* p2, Point* p3) {
  Triangle* new;
  if ( (new = (Triangle *) malloc( sizeof(Triangle) ) )!= NULL) {
    new -> point1 = p1;
    new -> point2 = p2;
    new -> point3 = p3;
  }
  else {
    printf("Out of memory!\n");
    exit(0);
  }
  return new;
}

Rectangle* make_rectangle(Point* pt1, Point* pt4) {
  Rectangle* new;
  if ( (new = (Rectangle *) malloc( sizeof(Rectangle) ) )!= NULL) {
    new -> pt1 = pt1;
    new -> pt4 = pt4;
  }
  else {
    printf("Out of memory!\n");
    exit(0);
  }
  return new;
}

Circle* make_circle(Point* center, Point** points) {
  Circle* new;
  if ( (new = (Circle *) malloc( sizeof(Circle) ) )!= NULL) {
    new -> center = center;
    new -> circle_points = points;
  }
  else {
    printf("Out of memory!\n");
    exit(0);
  }
  return new;
}

Polygon* make_polygon(Point** points, int size) {
  Polygon* new;
  if ( (new = (Polygon *) malloc( sizeof(Polygon) ) )!= NULL) {
    new -> polygon_points = points;
    new -> polygon_size = size;
  }
  else {
    printf("Out of memory!\n");
    exit(0);
  }
  return new;
}

Shape* make_shape(int type, void* shape, color_type color, int isFilled) {
  Shape* new;
  if ( (new = (Shape *) malloc( sizeof(Shape) ) )!=NULL) {
    new -> type = type;
    new -> shape = shape;
    new -> color = color;
    new -> isFilled = isFilled;
    new -> prev = NULL;
    new -> next = NULL;
  }
  else {
    printf("Out of memory!\n");
    exit(0);
  }
  return new;
}
