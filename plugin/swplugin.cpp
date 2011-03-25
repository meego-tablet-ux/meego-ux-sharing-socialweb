/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "swplugin.h"
#include "meegouxsharingdbustypes.h"

#include <QDebug>
#include <libsocialweb-qt/swclient.h>
#include <libsocialweb-qt/swclientservice.h>
#include <QTranslator>
#include <QLibraryInfo>
#include <QCoreApplication>


SwPlugin::SwPlugin(QObject *parent):
        MeeGoUXSharingServicePluginInterface(parent),
        mClient(new SwClient(QDBusConnection::sessionBus(), this)),
        mTranslator(new QTranslator(this)),
        mShareID(0)
{
}

SwPlugin::~SwPlugin()
{
}

bool SwPlugin::init()
{
    qDebug() << "SwPlugin Plugin initialization";

    foreach (QString svcName, mClient->getServices()) {
        SwClientService *svc = mClient->getService(svcName);
        if ((svc) && (svc->hasPhotoUpload() || svc->hasVideoUpload())) {
            MeeGoUXSharingServiceInfo info;
            info.serviceName = svcName;
            if (svc->hasPhotoUpload()) {
                info.serviceTypes << MEEGO_SHARE_TYPE_IMAGE;
            }
            if (svc->hasVideoUpload()) {
                //2011-03-18, JEA: Temp workaround for missing smugmug & pb video upload UIs
                if (svcName != "smugmug" && svcName != "photobucket")
                    info.serviceTypes << MEEGO_SHARE_TYPE_VIDEO;
            }
            //Currently no lsw services have an audio upload capability,
            //or even an interface defined for one...
            if (0) {
                info.serviceTypes << MEEGO_SHARE_TYPE_AUDIO;
            }
            mServiceInfoList.append(info);
            SwSharingService *shareSvc = new SwSharingService(svc, info, this);
            mServices.append(shareSvc);

            connect(shareSvc,
                    SIGNAL(ShareProgress(QString,int,int,QString)),
                    this,
                    SLOT(onServiceShareProgress(QString,int,int,QString)));
        }
    }


    QString catName = QString("%1_%2").arg("meego-ux-sharing-socialweb-plugin", QLocale::system().name());
    mTranslator->load(catName, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    QCoreApplication::installTranslator(mTranslator);

    return true;
}

QHash<QString, QString> SwPlugin::metaData()
{
    QHash<QString, QString> info;
    info[MEEGO_UX_SHARING_SERVICE_PLUGIN_NAME]    = QString("meego-ux-sharing-socialweb");
    info[MEEGO_UX_SHARING_SERVICE_PLUGIN_VERSION] = QString("0.1.3");
    info[MEEGO_UX_SHARING_SERVICE_PLUGIN_DESC]    = QString("libsocialweb plugin for meego-ux-sharing");
    return info;
}

QList<MeeGoUXSharingServiceInfo> SwPlugin::getServices()
{
    return mServiceInfoList;
}

int SwPlugin::Share(QString serviceName, const QString &sharingType, ArrayOfShareItemStruct items, QString &errmessage)
{
    int i = -1;
    foreach (SwSharingService *svc, mServices) {
        if (svc->GetServiceName() == serviceName) {
            i = svc->Share(sharingType, items, errmessage);
            if (i != -1) {
                mShareOpsMap[mShareID] = i;
                return mShareID++;
            } else {
                return -1;
            }

        }
    }
    errmessage = QString("Invalid service %1 specified!").arg(serviceName);
    return -1;
}

bool SwPlugin::CancelShare(QString serviceName, int opid)
{
    if (!mShareOpsMap.contains(opid))
        return false;
    foreach (SwSharingService *svc, mServices) {
        if (svc->GetServiceName() == serviceName)
            return svc->CancelShare(mShareOpsMap[opid]);
    }
    qWarning() << QString("Invalid service %1 specified in CancelShare for opid %2!").arg(serviceName, QString::number(opid));
    return false;
}

//Private slots:

void SwPlugin::onServiceShareProgress(QString serviceName, int opid, int progress, QString message)
{
    int i, j = -1;
    for (i = 0; i < mShareOpsMap.count(); ++i) {
        if (mShareOpsMap[i] == opid) {
            j = i;
            break;
        }
    }
    if (j == -1) {
        qDebug() << QString("Unmapped opid %1 for service %2 in onServiceShareProgress!").arg(QString::number(opid), serviceName);
        return;
    }
    emit this->ShareProgress(serviceName, j, progress, message);
}



Q_EXPORT_PLUGIN2(swplugin, SwPlugin);

