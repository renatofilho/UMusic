import QtQuick 1.1

Item {
    height: 800
    width: 480

    ListView {
        id: listView
        model: umodel
        anchors.margins: 6
        anchors.fill: parent
        delegate: Rectangle {
            height: 64
            width: parent.width
            Image {
                id: coverArt
                anchors.margins: 6
                anchors.top: parent.top
                anchors.left: parent.left
                source: "artwork:cover.png"
                width: sourceSize.width
                height: sourceSize.height
            }
            Text {
                id: title
                font.bold: true
                font.pointSize: 11
                anchors.margins: 6
                anchors.left: coverArt.right
                anchors.right: parent.right
                text: model.title
            }
            Text {
                id: subTitle
                font.pointSize: 11
                anchors.margins: 6
                anchors.left: coverArt.right
                anchors.top: title.bottom
                anchors.right: parent.right
                text: model.subTitle
            }
            Rectangle {
                anchors.top: coverArt.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                height: 1
                color: "gray"
                anchors.margins: 6
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (model.hasChildren) {
                        umodel.loadFolder(model.id)
                    } else {
                        umodel.playSong(model.id)
                    }
                }
            }
        }
    }
}
