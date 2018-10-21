import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK
import QTelegram 1.0

/*
 * Not used, candidate for removal.
 * GenericPhoto used instead, combining both chat and message profile photos.
 */

Item {
    property QTdChatPhoto photo: null
    property string initials: ""
    property string avatarColor: "#000"
    property bool photoExists: photo && photo.small.local.path
    
    // Placed under shape, so it's hidden
    ShaderEffectSource {
        id: source
        anchors.centerIn: parent
        width: 1
        height: 1
        sourceItem: photoExists ? itemPicture : bgColorFill
    }

    Rectangle {
        id: bgColorFill
        anchors.fill: parent
        color: photoExists ? "transparent" : avatarColor
        visible: false
    }
    
    Image {
        id: itemPicture
        anchors.centerIn: parent
        width: parent.width
        height: parent.height
        source: photoExists ? Qt.resolvedUrl("file://" + photo.small.local.path) : ""
        sourceSize: Qt.size(width, height)
        antialiasing: true
        asynchronous: true
        visible: false
    }
    
    UITK.Shape {
        id: shape
        image: source
        anchors.fill: parent
        aspect: UITK.UbuntuShape.DropShadow
    }
    
    UITK.Label {
        id: initialsLabel
        anchors.centerIn: parent
        textSize: UITK.Label.Large
        color: photoExists ? "transparent" : "#fff"
        text: initials
    }
}
