import QtQuick 2.9
import QTelegram 1.0
import "../actions"

MessageActionItem {

    property QTdMessagePhoto photoContent: message.content
    property QTdPhotoSize size: photoContent.photo.sizes.get(1)
    property QTdFile photo: size.photo
    property QTdLocalFile photoLocal: photo.local

    text:   i18n.tr("%1 changed the chat photo").arg(message.isCollapsed
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
    Image {
        id: media_img
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        property url localFileSource: photo && photoLocal.path ? Qt.resolvedUrl("file://" + photo.local.path) : ""
        function reload() {
            media_img.source = Qt.resolvedUrl();
            media_img.source = localFileSource;
        }

        source: localFileSource
        BusyIndicator {
            anchors.centerIn: parent
            running: media_img.status === Image.Loading
                        || media_img.status === Image.Null
        }
    }
    Connections {
        target: photo
        onFileChanged: {
            media_img.reload();
        }
    }

    Component.onCompleted: {
        if (photo.canBeDownloaded && !photo.isDownloadingCompleted) {
      s      photo.downloadFile();
        }
    }
}
