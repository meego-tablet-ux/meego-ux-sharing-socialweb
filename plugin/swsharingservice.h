/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef EXAMPLESERVICE_H
#define EXAMPLESERVICE_H

#include <QObject>
#include <QMetaType>
#include <meegouxsharingservice.h>
#include <meegouxsharingcommon.h>

#include <libsocialweb-qt/swclientdbustypes.h>

class SwClientService;
class CollectionsAdaptor;

class SwSharingService : public MeeGoUXSharingService
{
    Q_OBJECT
public:
    explicit SwSharingService(SwClientService *swService, MeeGoUXSharingServiceInfo serviceInfo, QObject *parent = 0);
    ~SwSharingService();

public slots:
    //For ServiceAdaptor auto-invoking
    bool CanShareType(const QString &sharetype);
    bool CancelShare(int opid);
    uint GetCredsState();
    QString GetDisplayName();
    QString GetIconPath();
    bool GetServiceAvailable();
    QString GetServiceDesc();
    QString GetServiceName();
    QString GetServiceStateText();
    QString GetServiceType();
    QString GetSettingsURI(const QString &platform, const QString &product);
    QString GetUIName(const QString &widgettype, const QString &platform, const QString &product, const QString &sharetype, uint sharecount);
    int Share(const QString &sharetype, ArrayOfShareItemStruct items, QString &errmessage);


    //For CollectionsAdaptor auto-invoking
    bool Create(const QString &collection_name, uint supported_types, const QString &collection_parent, SwParams extra_parameters);
    bool GetDetails(const QString &collection_id);
    bool GetList();
    bool GetSupportsCollections();



signals:
    //For ServiceAdaptor auto-connecting
    void ServiceChanged(const QString &service);
    void ShareProgress(const QString &service, int opid, int progress, const QString &message);

    //For CollectionsAdaptor auto-connecting
    void CollectionCreated(const QString &collID, bool error);
    void DetailsRetrieved(const SwCollectionDetails &collDetails, bool error);
    void ListRetrieved(const ArrayOfSwCollectionDetails &collList, bool error);


private slots:
    void onShareUploadProgress(SwClientService *service, int opid, int progress, QString message);

    void onCollCreated(SwClientService *service, const QString &collID, bool error);
    void onCollDetailsRetrieved(SwClientService *service,
                                const SwCollectionDetails &collDetails, bool error);
    void onCollListRetrieved(SwClientService *service,
                             const ArrayOfSwCollectionDetails &collList, bool error);

private:
    SwClientService *mService;
    CollectionsAdaptor *mCollections;
    QMap<int, QList<int> > mShareOpsMap;
    int mShareID;
    QMap<int, QList<QPair<int, int> > > mShareOpProgressMap;

};

Q_DECLARE_METATYPE(SwSharingService *)

#endif // EXAMPLESERVICE_H
