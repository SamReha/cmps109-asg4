// $Id: shape.cpp,v 1.1 2015-07-16 16:47:51-07 - - $

#include <cmath>
#include <typeinfo>
#include <unordered_map>
using namespace std;

#include "shape.h"
#include "util.h"

static unordered_map<void*,string> fontname {
   {GLUT_BITMAP_8_BY_13       , "Fixed-8x13"    },
   {GLUT_BITMAP_9_BY_15       , "Fixed-9x15"    },
   {GLUT_BITMAP_HELVETICA_10  , "Helvetica-10"  },
   {GLUT_BITMAP_HELVETICA_12  , "Helvetica-12"  },
   {GLUT_BITMAP_HELVETICA_18  , "Helvetica-18"  },
   {GLUT_BITMAP_TIMES_ROMAN_10, "Times-Roman-10"},
   {GLUT_BITMAP_TIMES_ROMAN_24, "Times-Roman-24"},
};

ostream& operator<< (ostream& out, const vertex& where) {
   out << "(" << where.xpos << "," << where.ypos << ")";
   return out;
}

/*
   Shape
*/
shape::shape() {
   DEBUGF ('c', this);
}

void shape::show (ostream& out) const {
   out << this << "->" << demangle (*this) << ": ";
}

unordered_map<string,void*> shape::fontcodes() {
   return fontcode;
}

/*
   Text
*/
text::text (void* glut_bitmap_font, const string& textdata):
      glut_bitmap_font(glut_bitmap_font), textdata(textdata) {
   DEBUGF ('c', this);
}

void text::draw (const vertex& center,
                 const rgbcolor& color,
                 const bool linemode,
                 const int lineWidth) const {
   DEBUGF ('d', this << "(" << center << "," << color << ")");
   glColor3ubv (color.ubvec);
   glRasterPos2i (center.xpos, center.ypos);
   glutBitmapString (glut_bitmap_font, (GLubyte*) textdata.c_str());
}

void text::show (ostream& out) const {
   shape::show (out);
   out << glut_bitmap_font << "(" << fontname[glut_bitmap_font]
       << ") \"" << textdata << "\"";
}

/*
   Ellipse
*/
ellipse::ellipse (GLfloat width, GLfloat height):
dimension ({width, height}) {
   DEBUGF ('c', this);
}

void ellipse::draw(const vertex& center,
                   const rgbcolor& color,
                   const bool linemode,
                   const int lineWidth) const {
   DEBUGF ('d', this << "(" << center << "," << color << ")");

   vertex drawingVertex;
   if (linemode) {
      drawingVertex = vertex{dimension.xpos + lineWidth,
                             dimension.ypos + lineWidth};
   } else {
      drawingVertex = dimension;
   }
   glBegin(GL_POLYGON);

   glEnable(GL_LINE_SMOOTH);
   glColor3ubv(color.ubvec3());
   const float delta = 2 * M_PI / 32;

   for (float theta = 0; theta < 2*M_PI; theta += delta) {
      float xpos = drawingVertex.xpos*cos(theta) + center.xpos;
      float ypos = drawingVertex.ypos*sin(theta) + center.ypos;
      glVertex2f(xpos, ypos);
   }
   glEnd();
}

void ellipse::show (ostream& out) const {
   shape::show (out);
   out << "{" << dimension << "}";
}

/*
   Circle
*/
circle::circle (GLfloat diameter): ellipse (diameter, diameter) {
   DEBUGF ('c', this);
}

/*
   Polygon
*/
polygon::polygon (const vertex_list& vertices): vertices(vertices) {
   DEBUGF ('c', this);
}

void polygon::draw (const vertex& center,
                    const rgbcolor& color,
                    const bool linemode,
                    const int lineWidth) const {
   DEBUGF ('d', this << "(" << center << "," << color << ")");

   glBegin(GL_POLYGON);
   glEnable(GL_LINE_SMOOTH);
   glColor3ubv(color.ubvec3());

   for (auto& vert : vertices) {
      vertex drawingVertex;
      if (linemode) {
         drawingVertex = vertex{vert.xpos + lineWidth,
                                vert.ypos + lineWidth};
      } else {
         drawingVertex = vert;
      }
      glVertex2f(drawingVertex.xpos + center.xpos,
                 drawingVertex.ypos + center.ypos);
   }
   glEnd();
}

void polygon::show (ostream& out) const {
   shape::show (out);
   out << "{" << vertices << "}";
}

/*
   Rectangle
*/
rectangle::rectangle (GLfloat width, GLfloat height): polygon(
   {
      {-width/2, height/2},
      { width/2, height/2},
      { width/2,-height/2},
      {-width/2,-height/2}
   }) {
   DEBUGF ('c', this << "(" << width << "," << height << ")");
}

/*
   Square
*/
square::square (GLfloat width): rectangle (width, width) {
   DEBUGF ('c', this);
}

/*
   Diamond
*/
diamond::diamond (GLfloat width, GLfloat height): polygon(
   {
      { 0, height/2},
      { width/2, 0},
      { 0,-height/2},
      {-width/2, 0}
   }) {
   DEBUGF ('c', this << "(" << width << "," << height << ")");
}

/*
   Equilateral
*/
equilateral::equilateral (GLfloat width): polygon(
   {
      { 0, static_cast<GLfloat>((width*sqrt(3.0)*0.5)/2)},
      { width/2, static_cast<GLfloat>(-(width*sqrt(3.0)*0.5)/2)},
      {-width/2, static_cast<GLfloat>(-(width*sqrt(3.0)*0.5)/2)},
   }) {
   DEBUGF ('c', this << "(" << width << ")");
}

ostream& operator<< (ostream& out, const shape& obj) {
   obj.show (out);
   return out;
}

