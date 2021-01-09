# Header files
HEADERS =  sources/xtoolform.h
HEADERS += sources/serialform.h
HEADERS += sources/serial.h
HEADERS += sources/xtool.h
HEADERS += sources/msgserver.h
HEADERS += sources/transferform.h
HEADERS += sources/xmlparse.h
HEADERS += sources/xmlnode.h
HEADERS += sources/xmlwidget.h

# Resource files
RESOURCES   = xtool.qrc

# Source files
SOURCES =  sources/main.cc
SOURCES += sources/xtoolform.cc
SOURCES += sources/serialform.cc
SOURCES += sources/serial.cc
SOURCES += sources/transferform.cc
SOURCES += sources/xmlparse.cc
SOURCES += sources/xmlnode.cc
SOURCES += sources/xmlwidget.cc


# Link libraries
QT += widgets
QT += core
QT += uitools
QT += serialport
QT += xml

