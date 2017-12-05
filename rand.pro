TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11

LIBS += -lsfml-system -lsfml-graphics -lsfml-window -lsfml-audio

SOURCES += main.cpp \
            star.cpp \
            textBox.cpp \
            moon.cpp \
    snowSystem.cpp

HEADERS += textBox.h \
            star.h \
            moon.h \
    snowSystem.h

