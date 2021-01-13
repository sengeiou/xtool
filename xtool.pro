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
HEADERS += sources/stp.h
HEADERS += sources/abstractport.h
HEADERS += sources/dlist.h
HEADERS += sources/dlistbuf.h
HEADERS += sources/observer.h

# Resource files
RESOURCES   = xtool.qrc

# ICON
RC_ICONS = images/main_icon.ico

# Source files
SOURCES =  sources/override.cc
SOURCES += sources/main.cc
SOURCES += sources/xtoolform.cc
SOURCES += sources/serialform.cc
SOURCES += sources/serial.cc
SOURCES += sources/transferform.cc
SOURCES += sources/xmlparse.cc
SOURCES += sources/xmlnode.cc
SOURCES += sources/xmlwidget.cc
SOURCES += sources/stp.cc
SOURCES += sources/dlistbuf.cc
SOURCES += sources/observer.cc

# Link libraries
QT += widgets
QT += core
QT += uitools
QT += serialport
QT += xml

