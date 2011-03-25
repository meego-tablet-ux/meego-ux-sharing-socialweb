include(common.pri)

PLUGIN_QML_TARGET_BASE_PATH = $$[QT_INSTALL_IMPORTS]/MeeGo/Sharing/plugins/
TEMPLATE = subdirs
SUBDIRS = plugin qml
QT += dbus
CONFIG += plugin

OTHER_FILES += interfaces/*.xml

#********* Translations *********
dist.commands += rm -Rf $${DISTDIR} &&
dist.commands += git clone . $${DISTDIR} &&
dist.commands += rm -Rf $${DISTDIR}/.git &&
dist.commands += mkdir -p $${TSDIR} &&
dist.commands += cd plugin && make dist && cd .. &&
dist.commands += cd qml && make dist && cd .. &&
dist.commands += tar jcpvf $${PROJECT_NAME}-$${VERSION}.tar.bz2 $${PROJECT_NAME}-$${VERSION}
QMAKE_EXTRA_TARGETS += dist
#********* End Translations ******
