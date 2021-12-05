import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK
import QtMultimedia 5.8
import QTelegram 1.0
import QuickFlux 1.1
import "../actions"
import "../components"

MessageContentBase
{
    property QTdMessageVideo videoContent: message.content
    property QTdFile tdFile: videoContent.video.video
    property QTdLocalFile localFile: tdFile.local
    property QTdRemoteFile remoteFile: tdFile.remote
    property QTdPhotoSize thumbnail: videoContent.video.thumbnail
    property QTdLocalFile thumbnailLocal: thumbnail.photo.local ? thumbnail.photo.local : null
    property real uniqeId: Math.floor(Math.random() * Math.floor(10000));

    property real maximumMediaHeight: message.isCollapsed ? Suru.units.gu(24/5) : Suru.units.gu(24)
    property real minimumMediaHeight: message.isCollapsed ? Suru.units.gu(16/5) : Suru.units.gu(16)
    property real maximumMediaWidth: message.isCollapsed ? Suru.units.gu(30/5) : Suru.units.gu(30)
    property real maximumMediaRatio: maximumMediaWidth / maximumMediaHeight
    property real mediaWidth:videoContent.video.width
    property real mediaHeight:videoContent.video.height
    property url localFileSource: videoContent && localFile.path ? Qt.resolvedUrl("file://" + localFile.path) : ""
    Item {
        id: videoContainer
        width: {
            if(videoContent.video) {
                var photoWidth = mediaWidth > mediaHeight?
                    Math.min(mediaWidth, maximumMediaWidth):
                    mediaWidth * Math.min(1, maximumMediaHeight / mediaHeight);
                return Math.max(photoWidth, units.gu(7));
            }
            return units.gu(7);
        }
        height: {
            if(videoContent.video) {
                var photoHeight = mediaHeight >= mediaWidth?
                    Math.min(mediaHeight, maximumMediaHeight):
                    Math.max(mediaHeight * Math.min(1, maximumMediaWidth / mediaWidth), minimumMediaHeight);
                return Math.max(photoHeight, units.gu(7)); 
            }
            return units.gu(7);
        }

        Image {
            id: thumbnailImg
            anchors.fill: parent
            source:videoContent && thumbnailLocal.path? Qt.resolvedUrl("file://" + thumbnailLocal.path) : ""
        }
        Item {
            id: fileIcon
            width: units.gu(7)
            height: units.gu(7)
            anchors.centerIn: parent
            UITK.Icon {
                visible: localFile.isDownloadingCompleted
                source: "qrc:/qml/icons/play.svg"
                anchors.fill: parent
            }
            UITK.Icon {
                visible: !localFile.isDownloadingCompleted
                    && !localFile.isDownloadingActive
                    && !remoteFile.isDownloadingCompleted
                    && !remoteFile.isDownloadingActive
                source: "qrc:/qml/icons/download.svg"
                anchors.fill: parent
            }
            BusyPercentageIndicator {
                anchors.fill: parent
                visible: localFile.isDownloadingActive || remoteFile.isUploadingActive
                running: !localFile.isDownloadingCompleted || !remoteFile.isUploadingCompleted
                percentage: {
                    if (localFile.isDownloadingActive) {
                        return parseInt(localFile.downloadedSize) / parseInt(tdFile.size) * 100;
                    } else if (remoteFile.isUploadingActive) {
                        return parseInt(remoteFile.uploadedSize) / parseInt(tdFile.size) * 100;
                    }
                    return 0.0;
                }
            }
        }
        Component.onCompleted: {
            if (thumbnailLocal.canBeDownloaded && !thumbnailLocal.isDownloadingCompleted) {
                thumbnail.downloadFile();
            }
        }
    }

    FormattedText {
        anchors {
            top: videoContainer.bottom
            topMargin: message.isOutgoing ? Suru.units.dp(10) : Suru.units.dp(5)
        }

        formattedText: videoContent.caption
        maximumWidth: maximumAvailableContentWidth
        isPreview: message.isCollapsed
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (localFile.canBeDownloaded && !localFile.isDownloadingCompleted) {
                tdFile.downloadFile();
            }
            if(localFile.isDownloadingCompleted){
                messageContentOpening()
                AppActions.view.pushToStack("qrc:///pages/PreviewPage.qml", {
                                                "fileName": videoContent.video.fileName,
                                                "videoPreviewSource": localFileSource
                                            });
            }
        }
    }
}
