#include <iostream>
#include <QApplication>
#include "panel.h"

using namespace std;

int main(int argc, char **argv)
{
    cout << "test" << endl;

    QApplication app(argc, argv);
    Panel panel;
    panel.show();
    return app.exec();
}
