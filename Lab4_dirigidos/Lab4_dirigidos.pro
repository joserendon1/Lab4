TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        conexion_dirigida.cpp \
        enrutador_dirigido.cpp \
        main.cpp \
        menu_dirigido.cpp \
        red_dirigida.cpp

HEADERS += \
    conexion_dirigida.h \
    enrutador_dirigido.h \
    menu_dirigido.h \
    red_dirigida.h
