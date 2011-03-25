/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef COLLECTIONS_H
#define COLLECTIONS_H

#include <QObject>

#include <libsocialweb-qt/swclientdbustypes.h>
#include "collection.h"

class CollectionModel;
class ComMeegoUxSharingServiceCollectionsInterface;

class Collections : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString serviceName READ getServiceName WRITE setServiceName NOTIFY serviceNameChanged);
    Q_PROPERTY(bool hasCollections READ getHasCollections NOTIFY hasCollectionsChanged);

public:
    Collections(QObject *parent = 0);

    Q_INVOKABLE CollectionModel *getCollModel() const { return mCollModel; }
    Q_INVOKABLE bool getCollList() const;
    Q_INVOKABLE bool getCollDetails(const QString &collID) const;
    Q_INVOKABLE bool createColl(Collection *coll);
    Q_INVOKABLE bool createColl(QString collName, QString collParentID,
                                uint collMediaTypes);    //QML isn't liking using Collection::MediaTypes as a param type... :(
    Q_INVOKABLE bool createCollWithParams(QString collName, QString collParentID,
                                /*Collection::MediaTypes*/ uint collMediaTypes, SwParams collAttributes);

signals:
    void serviceNameChanged(QString serviceName);
    void hasCollectionsChanged(bool hasCollections);
    void collectionCreated(const QString &collID, bool error);
    void collectionListRetrieved(const ArrayOfSwCollectionDetails &collList, bool error);
    void collectionDetailsRetrieved(const SwCollectionDetails &collDetails, bool error);

private slots:
    void onCollectionCreated(const QString &collID, bool error);

private:
    const QString& getServiceName() const { return mServiceName; }
    bool getHasCollections() const { return mSupportsCollections; }

    void setServiceName(const QString &serviceName);

    CollectionModel *mCollModel;
    ComMeegoUxSharingServiceCollectionsInterface *mCollIntf;
    QString mServiceName;
    bool mSupportsCollections;

};

#endif //COLLECTIONS_H
