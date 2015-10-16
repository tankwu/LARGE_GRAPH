#ifndef PANEL
#define PANEL

#include <QMainWindow>
#include <QWidget>
#include <QScrollArea>
#include <QLabel>
#include <QGridLayout>
#include <QDebug>

#include "gldraw.h"

#define WIDTH   400
#define HEIGHT  400

class GLWidget;

class Panel : public QMainWindow
{
    Q_OBJECT

public:

    Panel(QWidget *parent = 0);

protected:

private:

    QWidget         *m_window;

    GLDraw          *m_glDraw;

    QGridLayout     *m_glLayout;

    QScrollArea     *m_glWidgetArea;

};



#endif // PANEL

