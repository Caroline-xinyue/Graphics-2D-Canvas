// shapes.h
typedef enum {POINT, LINE, TRIANGLE, RECTANGLE, CIRCLE, POLYGON} mode_type;
typedef enum {BLACK, RED, GREEN, BLUE} color_type;

#define NUM_POINTS_IN_LINE 2
#define NUM_POINTS_IN_TRIANGLE 3
#define NUM_POINTS_IN_RECTANGLE 2
#define NUM_POINTS_IN_CIRCLE 2

#define TRUE 1
#define FALSE 0

#define NUM_CIRCLE_TRIANGULATION 100
#define INITIAL_POLYGON_POINT_NUM 100

typedef struct _point {
  double x;
  double y;
  int color;
  int isFilled;
} Point;

typedef struct _shape {
  int type;
  void* shape;
  int color;
  int isFilled;
  struct _shape* prev;
  struct _shape* next;
} Shape;

typedef struct _line {
  Point* point1;
  Point* point2;
} Line;

typedef struct _triangle {
  Point* point1;
  Point* point2;
  Point* point3;
} Triangle;

typedef struct _rectangle {
  Point* pt1;
  Point* pt4;
} Rectangle;

typedef struct _circle {
  Point* center;
  Point** circle_points;
} Circle;

typedef struct _polygon {
  Point** polygon_points;
  int polygon_size;
} Polygon;

Point* make_point(double x, double y, color_type color, int isFilled);
Line* make_line(Point* p1, Point* p2);
Triangle* make_triangle(Point* p1, Point* p2, Point* p3);
Rectangle* make_rectangle(Point* pt1, Point* pt4);
Circle* make_circle(Point* center, Point** points);
Polygon* make_polygon(Point** points, int size);
Shape* make_shape(int type, void* shape, color_type color, int isFilled);
