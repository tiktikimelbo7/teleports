import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK
import QTelegram 1.0

/*
 * Has to be replaced in UserProfile before it can be removed.
 * Candidate for removal.
 * GenericPhoto used instead, combining both chat and message profile photos.
 */

Item {
    property QTdProfilePhoto photo: null
    property string initials: ""
    property var avatarColor: "#000"
        
    UITK.UbuntuShape {
        anchors.fill: parent
        aspect: UITK.UbuntuShape.Flat
        backgroundColor: avatarColor
        source: Image {
            id: avatarImg
            asynchronous: true
            source: Qt.resolvedUrl("file://" + photo.small.local.path)

            onStatusChanged: { 
                avatarImg.status == avatarImg.Error ? initialsLabel.visible = true : initialsLabel.visible = false;
            }
        }
    }
    
    UITK.Label {
        id: initialsLabel
        anchors.centerIn: parent
        visible: !photo.small.local.path
        textSize: UITK.Label.Large
        color: "#fff"
        text: initials
    }
}
