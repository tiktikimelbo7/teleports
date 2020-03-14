import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK
import QTelegram 1.0
import QuickFlux 1.1
import "../actions"
import "../components"

ColumnLayout {
    anchors.fill: parent
    property alias message: actionItem.message
    spacing: units.gu(1)
    Layout.alignment: Qt.AlignHCenter
    MessageActionItem {
        id: actionItem
        text:  message.isChannelPost
            ? i18n.tr("Channel photo has been changed")
            : i18n.tr("%1 changed the chat photo").arg(message.isCollapsed
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
    }
    Image {
        id: thumbnailImg
        width: actionItem.maximumAvailableContentWidth
        property QTdMessageChatChangePhoto photoContent: message.content
        property QTdPhotoSize size: photoContent.photo.sizes.get(1)
        property QTdFile photo: size.photo
        property QTdLocalFile photoLocal: photo.local
        fillMode: Image.PreserveAspectFit
        property url localFileSource: photo && photoLocal.path ? Qt.resolvedUrl("file://" + photo.local.path) : ""

        function reload() {
            thumbnailImg.source = Qt.resolvedUrl();
            thumbnailImg.source = localFileSource;
        }
        source: localFileSource
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
