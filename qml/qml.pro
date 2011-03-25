include(../common.pri)
PLUGIN_QML_TARGET_BASE_PATH = $$[QT_INSTALL_IMPORTS]/MeeGo/Sharing/plugins/
TEMPLATE = lib
QT += dbus
CONFIG += plugin link_pkgconfig

PKGCONFIG += libsocialweb-qt

!exists(../interfaces/collections_adaptor.h) {
    system(cd ../interfaces && ./generate-adaptors.sh)
}

!exists(../interfaces/collections_interface.h) {
    system(cd ../interfaces && ./generate-interfaces.sh)
}

INCLUDEPATH += ../interfaces

HEADERS = collections.h \
    collection.h \
    collectionmodel.h \
    qmlreg.h \
    ../interfaces/*_interface.h

OTHER_FILES += *.qml

SOURCES = collections.cpp \
    collection.cpp \
    collectionmodel.cpp \
    qmlreg.cpp \
    ../interfaces/*_interface.cpp

TARGET = Socialweb

qml.files = *.qml libSocialweb.so qmldir
qml.path = $$PLUGIN_QML_TARGET_BASE_PATH/socialweb/

INSTALLS += qml

QMAKE_INSTALL_FILE = cp -fP

#********* Translations *********
PKG_NAME = meego-ux-sharing-socialweb-qml
TRANSLATIONS = $${OTHER_FILES} $${HEADERS} $${SOURCES}

dist.commands += lupdate $${TRANSLATIONS} -ts $${TSDIR}/$${PKG_NAME}.ts
QMAKE_EXTRA_TARGETS += dist
#********* End Translations ******
