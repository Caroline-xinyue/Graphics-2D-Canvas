// hw1_arrRevisedVersion.c 
/*************************************************************************
This program simulates a 2D canvas and draws various 2D shapes, including
Lines, Triangles, Rectangles, Circles and all types of Polygons.
*************************************************************************/
#include "shapes.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#ifdef __APPLE__
#include <GLFW/glfw3.h>
#else
#include <GLFW/glfw3.h>
#endif
#include <GL/glu.h>

Point **circle_points; // stores all the points on a circle in the Circle struct
Point **polygon_points; // stores all the points for a polygon in the Polygon struct
Point **array_polygonMode; // stores all the points for a polygon to draw the temporary mode
Point *array_drawMode[3]; // stores all the points for the shapes(except polygons) to draw the temporary mode
Shape *head_displayMode = NULL;
Shape *tail_displayMode = NULL;
Shape *prev_displayMode = NULL;

int num_polygon_points = INITIAL_POLYGON_POINT_NUM; // stores the number of points user clicked to draw a polygon
int size_polygonMode = 0;
int size_drawMode = 0;
int size_displayMode = 0;

mode_type mode = LINE;
color_type color = BLACK;
int mousePressed = FALSE;
int keyPressed = FALSE;
int isFilled = TRUE;
int isPolygonMode = FALSE;
int isPolygonFinished = FALSE;

void resize(int, int);
void clear_drawMode();
void clear_polygonMode();
void clear_displayMode();
void store_points_on_circle();
void store_points_on_polygon();

void init() {
  // Set the clear color to white
  glClearColor(1.0, 1.0, 1.0, 1.0);
  resize(1000, 500);
  for(int i = 0; i < 3; i++) {
    array_drawMode[i] = NULL;
  }
  array_polygonMode = malloc(sizeof(Point*) * INITIAL_POLYGON_POINT_NUM);
  for(int i = 0; i < INITIAL_POLYGON_POINT_NUM; i++) {
    array_polygonMode[i] = NULL;
  }
}

void reshape(GLFWwindow *w, int width, int height) {
  resize(width, height);
}

void resize(int width, int height) {
  // Switch to projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // Viewing area set to window size
  gluOrtho2D(0.0, width, height, 0.0);
  // Return to modelview mode
  glMatrixMode(GL_MODELVIEW);
}

void framebuffer_size_callback(GLFWwindow *w, int width, int height) {
  // Set the new viewport size
  glViewport(0, 0, width, height);
}

void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods) {
  switch(action) {
  case GLFW_PRESS :
    keyPressed = TRUE;
    break;
  case GLFW_RELEASE :
    keyPressed = FALSE;
    break;
  case GLFW_REPEAT :
    break;
  default :
    break;
  }

  if(keyPressed) {
    switch(key) {
    case GLFW_KEY_F1 :
      mode = LINE;
      isPolygonMode = FALSE;
      clear_drawMode();
      clear_polygonMode();
      printf("Mode changed. Please start over.\n");
      printf("Now drawing in line mode.\n");
      break;
    case GLFW_KEY_F2 :
      mode = TRIANGLE;
      isPolygonMode = FALSE;
      clear_drawMode();
      clear_polygonMode();
      printf("Mode changed. Please start over.\n");
      printf("Now drawing in triangle mode.\n");
      break;
    case GLFW_KEY_F3 :
      mode = RECTANGLE;
      isPolygonMode = FALSE;
      clear_drawMode();
      clear_polygonMode();
      printf("Mode changed. Please start over.\n");
      printf("Now drawing in rectangle mode.\n");
      break;
    case GLFW_KEY_F4 :
      mode = CIRCLE;
      isPolygonMode = FALSE;
      clear_drawMode();
      clear_polygonMode();
      printf("Mode changed. Please start over.\n");
      printf("Now drawing in circle mode.\n");
      break;
    case GLFW_KEY_F5 :
      mode = POLYGON;
      isPolygonMode = TRUE;
      clear_drawMode();
      clear_polygonMode();
      printf("Mode changed. Please start over.\n");
      printf("Now drawing in polygon mode.\n");
      break;
    case 'r' :
    case 'R' :
      if (color == RED) {
	color = BLACK;
	printf("Now drawing in black\n");
      } else {
	color = RED;
	printf("Now drawing in red\n");
      }
      break;
    case 'g' :
    case 'G' :
      if (color == GREEN) {
	color = BLACK;
	printf("Now drawing in black\n");
      } else {
	color = GREEN;
	printf("Now drawing in green\n");
      }
      break;
    case 'b' :
    case 'B' :
      if (color == BLUE) {
	color = BLACK;
	printf("Now drawing in black\n");
      } else {
	color = BLUE;
	printf("Now drawing in blue\n");
      }
      break;
    case 'f' :
    case 'F' :
      if (isFilled == TRUE) {
	isFilled = FALSE;
	printf("Now drawing in nofill mode\n");
      } else {
	isFilled = TRUE;
	printf("Now drawing in fill mode\n");
      }
      break;
    case 'c' :
    case 'C' :
      clear_drawMode();
      clear_polygonMode();
      clear_displayMode();
      break;
    case GLFW_KEY_ESCAPE :
    case 'q' :
    case 'Q' :
      clear_drawMode();
      clear_polygonMode();
      clear_displayMode();
      free(array_polygonMode);
      circle_points = NULL;
      glfwSetWindowShouldClose(w, GL_TRUE);
      break;
    default:
      break;
    }
  }
}

