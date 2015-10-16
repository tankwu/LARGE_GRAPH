#include "gldraw.h"
#include "jitter.h"

double                  simulation_time;
double                  compatibility_time;

static const GLfloat m_pQuadPoints[] = {
    -1.0f, -1.0f,
    1.0f, -1.0f,
    1.0f,  1.0f,
    -1.0f,  1.0f
};

static const GLfloat m_pTexPoints[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f
};

void accFrustum(GLdouble left, GLdouble right, GLdouble bottom,
                GLdouble top, GLdouble near, GLdouble far, GLdouble pixdx,
                GLdouble pixdy, GLdouble eyedx, GLdouble eyedy, GLdouble focus)
{
    GLdouble xwsize, ywsize;
    GLdouble dx, dy;
    GLint viewport[4];

    glGetIntegerv (GL_VIEWPORT, viewport);

    xwsize = right - left;
    ywsize = top - bottom;

    dx = -(pixdx*xwsize/(GLdouble) viewport[2] + eyedx*near/focus);
    dy = -(pixdy*ywsize/(GLdouble) viewport[3] + eyedy*near/focus);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum (left + dx, right + dx, bottom + dy, top + dy, near, far);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef (-eyedx, -eyedy, 0.0);
}

void accPerspective(GLdouble fovy, GLdouble aspect,
                    GLdouble near, GLdouble far,
                    GLdouble pixdx, GLdouble pixdy,
                    GLdouble eyedx, GLdouble eyedy, GLdouble focus)
{
    double PI = 3.1415926;

    GLdouble fov2, left, right, bottom, top;

    fov2 = ((fovy*PI) / 180.0) / 2.0;

    top = near / (cos(fov2) / sin(fov2));
    bottom = -top;

    right = top * aspect;
    left = -right;

    accFrustum (left, right, bottom, top, near, far,
                pixdx, pixdy, eyedx, eyedy, focus);
}

GLDraw::GLDraw(QWidget *parent)
    : QGLWidget(parent)
{
    resize(WIDTH, HEIGHT);
    m_bVertices     = false;
    m_bEdges        = true;
    m_bPolygons     = false;
    m_bDivEven      = false;
    m_bSmooth       = false;
    m_bData1        = true;
    m_bFirst        = true;
    m_nSegment      = 1;
    m_nLineNum      = 0;

    float cycle     = CYCLE * 1.0f;
    if (m_bDivEven) {
        m_nMax = pow(2.0f, cycle) + 2;
    } else {
        m_nMax = pow(2.0f, (cycle + 1)) - 1 + 2;
    }

    if (m_bData1) {
        m_sDataNode = "../CDATA/nodes_airlines.csv";
        m_sDataEdge = "../CDATA/edges_airlines.csv";
    } else {
        m_sDataNode = "../CDATA/nodes_migrations.csv";
        m_sDataEdge = "../CDATA/edges_migrations.csv";
    }
}

double GLDraw::read_timer()
{
    static bool initialized = false;
    static struct timeval start;
    struct timeval end;
    if( !initialized )
    {
        gettimeofday( &start, NULL );
        initialized = true;
    }
    gettimeofday( &end, NULL );
    return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
}

