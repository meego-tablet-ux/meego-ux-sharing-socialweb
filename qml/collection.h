/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef COLLECTION_H
#define COLLECTION_H

#include <QObject>

#include <libsocialweb-qt/swclientdbustypes.h>

class Collection : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString collName READ getCollName WRITE setCollName NOTIFY collNameChanged);
    Q_PROPERTY(QString collID READ getCollId NOTIFY collIdChanged);
    Q_PROPERTY(QString collParentID READ getCollParentId WRITE setCollParentId NOTIFY collParentIdChanged);
    Q_PROPERTY(MediaTypes collMediaTypes READ getCollMediaTypes WRITE setCollMediaTypes NOTIFY collMediaTypesChanged);
    Q_PROPERTY(uint collItemCount READ getCollItemCount NOTIFY collItemCountChanged);
    Q_PROPERTY(SwParams collAttributes READ getCollAttributes WRITE setCollAttributes NOTIFY collAttributesChanged);

public:

    enum MediaTypes {
        MediaTypeCollections = 1,   //With some services, you can have a collection of collections, or even mixed...
        MediaTypePhotos = 2,
        MediaTypePhotosAndCollections = 3,
        MediaTypeVideos = 4,
        MediaTypeVideosAndCollections = 5,
        MediaTypeVideosAndPhotos = 6,
        MediaTypeVideosPhotosAndCollections = 7,
    };

    Q_ENUMS(MediaTypes);

    Collection(QObject *parent = 0);
    Collection(SwCollectionDetails dets, QObject *parent = 0);

    QString getCollName() const { return mCollName; }
    QString getCollId() const { return mCollID; }
    QString getCollParentId() const { return mCollParentID; }
    MediaTypes getCollMediaTypes() const { return mCollMediaTypes; }
    int getCollItemCount() const { return mCollItemCount; }
    SwParams getCollAttributes() const { return mCollAttributes; }

    void setCollName(const QString &name);
    void setCollParentId(const QString &parentID);
    void setCollMediaTypes(MediaTypes mediaTypes);
    void setCollAttributes(SwParams attribs);

    Q_INVOKABLE QStringList getAttributeNames() const;
    Q_INVOKABLE QString getAttributeValue(const QString &attribName) const;
    Q_INVOKABLE void setAttributeValue(const QString &attribName, const QString &attribValue);

signals:
    void collNameChanged();
    void collIdChanged();
    void collParentIdChanged();
    void collMediaTypesChanged();
    void collItemCountChanged();
    void collAttributesChanged();

private:
    QString mCollID;
    QString mCollName;
    QString mCollParentID;
    MediaTypes mCollMediaTypes;
    int mCollItemCount;
    SwParams mCollAttributes;

};

Q_DECLARE_METATYPE(Collection *);
Q_DECLARE_METATYPE(Collection::MediaTypes);

#endif //COLLECTION_H
