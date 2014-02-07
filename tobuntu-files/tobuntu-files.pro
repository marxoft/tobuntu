TEMPLATE = app
TARGET = tobuntu
INSTALLS += target

QT += network xml sql

HEADERS += \
    src/shared/definitions.h \
    src/shared/languagemodel.h \
    src/shared/notifications.h \
    src/shared/screenorientation.h \
    src/shared/screenorientationmodel.h \
    src/shared/selectionmodel.h \
    src/shared/settings.h \
    src/shared/storage.h \
    src/shared/transfermanager.h \
    src/shared/utils.h

SOURCES += \
    src/shared/languagemodel.cpp \
    src/shared/notifications.cpp \
    src/shared/screenorientationmodel.cpp \
    src/shared/selectionmodel.cpp \
    src/shared/settings.cpp \
    src/shared/storage.cpp \
    src/shared/transfermanager.cpp \
    src/shared/utils.cpp

maemo5 {
    system(lupdate src/shared/*.* src/maemo5/*.* -ts translations/maemo5/base.ts)
    system(cp translations/maemo5/base.ts translations/maemo5/en.ts)
    system(lrelease translations/maemo5/en.ts)

    LIBS += -L/usr/lib -lqubuntuone

    INCLUDEPATH += /usr/include/qubuntuone

    CONFIG += link_prl
    PKGCONFIG = libqubuntuone

    QT += maemo5 dbus

    HEADERS += \
        src/shared/nodemodel.h \
        src/shared/transfermodel.h \
        $$files(src/maemo5/*.h)

    SOURCES += \
        src/shared/nodemodel.cpp \
        src/shared/transfermodel.cpp \
        $$files(src/maemo5/*.cpp)

    target.path = /opt/tobuntu/bin

    desktop.files = desktop/maemo5/tobuntu.desktop
    desktop.path = /usr/share/applications/hildon

    icon.files = desktop/maemo5/64x64/tobuntu.png
    icon.path = /usr/share/icons/hicolor/64x64/apps

    translations.files += $$files(translations/maemo5/*.qm)
    translations.path = /opt/tobuntu/translations

    INSTALLS += desktop icon translations
}

contains(MEEGO_EDITION,harmattan) {
    system(lupdate src/shared/*.* src/harmattan/*.* -ts translations/harmattan/base.ts)
    system(cp translations/harmattan/base.ts translations/harmattan/en.ts)
    system(lrelease translations/harmattan/en.ts)

    DEFINES += QML_USER_INTERFACE

    LIBS += -L../qubuntuone/lib -lqubuntuone

    INCLUDEPATH += ../qubuntuone/src

    CONFIG += link_pkgconfig qdeclarative-boostable libtuiclient

    QMAKE_CXXFLAGS += `pkg-config --cflags qdeclarative-boostable`
    QMAKE_LFLAGS += `pkg-config --libs qdeclarative-boostable`

    QT += declarative opengl

    HEADERS += \
        $$files(src/harmattan/*.h)

    SOURCES += \
        $$files(src/harmattan/*.cpp)

    RESOURCES = src/harmattan/resources.qrc

    OTHER_FILES += \
        src/harmattan/*.qml

    target.path = /opt/tobuntu/bin

    desktop.files = desktop/harmattan/tobuntu.desktop
    desktop.path = /usr/share/applications

    icon.files = desktop/harmattan/80x80/tobuntu.png
    icon.path = /usr/share/icons/hicolor/80x80/apps

    splash.files += $$files(desktop/harmattan/splash/*.png)
    splash.path = /opt/tobuntu/splash

    translations.files += $$files(translations/harmattan/*.qm)
    translations.path = /opt/tobuntu/translations

    INSTALLS += desktop icon splash translations
}