void GLDraw::readBuffer(string file)
{
    ifstream input(file.c_str());

    float   attraction;
    float   electro;
    float   eps;
    int     segment;

    float   s_initial;
    int     p_initial;
    int     p_rate;
    int     cycle;
    int     i_initial;
    float   i_rate;

    int     lines;
    int     maximum;

    //int     ballnum;

    /* ATTRACTION */
    input.read(reinterpret_cast<char *>(&attraction), sizeof(float));

    /* REPULSION */
    input.read(reinterpret_cast<char *>(&electro), sizeof(float));

    /* EPS */
    input.read(reinterpret_cast<char *>(&eps), sizeof(float));

    /* SEGMENT */
    input.read(reinterpret_cast<char *>(&segment), sizeof(float));

    /* S_INITIAL */
    input.read(reinterpret_cast<char *>(&s_initial), sizeof(float));

    /* P_INITIAL */
    input.read(reinterpret_cast<char *>(&p_initial), sizeof(int));

    /* P_RATE */
    input.read(reinterpret_cast<char *>(&p_rate), sizeof(int));

    /* CYCLE */
    input.read(reinterpret_cast<char *>(&cycle), sizeof(int));

    /* I_INITIAL */
    input.read(reinterpret_cast<char *>(&i_initial), sizeof(int));

    /* I_RATE */
    input.read(reinterpret_cast<char *>(&i_rate), sizeof(float));

    /* LINE NUMBER */
    input.read(reinterpret_cast<char *>(&lines), sizeof(int));

    /* MAXIMUM NUMBER */
    input.read(reinterpret_cast<char *>(&maximum), sizeof(int));

    /* BALL NUMBER */
    //input.read(reinterpret_cast<char *>(&ballnum), sizeof(int));

    /* LINE SUBPOINTS */

    size_t size0;
    float subpoint;
    if (m_nLineNum != lines) {
        if (!m_bFirst) {
            delete[] m_pGL_dat_lines;
            m_pGL_dat_lines = NULL;
        }
        m_nLineNum  = lines;
        m_nMax      = maximum;
        m_nSegment  = maximum - 1;
        m_pGL_dat_lines = new GLfloat[6 * m_nSegment * m_nLineNum];
    } else {
        m_nLineNum  = lines;
        m_nMax      = maximum;
        m_nSegment  = maximum - 1;
    }

    m_pSubPoints = new GLfloat[m_nLineNum * m_nMax * 3];
    for (int i = 0; i < m_nLineNum; i++) {
        for (int j = 0; j < m_nMax * 3; j++) {
            input.read(reinterpret_cast<char *>(&subpoint), sizeof(float));
            m_pSubPoints[m_nMax * i * 3 + j] = subpoint;
        }
    }

    input.close();

    size0 = 0;
    for (int i = 0; i < m_nLineNum; i++) {
        for (int j = 0; j < m_nSegment; j++) {
            m_pGL_dat_lines[size0++] = m_pSubPoints[m_nMax * i * 3 + j * 3 + 0];
            m_pGL_dat_lines[size0++] = m_pSubPoints[m_nMax * i * 3 + j * 3 + 1];
            m_pGL_dat_lines[size0++] = m_pSubPoints[m_nMax * i * 3 + j * 3 + 2];
            m_pGL_dat_lines[size0++] = m_pSubPoints[m_nMax * i * 3 + (j + 1)
                                                                * 3 + 0];
            m_pGL_dat_lines[size0++] = m_pSubPoints[m_nMax * i * 3 + (j + 1)
                                                                * 3 + 1];
            m_pGL_dat_lines[size0++] = m_pSubPoints[m_nMax * i * 3 + (j + 1)
                                                                * 3 + 2];
        }
    }

    delete[] m_pSubPoints;
    m_pSubPoints = NULL;
    assert(m_pSubPoints == NULL);

    /*
    string fname = "../verification";
    ofstream output(fname.c_str());
    for (int i = 0; i < m_nLineNum * m_nMax * 3; i++) {
        output << m_pSubPoints[i] << endl;
    }
    output.close();
    */

    /*
    qDebug() << attraction;

    qDebug() << electro;

    qDebug() << force;

    qDebug() << distance;

    qDebug() << s_initial;

    qDebug() << i_rate;

    qDebug() << p_initial;

    qDebug() << p_rate;

    qDebug() << c;

    qDebug() << i_initial;

    qDebug() << segment;

    qDebug() << size;

    qDebug() << lines;

    for (int i = 0; i < n_LineNum; i++) {
        for (int j = 0; j < size; j++) {
            qDebug() << subpoints[i * size + j];
        }
    }
    */
}

void GLDraw::initBuffer()
{
    string fname = "../1";
    readBuffer(fname);
    m_bFirst = false;

    /*
    string file = "../groundtruth_output";
    ofstream output(file.c_str());
    for (int i = 0; i < m_nLineNum * m_nMax * 3; i++) {
        output << outpoints[i] << endl;
    }
    output.close();
    */
}


