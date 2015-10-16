#include "panel.h"

Panel::Panel(QWidget *parent)
    : QMainWindow(parent)
{
    m_window = new QWidget;
    setCentralWidget(m_window);

    m_glDraw = new GLDraw();
    m_glDraw->resize(WIDTH, HEIGHT);

    m_glWidgetArea = new QScrollArea;
    m_glWidgetArea->setWidget(m_glDraw);
    m_glWidgetArea->setWidgetResizable(false);
    m_glWidgetArea->setMinimumSize(WIDTH, HEIGHT);

    QLabel *label_1 = new QLabel();
    QLabel *label_2 = new QLabel();
    QFont font = QFont("Arial", 12, QFont::Bold);
    label_1->setText("Line Width");
    label_1->setFont(font);
    label_2->setText("Line Segment");
    label_2->setFont(font);

    m_glLayout = new QGridLayout;;
    m_glLayout->addWidget(m_glWidgetArea, 0, 0);
    m_window->setLayout(m_glLayout);
}
