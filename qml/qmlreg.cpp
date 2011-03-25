/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "qmlreg.h"

#include "collection.h"
#include "collections.h"
#include "collectionmodel.h"

void QmlReg::registerTypes(const char *uri)
{
    qmlRegisterType<Collection>(uri, 0, 0, "Collection");
    qmlRegisterType<Collections>(uri, 0, 0, "Collections");
    qmlRegisterType<CollectionModel>(uri, 0, 0, "CollectionModel");
}

Q_EXPORT_PLUGIN(QmlReg);
