# Header files
INCLUDEPATH = sources
HEADERS =

HEADERS += sources/com/serial/serialform.h
HEADERS += sources/com/serial/serial.h
HEADERS += sources/xml/xmlparse.h
HEADERS += sources/xml/xmlnode.h
HEADERS += sources/xml/xmlwidget.h
HEADERS += sources/stp/stp.h

HEADERS += sources/dlist.h
HEADERS += sources/dlistbuf.h
HEADERS += sources/observer.h

HEADERS += sources/transfer/file_transfer.h
HEADERS += sources/transfer/transfer_controller.h
HEADERS += sources/transfer/transfer_view.h

HEADERS += sources/xtool_model.h
HEADERS += sources/xtool_controller.h
HEADERS += sources/xtool_view.h

# Source files
SOURCES =
SOURCES += sources/override.cc
SOURCES += sources/main.cc
SOURCES += sources/dlistbuf.cc

SOURCES += sources/stp/stp.cc

SOURCES += sources/com/serial/serialform.cc
SOURCES += sources/com/serial/serial.cc

SOURCES += sources/xml/xmlparse.cc
SOURCES += sources/xml/xmlnode.cc
SOURCES += sources/xml/xmlwidget.cc

SOURCES += sources/transfer/file_transfer.cc
SOURCES += sources/transfer/transfer_controller.cc
SOURCES += sources/transfer/transfer_view.cc

SOURCES += sources/xtool_model.cc
SOURCES += sources/xtool_controller.cc
SOURCES += sources/xtool_view.cc


# Link libraries
QT += widgets
QT += core
QT += uitools
QT += serialport
QT += xml


# Resource files
RESOURCES   = xtool.qrc

# ICON
RC_ICONS = images/main_icon.ico

# Form
FORMS =
FORMS += forms/transferform.ui
FORMS += forms/serialform.ui
FORMS += forms/xtoolform.ui

