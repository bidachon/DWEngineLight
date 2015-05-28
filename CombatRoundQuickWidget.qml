import QtQuick 2.0
import QtQuick.Controls 1.2

Item {
    width: 500
    height: 500

    ListView {
        id: listView1
        x: 21
        y: 68
        width: 110
        height: 160
        highlightMoveDuration: 4
        model: ListModel {
            ListElement {
                name: "Grey"
                colorCode: "grey"
            }

            ListElement {
                name: "Red"
                colorCode: "red"
            }

            ListElement {
                name: "Blue"
                colorCode: "blue"
            }

            ListElement {
                name: "Green"
                colorCode: "green"
            }
        }
        delegate: Item {
            x: 5
            width: 80
            height: 40
            Row {
                id: row1
                spacing: 10
                Rectangle {
                    width: 40
                    height: 40
                    color: colorCode
                }

                Text {
                    text: name
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }

    TextEdit {
        id: textEdit1
        x: 16
        y: 23
        width: 80
        height: 20
        text: qsTr("Text Edit")
        font.pixelSize: 12
    }

    Button {
        id: button1
        x: 115
        y: 23
        width: 47
        height: 26
        text: qsTr("Add")
    }
}