void append_displayMode(Shape* new_displayMode) {
  if (head_displayMode == NULL) {
    tail_displayMode = head_displayMode = new_displayMode;
    head_displayMode -> next = NULL;
    head_displayMode -> prev = NULL;
    prev_displayMode = new_displayMode;
  } else {
    prev_displayMode -> next = new_displayMode;
    new_displayMode -> prev = prev_displayMode;
    prev_displayMode = new_displayMode;
    tail_displayMode = new_displayMode;
    tail_displayMode -> next = NULL;
  }
  size_displayMode++;
}

void clear_drawMode() {
  for(int i = 0; i < size_drawMode; i++) {
    if(array_drawMode[i] != NULL) {
      free(array_drawMode[i]);
      array_drawMode[i] = NULL;
    }
  }
  size_drawMode = 0;
}

void clear_polygonMode() {
  for(int i = 0; i < size_polygonMode; i++) {
    if(array_polygonMode[i] != NULL) {
      free(array_polygonMode[i]);
      array_polygonMode[i] = NULL;
    }
  }
  size_polygonMode = 0;
}

void clear_displayMode() {
  Shape *tmp, *tmp2;
  for(tmp = head_displayMode; tmp != NULL; tmp = tmp2) {
    tmp2 = tmp -> next;
    switch(tmp -> type) {
    case LINE:
      free(((Line*)tmp -> shape) -> point1);
      free(((Line*)tmp -> shape) -> point2);
      free((Line*)tmp -> shape);
      break;
    case TRIANGLE:
      free(((Triangle*)tmp -> shape) -> point1);
      free(((Triangle*)tmp -> shape) -> point2);
      free(((Triangle*)tmp -> shape) -> point3);
      free((Triangle*)tmp -> shape);
      break;
    case RECTANGLE:
      free(((Rectangle*)tmp -> shape) -> pt1);
      free(((Rectangle*)tmp -> shape) -> pt4);
      free((Rectangle*)tmp -> shape);
      break;
    case CIRCLE: {
      free(((Circle*)tmp -> shape) -> center);
      Point** circle_pts = ((Circle*)tmp -> shape) -> circle_points;
      for(int i = 0; i < NUM_CIRCLE_TRIANGULATION; i++) {
	if(circle_pts[i] != NULL) {
	  free(circle_pts[i]);
	}
      }
      free(circle_pts);
      free((Circle*)tmp -> shape);
      break;
    }
    case POLYGON: {
      Point** polygon_pts = ((Polygon*)tmp -> shape) -> polygon_points;
      int polygon_sz = ((Polygon*)tmp -> shape) -> polygon_size;
      for(int i = 0; i < polygon_sz; i++) {
	if(polygon_pts[i] != NULL) {
	  free(polygon_pts[i]);
	}
      }
      free(polygon_pts);
      free((Polygon*)tmp -> shape);
      break;
    }
    default:
      break;
    }
    free(tmp);
  }
  head_displayMode = NULL;
  size_displayMode = 0;
}