void GLDraw::initShaders1()
{
    m_qVertexProgram = new QGLShaderProgram(this);

    // Override system locale until shaders are compiled
    setlocale(LC_NUMERIC, "C");

    // Compile vertex shader
    if (!m_qVertexProgram->addShaderFromSourceFile(QGLShader::Vertex,
                                            ":/Vertices.vert")) {
        close();
    }

    // Compile fragment shader
    if (!m_qVertexProgram->addShaderFromSourceFile(QGLShader::Fragment,
                                            ":/Vertices.frag")) {
        close();
    }

    // Link shader pipeline
    if (!m_qVertexProgram->link()) {
        close();
    }

    // Bind shader pipeline for use
    if (!m_qVertexProgram->bind()) {
        close();
    }

    // Restore system locale
    setlocale(LC_ALL, "");
}

void GLDraw::initShaders2()
{
    m_qLineProgram = new QGLShaderProgram(this);

    // Override system locale until shaders are compiled
    setlocale(LC_NUMERIC, "C");

    // Compile vertex shader
    if (!m_qLineProgram->addShaderFromSourceFile(QGLShader::Vertex,
                                            ":/Lines.vert")) {
        close();
    }

    // Compile fragment shader
    if (!m_qLineProgram->addShaderFromSourceFile(QGLShader::Fragment,
                                            ":/Lines.frag")) {
        close();
    }

    // Link shader pipeline
    if (!m_qLineProgram->link()) {
        close();
    }

    // Bind shader pipeline for use
    if (!m_qLineProgram->bind()) {
        close();
    }

    // Restore system locale
    setlocale(LC_ALL, "");
}

void GLDraw::initShaders3()
{
    m_qPolyProgram = new QGLShaderProgram(this);

    // Override system locale until shaders are compiled
    setlocale(LC_NUMERIC, "C");

    // Compile vertex shader
    if (!m_qPolyProgram->addShaderFromSourceFile(QGLShader::Vertex,
                                            ":/Poly.vert")) {
        close();
    }

    // Compile fragment shader
    if (!m_qPolyProgram->addShaderFromSourceFile(QGLShader::Fragment,
                                            ":/Poly.frag")) {
        close();
    }

    // Link shader pipeline
    if (!m_qPolyProgram->link()) {
        close();
    }

    // Bind shader pipeline for use
    if (!m_qPolyProgram->bind()) {
        close();
    }

    // Restore system locale
    setlocale(LC_ALL, "");
}

void GLDraw::initShaders4()
{
    m_qCanvasProgram = new QGLShaderProgram(this);

    // Override system locale until shaders are compiled
    setlocale(LC_NUMERIC, "C");

    // Compile vertex shader
    if (!m_qCanvasProgram->addShaderFromSourceFile(QGLShader::Vertex,
                                            ":/Coord.vert")) {
        close();
    }

    // Compile fragment shader
    if (!m_qCanvasProgram->addShaderFromSourceFile(QGLShader::Fragment,
                                            ":/Coord.frag")) {
        close();
    }

    // Link shader pipeline
    if (!m_qCanvasProgram->link()) {
        close();
    }

    // Bind shader pipeline for use
    if (!m_qCanvasProgram->bind()) {
        close();
    }

    // Restore system locale
    setlocale(LC_ALL, "");
}

/*
 * create buffer and bind
 */
void GLDraw::initVBO()
{
    /*
     * vertex
     */
    if (m_bVertices) {
        glGenBuffers(1, &m_pVertexBuffer[B_VERTICES]);
        glBindBuffer(GL_ARRAY_BUFFER, m_pVertexBuffer[B_VERTICES]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_nBallNum * 18,
                     m_pGL_dat_points, GL_STATIC_DRAW);

        glGenBuffers(1, &m_pVertexBuffer[B_COLOR]);
        glBindBuffer(GL_ARRAY_BUFFER, m_pVertexBuffer[B_COLOR]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_nBallNum * 24,
                     m_pGL_dat_colors, GL_STATIC_DRAW);

        glGenBuffers(1, &m_pVertexBuffer[B_CENTER]);
        glBindBuffer(GL_ARRAY_BUFFER, m_pVertexBuffer[B_CENTER]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_nBallNum * 18,
                     m_pGL_dat_centers, GL_STATIC_DRAW);

        glGenBuffers(1, &m_pVertexBuffer[B_RADIUS]);
        glBindBuffer(GL_ARRAY_BUFFER, m_pVertexBuffer[B_RADIUS]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_nBallNum * 6,
                     m_pGL_dat_radius, GL_STATIC_DRAW);
    }

    /*
     * edges
     */
    if (m_bEdges) {
        glGenBuffers(1, &m_pVertexBuffer[B_LINES]);
        glBindBuffer(GL_ARRAY_BUFFER, m_pVertexBuffer[B_LINES]);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(GLfloat) * m_nSegment * m_nLineNum * 6,
                     m_pGL_dat_lines, GL_STATIC_DRAW);
    }

    /*
     * polygon
     */
    if (m_bPolygons) {
        glGenBuffers(1, &m_pVertexBuffer[B_POLYGONS]);
        glBindBuffer(GL_ARRAY_BUFFER, m_pVertexBuffer[B_POLYGONS]);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(GLfloat) * (m_nSegment + 1) * m_nLineNum * 6,
                     m_pGL_dat_polygons, GL_STATIC_DRAW);
    }


}

