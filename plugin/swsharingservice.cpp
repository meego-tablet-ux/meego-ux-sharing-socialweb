/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "swsharingservice.h"
#include "collections_adaptor.h"
#include <libsocialweb-qt/swclientdbustypes.h>

#include <QFile>
#include <libsocialweb-qt/swclientservice.h>
#include <libsocialweb-qt/swclientserviceconfig.h>

SwSharingService::SwSharingService(SwClientService *swService,
                                   MeeGoUXSharingServiceInfo serviceInfo,
                                   QObject *parent) :
    MeeGoUXSharingService(serviceInfo, parent),
    mService(swService),
    mShareID(0)
{
    qDebug() << QString("Starting up SwSharingService with mServiceInfo.serviceName of %1!").arg(mServiceInfo.serviceName);
    connect(mService,
            SIGNAL(PhotoUploadProgress(SwClientService*,int,int,QString)),
            this,
            SLOT(onShareUploadProgress(SwClientService*,int,int,QString)));
    connect(mService,
            SIGNAL(VideoUploadProgress(SwClientService*,int,int,QString)),
            this,
            SLOT(onShareUploadProgress(SwClientService*,int,int,QString)));


    registerDataTypes();    //lsw-qt DBUS data types

    //QDBusConnection::sessionBus().registerObject(GetDBusObjPath().append("/Collections"), this);
    mCollections = new CollectionsAdaptor(this);

    connect(mService,
            SIGNAL(collCreated(SwClientService*,QString,bool)),
            this,
            SLOT(onCollCreated(SwClientService*,QString,bool)));
    connect(mService,
            SIGNAL(collDetailsRetrieved(SwClientService*,SwCollectionDetails,bool)),
            this,
            SLOT(onCollDetailsRetrieved(SwClientService*,SwCollectionDetails,bool)));
    connect(mService,
            SIGNAL(collListRetrieved(SwClientService*,ArrayOfSwCollectionDetails,bool)),
            this,
            SLOT(onCollListRetrieved(SwClientService*,ArrayOfSwCollectionDetails,bool)));

/*    connect(mService,
            SIGNAL(AudioUploadProgress(SwClientService*,int,int,QString)),
            this,
            SLOT(onAudioUploadProgress(SwClientService*,int,int,QString)));*/
}

SwSharingService::~SwSharingService()
{
}

//Begin MeeGoUXSharingService methods

bool SwSharingService::CanShareType(const QString &sharetype)
{
    return (mServiceInfo.serviceTypes.contains(sharetype));
}

bool SwSharingService::CancelShare(int opid)
{
    Q_UNUSED(opid);
    qDebug() << QString("Cannot cancel LSW sharing uploads!");
    return false;
}

uint SwSharingService::GetCredsState()
{
    qDebug() << QString("SwSharingService::GetCredsState with mServiceInfo.serviceName of %1!").arg(mServiceInfo.serviceName);
    return mService->credsState();
}

QString SwSharingService::GetDisplayName()
{
    qDebug() << QString("SwSharingService::GetDisplayName with mServiceInfo.serviceName of %1!").arg(mServiceInfo.serviceName);
    return mService->getDisplayName();
}

QString SwSharingService::GetIconPath()
{
    qDebug() << QString("SwSharingService::GetIconPath with mServiceInfo.serviceName of %1!").arg(mServiceInfo.serviceName);
    return mService->getServiceConfig()->getIconPath();
}

bool SwSharingService::GetServiceAvailable()
{
    qDebug() << QString("SwSharingService::GetServiceAvailable with mServiceInfo.serviceName of %1!").arg(mServiceInfo.serviceName);
    return mService->isConfigured();
}

QString SwSharingService::GetServiceDesc()
{
    qDebug() << QString("SwSharingService::GetServiceDesc with mServiceInfo.serviceName of %1!").arg(mServiceInfo.serviceName);
    return mService->getServiceConfig()->getDescription();
}

QString SwSharingService::GetServiceName()
{
    qDebug() << QString("SwSharingService::GetServiceName with mServiceInfo.serviceName of %1!").arg(mServiceInfo.serviceName);
    return mServiceInfo.serviceName;
}

QString SwSharingService::GetServiceStateText()
{
    qDebug() << QString("SwSharingService::GetServiceStateText with mServiceInfo.serviceName of %1!").arg(mServiceInfo.serviceName);
    //TODO: make meaningful text based on service/creds state
    return QString("Blah");
}

QString SwSharingService::GetServiceType()
{
    qDebug() << QString("SwSharingService::GetServiceType with mServiceInfo.serviceName of %1!").arg(mServiceInfo.serviceName);
    return tr("Web");
}

QString SwSharingService::GetSettingsURI(const QString &platform,
                                       const QString &product)
{
    QString retVal = "";

    if (platform == "MeeGo") {
        if (product == "UX") {
            retVal = QString("meego-qml-launcher --opengl --app meego-ux-settings --fullscreen --cmd showPage --cdata \"Web\ Accounts\" ");
        } else {
            qDebug() << QString("Invalid product in SwSharingService::GetSettingsURI: %1").arg(product);
        }
    } else {
        qDebug() << QString("Invalid platform in SwSharingService::GetSettingsURI: %1").arg(platform);
    }
    return retVal;
}

