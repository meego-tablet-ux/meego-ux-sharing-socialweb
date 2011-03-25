/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "collection.h"


Collection::Collection(QObject *parent):
    QObject(parent),
    mCollID(QString()),
    mCollName(QString()),
    mCollParentID(QString()),
    mCollMediaTypes((MediaTypes)0),
    mCollItemCount(-1)
{

}

Collection::Collection(SwCollectionDetails dets, QObject *parent):
    QObject(parent),
    mCollID(dets.collID),
    mCollName(dets.collName),
    mCollParentID(dets.collParentID),
    mCollMediaTypes((MediaTypes)dets.collMediaTypes),
    mCollItemCount(dets.collItemCount),
    mCollAttributes(dets.collAttributes)
{

}

void Collection::setCollName(const QString &name)
{
    mCollName = name;
    emit this->collNameChanged();
}

void Collection::setCollParentId(const QString &parentID)
{
    mCollParentID = parentID;
    emit this->collParentIdChanged();
}

void Collection::setCollMediaTypes(MediaTypes mediaTypes)
{
    mCollMediaTypes = mediaTypes;
    emit this->collMediaTypesChanged();
}

void Collection::setCollAttributes(SwParams attribs)
{
    mCollAttributes = attribs;
    emit this->collAttributesChanged();
}

QStringList Collection::getAttributeNames() const
{
    return mCollAttributes.keys();
}

QString Collection::getAttributeValue(const QString &attribName) const
{
    return mCollAttributes.value(attribName, QString());
}

void Collection::setAttributeValue(const QString &attribName, const QString &attribValue)
{
    mCollAttributes.insert(attribName, attribValue);
}
