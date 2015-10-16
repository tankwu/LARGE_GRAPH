#include "point.h"

Point::Point(GLfloat r = 0.02)
{
    m_fRadius = r;
    m_pForce = new GLfloat[2];
    m_pVelocity = new GLfloat[2];
    m_pColor = new GLfloat[4];
    m_pColor[0] = this->getRandomArbitrary();
    m_pColor[1] = this->getRandomArbitrary();
    m_pColor[2] = this->getRandomArbitrary();
    m_pColor[3] = 1.0;

    m_pForce[0] = 0.0;
    m_pForce[1] = 0.0;

    m_pVelocity[0] = 0.0;
    m_pVelocity[1] = 0.0;
}

Point::Point(GLfloat x, GLfloat y, GLfloat r)
{
    m_fRadius = r;
    m_pForce = new GLfloat[2];
    m_pVelocity = new GLfloat[2];
    m_pPos = new GLfloat[3];
    m_pPos[0] = x;
    m_pPos[1] = y;
    m_pPos[2] = 0.0;

    m_pColor = new GLfloat[4];
    m_pColor[0] = 1.0;
    m_pColor[1] = 1.0;
    m_pColor[2] = 0.0;
    m_pColor[3] = 1.0;

    m_pVertex = new GLfloat[6 * 3];

    memset(m_pVertex, 0.0, sizeof(GLfloat) * 6 * 3);

    //vertex#1
    m_pVertex[0] = m_pPos[0] - m_fRadius;
    m_pVertex[1] = m_pPos[1] - m_fRadius;

    //vertex#2
    m_pVertex[3] = m_pPos[0] + m_fRadius;
    m_pVertex[4] = m_pPos[1] - m_fRadius;

    //vertex#3
    m_pVertex[6] = m_pPos[0] + m_fRadius;
    m_pVertex[7] = m_pPos[1] + m_fRadius;

    //vertex#4
    m_pVertex[9] = m_pPos[0] - m_fRadius;
    m_pVertex[10] = m_pPos[1] + m_fRadius;

    //vertex#5
    m_pVertex[12] = m_pPos[0] - m_fRadius;
    m_pVertex[13] = m_pPos[1] - m_fRadius;

    //vertex#6
    m_pVertex[15] = m_pPos[0] + m_fRadius;
    m_pVertex[16] = m_pPos[1] + m_fRadius;

    m_pForce[0] = 0.0;
    m_pForce[1] = 0.0;

    m_pVelocity[0] = 0.0;
    m_pVelocity[1] = 0.0;
}

void Point::setVertices() {
    if (m_pVertex == NULL) {
        m_pVertex = new GLfloat[6 * 3];
    }
    memset(m_pVertex, 0.0, sizeof(GLfloat) * 6 * 3);

    assert(m_pPos != NULL);
    //vertex#1
    m_pVertex[0] = m_pPos[0] - m_fRadius;
    m_pVertex[1] = m_pPos[1] - m_fRadius;

    //vertex#2
    m_pVertex[3] = m_pPos[0] + m_fRadius;
    m_pVertex[4] = m_pPos[1] - m_fRadius;

    //vertex#3
    m_pVertex[6] = m_pPos[0] + m_fRadius;
    m_pVertex[7] = m_pPos[1] + m_fRadius;

    //vertex#4
    m_pVertex[9] = m_pPos[0] - m_fRadius;
    m_pVertex[10] = m_pPos[1] + m_fRadius;

    //vertex#5
    m_pVertex[12] = m_pPos[0] - m_fRadius;
    m_pVertex[13] = m_pPos[1] - m_fRadius;

    //vertex#6
    m_pVertex[15] = m_pPos[0] + m_fRadius;
    m_pVertex[16] = m_pPos[1] + m_fRadius;
}

void Point::setPosition(GLfloat x, GLfloat y)
{
    assert(m_pPos != NULL);
    m_pPos[0] = x;
    m_pPos[1] = y;
    m_pPos[2] = 0.0;
    setVertices();
}

GLfloat *Point::getPosition()
{
    return m_pPos;
}

void Point::setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    assert(m_pColor != NULL);
    m_pColor[0] = r;
    m_pColor[1] = g;
    m_pColor[2] = b;
    m_pColor[3] = a;
}

GLfloat *Point::getColor()
{
    assert(m_pColor != NULL);
    return m_pColor;
}

GLfloat *Point::getVertices()
{
    assert(m_pVertex != NULL);
    return m_pVertex;
}

GLfloat Point::getRadius()
{
    return m_fRadius;
}

GLfloat Point::getRandomArbitrary()
{
    return ((double) rand() / (RAND_MAX));
}

void Point::setXvelocity(float v)
{
    m_pVelocity[0] = v;
}

void Point::setYvelocity(float v)
{
    m_pVelocity[1] = v;
}


float *Point::getVelocity()
{
    return m_pVelocity;
}

void Point::setXforce(GLfloat x)
{
    assert(m_pForce != NULL);
    m_pForce[0] = x;
}

void Point::setYforce(GLfloat y)
{
    assert(m_pForce != NULL);
    m_pForce[1] = y;
}

void Point::setForce(GLfloat *f)
{
    m_pForce = f;
}

GLfloat *Point::getForce()
{
    assert(m_pForce != NULL);
    return m_pForce;
}

Point::~Point()
{
    delete[] m_pPos;
    m_pPos = NULL;
    delete[] m_pColor;
    m_pColor = NULL;
    delete[] m_pVertex;
    m_pVertex = NULL;
    delete[] m_pVertex;
}
