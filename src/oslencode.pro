TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    BmpParser.cpp \
    main.cpp \
    osfEncoder.cpp

HEADERS += \
    BmpParser.h \
    OsfConfig.h \
    osfEncoder.h


win32 {
    LIBS += -luser32 -lws2_32
}