void GLDraw::initializeGL()
{
    initializeGLFunctions();

    if (m_bVertices) {
        initShaders1();
    }

    if (m_bEdges) {
        initShaders2();
    }

    if (m_bPolygons) {
        initShaders3();
    }

    initShaders4();

    initBuffer();

    initVBO();

    initDrawCanvas();
}

void GLDraw::drawCanvas()
{
    /*
     * the antialiasing code
     */

    /*
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    */

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /*
    glPushMatrix();

    int nImageQuality = 8;
    int nLoop = 3;
    float dx, dy;

    glClear(GL_ACCUM_BUFFER_BIT);

    switch(nImageQuality) {
        case 1: nLoop = 1; break;
        case 2: nLoop = 2; break;
        case 3: nLoop = 3; break;
        case 4: nLoop = 4; break;
        case 5: nLoop = 8; break;
        case 6: nLoop = 15; break;
        case 7: nLoop = 24; break;
        case 8: nLoop = 66; break;
        default:nLoop = 1;
    }
    */

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    /*
    for (int mm = 0; mm < nLoop; mm++) {
        glClearColor(0.043, 0.075, 0.3372, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPushMatrix();

        switch(nImageQuality) {
            case 1: dx = 0; dy = 0; break;
            case 2: dx = j2[mm].x; dy = j2[mm].y; break;
            case 3: dx = j3[mm].x; dy = j3[mm].y; break;
            case 4: dx = j4[mm].x; dy = j4[mm].y; break;
            case 5: dx = j8[mm].x; dy = j8[mm].y; break;
            case 6: dx = j15[mm].x; dy = j15[mm].y; break;
            case 7: dx = j24[mm].x; dy = j24[mm].y; break;
            case 8: dx = j66[mm].x; dy = j66[mm].y; break;
            default: break;
        }

        glTranslatef(dx * viewport[2] / viewport[2],
                     dy * viewport[3] / viewport[3],
                     0);
    */
        /*
         * draw edges
         */
        if (m_bEdges) {
            m_qLineProgram->bind();

            glBindBuffer(GL_ARRAY_BUFFER, m_pVertexBuffer[B_LINES]);
            glBufferSubData(GL_ARRAY_BUFFER, 0,
                            sizeof(GLfloat) * m_nSegment * m_nLineNum * 6,
                            m_pGL_dat_lines);

            GLint lPosition = m_qLineProgram->attributeLocation("lPosition");
            glBindBuffer(GL_ARRAY_BUFFER, m_pVertexBuffer[B_LINES]);
            glVertexAttribPointer(lPosition, 3, GL_FLOAT,
                                  GL_FALSE, 0, (GLubyte*)NULL);
            m_qLineProgram->enableAttributeArray(lPosition);

            glLineWidth(1.0);
            glEnable(GL_LINE_SMOOTH);
            glDrawArrays(GL_LINES, 0, m_nSegment * m_nLineNum * 2);
            m_qLineProgram->release();
        }

        /*
         * draw polygons
         */
        if (m_bPolygons) {
            m_qPolyProgram->bind();

            glBindBuffer(GL_ARRAY_BUFFER, m_pVertexBuffer[B_POLYGONS]);
            glBufferSubData(GL_ARRAY_BUFFER, 0,
                            sizeof(GLfloat) * (m_nSegment + 1) * m_nLineNum * 6,
                            m_pGL_dat_polygons);

            GLint pPosition = m_qPolyProgram->attributeLocation("pPosition");
            glBindBuffer(GL_ARRAY_BUFFER, m_pVertexBuffer[B_POLYGONS]);
            glVertexAttribPointer(pPosition, 3, GL_FLOAT,
                                  GL_FALSE, 0, (GLubyte*)NULL);
            m_qPolyProgram->enableAttributeArray(pPosition);

            glEnable(GL_POLYGON_SMOOTH);
            for (int i = 0; i < m_nLineNum; i++) {
                glDrawArrays(GL_TRIANGLE_STRIP,
                             (m_nSegment + 1) * 2 * i,
                             (m_nSegment + 1) * 2);
            }
            m_qPolyProgram->release();
        }

        /*
         * draw balls
         */
        if (m_bVertices) {
            m_qVertexProgram->bind();

            glBindBuffer(GL_ARRAY_BUFFER, m_pVertexBuffer[B_VERTICES]);
            glBufferSubData(GL_ARRAY_BUFFER, 0,
                            sizeof(GLfloat) * m_nBallNum * 18,
                            m_pGL_dat_points);
            glBindBuffer(GL_ARRAY_BUFFER, m_pVertexBuffer[B_COLOR]);
            glBufferSubData(GL_ARRAY_BUFFER, 0,
                            sizeof(GLfloat) * m_nBallNum * 24,
                            m_pGL_dat_colors);
            glBindBuffer(GL_ARRAY_BUFFER, m_pVertexBuffer[B_CENTER]);
            glBufferSubData(GL_ARRAY_BUFFER, 0,
                            sizeof(GLfloat) * m_nBallNum * 18,
                            m_pGL_dat_centers);
            glBindBuffer(GL_ARRAY_BUFFER, m_pVertexBuffer[B_RADIUS]);
            glBufferSubData(GL_ARRAY_BUFFER, 0,
                            sizeof(GLfloat) * m_nBallNum * 6,
                            m_pGL_dat_radius);

            GLint vPosition = m_qVertexProgram->attributeLocation("vPosition");
            glBindBuffer(GL_ARRAY_BUFFER, m_pVertexBuffer[B_VERTICES]);
            glVertexAttribPointer(vPosition, 3, GL_FLOAT,
                                  GL_FALSE, 0, (GLubyte*)NULL);
            m_qVertexProgram->enableAttributeArray(vPosition);

            GLint vColor = m_qVertexProgram->attributeLocation("vColor");
            glBindBuffer(GL_ARRAY_BUFFER, m_pVertexBuffer[B_COLOR]);
            glVertexAttribPointer(vColor, 4, GL_FLOAT,
                                  GL_FALSE, 0, (GLubyte*)NULL);
            m_qVertexProgram->enableAttributeArray(vColor);

            GLint vCenter = m_qVertexProgram->attributeLocation("vCenter");
            glBindBuffer(GL_ARRAY_BUFFER, m_pVertexBuffer[B_CENTER]);
            glVertexAttribPointer(vCenter, 3, GL_FLOAT,
                                  GL_FALSE, 0, (GLubyte*)NULL);
            m_qVertexProgram->enableAttributeArray(vCenter);

            GLint vRadius = m_qVertexProgram->attributeLocation("vRadius");
            glBindBuffer(GL_ARRAY_BUFFER, m_pVertexBuffer[B_RADIUS]);
            glVertexAttribPointer(vRadius, 1, GL_FLOAT,
                                  GL_FALSE, 0, (GLubyte*)NULL);
            m_qVertexProgram->enableAttributeArray(vRadius);

            glDrawArrays(GL_TRIANGLES, 0, 6 * m_nBallNum);

            m_qVertexProgram->release();
        }
    /*
        glPopMatrix();
        glAccum(GL_ACCUM, 1.0/nLoop);
    }
    glAccum(GL_RETURN, 1.0);
    glFinish();
    */

    glDisable(GL_BLEND);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void GLDraw::initDrawCanvas()
{
    glGenBuffers(1, &m_pVertexBuffer[B_CAN]);
    glBindBuffer(GL_ARRAY_BUFFER, m_pVertexBuffer[B_CAN]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_pTexPoints),
                 m_pTexPoints, GL_STATIC_DRAW);

    glGenBuffers(1, &m_pVertexBuffer[B_QUAD]);
    glBindBuffer(GL_ARRAY_BUFFER, m_pVertexBuffer[B_QUAD]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_pQuadPoints),
                 m_pQuadPoints, GL_STATIC_DRAW);

    glActiveTexture(GL_TEXTURE0 + T_CAN);
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &m_pTexBuffer[T_CAN]);
    glBindTexture(GL_TEXTURE_2D, m_pTexBuffer[T_CAN]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glDisable(GL_TEXTURE_2D);

    glGenFramebuffers(1, &m_pFrameBuffer[F_CAN]);
    glBindFramebuffer(GL_FRAMEBUFFER, m_pFrameBuffer[F_CAN]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           m_pTexBuffer[T_CAN], 0);
    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        qDebug() << "Framebuffer not complete.";
    }

    glViewport(0, 0, WIDTH, HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, m_pFrameBuffer[F_CAN]);

    glClearColor(0.043, 0.075, 0.3372, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawCanvas();

    if (m_bEdges && FILENUM != 1) {
        glDeleteBuffers(1, &m_pVertexBuffer[B_LINES]);
    }

    string fname;
    int n = 1;
    for (n = 2; n <= FILENUM; n++) {
        fname = "../";
        ostringstream convert;
        convert.clear();
        convert << n;
        fname.append(convert.str());
        //printf("%s\n", fname.c_str());
        readBuffer(fname);

        /*
         * edges
         */
        if (m_bEdges) {
            glDeleteBuffers(1, &m_pVertexBuffer[B_LINES]);
            glGenBuffers(1, &m_pVertexBuffer[B_LINES]);
            glBindBuffer(GL_ARRAY_BUFFER, m_pVertexBuffer[B_LINES]);
            glBufferData(GL_ARRAY_BUFFER,
                         sizeof(GLfloat) * m_nSegment * m_nLineNum * 6,
                         m_pGL_dat_lines, GL_STATIC_DRAW);
        }

        drawCanvas();
    }
}

