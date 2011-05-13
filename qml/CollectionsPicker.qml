/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Components 0.1
import MeeGo.Sharing 0.1

Item  {
    id: collectionsPicker

    anchors.centerIn: parent

    height: 450
    width: childrenRect.width + (defaultMargin * 2)

    state: "loading"

    property string service: sharingObj.serviceName
    property int defaultMargin: 15
    property QtObject currentCollectionObj: null
    property alias hasCollections: collections.hasCollections
    property bool isLoading: false

    // Signal to notify that the user has cancelled the operation.
    signal cancel()

    // Signal to notify the current operation's ID.
    signal collectionSelected(string name)

    // Signal to notify a possible error.
    signal collectionError(string errMsg)

    Theme {
        id: theme
    }

    Translator {
        catalog: "meego-ux-sharing-socialweb-qml"
    }

    Text {
        id: titleText

        font.pixelSize: theme.fontPixelSizeLarge
        font.family: theme.fontFamily
        color: theme.fontColorNormal

        width: parent.width
        wrapMode: Text.Wrap
        horizontalAlignment: Text.AlignHCenter

        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
            margins: defaultMargin
        }
    }

    Text {
        id: descriptionText

        text: qsTr("Choose an album to upload to")

        font.pixelSize: theme.fontPixelSizeMedium
        font.family: theme.fontFamily
        color: theme.fontColorNormal

        width: parent.width
        wrapMode: Text.Wrap

        anchors {
            top: titleText.bottom
            left: parent.left
            margins: defaultMargin
        }
    }


    ListView {
        id: collectionsList

        width: parent.width - 60

        clip: true
        focus: true
        currentIndex: -1

        model: collections.getCollModel()

        delegate: BorderImage {
            id: itemSurface

            width: parent.width
            height: 50

            border.left: 5; border.top: 5
            border.right: 5; border.bottom: 5

            source: {
                if (itemSurface.ListView.isCurrentItem)
                    return "image://theme/media/music_row_highlight_landscape"
                else
                    return "image://theme/media/music_row_landscape"
            }

            Text {
                text: collName

                font.pixelSize: theme.fontPixelSizeMediumLarge
                font.family: theme.fontFamily
                color: theme.fontColorNormal

                elide: Text.ElideRight

                anchors {
                    centerIn: parent
                    margins: defaultMargin
                }
            }

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    collectionsList.currentIndex = model.index
                    collectionsPicker.currentCollectionObj = collObj
                    applyButton.active = true
                }
            }
        }

        Spinner {
            id: spinner

            visible: collectionsPicker.isLoading
            spinning: collectionsPicker.isLoading

            onSpinningChanged: {
                if (!spinning)
                    spinning = true
            }
        }

        anchors {
            top: descriptionText.bottom
            bottom: newButton.top
            horizontalCenter: parent.horizontalCenter
            margins: defaultMargin
        }
    }

    Button {
        id: newButton

        width: parent.width - 60

        anchors {
            bottom: buttonGroup.top
            horizontalCenter: parent.horizontalCenter
            margins: defaultMargin
        }

        text: qsTr("Add a new album")

        onClicked: collectionsPicker.state = "create"
    }

    Item {
        id: buttonGroup

        height: childrenRect.height
        width: childrenRect.width

        anchors {
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
            margins: defaultMargin
        }

        Button {
            id: applyButton

            anchors.bottom: parent.bottom

            text: qsTr("Add a title")

            active: false

            onClicked: {
                // Add the collection to each file.
                for (var i = 0; i < sharingObj.fileCount; ++i)
                    sharingObj.addHashEntryToFile(sharingObj.filesToShare[i],
                    "collection", currentCollectionObj.collID)

                // Notify other screens which album was selected.
                collectionsPicker.collectionSelected(
                    collectionsPicker.currentCollectionObj.collName)
            }
        }

        Button {
            id: cancelButton

            text: qsTr("Cancel")

            anchors {
                bottom: parent.bottom
                left: applyButton.right
                leftMargin: defaultMargin
            }

            onClicked: {
                // User cancelled the operation.
                collectionsPicker.cancel();
            }
        }
    }

    // Create Collections component.
    Collections {
        id: collections

        serviceName: sharingObj.serviceName

        onCollectionListRetrieved: {
            if (collectionsPicker.visible)
                collectionsPicker.state = "chooser"
        }
    }

    CreateCollection {
        id: newCollection

        parent: customLoader
        service: collectionsPicker.service

        onCancel: collectionsPicker.state = "chooser"
        onCollectionCreated: collectionsPicker.state = "loading"
        onCreationError: collectionsPicker.collectionError(errMsg)
    }

    // Declare possible states.
    states: [
        // State to display the CollectionsPicker view.
        State {
            name: "chooser"

            PropertyChanges {
                target: collectionsPicker
                visible: true
                isLoading: false
            }

            PropertyChanges {
                target: newCollection
                visible: false
            }

            PropertyChanges {
                target: customLoader
                width: collectionsPicker.width
                height: collectionsPicker.height
            }
        },

        // State to display a Spinning icon when the list is loading.
        State {
            name: "loading"
            extend: "chooser"

            PropertyChanges {
                target: collectionsPicker
                isLoading: true
            }
        },

        // State to display the CreateCollection view.
        State {
            name: "create"

            PropertyChanges {
                target: collectionsPicker
                visible: false
            }

            PropertyChanges {
                target: newCollection
                visible: true
            }

            PropertyChanges {
                target: customLoader
                width: newCollection.width
                height: newCollection.height
            }
        }
    ]

    function changeTitle() {
        var mediaType;

        if (sharingObj.shareType == MeeGoUXSharingClientQmlObj.ShareTypeVideo)
            mediaType = qsTr("Video");
        else if (sharingObj.shareType == MeeGoUXSharingClientQmlObj.ShareTypeImage)
            mediaType = qsTr("Photo");
        else if (sharingObj.shareType == MeeGoUXSharingClientQmlObj.ShareType)
            mediaType = qsTr("Music");

       titleText.text = qsTr("Upload %1 to %2")
           .arg(mediaType).arg(service);
    }

    Component.onCompleted: {
        // Display initial data.
        changeTitle();
    }
}
