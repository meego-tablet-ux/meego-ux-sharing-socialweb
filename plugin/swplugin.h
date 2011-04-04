/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <meegouxsharingserviceplugininterface.h>
#include <QObject>
#include <QStringList>
#include <meegouxsharingservice.h>

#include "swsharingservice.h"

class SwClient;
class QTranslator;

class SwPlugin : public MeeGoUXSharingServicePluginInterface
{
    Q_OBJECT
    Q_INTERFACES(MeeGoUXSharingServicePluginInterface)

public:
    SwPlugin(QObject *parent = 0);
    ~SwPlugin();
    bool init();
    QHash<QString, QString> metaData();
    QList<MeeGoUXSharingServiceInfo> getServices();

    int Share(QString serviceName, const QString &sharingType, ArrayOfShareItemStruct items, QString &errmessage);
    bool CancelShare(QString serviceName, int opid);

signals:
    void ServiceChanged(QString serviceName);

private slots:
    void onServiceShareProgress(QString serviceName, int opid, int progress, QString message);

private:
    QList<MeeGoUXSharingServiceInfo> mServiceInfoList;
    QList<SwSharingService *> mServices;
    SwClient *mClient;
    QTranslator *mTranslator;
    int mShareID;
    QMap<int, int> mShareOpsMap;

};