void GLDraw::resizeGL(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, 0, HEIGHT, 0, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, w, h);
}

void GLDraw::paintGL()
{
    glViewport(0, 0, 2 * WIDTH, 2 * HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    m_qLineProgram->bind();

    glBindBuffer(GL_ARRAY_BUFFER, m_pVertexBuffer[B_LINES]);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    sizeof(GLfloat) * m_nSegment * m_nLineNum * 6,
                    m_pGL_dat_lines);

    GLint lPosition = m_qLineProgram->attributeLocation("lPosition");
    glBindBuffer(GL_ARRAY_BUFFER, m_pVertexBuffer[B_LINES]);
    glVertexAttribPointer(lPosition, 3, GL_FLOAT,
                          GL_FALSE, 0, (GLubyte*)NULL);
    m_qLineProgram->enableAttributeArray(lPosition);

    glLineWidth(1.0);
    glEnable(GL_LINE_SMOOTH);
    glDrawArrays(GL_LINES, 0, m_nSegment * m_nLineNum * 2);
    m_qLineProgram->release();

    glActiveTexture(GL_TEXTURE0 + T_CAN);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_pTexBuffer[T_CAN]);
    glDisable(GL_TEXTURE_2D);

    m_qCanvasProgram->bind();

    glUniform1i(m_qCanvasProgram->uniformLocation("texture"), T_CAN);

    GLint vTexCoord = m_qCanvasProgram->attributeLocation("vTexCoord");
    glBindBuffer(GL_ARRAY_BUFFER, m_pVertexBuffer[B_CAN]);
    glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
    m_qCanvasProgram->enableAttributeArray(vTexCoord);

    GLint vPosition = m_qCanvasProgram->attributeLocation("vPosition");
    glBindBuffer(GL_ARRAY_BUFFER, m_pVertexBuffer[B_QUAD]);
    glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
    m_qCanvasProgram->enableAttributeArray(vPosition);

    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_QUADS, 0, 4);

    m_qCanvasProgram->release();
}

GLDraw::~GLDraw()
{

}
