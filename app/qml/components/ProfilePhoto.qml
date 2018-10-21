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
    property QTdProfilePhoto photo: null
    property string initials: ""
    property var avatarColor: "#000"
    
    onAvatarColorChanged: {
        console.log("avatarColor", avatarColor);
    }
    
    Rectangle {
        id: rectImage
        anchors.fill: parent
        color: photo.small.local.path ? "transparent" : avatarColor
    }

    UITK.Label {
        id: initialsLabel
        anchors.centerIn: parent
        textSize: UITK.Label.Large
        color: photo.small.local.path ? "transparent" : "#fff"
        text: initials
    }

    UITK.UbuntuShape {
        anchors.fill: parent
        aspect: UITK.UbuntuShape.Flat
        source: Image {
            asynchronous: true
            source: photo && photo.small.local.path ? Qt.resolvedUrl("file://" + photo.small.local.path) : ""
        }
    }
}
