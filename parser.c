#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"

/*======== void parse_file () ==========
Inputs:   char * filename
          struct matrix * transform,
          struct matrix * pm,
          screen s
Returns:

Goes through the file named filename and performs all of the actions listed in that file.
The file follows the following format:
     Every command is a single character that takes up a line
     Any command that requires arguments must have those arguments in the second line.
     The commands are as follows:

         circle: add a circle to the edge matrix -
                 takes 4 arguments (cx, cy, cz, r)
         hermite: add a hermite curve to the edge matrix -
                  takes 8 arguments (x0, y0, x1, y1, rx0, ry0, rx1, ry1)
         bezier: add a bezier curve to the edge matrix -
                 takes 8 arguments (x0, y0, x1, y1, x2, y2, x3, y3)


         line: add a line to the edge matrix -
               takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
         ident: set the transform matrix to the identity matrix -
         scale: create a scale matrix,
                then multiply the transform matrix by the scale matrix -
                takes 3 arguments (sx, sy, sz)
         move: create a translation matrix,
               then multiply the transform matrix by the translation matrix -
               takes 3 arguments (tx, ty, tz)
         rotate: create a rotation matrix,
                 then multiply the transform matrix by the rotation matrix -
                 takes 2 arguments (axis, theta) axis should be x y or z
         apply: apply the current transformation matrix to the edge matrix
         display: clear the screen, then
                  draw the lines of the edge matrix to the screen
                  display the screen
         save: clear the screen, then
               draw the lines of the edge matrix to the screen
               save the screen to a file -
               takes 1 argument (file name)
         quit: end parsing

See the file script for an example of the file format

IMPORTANT MATH NOTE:
the trig functions int math.h use radian mesure, but us normal
humans use degrees, so the file will contain degrees for rotations,
be sure to conver those degrees to radians (M_PI is the constant
for PI)
====================*/
void parse_file ( char * filename,
                  struct matrix * transform,
                  struct matrix * edges,
                  screen s) {

  FILE *f;
  char line[255];
  clear_screen(s);
  color c;
  c.red = 255;
  c.green = 0;
  c.blue = 255;
  
  if ( strcmp(filename, "stdin") == 0 ) 
    f = stdin;
  else
    f = fopen(filename, "r");
  
  while ( fgets(line, sizeof(line), f) != NULL ) {
    line[strlen(line)-1]='\0';
    //printf(":%s:\n",line);

    double xvals[3];
    double yvals[3];
    double zvals[4];

    if ( strncmp(line, "circle", strlen(line)) == 0 ) {
      double radius;
      fgets(line, sizeof(line), f);
      sscanf(line, "%lf %lf %lf %lf",
             xvals, yvals, zvals, &radius);
      add_circle(edges, xvals[0], yvals[0], zvals[0], radius, 0.01);
    }
		
    else if ( strncmp(line, "hermite", strlen(line)) == 0 ) {
      fgets(line, sizeof(line), f);
			double x0,y0,x1,y1,rx0,ry0,rx1,ry1;
      sscanf(line, "%lf %lf %lf %lf %lf %lf %lf %lf",
             &x0,&y0,&x1,&y1,&rx0,&ry0,&rx1,&ry1);
      add_curve(edges, x0,y0,x1,y1,rx0,ry0,rx1,ry1, 0.01, 0);
    }
		
    else if ( strncmp(line, "bezier", strlen(line)) == 0 ) {
      fgets(line, sizeof(line), f);
			double x0,y0,x1,y1,x2,y2,x3,y3;
      sscanf(line, "%lf %lf %lf %lf %lf %lf %lf %lf",
             &x0,&y0,&x1,&y1,&x2,&y2,&x3,&y3);
      add_curve(edges, x0,y0,x1,y1,x2,y2,x3,y3, 0.01, 1);
    }
    
    else if(!(strcmp(line, "line"))) {
      fgets(line, 255, f);
      double x0, y0, z0, x1, y1, z1;
      sscanf(line, "%lf %lf %lf %lf %lf %lf", &x0, &y0, &z0, &x1, &y1, &z1);
      add_edge(edges, x0, y0, z0, x1, y1, z1);
    }

    else if(!(strcmp(line, "ident"))) {
      ident(transform);
    }

    else if(!(strcmp(line, "scale"))) {
      fgets(line, 255, f);
      double x0, y0, z0;
      sscanf(line, "%lf %lf %lf", &x0, &y0, &z0);
      struct matrix * temp;
      temp = make_scale(x0, y0, z0);
      matrix_mult(temp, transform);
      free_matrix(temp);
    }

    else if(!(strcmp(line,"move"))) {
      fgets(line, 255, f);
      double x0, y0, z0;
      sscanf(line, "%lf %lf %lf", &x0 ,&y0 ,&z0);
      struct matrix * temp = make_translate(x0, y0, z0);
      matrix_mult(temp, transform);
      free_matrix(temp);
    }

    else if(!(strcmp(line,"rotate"))) {
      fgets(line, 255, f);
      char axis;
      double degree;
      struct matrix * temp;
      sscanf(line, "%c %lf", &axis, &degree);
      if(axis == 'x')
        temp = make_rotX(degree);
      if(axis == 'y')
        temp = make_rotY(degree);
      if(axis == 'z')
        temp = make_rotZ(degree);
      matrix_mult(temp, transform);
      free_matrix(temp);
    }

    else if(!(strcmp(line,"apply"))) {
      matrix_mult(transform, edges);
    }

    else if(!(strcmp(line,"display"))) {
      clear_screen(s);
      draw_lines(edges, s, c);
      display(s);
    }

    else if(! strcmp(line,"save")){
      fgets(line,255,f);
      clear_screen(s);
      draw_lines(edges,s,c);
      save_extension(s,line);
    }

    else if(!(strcmp(line,"quit"))) {
      exit(0);
    }
  }
}
