#include "line.h"

Line::Line(int sgm = 1, GLfloat width = 0.01)
{
    m_nSegment  = sgm + 1;
    m_fWidth    = width;
    m_pSource   = new GLfloat[3];
    m_pSink     = new GLfloat[3];
}

Line::Line(GLfloat *p1, GLfloat *p2, GLboolean even, int cycle, int sgm = 1,
           GLfloat width = 0.01)
{
    int max;
    if (even) {
        max = (pow(2, cycle) + 2) * 3;
    } else {
        max = (pow(2, (cycle + 1)) - 1 + 2) * 3;
    }

    m_nSegment      = sgm + 1;
    m_fWidth        = width;

    m_pSource       = new GLfloat[3];
    m_pSink         = new GLfloat[3];
    m_pSource       = p1[0] > p2[0] ? p2 : p1;
    m_pSink         = p1[0] > p2[0] ? p1 : p2;

    m_pSegment      = new GLfloat[9];
    m_pForce        = new GLfloat[9];

    m_fLength       = CalcDistance(m_pSource, m_pSink);
    m_fOriLength    = m_fLength;
    m_fSubLength    = m_fLength / m_nSegment;
    m_fConstant     = ATTRACTION / m_fLength;

    CalcSegments();
}

void Line::norVector(GLfloat *n)
{
    assert(n != NULL);
    GLfloat length = sqrt((n[0] * n[0]) + (n[1] * n[1]) + (n[2] * n[2]));
    n[0] = n[0] / length;
    n[1] = n[1] / length;
    n[2] = n[2] / length;
}

void Line::CalcSegments()
{
    size_t size0 = 0;
    size_t size1 = 0;

    GLfloat *tmp = new GLfloat[3];
    for (int i = 0; i < 3; i++) {
        m_pSegment[size0++] = m_pSource[i];
        m_pForce[size1++] = 0.0;
        tmp[i] = m_pSource[i];
    }

    GLfloat *vector = new GLfloat[3];
    vector[0] = m_pSink[0] - m_pSource[0];
    vector[1] = m_pSink[1] - m_pSource[1];
    vector[2] = m_pSink[2] - m_pSource[2];

    norVector(vector);

    vector[0] = vector[0] * m_fSubLength;
    vector[1] = vector[1] * m_fSubLength;
    vector[2] = vector[2] * m_fSubLength;

    for (int i = 1; i < m_nSegment; i++) {
           tmp[0] += vector[0];
           tmp[1] += vector[1];
           tmp[2] += vector[2];

           m_pSegment[size0++] = tmp[0];
           m_pSegment[size0++] = tmp[1];
           m_pSegment[size0++] = tmp[2];

           m_pForce[size1++] = 0.0;
           m_pForce[size1++] = 0.0;
           m_pForce[size1++] = 0.0;
    }
    m_pSegment[size0++] = m_pSink[0];
    m_pSegment[size0++] = m_pSink[1];
    m_pSegment[size0++] = m_pSink[2];

    m_pForce[size1++] = 0.0;
    m_pForce[size1++] = 0.0;
    m_pForce[size1++] = 0.0;
}

GLfloat *Line::getSource()
{
    return m_pSource;
}

GLfloat *Line::getSink()
{
    return m_pSink;
}

GLfloat Line::getWidth()
{
    return m_fWidth;
}

void Line::setWidth(GLfloat width)
{
    m_fWidth = width;
}

GLfloat Line::getOriLength()
{
    return m_fOriLength;
}

GLfloat Line::getLength()
{
    return m_fLength;
}

void Line::setLength(GLfloat length)
{
    m_fLength = length;
}

void Line::setSegmentNum(int sgm)
{
    m_nSegment = sgm;
}

GLfloat Line::getConstant()
{
    return m_fConstant;
}

void Line::setConstant(GLfloat constant)
{
    m_fConstant = constant;
}

void Line::setSegment(GLfloat *v)
{
    m_pSegment = v;
}

GLfloat *Line::getSegment()
{
    return m_pSegment;
}

GLfloat *Line::getForce()
{
    return m_pForce;
}

void Line::setForce(GLfloat *v)
{
    m_pForce = v;
}

GLfloat Line::CalcDistance(GLfloat *p1, GLfloat *p2)
{
    GLfloat x = p1[0] - p2[0];
    GLfloat y = p1[1] - p2[1];
    GLfloat z = p1[2] - p2[2];
    return sqrt(x * x + y * y + z * z);
}

void Line::setCompatibility(int i)
{
    m_vCompatibility.push_back(i);
}

vector<int> &Line::getCompatibility()
{
    return m_vCompatibility;
}
