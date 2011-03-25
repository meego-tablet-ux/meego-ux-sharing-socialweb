import Qt 4.7
import MeeGo.Labs.Components 0.1
import MeeGo.Sharing.plugins.socialweb 0.1


Rectangle {


    height: 400
    width: 400
    color: "black"

    Collections {
        id: colls
        serviceName: "smugmug"
	Component.onCompleted: {
	    console.log("Supports collections: " + colls.hasCollections);
	}
    }

    Rectangle {
        id: rectLV
        color: "white"
        height: 300
        width: 350
        anchors.centerIn: parent

        ListView {
            anchors.fill: parent
            interactive: true
            id: lvAlbums
            model: colls.getCollModel()
            delegate: Rectangle {
                id: collDel
                color: "grey"
                height: childrenRect.height

                Text {
                    id: txtName
                    text: "Name: " + collName
                    anchors.top: parent.top
                    anchors.topMargin: 2
                    anchors.left: parent.left
                    anchors.leftMargin: 2
                }

                Text {
                    id: txtCount
                    text: "Count: " + collItemCount
                    anchors.top: txtName.bottom
                    anchors.topMargin: 2
                    anchors.left: collName.left
                }
                Text {
                    id: txtId
                    text: "ID: " + collID
                    anchors.top: txtCount.bottom
                    anchors.topMargin: 2
                    anchors.left: collName.left
                }
                Text {
                    id: collParent
                    text: "Parent: " + collParentID
                    anchors.top: txtId.bottom
                    anchors.topMargin: 2
                    anchors.left: collName.left
                }
                Text {
                    id: collMediaType
                    text: "Types: " + Math.round(collMediaTypes)
                    anchors.top: collParent.bottom
                    anchors.topMargin: 2
                    anchors.left: collName.left
                }
            }
        }
    }
    Button {
        id: btnAdd
        title: "Add Collection"
        anchors.top: rectLV.bottom
        anchors.topMargin: 5
        onClicked: {
            var callWorked = colls.createColl("testingFromQML", "" /*parentID */, 6 /*Collection.MediaTypeVideosAndPhotos*/);
            console.log("Create coll call succeeded: " + callWorked);
        }
    }
}