QString SwSharingService::GetUIName(const QString &widgettype,
                                  const QString &platform,
                                  const QString &product,
                                  const QString &sharetype,
                                  uint sharecount)
{
    qDebug() << QString("SwSharingService::GetUIName with mServiceInfo.serviceName of %1!").arg(mServiceInfo.serviceName);

    //No LSW support for audio yet...
    if (sharetype == MEEGO_SHARE_TYPE_AUDIO)
        return QString();

    QString type;
    QString mult;
    if (widgettype == "QML") {
        if (sharetype == MEEGO_SHARE_TYPE_IMAGE) {
            type = "image";
        } else if (sharetype == MEEGO_SHARE_TYPE_VIDEO) {
            type = "video";
        } else if (sharetype == MEEGO_SHARE_TYPE_AUDIO) {
            type = "audio";
        } else {
            type = QString(sharetype).replace(QString("/"), QString("_"));   //Custom type support
        }

        mult = (sharecount > 1 ? "multi" : "single");
        QString filename = QString("%1/%2/%3_%4_%5_%6_%7").arg(QML_TARGET_BASE_PATH, "socialweb", mServiceInfo.serviceName, platform, product, type, mult);
        //If we don't have a file for this prodct/platform
        if (!QFile::exists(filename + ".qml")) {
            //Try just this product
            filename = QString("%1/%2/%3_%4_%5_%6").arg(QML_TARGET_BASE_PATH, "socialweb", mServiceInfo.serviceName, platform, type, mult);
            if (!QFile::exists(filename + ".qml")) {
                //Otherwise, just return the standard one
                filename = QString("%1/%2/%3_%4_%5").arg(QML_TARGET_BASE_PATH, "socialweb", mServiceInfo.serviceName, type, mult);
            }
        }
        return filename;

    } else {
        //Handle other UI types here...
        return QString();
    }
}

int SwSharingService::Share(const QString &sharetype, ArrayOfShareItemStruct items, QString &errmessage)
{
    if (!CanShareType(sharetype)) {
        errmessage = QString("Invalid share type %1!").arg(sharetype);
	return -1;
    }
    int i = -1;
    if (items.count() < 1) {
        errmessage = "No items to share!";
        return -1;
    }

    foreach(ShareItemStruct sis, items) {
        qDebug() << QString("Received file %1!").arg(sis.shareURI);
        qDebug() << sis.params;
        //LSW can't deal with "file://" URIs, so we trim it to a local file instead
        if (sis.shareURI.startsWith("file://", Qt::CaseInsensitive))
            sis.shareURI = sis.shareURI.remove(0, 7);

        if (sharetype == MEEGO_SHARE_TYPE_IMAGE) {
            i = mService->uploadPhoto(sis.shareURI, sis.params);
        } else if (sharetype == MEEGO_SHARE_TYPE_VIDEO) {
            i = mService->uploadVideo(sis.shareURI, sis.params);
        } else if (sharetype == MEEGO_SHARE_TYPE_AUDIO) {
            //No audio share type in LSW yet...
        }
        //TODO: Figure out how to handle partial success, partial failure...
        //Maybe make Share return a QList< struct {sis.shareURI, shareID, message} > ?
        if (i != -1) {
            mShareOpsMap[mShareID].append(i);
            mShareOpProgressMap[mShareID].append(QPair<int, int>(i, 0));
        } else {
            qDebug() << QString ("Got %1 back from upload attempt").arg(i);
            return i;
        }
    }

    return mShareID++;
}


//Private slots:

void SwSharingService::onShareUploadProgress(SwClientService *service, int opid, int progress, QString message)
{
    int i, j = -1;
    int totalProgress = 0, count = 0;

    for (i = 0; i < mShareOpsMap.count(); ++i) {
        if (mShareOpsMap[i].contains(opid)) {
            j = i;
            break;
        }
    }
    if (j == -1) {
        qDebug() << QString("Couldn't find the aggregate opid for %1 sharing opid %2 in ShareUploadProgress!").arg(service->getServiceName(), opid);
        return;
    }
    QList<QPair<int, int> >::iterator it;

    for (it = mShareOpProgressMap[j].begin(); it != mShareOpProgressMap[j].end(); ++it) {

        if ((*it).first == opid)
            (*it).second = progress;
        if ((*it).second != -1) {
            count++;
            totalProgress += (*it).second;
        }
    }

    emit this->ShareProgress(service->getServiceName(), j, (count == 0 ? 0 : (int)(totalProgress/count)), message);
}

//End MeeGoUXSharingService methods

//Begin CollectionsAdaptor methods

bool SwSharingService::Create(const QString &collection_name,
                                 uint supported_types,
                                 const QString &collection_parent,
                                 SwParams extra_parameters)
{
    return mService->createColl(collection_name,
                                (SwClientService::CollectionMediaTypes)supported_types,
                                collection_parent,
                                extra_parameters);
}

bool SwSharingService::GetList()
{
    return mService->getCollList();
}

bool SwSharingService::GetDetails(const QString &collection_id)
{
    return mService->getCollDetails(collection_id);
}

bool SwSharingService::GetSupportsCollections()
{
    return mService->hasCollections();
}

//Private slots:

void SwSharingService::onCollCreated(SwClientService *service,
                                     const QString &collID,
                                     bool error)
{
    Q_UNUSED(service);
    emit this->CollectionCreated(collID, error);
}

void SwSharingService::onCollDetailsRetrieved(SwClientService *service,
                                              const SwCollectionDetails &collDetails,
                                              bool error)
{
    Q_UNUSED(service);
    emit this->DetailsRetrieved(collDetails, error);
}

void SwSharingService::onCollListRetrieved(SwClientService *service,
                                           const ArrayOfSwCollectionDetails &collList,
                                           bool error)
{
    Q_UNUSED(service);
    qDebug() << QString("SwSharingService::onCollListRetrieved, got %1 albums").arg(collList.count());
    emit this->ListRetrieved(collList, error);
}

//End CollectionsAdaptor methods





