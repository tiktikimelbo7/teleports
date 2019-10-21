import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK
import QTelegram 1.0
import QuickFlux 1.1
import "../actions"
import "../components"

MessageContentBase {
    property QTdMessagePhoto photoContent: message.content
    property QTdPhotoSize size: {
        var photoSize = photoContent.photo.sizes.get(1);
        if (!photoSize)
            photoSize = photoContent.photo.sizes.get(0);
        return photoSize;
    }
    property QTdFile photo: size.photo
    property QTdLocalFile photoLocal: photo.local
    property QTdRemoteFile photoRemote: photo.remote

    property real maximumMediaHeight: message.isCollapsed ? Suru.units.gu(24/5) : Suru.units.gu(24)
    property real minimumMediaHeight: message.isCollapsed ? Suru.units.gu(16/5) : Suru.units.gu(16)
    property real maximumMediaWidth: message.isCollapsed ? Suru.units.gu(30/5) : Suru.units.gu(30)
    property real maximumMediaRatio: maximumMediaWidth / maximumMediaHeight
    property real mediaWidth:size.width
    property real mediaHeight:size.height

    Item {
        id: imgContainer
        width: {
            if(size) {
                var photoWidth = mediaWidth > mediaHeight?
                    Math.min(mediaWidth, maximumMediaWidth):
                    mediaWidth * Math.min(1, maximumMediaHeight / mediaHeight);
                return Math.max(photoWidth, units.gu(7));
            }
            return units.gu(7);
        }
        height: {
            if(size) {
                var photoHeight = mediaHeight >= mediaWidth?
                    Math.min(mediaHeight, maximumMediaHeight):
                    Math.max(mediaHeight * Math.min(1, maximumMediaWidth / mediaWidth), minimumMediaHeight);
                return Math.max(photoHeight, units.gu(7)); 
            }
            return units.gu(7);
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

        }
        BusyPercentageIndicator {
            width: units.gu(7)
            height: units.gu(7)
            id: busyIndicator
            anchors.centerIn: parent
            visible: photoLocal.isDownloadingActive || photoRemote.isUploadingActive
            running: !photoLocal.isDownloadingCompleted || !photoRemote.isUploadingCompleted
            percentage: {
                if (photoLocal.isDownloadingActive) {
                    return parseInt(photoLocal.downloadedSize) / parseInt(photo.size) * 100;
                } else if (photoRemote.isUploadingActive) {
                    return parseInt(photoRemote.uploadedSize) / parseInt(photo.size) * 100;
                }
                return 0.0;
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
                photo.downloadFile();
            }
        }
    }


    FormattedText {
        anchors {
            top: imgContainer.bottom
            topMargin: message.isOutgoing ? Suru.units.dp(10) : Suru.units.dp(5)
        }

        formattedText: photoContent.caption
        maximumWidth: imgContainer.width
        isPreview: message.isCollapsed
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            var largeSize = photoContent.photo.sizes.get(2);
            if( largeSize === null)
                largeSize = photoContent.photo.sizes.get(1);
            var largePhoto = largeSize.photo;
            if (largePhoto.canBeDownloaded && !largePhoto.isDownloadingCompleted) {
                largePhoto.downloadFile();
            }
            AppActions.view.pushToStack("qrc:///pages/PreviewPage.qml", {
                                            "senderName": message.sender.username,
                                            "photoPreviewSource": Qt.resolvedUrl("file://" + largePhoto.local.path)
                                        });

        }
    }
}
