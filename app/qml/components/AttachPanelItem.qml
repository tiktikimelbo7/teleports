import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK

Column {
    id: attach_item

    property bool showTick: false

    property alias text: attach_label.text
    property alias image: attach_image.source
    property alias icon: attach_icon.name
    signal clicked(var mouse)

    Item {
        width: attach_image.source? attach_image.width : attach_icon.width
        height: attach_image.source? attach_image.height : attach_icon.height

        Image {
            id: attach_image
            objectName: "attach_gallery"
            asynchronous: true
            width: units.gu(5)
            height: width
            fillMode: Image.PreserveAspectFit
            sourceSize: Qt.size(width, height)

            Image {
                anchors.centerIn: parent
                source: attach_item.showTick ? Qt.resolvedUrl("qrc:/qml/icons/attach_hide2.png") : ""
                visible: attach_item.showTick
            }
        }

        UITK.Icon {
            id: attach_icon
            width: units.gu(5)
            height: width
        }
        MouseArea {
            anchors.fill: parent
            onClicked: attach_item.clicked(mouse)
        }
    }

    Label {
        id: attach_label
        anchors.horizontalCenter: parent.horizontalCenter
        // fontSize: "x-small"
        font.weight: Font.DemiBold
        visible: text != ""
    }
}