/* When the user presses the left button, a Point struct will be created
to store the current x and y values and all the Point structs wil be stored
either in the arr_drawMode or the arr_polygonMode. When the mouse is released,
based on the current mode_type(types of shapes), check if the current number
of points stores are the same as the number of points needed for the given shape.
If so, create the corresponding struct and store the struct into the list of Shape
structs and clears the momery for the temporary arr_drawMode or arr_polygonMode arrays.
*/
void mouse(GLFWwindow* window, int button, int action, int mods) {
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);

  switch (button) {
  case GLFW_MOUSE_BUTTON_LEFT:
    isPolygonFinished = FALSE;
    break;
  case GLFW_MOUSE_BUTTON_RIGHT:
    isPolygonFinished = TRUE;
    break;
  case GLFW_MOUSE_BUTTON_MIDDLE: 
    break;
  default :
    break;
  }

  switch(action) {
  case GLFW_PRESS : {
    if(button == GLFW_MOUSE_BUTTON_LEFT) {
      Point* point = make_point(xpos, ypos, color, isFilled);
      if(size_polygonMode >= num_polygon_points) {
	array_polygonMode = realloc(array_polygonMode, 2 * num_polygon_points);
	num_polygon_points *= 2;
      }
      if(!isPolygonMode) {
	array_drawMode[size_drawMode++] = point;
      } else {
	array_polygonMode[size_polygonMode++] = point;
      }
      mousePressed = TRUE;
    }
    break;
  }
  case GLFW_RELEASE :
    switch(mode) {
    case LINE : {
      if(size_drawMode == NUM_POINTS_IN_LINE) {
	Point* pt1 = make_point(array_drawMode[0] -> x, array_drawMode[0] -> y, BLACK, TRUE);
	Point* pt2 = make_point(array_drawMode[1] -> x, array_drawMode[1] -> y, BLACK, TRUE);
	Line* line = make_line(pt1, pt2);
	Shape* shape = make_shape(LINE, line, color, isFilled);
	append_displayMode(shape);
	clear_drawMode();
      }
      break;
    }
    case TRIANGLE : {
      if(size_drawMode == NUM_POINTS_IN_TRIANGLE) {
	Point* pt1 = make_point(array_drawMode[0] -> x, array_drawMode[0] -> y, BLACK, TRUE);
	Point* pt2 = make_point(array_drawMode[1] -> x, array_drawMode[1] -> y, BLACK, TRUE);
	Point* pt3 = make_point(array_drawMode[2] -> x, array_drawMode[2] -> y, BLACK, TRUE);
	Triangle* triangle = make_triangle(pt1, pt2, pt3);
	Shape* shape = make_shape(TRIANGLE, triangle, color, isFilled);
	append_displayMode(shape);
	clear_drawMode();
      }
      break;
    }
    case RECTANGLE : {
      if(size_drawMode == NUM_POINTS_IN_RECTANGLE) {
	Point* pt1 = make_point(array_drawMode[0] -> x, array_drawMode[0] -> y, BLACK, TRUE);
	Point* pt4 = make_point(array_drawMode[1] -> x, array_drawMode[1] -> y, BLACK, TRUE);
	Rectangle* rectangle = make_rectangle(pt1, pt4);
	Shape* shape = make_shape(RECTANGLE, rectangle, color, isFilled);
	append_displayMode(shape);
	clear_drawMode();
      }
      break;
    }
    case CIRCLE : {
      if(size_drawMode == NUM_POINTS_IN_CIRCLE) {
	store_points_on_circle();
	Point* center = make_point(array_drawMode[0] -> x, array_drawMode[0] -> y, BLACK, TRUE);
	Circle* circle = make_circle(center, circle_points);
	Shape* shape = make_shape(CIRCLE, circle, color, isFilled);
	append_displayMode(shape);
	clear_drawMode();
      }
      break;
    }
    case POLYGON : {
      if(isPolygonFinished) {
	store_points_on_polygon();
	Polygon* polygon = make_polygon(polygon_points, size_polygonMode);
	Shape* shape = make_shape(POLYGON, polygon, color, isFilled);
	append_displayMode(shape);
	clear_polygonMode();
      }
      break;
    }
    default:
      break;
    }
    mousePressed = FALSE;
    break;
  default :
    break;
  }
}

