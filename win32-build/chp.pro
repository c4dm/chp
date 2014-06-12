TEMPLATE = lib

INCLUDEPATH += ../../sv-dependency-builds/win32-mingw/include
LIBS += ../../sv-dependency-builds/win32-mingw/lib/libvamp-sdk.a -Wl,--version-script=../win32-build/vamp-plugin.map

CONFIG -= qt
CONFIG += plugin release warn_on

TARGET = chp

SOURCES += \
    ../ConstrainedHarmonicPeak.cpp \
    ../plugins.cpp

HEADERS += \
    ../ConstrainedHarmonicPeak.h

