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

Item {
    id: mediaUploader

    anchors.centerIn: parent

    height: 0
    width: 0

    state: "uploader"

    property string service: sharingObj.serviceName
    property bool autoUpload: true
    property bool forceCollection: false
    property ListModel fields: null
    property Component customContent: null
    property Item customItem: null
    property int defaultMargin: 15
    property int defaultHeight: childrenRect.height + (defaultMargin * 2)
    property int defaultWidth: childrenRect.width + (defaultMargin * 2)
    property string selectedCollection: qsTr("No album")

    // Signal to notify that the user has cancelled the operation.
    signal cancel()

    // Signal to notify the current operation's ID.
    signal shared(int shareid)

    // Signal to notify a possible error.
    signal shareError(string errMsg)

    // Signal to nofify that the share was requested.
    signal shareRequested()

    Translator {
        catalog: "meego-ux-sharing-socialweb-qml"
    }

    Theme {
        id: theme
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
        id: imageCounter

        font.pixelSize: theme.fontPixelSizeNormal
        font.family: theme.fontFamily
        color: theme.fontColorNormal

        width: parent.width
        wrapMode: Text.Wrap
        horizontalAlignment: Text.AlignHCenter

        anchors {
            top: titleText.bottom
            horizontalCenter: parent.horizontalCenter
            margins: defaultMargin
        }
    }

    Image {
        id: filePreview

        fillMode: Image.PreserveAspectCrop

        sourceSize.width: 130
        sourceSize.height: 130
        width: 130
        height: 130

        anchors {
            top: imageCounter.bottom
            horizontalCenter: parent.horizontalCenter
            margins: defaultMargin
        }
    }

    Column {
        id: repFields

        width: parent.width - (defaultMargin * 2)
        height: childrenRect.height
        spacing: defaultMargin

        anchors {
            top: filePreview.bottom
            horizontalCenter: parent.horizontalCenter
            margins: defaultMargin
        }

        Repeater {
            id: inputRepeater

            model: fields

            delegate: TextEntry {
                property string name: model.name

                defaultText: qsTr(model.defaultText)
                width: parent.width
            }
        }
    }

    Item {
        id: customArea

        width: childrenRect.width
        height: childrenRect.height

        anchors {
            top: repFields.bottom
            horizontalCenter: parent.horizontalCenter
            margins: defaultMargin
        }
    }

    Text {
        id: collectionText

        visible: forceCollection

        font.pixelSize: theme.fontPixelSizeNormal
        font.family: theme.fontFamily
        color: theme.fontColorNormal

        width: parent.width
        wrapMode: Text.Wrap

        anchors {
            top: repFields.bottom
            left: parent.left
            margins: defaultMargin
        }
    }

    Item {
        id: buttonGroup

        height: childrenRect.height
        width: childrenRect.width

        anchors {
            top: collectionText.bottom
            horizontalCenter: parent.horizontalCenter
            margins: defaultMargin
        }

        Button {
            id: uploadButton

            anchors.bottom: parent.bottom

            onClicked: {
                for (var i = 0; i < inputRepeater.count; ++i) {
                    if (repFields.children[i].text != "") {
                        sharingObj.addHashEntryToFile(
                            sharingObj.filesToShare[refreshData.counter],
                            repFields.children[i].name, repFields.children[i].text);
                    }
                }

                // Extract data from the custom component.
                if (mediaUploader.customItem) {
                    var data = mediaUploader.customItem.fetchData();

                    for (var key in data) {
                        sharingObj.addHashEntryToFile(
                            sharingObj.filesToShare[refreshData.counter],
                            key, data[key]);
                    }
                }

                if (refreshData.counter == sharingObj.fileCount - 1) {
                    if (autoUpload)
                        shareContent();
                    else
                        shareRequested();
                }
                else {
                    // Increment the counter and show the next file.
                    refreshData.counter++;
                    refreshData();
                }
            }
        }

        Button {
            id: cancelButton

            text: qsTr("Cancel")

            anchors {
                bottom: parent.bottom
                left: uploadButton.right
                leftMargin: defaultMargin
            }

            onClicked: {
                // User cancelled the operation.
                mediaUploader.cancel();
            }
        }
    }

    function shareContent() {
        // Actually perform the sharing operation.
        var opID = sharingObj.shareSimple();

        // Report error or send the operation ID.
        if (opID == -1)
            mediaUploader.shareError(
                shareObj.sharingObj.getLastShareError());
        else
            mediaUploader.shared(opID);
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

    function refreshData() {
        if (typeof refreshData.counter == 'undefined')
            refreshData.counter = 0

        // Show the screen title.
        changeTitle();

        // Hide imageCounter if there's only one file to be uploaded.
        if (sharingObj.fileCount == 1) {
            imageCounter.visible = false;
            filePreview.anchors.top = titleText.bottom;
        }

        // Show the file  counter.
        imageCounter.text = qsTr("File %1 of %2")
            .arg((refreshData.counter + 1)).arg(sharingObj.fileCount);

        // Preview the file.
        if (sharingObj.shareType == MeeGoUXSharingClientQmlObj.ShareTypeVideo)
            filePreview.source = "image://themedimage/images/media/video_thumb_med";
        else if (sharingObj.shareType == MeeGoUXSharingClientQmlObj.ShareTypeImage)
            filePreview.source = sharingObj.filesToShare[refreshData.counter];

        // Clear fields.
        var repChildren = repFields.children
        for (var i = 0; i < inputRepeater.count; ++i)
            repChildren[i].text = "";

        // Clear custom components.
        if (mediaUploader.customItem)
            mediaUploader.customItem.clearData()

        // Set collections.
        if (forceCollection)
            collectionText.text = qsTr("<b>Album:</b> %1")
                .arg(selectedCollection);

        // Update the upload button title.
        if (refreshData.counter == sharingObj.fileCount - 1)
            uploadButton.text = qsTr("Upload");
        else
            uploadButton.text = qsTr("Next");
    }

    // Creates the CollectionPicker component.
    CollectionsPicker {
        id: collectionPicker

        parent: customLoader

        visible: false
        service: mediaUploader.service

        onCollectionSelected: {
            mediaUploader.selectedCollection = name
            mediaUploader.refreshData()
            mediaUploader.state = "uploader"
        }

        onCancel: mediaUploader.cancel()

        onCollectionError: mediaUploader.shareError(errMsg)
    }

    // Declare possible states.
    states: [
        // State to display the MediaUploader view.
        State {
            name: "uploader"

            PropertyChanges {
                target: collectionPicker
                visible: false
            }

            PropertyChanges {
                target: mediaUploader
                visible: true
                width: defaultWidth
                height: defaultHeight
            }

            PropertyChanges {
                target: customLoader
                width: defaultWidth
                height: defaultHeight
            }
        },

        // State to display the CollectionPicker view.
        State {
            name: "collectionPicker"

            PropertyChanges {
                target: mediaUploader
                visible: false
            }

            PropertyChanges {
                target: collectionPicker
                visible: true
            }

            PropertyChanges {
                target: customLoader
                width: collectionPicker.width
                height: collectionPicker.height
            }
        }
    ]

    Component.onCompleted: {
        // Display initial data.
        refreshData();

        // Load the custom component.
        if (visible) {
            if(customItem)
                customItem.destroy();

            if (customContent != null) {
                customItem = customContent.createObject(customArea);
                collectionText.anchors.top = customArea.bottom
            }

            // Check if there's collections.
            if (!forceCollection)
                if (customContent)
                    buttonGroup.anchors.top = customArea.bottom
                else
                    buttonGroup.anchors.top = repFields.bottom

            if (mediaUploader.height == 0)
                defaultHeight = childrenRect.height + (defaultMargin * 2)

            if (mediaUploader.width == 0)
                defaultWidth = childrenRect.width
        }

        // If force collectio is set move to the CollectionPicker state.
        if (forceCollection && collectionPicker.hasCollections)
            state = "collectionPicker"
    }
}
