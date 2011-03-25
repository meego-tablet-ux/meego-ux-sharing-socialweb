/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

import Qt 4.7

MediaUploader {
    width: 330

    service: "TwitPic"

    fields: ListModel {
        Component.onCompleted: {
            append({"name": "title",
                "defaultText": qsTr("Photo name")});
        }
    }
}
