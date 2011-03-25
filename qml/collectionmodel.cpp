/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "collectionmodel.h"
#include "collections.h"
#include "collection.h"

CollectionModel::CollectionModel(QObject *parent):
    QAbstractListModel(parent),
    mCollections(0)
{
    setupRoles();
}

CollectionModel::CollectionModel(Collections *collections, QObject *parent):
    QAbstractListModel(parent)
{
    setupRoles();
    setCollections(collections);
}

int CollectionModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mCollList.count();
}

QVariant CollectionModel::data(const QModelIndex &index, int role) const
{
    int i = index.row();
    if (i < 0 || i >= mCollList.count())
        return QVariant();

    Collection *coll = mCollList.at(i);

    switch (role) {
    case CollectionName:
        return QVariant::fromValue<QString>(coll->getCollName());
        break;
    case CollectionId:
        return QVariant::fromValue<QString>(coll->getCollId());
        break;
    case CollectionParentId:
        return QVariant::fromValue<QString>(coll->getCollParentId());
        break;
    case CollectionMediaTypes:
        return QVariant::fromValue<int>((int)coll->getCollMediaTypes());
        break;
    case CollectionItemCount:
        return QVariant::fromValue<int>(coll->getCollItemCount());
        break;
    case CollectionAttributes:
        return QVariant::fromValue<SwParams>(coll->getCollAttributes());
        break;
    case CollectionObject:
        return QVariant::fromValue<Collection *>(coll);
        break;
    default:
        qDebug() << QString("Unhandled data role requested: %1").arg(role);
    }
    return QVariant();
}

//Private slots

void CollectionModel::onCollectionListRetrieved(const ArrayOfSwCollectionDetails &colls, bool error)
{
    qDebug() << QString("CollectionModel::onCollectionListRetrieved, count: %1").arg(colls.count());
    if (error) {
        qDebug() << QString("Error returned in CollectionModel::onCollectionListRetrieved!");
    } else {
        if (mCollList.count()) {
            this->beginRemoveRows(QModelIndex(), 0, mCollList.count());
            foreach(Collection *coll, mCollList)
                delete coll;
            mCollList.clear();
            this->endRemoveRows();
        }

        this->beginInsertRows(QModelIndex(), 0, colls.count()-1);
        foreach (SwCollectionDetails dets, colls)
            mCollList.append(new Collection(dets, this));
        this->endInsertRows();
    }
}

void CollectionModel::onServiceNameChanged()
{
    if (mCollections)
        mCollections->getCollList();
}

//Private methods

void CollectionModel::setCollections(Collections *collections)
{
    mCollections = collections;
    emit this->collectionsChanged();
    if (mCollections){
        connect(mCollections,
                SIGNAL(collectionListRetrieved(const ArrayOfSwCollectionDetails &,bool)),
                this,
                SLOT(onCollectionListRetrieved(const ArrayOfSwCollectionDetails &,bool)));
        connect(mCollections,
                SIGNAL(serviceNameChanged(QString)),
                this,
                SLOT(onServiceNameChanged()));
    }
    reloadColls();
}

void CollectionModel::reloadColls()
{
    if (mCollections) {
        if (!mCollections->getCollList() && mCollList.count()) {
            this->beginRemoveRows(QModelIndex(), 0, mCollList.count());
            foreach(Collection *coll, mCollList)
                delete coll;
            mCollList.clear();
            this->endRemoveRows();
        }
    }
}

void CollectionModel::setupRoles()
{
    QHash<int, QByteArray> roles;
    roles.insert(CollectionName, "collName");
    roles.insert(CollectionId, "collID");
    roles.insert(CollectionParentId, "collParentID");
    roles.insert(CollectionMediaTypes, "collMediaTypes");
    roles.insert(CollectionItemCount, "collItemCount");
    roles.insert(CollectionAttributes, "collAttribs");
    roles.insert(CollectionObject, "collObj");
    setRoleNames(roles);
}


