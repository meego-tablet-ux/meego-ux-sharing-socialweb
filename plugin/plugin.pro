include(../common.pri)
MSD_PLUGIN_PATH=/usr/lib/meego-ux-sharing/plugins
TEMPLATE = lib
QT += dbus
CONFIG += plugin link_pkgconfig

#For some reason, moc doesn't pick up the cflags provided by pkgconfig.
#So, we add them by hand. I'm probably doing something wrong here - please
#let me know if you know the Right Way to do this - james.ausmus@intel.com

INCLUDEPATH += $$system(pkg-config --cflags libmeegouxsharingserviceplugin \
| tr \' \' \'\\n\' | grep ^-I | cut -d 'I' -f 2-)

INCLUDEPATH += ../interfaces

PKGCONFIG += libmeegouxsharingserviceplugin libsocialweb-qt

!exists(../interfaces/collections_adaptor.h) {
    system(cd ../interfaces && ./generate-adaptors.sh)
}

!exists(../interfaces/collections_interface.h) {
    system(cd ../interfaces && ./generate-interfaces.sh)
}

ACTUAL_HEADERS = swplugin.h \
    swsharingservice.h

HEADERS = $${ACTUAL_HEADERS} \
    ../interfaces/*_adaptor.h

ACTUAL_SOURCES = swplugin.cpp \
    swsharingservice.cpp

SOURCES  = $${ACTUAL_SOURCES} \
    ../interfaces/*_adaptor.cpp

TARGET = swsharingplugin
target.path = $$MSD_PLUGIN_PATH

INSTALLS += target

#********* Translations *********
TRANSLATIONS += $${ACTUAL_SOURCES} $${ACTUAL_HEADERS} $${OTHER_FILES}
PKG_NAME = meego-ux-sharing-socialweb-plugin

dist.commands += lupdate $${TRANSLATIONS} -ts $${TSDIR}/$${PKG_NAME}.ts
QMAKE_EXTRA_TARGETS += dist
#********* End Translations ******
