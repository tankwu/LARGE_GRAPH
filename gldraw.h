#ifndef GLDRAW_H
#define GLDRAW_H

#include <vector>
#include <cmath>
#include <math.h>
#include <locale.h>

#include <QGLWidget>
#include <QtOpenGL>
#include <QGLFunctions>
#include <QOpenGLBuffer>
#include <QGLShaderProgram>
#include <QOpenGLFunctions>
#include <QBasicTimer>
#include <QDebug>

#include <fstream>
#include <iostream>
#include <sstream>
#include <time.h>
#include <sys/time.h>
#include <iomanip>
#include <assert.h>
#include <cmath>

#define MU                  0.0
#define SIGMA               1.0

#define UPPERBOUND          1.0
#define LOWERBOUND          -1.0

#define BALLNUM             100
#define RADIUS              0.005
#define LINEWIDTH           0.01

#define B_VERTICES          0
#define B_COLOR             1
#define B_CENTER            2
#define B_RADIUS            3
#define B_LINES             4
#define B_POLYGONS          5
#define B_CAN               6
#define B_QUAD              7

#define C_COM_COOR          0
#define C_POS_COOR          1

#define T_TEX               0
#define T_POS_1             1
#define T_POS_2             2
#define T_CAN               3

#define F_POS               0
#define F_TEX               1
#define F_CAN               2

#define ATTRACTION          0.1
#define ELECTRO             1

#define WIDTH               400
#define HEIGHT              400

#define EPS                 1.0e-6
#define COM                 0.7
#define SMOOTH              0.5

#define FILENUM             2

#define S_INITIAL           0.0004       // init. distance to move points
#define P_INITIAL           1            // init. subdivision number
#define P_RATE              2            // subdivision rate increase
#define CYCLE               6            // number of cycles to perform
#define I_INITIAL           50           // init. number of iterations for cycle
#define I_RATE              0.6666667    // rate at which iteration number decreases i.e. 2/3

using namespace std;

class GLDraw : public QGLWidget, protected QGLFunctions
{
    Q_OBJECT

public:

    explicit GLDraw(QWidget *parent = 0);

    ~GLDraw();

protected:

    void initBuffer();

    void initShaders1();

    void initShaders2();

    void initShaders3();

    void initShaders4();

    void initVBO();

    void initDrawCanvas();

    void initializeGL();

    void paintGL();

    void resizeGL(int w, int h);

    void drawCanvas();

    void saveBuffer();

    void readBuffer(string file);

    double read_timer();

private:

    QGLShaderProgram        *m_qVertexProgram;

    QGLShaderProgram        *m_qLineProgram;

    QGLShaderProgram        *m_qPolyProgram;

    QGLShaderProgram        *m_qCanvasProgram;

    QBasicTimer             timer;

    GLfloat                 *m_pGL_dat_lines;

    GLfloat                 *m_pGL_dat_points;

    GLfloat                 *m_pGL_dat_colors;

    GLfloat                 *m_pGL_dat_centers;

    GLfloat                 *m_pGL_dat_radius;

    GLfloat                 *m_pGL_dat_attrs;

    GLfloat                 *m_pGL_dat_polygons;

    GLfloat                 *m_pSubPoints;

    GLint                   m_nLineNum;

    GLint                   m_nSegment;

    GLint                   m_nWidth;

    GLint                   m_nHeight;

    GLint                   m_nBallNum;

    GLint                   m_nMax;

    GLuint                  m_pVertexBuffer[8];

    GLuint                  m_pCalBuffer[2];

    GLuint                  m_pTexBuffer[4];

    GLuint                  m_pFrameBuffer[3];

    GLboolean               m_bVertices;

    GLboolean               m_bEdges;

    GLboolean               m_bPolygons;

    GLboolean               m_bDivEven;

    GLboolean               m_bSmooth;

    GLboolean               m_bData1;

    GLboolean               m_bFirst;

    string                  m_sDataNode;

    string                  m_sDataEdge;
};

#endif // GLDRAW_H
