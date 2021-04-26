#include "graphics.h"
#include "rect.h"
#include <iostream>
using namespace std;

/********************* Dimensions Struct ********************/

dimensions::dimensions() : width(0), height(0) {}

dimensions::dimensions(double w, double h) : width(w), height(h) {}

ostream& operator << (ostream& outs, const dimensions &d) {
    outs << "[" << d.width << ", " << d.height << "]";
    return outs;
}


Rect::Rect() : Shape(), size({0, 0}) {
}

Rect::Rect(dimensions size) : Shape() {
    setSize(size);
}

Rect::Rect(color fill) : Shape(fill), size({0, 0}) {
}

Rect::Rect(point2D center) : Shape(center), size({0, 0}) {
}

Rect::Rect(color fill, point2D center) : Shape(fill, center), size({0, 0}) {
}

Rect::Rect(double red, double green, double blue, double alpha) : Shape(red, green, blue, alpha), size({0, 0}) {
}

Rect::Rect(double x, double y) : Shape(x, y), size({0, 0}) {
}

Rect::Rect(double red, double green, double blue, double alpha, double x, double y) : Shape(red, green, blue, alpha, x, y), size({0, 0}) {
}

Rect::Rect(color fill, double x, double y) : Shape(fill, x, y), size({0, 0}) {
}

Rect::Rect(double red, double green, double blue, double alpha, point2D center) : Shape(red, green, blue, alpha, center), size({0, 0}) {
}

Rect::Rect(color fill, dimensions size) : Shape(fill) {
    setSize(size);
}

Rect::Rect(point2D center, dimensions size) : Shape(center) {
    setSize(size);
}

Rect::Rect(color fill, point2D center, dimensions size) : Shape(fill, center) {
    setSize(size);
}

Rect::Rect(double red, double green, double blue, double alpha, dimensions size) : Shape(red, green, blue, alpha) {
    setSize(size);
}

Rect::Rect(double x, double y, dimensions size) : Shape(x, y) {
    setSize(size);
}

Rect::Rect(double red, double green, double blue, double alpha, double x, double y, dimensions size) : Shape(red, green, blue, alpha, x, y) {
    setSize(size);
}

Rect::Rect(color fill, double x, double y, dimensions size) : Shape(fill, x, y) {
    setSize(size);
}

Rect::Rect(double red, double green, double blue, double alpha, point2D center, dimensions size) : Shape(red, green, blue, alpha, center) {
    setSize(size);
}

dimensions Rect::getSize() const {
    return size;
}

double Rect::getWidth() const {
    return size.width;
}

double Rect::getHeight() const {
    return size.height;
}

double Rect::getLeftX() const {
    return center.x - (size.width / 2.0);
}

double Rect::getRightX() const {
    return center.x + (size.width / 2.0);
}

double Rect::getTopY() const {
    return center.y - (size.height / 2.0);
}

double Rect::getBottomY() const {
    return center.y + (size.height / 2.0);
}

void Rect::setSize(dimensions size) {
    if (size.width >= 0 && size.height >= 0) {
        this->size = size;
    }
}

void Rect::setSize(double width, double height) {
    setSize({width, height});
}

void Rect::setWidth(double width) {
    setSize({width, size.height});
}

void Rect::setHeight(double height) {
    setSize({size.width, height});
}

void Rect::changeSize(double deltaWidth, double deltaHeight) {
    setSize({size.width + deltaWidth, size.height + deltaHeight});
}

void Rect::changeWidth(double delta) {
    setSize({size.width + delta, size.height});
}

void Rect::changeHeight(double delta) {
    setSize({size.width, size.height + delta});
}

bool Rect::isOverlapping(const Rect &r) const {
    // There are only two cases when rectangles are *not* overlapping:
    // 1. when one is to the left of the other
    if (getLeftX() > r.getRightX() || getRightX() < r.getLeftX()) {
        return false;
    }
        // 2. when one is on top of the other
    else if (getTopY() > r.getBottomY() || getBottomY() < r.getTopY()) {
        return false;
    }
    // otherwise, they are overlapping so return true
    return true;
}

void Rect::draw() const {
    glColor3f(getRed(), getGreen(), getBlue());
    glBegin(GL_QUADS);
    glVertex2i(getLeftX(), getTopY());
    glVertex2i(getRightX(), getTopY());
    glVertex2i(getRightX(), getBottomY());
    glVertex2i(getLeftX(), getBottomY());
    glEnd();
}