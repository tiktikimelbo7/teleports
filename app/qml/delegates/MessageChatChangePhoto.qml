import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK
import QTelegram 1.0
import QuickFlux 1.1
import "../actions"
import "../components"

MessageActionItem {

    property QTdMessageChatChangePhoto photoContent: message.content
    property QTdPhotoSize size: photoContent.photo.sizes.get(1)
    property QTdFile photo: size.photo
    property QTdLocalFile photoLocal: photo.local

    property real maximumMediaHeight: message.isCollapsed ? Suru.units.gu(24/5) : Suru.units.gu(12)
    property real minimumMediaHeight: message.isCollapsed ? Suru.units.gu(16/5) : Suru.units.gu(8)
    property real maximumMediaWidth: message.isCollapsed ? Suru.units.gu(30/5) : Suru.units.gu(15)
    property real maximumMediaRatio: maximumMediaWidth / maximumMediaHeight
    property real mediaWidth:size.width
    property real mediaHeight:size.height

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
    Item {
        id: imgContainer
        anchors.top: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: mediaWidth > mediaHeight?
                   Math.min(mediaWidth, maximumMediaWidth):
                   mediaWidth * Math.min(1, maximumMediaHeight / mediaHeight)
        height: mediaHeight >= mediaWidth?
                    Math.min(mediaHeight, maximumMediaHeight):
                    Math.max(mediaHeight * Math.min(1, maximumMediaWidth / mediaWidth), minimumMediaHeight)
        Image {
            id: media_img
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
    }

    Component.onCompleted: {
        if (photo.canBeDownloaded && !photo.isDownloadingCompleted) {
            photo.downloadFile();
        }
    }
}
