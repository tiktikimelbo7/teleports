import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK
import QTelegram 1.0
import QuickFlux 1.1
import "../actions"
import "../components"

Item {
    property alias message: actionItem.message
    height: message.isCollapsed ? label.height : container.height
    width: message.isCollapsed ? childrenRect.width : parent.width
    Label {
        visible: message.isCollapsed
        id: label
        anchors {
            top: parent.top
        }
        text: actionItem.text
        width: contentWidth
    }
    Column {
        id: container
        visible: !message.isCollapsed
        height: visible ? childrenRect.height + Suru.units.gu(1) : 0
        width: visible ? parent.width : 0
        MessageActionItem {
            id: actionItem
            text: message.isChannelPost
            ? i18n.tr("Channel photo has been changed:")
            : i18n.tr("%1 changed the chat photo:").arg(getAddingUserString()).trim()

            function getAddingUserString() {
                if (message.isCollapsed) {
                    return ""
                }
                if(!message.sender)
                return "";
                if (message.sender.firstName !== "") {
                    var fullName = message.sender.firstName
                    if (message.sender.lastName !== "")
                    fullName = fullName + " " + message.sender.lastName
                    return fullName
                } else {
                    return message.sender.username
                }
            }
            anchors.horizontalCenter: parent.horizontalCenter
        }
        GenericPhoto {
            id: thumbnailImg
            property QTdMessageChatChangePhoto photoContent: message.content
            property QTdPhotoSize size: photoContent ? photoContent.photo.sizes.get(0) : null
            property QTdFile photo: size ? size.photo : null
            property QTdLocalFile photoLocal: photo ? photo.local : null

            property string localFileSource: photo && photoLocal.path ? photoLocal.path : ""

            function reload() {
                thumbnailImg.photoPath = Qt.resolvedUrl();
                thumbnailImg.photoPath = localFileSource;
            }
            photoPath: localFileSource
            anchors.horizontalCenter: parent.horizontalCenter
            height: Suru.units.gu(10)
            width: height

        }
        Connections {
            target: thumbnailImg.photo
            onFileChanged: {
                thumbnailImg.reload();
            }
        }
        Component.onCompleted: {
            if (thumbnailImg.photo.canBeDownloaded && !thumbnailImg.photo.isDownloadingCompleted) {
                thumbnailImg.photo.downloadFile();
            }
        }

    }
}
