/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */


#ifndef COLLECTIONMODEL_H
#define COLLECTIONMODEL_H

#include <QObject>
#include <QVariant>
#include <QAbstractListModel>

#include <libsocialweb-qt/swclientdbustypes.h>

class Collections;
class Collection;

class CollectionModel : public QAbstractListModel {
    Q_OBJECT

    Q_PROPERTY(Collections *collections READ getCollections WRITE setCollections NOTIFY collectionsChanged);

    enum CollectionRoles {
        CollectionName = Qt::DisplayRole,
        CollectionId = Qt::UserRole,
        CollectionParentId,
        CollectionMediaTypes,
        CollectionItemCount,
        CollectionAttributes,
        CollectionObject,
    };

public:

    CollectionModel(QObject *parent = 0);
    CollectionModel(Collections *collections, QObject *parent);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

signals:
    void collectionsChanged();

private slots:
    void onCollectionListRetrieved(const ArrayOfSwCollectionDetails &colls, bool error);
    void onServiceNameChanged();

private:
    Collections * getCollections() const { return mCollections; }
    void setCollections(Collections *collections);
    void reloadColls();
    void setupRoles();

    Collections *mCollections;
    QList<Collection *> mCollList;

};


#endif //COLLECTIONMODEL_H
