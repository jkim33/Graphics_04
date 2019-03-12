#ifndef DRAW_H
#define DRAW_H

#include <math.h>
#include "matrix.h"
#include "ml6.h"

//advanced shapes
void add_circle( struct matrix * points,
                 double cx, double cy, double cz,
                 double r, double step );
void add_curve( struct matrix *points,
                double x0, double y0,
                double x1, double y1,
                double x2, double y2,
                double x3, double y3,
                double step, int type );

void add_point( struct matrix * points, double x, double y, double z);
void add_edge( struct matrix * points,
               double x0, double y0, double z0,
               double x1, double y1, double z1);
void draw_lines( struct matrix * points, screen s, color c);
void draw_line(int x0, int y0, int x1, int y1, screen s, color c);

#endif