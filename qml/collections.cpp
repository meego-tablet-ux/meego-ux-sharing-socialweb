/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "collections.h"
#include "collection.h"
#include "collectionmodel.h"

#include "collections_interface.h"

#define DBUSOBJPATH QString("/com/meego/ux/sharing/services/%1")

Collections::Collections(QObject *parent):
    QObject(parent),
    mCollModel(new CollectionModel(this, this)),
    mCollIntf(0),
    mSupportsCollections(false)
{
    //Register libsocialweb-qt DBus data types...
    registerDataTypes();
}

bool Collections::getCollList() const
{
    if (mCollIntf && mCollIntf->isValid())
        return mCollIntf->GetList();
    else
        return false;
}

bool Collections::getCollDetails(const QString &collID) const
{
    if (mCollIntf && mCollIntf->isValid())
        return mCollIntf->GetDetails(collID);
    else
        return false;
}

bool Collections::createColl(Collection *coll)
{
    return this->createCollWithParams(coll->getCollName(), coll->getCollParentId(),
                            coll->getCollMediaTypes(), coll->getCollAttributes());
}

bool Collections::createColl(QString collName, QString collParentID,
                             uint collMediaTypes)
{
    SwParams emptyAttribs;
    return this->createCollWithParams(collName, collParentID, collMediaTypes, emptyAttribs);
}

bool Collections::createCollWithParams(QString collName, QString collParentID,
                             uint collMediaTypes, SwParams collAttributes)
{
    if (mCollIntf && mCollIntf->isValid())
        return mCollIntf->Create(collName, collMediaTypes, collParentID, collAttributes);
    else
        return false;
}


//Private slots

void Collections::onCollectionCreated(const QString &collID, bool error)
{
    Q_UNUSED(collID);
    Q_UNUSED(error);
    if (mCollIntf && mCollIntf->isValid())
        mCollIntf->GetList();
}

//Private methods

void Collections::setServiceName(const QString &serviceName)
{
    mServiceName = serviceName;
    if (mCollIntf)
        delete mCollIntf;
    mCollIntf = 0;
    if (mServiceName.isEmpty()) {
        qDebug("Empty service name in Collections::setServiceName");
        mSupportsCollections = false;
    } else {

        mCollIntf = new ComMeegoUxSharingServiceCollectionsInterface
                ("com.meego.ux.sharing", //ComMeegoUxSharingServiceCollectionsInterface::staticInterfaceName(),
                 DBUSOBJPATH.arg(mServiceName), QDBusConnection::sessionBus(), this);
        if (mCollIntf->isValid()) {
            if ((mSupportsCollections = mCollIntf->GetSupportsCollections())) {
                qDebug() << QString("serviceName %1 Supports collections...").arg(mServiceName);
                connect(mCollIntf,
                        SIGNAL(CollectionCreated(const QString &,bool)),
                        this,
                        SIGNAL(collectionCreated(const QString &,bool)));
                connect(mCollIntf,
                        SIGNAL(CollectionCreated(const QString &,bool)),
                        this,
                        SLOT(onCollectionCreated(const QString &,bool)));
                connect(mCollIntf,
                        SIGNAL(ListRetrieved(const ArrayOfSwCollectionDetails &,bool)),
                        this,
                        SIGNAL(collectionListRetrieved(const ArrayOfSwCollectionDetails &,bool)));
                connect(mCollIntf,
                        SIGNAL(DetailsRetrieved(const SwCollectionDetails &,bool)),
                        this,
                        SIGNAL(collectionDetailsRetrieved(const SwCollectionDetails &,bool)));
            }
        } else {
            qDebug() << QString("Invalid collections interface in Collections::setServiceName: %1").arg(mServiceName);
            mSupportsCollections = false;
        }
    }
    emit this->hasCollectionsChanged(mSupportsCollections);
    emit this->serviceNameChanged(mServiceName);
}