/*
  When mouse pressed and not released, check if there is one point in the temporary arr_drawMode
or arr_polygonMode arrays, and if the current mouse position is different from the first point
(indicating that the mouse moves). If so, create the second point and stores the point to the
correct temporary array, either arr_polygonMode for polygons and arr_drawMode for all others.
Else if there is already more than one points in the temporary arrays, update the x and y values
of the last point stored.
  This design simulates the demo hw1_mac, so that when user press and drag, the points and rubberband
will follows the user motion.
*/
void cursor(GLFWwindow* window, double xpos, double ypos) {
  if(mousePressed) {
    if(size_drawMode == 1 && array_drawMode[0] -> x != xpos && array_drawMode[0] -> y != ypos) {
      Point* pt2 = make_point(xpos, ypos, color, isFilled);
      if(!isPolygonMode) {
	array_drawMode[size_drawMode++] = pt2;
      } else {
	if(size_polygonMode == 1) {
	  array_polygonMode[size_polygonMode++] = pt2;
	}
      }
    } else {
      if(size_drawMode > 1) {
	array_drawMode[size_drawMode - 1] -> x = xpos;
	array_drawMode[size_drawMode - 1] -> y = ypos;
      } else if(size_polygonMode > 1) {
	array_polygonMode[size_polygonMode - 1] -> x = xpos;
	array_polygonMode[size_polygonMode - 1] -> y = ypos;
      }
    }
  }
}

void store_points_on_circle() {
  circle_points = malloc(sizeof(Point*) * NUM_CIRCLE_TRIANGULATION);
  int p1x = array_drawMode[0] -> x;
  int p1y = array_drawMode[0] -> y;
  int p2x = array_drawMode[1] -> x;
  int p2y = array_drawMode[1] -> y;
  double radius = sqrt((p2x - p1x) * (p2x - p1x) + (p2y - p1y) * (p2y - p1y));
  for(int i = 0; i < NUM_CIRCLE_TRIANGULATION; i++) {
    Point* point = make_point(p1x + radius * cos(i * 2 * M_PI / (NUM_CIRCLE_TRIANGULATION - 1)), p1y + radius * sin(i * 2 * M_PI / (NUM_CIRCLE_TRIANGULATION - 1)), BLACK, TRUE);
    *(circle_points + i) = point;
  }
}

void store_points_on_polygon() {
  polygon_points = malloc(sizeof(Point*) * size_polygonMode);
  for(int i = 0; i < size_polygonMode; i++) {
    Point* point = make_point(array_polygonMode[i] -> x, array_polygonMode[i] -> y, BLACK, TRUE);
    *(polygon_points + i) = point;
  }
}

void draw_points() {
  if(!isPolygonMode) {
    for (int i = 0; i < size_drawMode; i++) {
      glVertex2f(array_drawMode[i] -> x, array_drawMode[i] -> y);
    }
  } else {
    for (int i = 0; i < size_polygonMode; i++) {
      glVertex2f(array_polygonMode[i] -> x, array_polygonMode[i] -> y);
    }
  }
}

void draw_rectangle() {
  glVertex2f(array_drawMode[0] -> x, array_drawMode[0] -> y);
  glVertex2f(array_drawMode[1] -> x, array_drawMode[0] -> y);
  glVertex2f(array_drawMode[1] -> x, array_drawMode[1] -> y);
  glVertex2f(array_drawMode[0] -> x, array_drawMode[1] -> y);
}

void draw_circle() {
  glBegin(GL_LINE_LOOP);
  int p1x = array_drawMode[0] -> x;
  int p1y = array_drawMode[0] -> y;
  int p2x = array_drawMode[1] -> x;
  int p2y = array_drawMode[1] -> y;
  double radius = sqrt((p2x - p1x) * (p2x - p1x) + (p2y - p1y) * (p2y - p1y));
  for(int i = 0; i <= NUM_CIRCLE_TRIANGULATION; i++) {
    glVertex2f(p1x + radius * cos(i * 2 * M_PI / (NUM_CIRCLE_TRIANGULATION - 1)), p1y + radius * sin(i * 2 * M_PI / (NUM_CIRCLE_TRIANGULATION - 1)));
  }
  glEnd();
}

void draw_polygon(Shape* cur_displayMode) {
  int polygon_sz = ((Polygon *)cur_displayMode -> shape) -> polygon_size;
  for(int i = 0; i < polygon_sz; i++) {
    glVertex2f((*((((Polygon *)cur_displayMode -> shape) -> polygon_points) + i)) -> x, (*((((Polygon *)cur_displayMode -> shape) -> polygon_points) + i)) -> y);
  }
}

