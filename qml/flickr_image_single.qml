/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7
import MeeGo.Components 0.1

MediaUploader {
    service: "Flickr"

    fields: ListModel {
        Component.onCompleted: {
            append({"name": "title",
                "defaultText": qsTr("Photo name")});

            append({"name": "description",
                "defaultText": qsTr("Photo description")});

            append({"name": "x-flickr-tags",
                "defaultText": qsTr("Photo tags")});
        }
    }

    customContent: privacy

    Theme {
        id: theme
    }

    Component {
        id: privacy

        Item {
            height: childrenRect.height
            width: childrenRect.width

            RadioGroup {
                id: privacyGroup
            }

            RadioButton {
                id: privateRadio

                group: privacyGroup

                anchors {
                    left: parent.left
                    top: parent.top
                }
            }

            Text {
                text: qsTr("Only you (private)")

                font.pixelSize: theme.fontPixelSizeLarge
                font.family: theme.fontFamily
                color: theme.fontColorNormal

                anchors {
                    left: privateRadio.right
                    top: parent.top
                    leftMargin: 5
                }
            }

            CheckBox {
                id: friends

                anchors {
                    top: privateRadio.bottom
                    left: parent.left
                    topMargin: 10
                    leftMargin: 45
                }

                onIsCheckedChanged: {
                    if (privateRadio.checked != true)
                        privacyGroup.check(privateRadio)
                }
            }

            Text {
                text: qsTr("Your friends")

                font.pixelSize: theme.fontPixelSizeLarge
                font.family: theme.fontFamily
                color: theme.fontColorNormal

                anchors {
                    left: friends.right
                    top: privateRadio.bottom
                    topMargin: 10
                    leftMargin: 5
                }
            }

            CheckBox {
                id: family

                anchors {
                    top: friends.bottom
                    left: parent.left
                    topMargin: 10
                    leftMargin: 45
                }

                onIsCheckedChanged: {
                    if (privateRadio.checked != true)
                        privacyGroup.check(privateRadio)
                }

            }

            Text {
                text: qsTr("Your family")

                font.pixelSize: theme.fontPixelSizeLarge
                font.family: theme.fontFamily
                color: theme.fontColorNormal

                anchors {
                    left: family.right
                    top: friends.bottom
                    topMargin: 10
                    leftMargin: 5
                }
            }

            RadioButton {
                id: publicRadio

                group: privacyGroup

                anchors {
                    top : family.bottom
                    left: parent.left
                    topMargin: 10
                }

                onCheckedChanged: {
                    if (publicRadio.checked == true) {
                        family.isChecked = false
                        friends.isChecked = false
                    }
                }
            }

            Text {
                text: qsTr("Anyone (public)")

                font.pixelSize: theme.fontPixelSizeLarge
                font.family: theme.fontFamily
                color: theme.fontColorNormal

                anchors {
                    left: publicRadio.right
                    top: family.bottom
                    topMargin: 10
                    leftMargin: 5
                }
            }

            // Mandatory function!
            // Fetch the data from the component.
            function fetchData() {
                var data = new Array()

                data["x-flickr-is-public"] = +publicRadio.checked + ''
                data["x-flickr-is-friend"] = +friends.isChecked + ''
                data["x-flickr-is-family"] = +family.isChecked + ''

                return data
            }

            // Mandatory function!
            // Reset the component state.
            function clearData() {
                family.isChecked = false
                friends.isChecked = false
                privacyGroup.check(publicRadio)
            }

            Component.onCompleted: {
                clearData()
            }
        }
    }
}

