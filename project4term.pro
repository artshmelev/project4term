TARGET = project4term
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    structures.cpp \
    triangleaction.cpp \
    mainaction.cpp \
    isolineaction.cpp
HEADERS += mainwindow.h \
    structures.h \
    triangleaction.h \
    isolineaction.h \
    mainaction.h
FORMS += mainwindow.ui
QMAKE_CXXFLAGS += -std=c++0x
