// $Id: rgbcolor.h,v 1.1 2015-07-16 16:47:51-07 - - $

#ifndef __RGBCOLOR_H__
#define __RGBCOLOR_H__

#include <string>
#include <unordered_map>
using namespace std;

#include <GL/freeglut.h>

struct rgbcolor {
   union {
      GLubyte ubvec[3];
      struct {
         GLubyte red;
         GLubyte green;
         GLubyte blue;
      };
   };
   explicit rgbcolor(): red(0), green(0), blue(0) {}
   explicit rgbcolor (GLubyte red, GLubyte green, GLubyte blue):
               red(red), green(green), blue(blue) {}
   explicit rgbcolor (const string&);
   const GLubyte* ubvec3() const{ return ubvec; }
   operator string() const;
};

ostream& operator<< (ostream&, const rgbcolor&);

extern const std::unordered_map<std::string,rgbcolor> color_names;

#endif