/*
  Traverse the linked list of shapes, and display each shape in its corresponding color and filled/unfilled mode.
 */
void display_displayMode() {
  Shape* cur_displayMode = head_displayMode;
  glClear(GL_COLOR_BUFFER_BIT);
  glLineStipple(4, 0xAAAA);
  glLineWidth(2);
  while(cur_displayMode != NULL) {
    switch(cur_displayMode -> color) {
    case RED :
      glColor3f(1.0, 0.0, 0.0);
      break;
    case GREEN :
      glColor3f(0.0, 1.0, 0.0);
      break;
    case BLUE :
      glColor3f(0.0, 0.0, 1.0);
      break;
    case BLACK :
      glColor3f(0.0, 0.0, 0.0);
      break;
    default:
      break;
    }
    if(cur_displayMode -> isFilled) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } else {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    switch(cur_displayMode -> type) {
    case LINE :
      if(!cur_displayMode -> isFilled) {
	glEnable(GL_LINE_STIPPLE);
      } else {
	glDisable(GL_LINE_STIPPLE);
      }
      glBegin(GL_LINES);
      glVertex2f((((Line *)cur_displayMode -> shape) -> point1) -> x, (((Line *)cur_displayMode -> shape) -> point1) -> y);
      glVertex2f((((Line *)cur_displayMode -> shape) -> point2) -> x, (((Line *)cur_displayMode -> shape) -> point2) -> y);
      glEnd();
      cur_displayMode = cur_displayMode -> next;
      glDisable(GL_LINE_STIPPLE);
      break;
    case TRIANGLE :
      glBegin(GL_TRIANGLES);
      glVertex2f((((Triangle *)cur_displayMode -> shape) -> point1) -> x, (((Triangle *)cur_displayMode -> shape) -> point1) -> y);
      glVertex2f((((Triangle *)cur_displayMode -> shape) -> point2) -> x, (((Triangle *)cur_displayMode -> shape) -> point2) -> y);
      glVertex2f((((Triangle *)cur_displayMode -> shape) -> point3) -> x, (((Triangle *)cur_displayMode -> shape) -> point3) -> y);
      glEnd();
      cur_displayMode = cur_displayMode -> next;
      break;
    case RECTANGLE :
      //glRectd((((Rectangle *)cur_displayMode -> shape) -> pt1) -> x, (((Rectangle *)cur_displayMode -> shape) -> pt1) -> y, 
      // (((Rectangle *)cur_displayMode -> shape) -> pt4) -> x, (((Rectangle *)cur_displayMode -> shape) -> pt4) -> y);
      glBegin(GL_POLYGON);
      glVertex2f((((Rectangle *)cur_displayMode -> shape) -> pt1) -> x, (((Rectangle *)cur_displayMode -> shape) -> pt1) -> y);
      glVertex2f((((Rectangle *)cur_displayMode -> shape) -> pt4) -> x, (((Rectangle *)cur_displayMode -> shape) -> pt1) -> y);
      glVertex2f((((Rectangle *)cur_displayMode -> shape) -> pt4) -> x, (((Rectangle *)cur_displayMode -> shape) -> pt4) -> y);
      glVertex2f((((Rectangle *)cur_displayMode -> shape) -> pt1) -> x, (((Rectangle *)cur_displayMode -> shape) -> pt4) -> y);
      glEnd();
      cur_displayMode = cur_displayMode -> next;
      break;
    case CIRCLE : {
      int p1x = (((Circle *)cur_displayMode -> shape) -> center) -> x;
      int p1y = (((Circle *)cur_displayMode -> shape) -> center) -> y;
      if(!cur_displayMode -> isFilled) {
	glBegin(GL_LINE_LOOP);
	glDisable(GL_LINE_STIPPLE);
      } else {
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(p1x, p1y);
      }
      for(int i = 0; i < NUM_CIRCLE_TRIANGULATION; i++) {
	glVertex2f((*((((Circle *)cur_displayMode -> shape) -> circle_points) + i)) -> x, (*((((Circle *)cur_displayMode -> shape) -> circle_points) + i)) -> y);
      }
      glEnd();
      cur_displayMode = cur_displayMode -> next;
      break;
    }
    case POLYGON : {
      if(!cur_displayMode -> isFilled) {
	glBegin(GL_LINE_LOOP);
	draw_polygon(cur_displayMode);
	glEnd();
      } else {
	glEnable(GL_STENCIL_TEST);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glStencilFunc(GL_ALWAYS, 0x1, 0x1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_INVERT);
	// Draw concave geometry
	glBegin(GL_TRIANGLE_FAN);
	draw_polygon(cur_displayMode);
	glEnd();
 
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glStencilFunc(GL_EQUAL, 0x1, 0x1);
	// Reset the stencil mask to 0 where it was 1. Useful if we want to draw many concave geometry.
	glStencilOp(GL_KEEP, GL_INVERT, GL_INVERT);
	// Draw concave geometry again
	glBegin(GL_TRIANGLE_FAN);
	draw_polygon(cur_displayMode);
	glEnd();
	glDisable(GL_STENCIL_TEST);
      }
      cur_displayMode = cur_displayMode -> next;
      break;
    }
    default:
      break;
    }
  }
}

