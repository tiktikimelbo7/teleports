import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK
import QTelegram 1.0
import QuickFlux 1.1
import "../actions"
import "../components"

Column {
    height: childrenRect.height + Suru.units.gu(1)
    property alias message: actionItem.message
    width: parent.width
    MessageActionItem {
        id: actionItem
        text:  message.isChannelPost
            ? i18n.tr("Channel photo has been changed:")
            : i18n.tr("%1 changed the chat photo:").arg(message.isCollapsed
                ? ""
                : getAddingUserString())

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
        width: parent.width
    }
    GenericPhoto {
        id: thumbnailImg
        property QTdMessageChatChangePhoto photoContent: message.content
        property QTdPhotoSize size: photoContent.photo.sizes.get(0)
        property QTdFile photo: size.photo
        property QTdLocalFile photoLocal: photo.local

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
