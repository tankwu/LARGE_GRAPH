TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += qt
QT     += opengl

SOURCES += main.cpp \
    panel.cpp \
    gldraw.cpp

HEADERS += \
    jitter.h \
    panel.h \
    gldraw.h

RESOURCES += \
    shaders.qrc
