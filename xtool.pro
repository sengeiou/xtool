# Header files
INCLUDEPATH = sources
HEADERS =

HEADERS += sources/test/test_controller.h
HEADERS += sources/test/test_model.h
HEADERS += sources/test/testform_view.h
HEADERS += sources/com/serial/serialport_view.h
HEADERS += sources/com/serial/serialport_controller.h
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

SOURCES += sources/test/test_controller.cc
SOURCES += sources/test/test_model.cc
SOURCES += sources/test/testform_view.cc
SOURCES += sources/main.cc
SOURCES += sources/dlistbuf.cc
SOURCES += sources/stp/stp.cc
SOURCES += sources/com/serial/serialport_view.cc
SOURCES += sources/com/serial/serialport_controller.cc
SOURCES += sources/com/serial/serial.cc
SOURCES += sources/xml/xmlparse.cc
SOURCES += sources/xml/xmlnode.cc
# SOURCES += sources/xml/xmlwidget.cc
SOURCES += sources/transfer/file_transfer.cc
SOURCES += sources/transfer/transfer_controller.cc
SOURCES += sources/transfer/transfer_view.cc
SOURCES += sources/xtool_model.cc
SOURCES += sources/xtool_controller.cc
SOURCES += sources/xtool_view.cc

SOURCES += sources/protobuf/ota.pb.cc
SOURCES += sources/protobuf/remind.pb.cc
SOURCES += sources/protobuf/devinfo.pb.cc
SOURCES += sources/protobuf/time.pb.cc

# Form
FORMS =

FORMS += forms/deviceform.ui
FORMS += forms/messageform.ui
FORMS += forms/callform.ui
FORMS += forms/testform.ui
FORMS += forms/transferform.ui
FORMS += forms/serialform.ui
FORMS += forms/xtoolform.ui

# Resource files
RESOURCES = xtool.qrc

# ICON
RC_ICONS = images/main_icon.ico


# C++ Build flags
unix:!macx {
QMAKE_CXXFLAGS += -Wno-deprecated-copy
QMAKE_CXXFLAGS += -Wno-class-memaccess
LIBS += -L/usr/local/lib -lprotobuf
}

QMAKE_CXXFLAGS  += -DCONFIG_PROTOBUF

# Link libraries
QT += widgets
QT += core
QT += uitools
QT += serialport
QT += xml
