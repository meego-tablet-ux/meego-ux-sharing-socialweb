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
    id: createCollection

    anchors.centerIn: parent

    height: 0
    width: childrenRect.width + (defaultMargin * 2)

    state: "idle"

    property string service: sharingObj.serviceName
    property int defaultMargin: 15

    // Signal to notify that the user has cancelled the operation.
    signal cancel()

    // Signal to notify the current operation's ID.
    signal collectionCreated(string name)

    // Signal to notify a possible error.
    signal creationError(string errMsg)

    Translator {
        catalog: "meego-ux-sharing-socialweb-qml"
    }

    Text {
        id: titleText

        font.pixelSize: theme_fontPixelSizeLarge
        font.family: theme_fontFamily
        color: theme_fontColorNormal

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

        text: qsTr("Choose a name for your new album")

        font.pixelSize: theme_fontPixelSizeMedium
        font.family: theme_fontFamily
        color: theme_fontColorNormal

        width: parent.width
        wrapMode: Text.Wrap

        anchors {
            top: titleText.bottom
            left: parent.left
            margins: defaultMargin
        }
    }

    Spinner {
        id: spinner

        visible: false
        spinning: false

        anchors {
            top: titleText.bottom
            right: parent.right
            margins: defaultMargin
        }

        onSpinningChanged: {
            if (!spinning)
                spinning = true
        }
    }

    TextEntry {
        id: collectionName

        width: parent.width - 30

        anchors {
            top: descriptionText.bottom
            left: parent.left
            margins: defaultMargin
        }

        defaultText: qsTr("Album name")

        onTextChanged: {
            if (collectionName.text == "")
                createButton.active = false
            else
                createButton.active = true
        }
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
            id: createButton

            height: 42  // FIXME: Hardcoded.

            anchors.bottom: parent.bottom

            text: qsTr("Create album")

            active: false

            onClicked: {
                var ret = collections.createColl(collectionName.text, "", 2)

                if (ret)
                    createCollection.state = "creating"
                else
                    creationError(qsTr("Collection creation: General error"))
            }
        }

        Button {
            id: cancelButton

            text: qsTr("Cancel")

            height: createButton.height

            anchors {
                bottom: parent.bottom
                left: createButton.right
                leftMargin: defaultMargin
            }

            onClicked: {
                // User cancelled the operation.
                createCollection.state = "idle"
                createCollection.cancel();
            }
        }
    }

    states: [
        State {
            name: "idle"

            PropertyChanges {
                target: spinner
                visible: false
                spinning: false
            }

            PropertyChanges {
                target: collectionName
                text: undefined
                enabled: true
            }

            PropertyChanges {
                target: createButton
                active: false
            }

            PropertyChanges {
                target: cancelButton
                active: true
            }
        },

        State {
            name: "creating"

            PropertyChanges {
                target: spinner
                visible: true
                spinning: true
            }

            PropertyChanges {
                target: collectionName
                text: collectionName.text
                enabled: false
            }

            PropertyChanges {
                target: createButton
                active: false
            }

            PropertyChanges {
                target: cancelButton
                active: false
            }
        }
    ]

    Connections {
        target: collections
        onCollectionCreated: {
            createCollection.state = "idle"

            if (!error)
                createCollection.collectionCreated(collectionName.text)
            else
                creationError(qsTr("Collection creation: General error"))
        }
    }

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

        if (createCollection.height == 0)
            createCollection.height = childrenRect.height + defaultMargin
    }
}
