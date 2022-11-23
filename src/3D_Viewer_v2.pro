QT       += core gui

include(QtGifImage/src/gifimage/qtgifimage.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets opengl openglwidgets

CONFIG += c++17 warn_off

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Controller/facade.cpp \
    Model/parsingmodel.cpp \
    OpenGL/openglwidget.cpp \
    Settings/memento.cpp \
    Settings/settings.cpp \
    View/mainwindow.cpp \
    main.cpp

HEADERS += \
    Controller/facade.h \
    Model/parsingmodel.h \
    OpenGL/openglwidget.h \
    Settings/memento.h \
    Settings/settings.h \
    View/mainwindow.h

FORMS += \
    View/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Resources/Icons/arrow_to_down.png \
    Resources/Icons/left_arrow.png \
    Resources/Icons/main.icns \
    Resources/Icons/main.png \
    Resources/Icons/right_arrow.png \
    Resources/fshader.frag \
    Resources/vshader.vert

RESOURCES += \
    Resources/icons.qrc \
    Resources/shader.qrc

ICON = Resources/icons/main.icns
