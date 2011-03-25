/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -N -i libsocialweb-qt/swclientdbustypes.h -a ../interfaces/collections_adaptor.h:../interfaces/collections_adaptor.cpp ../interfaces/collections.xml
 *
 * qdbusxml2cpp is Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#ifndef COLLECTIONS_ADAPTOR_H_1299697758
#define COLLECTIONS_ADAPTOR_H_1299697758

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
#include "libsocialweb-qt/swclientdbustypes.h"
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;

/*
 * Adaptor class for interface com.meego.ux.sharing.service.collections
 */
class CollectionsAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.meego.ux.sharing.service.collections")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"com.meego.ux.sharing.service.collections\">\n"
"    <doc:doc>\n"
"      <doc:summary>An interface representing named collections of\n"
"      photos (eg. albums), videos (eg. channels) or both (eg. Flickr sets).\n"
"      </doc:summary>\n"
"    </doc:doc>\n"
"    <method name=\"GetSupportsCollections\">\n"
"      <doc:doc>\n"
"        <doc:description>\n"
"          <doc:para>\n"
"            Returns whether the service supports the Collections interface. If not, none of the other methods will function.\n"
"          </doc:para>\n"
"        </doc:description>\n"
"      </doc:doc>\n"
"      <arg direction=\"out\" type=\"b\" name=\"SupportsCollections\"/>\n"
"    </method>\n"
"    <signal name=\"ListRetrieved\">\n"
"      <annotation value=\"ArrayOfSwCollectionDetails\" name=\"com.trolltech.QtDBus.QtTypeName.In0\"/>\n"
"      <arg direction=\"out\" tp:type=\"Collection_Details[]\" type=\"a(sssuia{ss})\" name=\"collection_list\">\n"
"        <doc:doc>\n"
"          <doc:summary>A list of <tp:member-ref>Collection_Details</tp:member-ref>\n"
"              representing remote collections on the service.</doc:summary>\n"
"        </doc:doc>\n"
"      </arg>\n"
"      <arg direction=\"out\" type=\"b\" name=\"error\"/>\n"
"    </signal>\n"
"    <method name=\"GetList\" tp:name-for-bindings=\"Get_List\">\n"
"      <doc:doc>\n"
"        <doc:description>\n"
"          <doc:para>\n"
"            Retrieve a list of photo/video collections the user has on the service.\n"
"          </doc:para>\n"
"        </doc:description>\n"
"      </doc:doc>\n"
"      <arg direction=\"out\" type=\"b\" name=\"success\">\n"
"        <doc:doc>\n"
"          <doc:summary>\n"
"            Whether the async call was invoked successfully\n"
"          </doc:summary>\n"
"        </doc:doc>\n"
"      </arg>\n"
"    </method>\n"
"    <signal name=\"DetailsRetrieved\">\n"
"      <annotation value=\"SwCollectionDetails\" name=\"com.trolltech.QtDBus.QtTypeName.In0\"/>\n"
"      <arg direction=\"out\" tp:type=\"Collection_Details[]\" type=\"(sssuia{ss})\" name=\"collection_details\">\n"
"        <doc:doc>\n"
"          <doc:summary>The <tp:member-ref>Collection_Details</tp:member-ref>\n"
"              of the given collection.</doc:summary>\n"
"        </doc:doc>\n"
"      </arg>\n"
"      <arg direction=\"out\" type=\"b\" name=\"error\"/>\n"
"    </signal>\n"
"    <method name=\"GetDetails\" tp:name-for-bindings=\"Get_Details\">\n"
"      <doc:doc>\n"
"        <doc:description>\n"
"          <doc:para>\n"
"            Get details of given collection.\n"
"          </doc:para>\n"
"        </doc:description>\n"
"      </doc:doc>\n"
"      <arg direction=\"in\" type=\"s\" name=\"collection_id\">\n"
"        <doc:doc>\n"
"          <doc:summary>\n"
"            The service specific identifier of the collection.\n"
"          </doc:summary>\n"
"        </doc:doc>\n"
"      </arg>\n"
"      <arg direction=\"out\" type=\"b\" name=\"success\">\n"
"        <doc:doc>\n"
"          <doc:summary>\n"
"            Whether the async call was invoked successfully\n"
"          </doc:summary>\n"
"        </doc:doc>\n"
"      </arg>\n"
"    </method>\n"
"    <signal name=\"CollectionCreated\">\n"
"      <arg direction=\"out\" type=\"s\" name=\"collID\"/>\n"
"      <arg direction=\"out\" type=\"b\" name=\"error\"/>\n"
"    </signal>\n"
"    <method name=\"Create\" tp:name-for-bindings=\"Create\">\n"
"      <doc:doc>\n"
"        <doc:description>\n"
"          <doc:para>\n"
"            Create a new collection - returns false if we don't have collections\n"
"            support, or if the async invokation failed. Will emit a\n"
"            CollectionCreated signal on return of the async invokation, if successful.\n"
"          </doc:para>\n"
"        </doc:description>\n"
"      </doc:doc>\n"
"      <annotation value=\"SwParams\" name=\"com.trolltech.QtDBus.QtTypeName.In3\"/>\n"
"      <arg direction=\"out\" type=\"b\" name=\"success\">\n"
"        <doc:doc>\n"
"          <doc:summary>\n"
"            Whether the async call was invoked successfully\n"
"          </doc:summary>\n"
"        </doc:doc>\n"
"      </arg>\n"
"      <arg direction=\"in\" type=\"s\" name=\"collection_name\">\n"
"        <doc:doc>\n"
"          <doc:summary>The name of the new collection.</doc:summary>\n"
"        </doc:doc>\n"
"      </arg>\n"
"      <arg direction=\"in\" tp:type=\"Collection_Media_Types\" type=\"u\" name=\"supported_types\">\n"
"        <doc:doc>\n"
"          <doc:summary>The media types this collection should support.\n"
"          </doc:summary>\n"
"        </doc:doc>\n"
"      </arg>\n"
"      <arg direction=\"in\" type=\"s\" name=\"collection_parent\">\n"
"        <doc:doc>\n"
"          <doc:summary>The service specific identifier of the new\n"
"          collection's parent or an empty string for a top-level\n"
"          collection.</doc:summary>\n"
"        </doc:doc>\n"
"      </arg>\n"
"      <arg direction=\"in\" type=\"a{ss}\" name=\"extra_parameters\">\n"
"        <doc:doc>\n"
"          <doc:summary>Extra, service specific parameters,\n"
"            such as \"description\".</doc:summary>\n"
"        </doc:doc>\n"
"      </arg>\n"
"    </method>\n"
"    <tp:struct array-name=\"Collection_Details_List\" name=\"Collection_Details\">\n"
"      <tp:member type=\"s\" name=\"Collection_Id\">\n"
"        <doc:doc>A service-specific identifier for the collection.</doc:doc>\n"
"      </tp:member>\n"
"      <tp:member type=\"s\" name=\"Collection_Name\">\n"
"        <doc:doc>The human readable name of the collection.</doc:doc>\n"
"      </tp:member>\n"
"      <tp:member type=\"s\" name=\"Collection_Parent\">\n"
"        <doc:doc>A service-specific identifier for the collection's\n"
"        parent, or an empty string for a top-level collection.</doc:doc>\n"
"      </tp:member>\n"
"      <tp:member tp:name=\"Collection_Media_Types\" type=\"u\" name=\"Supported_Media_Types\">\n"
"        <doc:doc>Supported types of media in this collection.</doc:doc>\n"
"      </tp:member>\n"
"      <tp:member type=\"i\" name=\"Item_Count\">\n"
"        <doc:doc>The number of items currently in the collection, or -1 if unknown.</doc:doc>\n"
"      </tp:member>\n"
"      <tp:member type=\"a{ss}\" name=\"Attributes\">\n"
"        <doc:doc>Additional collection attributes.</doc:doc>\n"
"      </tp:member>\n"
"    </tp:struct>\n"
"    <tp:flags value-prefix=\"Collection_Media_Type\" name=\"Collection_Media_Types\">\n"
"      <tp:flag value=\"1\" name=\"Collections\"/>\n"
"      <tp:flag value=\"2\" name=\"Photos\"/>\n"
"      <tp:flag value=\"4\" name=\"Videos\"/>\n"
"    </tp:flags>\n"
"  </interface>\n"
        "")
public:
    CollectionsAdaptor(QObject *parent);
    virtual ~CollectionsAdaptor();

public: // PROPERTIES
public Q_SLOTS: // METHODS
    bool Create(const QString &collection_name, uint supported_types, const QString &collection_parent, SwParams extra_parameters);
    bool GetDetails(const QString &collection_id);
    bool GetList();
    bool GetSupportsCollections();
Q_SIGNALS: // SIGNALS
    void CollectionCreated(const QString &collID, bool error);
    void DetailsRetrieved(const SwCollectionDetails &collection_details, bool error);
    void ListRetrieved(const ArrayOfSwCollectionDetails &collection_list, bool error);
};

#endif
