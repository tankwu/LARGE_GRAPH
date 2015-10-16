#ifndef POINT_H
#define POINT_H

#include <assert.h>
#include <QtOpenGL>
#include <QGLFunctions>


class Point
{

public:

    Point(GLfloat r);

    ~Point();

    Point(GLfloat x, GLfloat y, GLfloat r);

    void setPosition(GLfloat x, GLfloat y);

    GLfloat *getPosition();

    void setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);

    GLfloat *getColor();

    void setVertices();

    GLfloat *getVertices();

    GLfloat getRadius();

    void setXvelocity(GLfloat v);

    void setYvelocity(GLfloat v);

    GLfloat *getVelocity();

    void setXforce(GLfloat x);

    void setYforce(GLfloat y);

    void setForce(GLfloat *f);

    GLfloat *getForce();

protected:

    GLfloat getRandomArbitrary();

private:

    GLfloat             m_fCenter;

    GLfloat             m_fRadius;

    GLfloat             *m_pPos;

    GLfloat             *m_pColor;

    GLfloat             *m_pVertex;

    GLfloat             *m_pForce;

    GLfloat             *m_pVelocity;
};

#endif // POINT_H
