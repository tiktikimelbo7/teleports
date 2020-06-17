import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK
import QTelegram 1.0

Item {
    property string photoPath: ""
    property string initials: ""
    property string avatarColor: "#000"
    property bool myself: false
    property bool photoExists: photoPath != "" ? true : false

    ShaderEffectSource {
        id: effectSource
        anchors.centerIn: parent
        width: 0
        height: 0
        sourceItem: imageContent
    }

    Item {
        id: imageContent
        anchors.fill: parent
        visible: false

        Rectangle {
            id: bgColorFill
            anchors.fill: parent
            color: myself ? "#65aadd" : (photoExists ? "transparent" : avatarColor)
        }

        Image {
            id: itemPicture
            anchors.centerIn: parent
            width: parent.width
            height: parent.height
            source: myself ? "qrc:/qml/icons/bookmark.svg" : (photoExists ? Qt.resolvedUrl("file://" + photoPath) : "")
            sourceSize: Qt.size(width, height)
            antialiasing: true
            asynchronous: true
        }
    }

    UITK.Shape {
        id: imgShape
        image: effectSource
        anchors.fill: parent
        aspect: UITK.UbuntuShape.DropShadow
        radius: width > Suru.units.gu(7) ? "medium" : "small"
    }

    UITK.Label {
        id: initialsLabel
        anchors.centerIn: parent
        textSize: UITK.Label.Large
        color: photoExists ? "transparent" : "#fff"
        text: initials
        visible: !myself && !photoExists
    }
}