/*
  Dynamically draws the points and rubberbands from the temporary arr_drawMode or arr_polygonMode arrays.
*/
void display_drawMode() {
  switch(color) {
  case RED :
    glColor3f(1.0, 0.0, 0.0);
    break;
  case GREEN :
    glColor3f(0.0, 1.0, 0.0);
    break;
  case BLUE :
    glColor3f(0.0, 0.0, 1.0);
    break;
  case BLACK :
    glColor3f(0.0, 0.0, 0.0);
    break;
  default:
    break;
  }
  glEnable(GL_LINE_STIPPLE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glBegin(GL_POINTS);
  draw_points();
  glEnd();
  switch(mode) {
  case LINE :
    if(size_drawMode == 2) {
      glBegin(GL_LINES);
      draw_points();
      glEnd();
    }
    break;
  case TRIANGLE :
    if(size_drawMode == NUM_POINTS_IN_LINE) {
      glBegin(GL_LINES);
    } else if(size_drawMode == NUM_POINTS_IN_TRIANGLE) {
      glBegin(GL_TRIANGLES);
    }
    draw_points();
    glEnd();
    break;
  case RECTANGLE :
    if(size_drawMode == NUM_POINTS_IN_RECTANGLE) {
      glBegin(GL_QUADS);
      draw_rectangle();
      glEnd();
    }
    break;
  case CIRCLE :
    if(size_drawMode == NUM_POINTS_IN_CIRCLE) {
      draw_circle();
    }
    break;
  case POLYGON :
    if(size_polygonMode > 1) {
      for(int i = 1; i < size_polygonMode; i++) {
	glBegin(GL_LINES);
	glVertex2f(array_polygonMode[i - 1] -> x, array_polygonMode[i - 1] -> y);
	glVertex2f(array_polygonMode[i] -> x, array_polygonMode[i] -> y);
	glEnd();
      }
    }
    break;
  default:
    break;
  }
  glDisable(GL_LINE_STIPPLE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glFlush();
}

void display() {
  display_displayMode();
  display_drawMode();
}

int main(int argc, char **argv) {
  GLFWwindow* window;
  
  // Initialize the library 
  if (!glfwInit())
    exit(EXIT_FAILURE);
  
  // Create a window and its OpenGL context 
  window = glfwCreateWindow(1000, 500, "Hello GL!", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetWindowPos(window, 500, 500);
  // Make the window's context current 
  glfwMakeContextCurrent(window);

 // Assign reshape() to be the function called whenever 
 // a reshape event occurs
  glfwSetWindowSizeCallback(window, reshape);

  // Set the new viewport size
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // Assign keyboard() to be the function called whenever
  // a key is pressed or released
  glfwSetKeyCallback(window, keyboard);

  // Assign mouse() to be the function called whenever
  // a mouse button is pressed or released
  glfwSetMouseButtonCallback(window, mouse);

  // Assign cursor() to be the function called whenever
  // a cursor moves
  glfwSetCursorPosCallback(window, cursor);

  printf("Drawing in black and in line mode\n");
  init();
  // Loop until the user closes the window 
  while (!glfwWindowShouldClose(window)) {
    // clear to background color
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0, 0.0, 0.0);
    glPointSize(10);
    display();
    
    // Swap front and back buffers
    glfwSwapBuffers(window);

    // Poll for and process events 
    glfwPollEvents();
  }

  glfwTerminate();
  exit(EXIT_SUCCESS);
}
