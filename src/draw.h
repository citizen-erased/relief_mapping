#ifndef DRAW_H
#define DRAW_H

#include <cml/cml.h>

void drawAxes(float scale=1.0f);
void drawLine(const cml::vector3f &p0, const cml::vector3f &p1);
void drawVector(cml::vector3f origin, cml::vector3f vec);
void drawGrid(cml::vector3f origin, float w, float h, float spacing);
void drawPlane(const cml::vector3f &p, const cml::vector3f &n, float w, float h);

void drawCube(float x, float y, float z, float scale=1.0f);
void drawCube(const cml::vector3f &pos, float scale=1.0f);
void drawSphere(float x, float y, float z, float scale=1.0f);
void drawSphere(const cml::vector3f &pos, float scale=1.0f);

#endif /* DRAW_H */

