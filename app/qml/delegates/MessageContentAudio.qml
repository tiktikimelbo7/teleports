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

MessageContentBase {
    property QTdMessageAudio audioContent: message.content
    property QTdFile tdFile: audioContent.audio.audio
    property QTdLocalFile localFile: tdFile.local
    property QTdRemoteFile remoteFile: tdFile.remote
    property QTdPhotoSize thumbnail: audioContent.audio.albumCoverThumbnail
    property QTdLocalFile thumbnailLocal: thumbnail.photo.local ? thumbnail.photo.local : null
    property url localFileSource: tdFile && localFile.path ? Qt.resolvedUrl("file://" + localFile.path) : ""
    property alias caption: captionText

    Item {
        id: audioContainer
        width: maximumAvailableContentWidth
        height:Math.max(fileNameLabel.height,fileIcon.height)
        Component.onCompleted: {
            if (thumbnailLocal && thumbnailLocal.canBeDownloaded && !thumbnailLocal.isDownloadingCompleted && thumbnail) {
                thumbnail.downloadFile();
            }
        }
        Item {
            id: fileIcon
            width: units.gu(7)
            height: units.gu(7)
            anchors.rightMargin: Suru.units.gu(2)
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

        TextEdit {
            id: fileNameLabel
            height: contentHeight
            width: parent.width - fileIcon.width
            wrapMode: TextEdit.WrapAtWordBoundaryOrAnywhere
            anchors{
                left: fileIcon.right
                right: parent.right
                top: parent.top
                leftMargin: Suru.units.gu(2)
            }
            text: audioContent.audio.performer + " - " + audioContent.audio.title
            color: Suru.foregroundColor
        }
    }

    FormattedText {
        id: captionText
        anchors {
            top: audioContainer.bottom
            topMargin: message.isOutgoing ? Suru.units.dp(10) : Suru.units.dp(5)
        }

        formattedText: audioContent.caption
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
                AppActions.view.pushToStack("qrc:///pages/PreviewPage.qml", {
                                                "fileName": audioContent.audio.fileName,
                                                "audioPreviewSource": localFileSource
                                            })
            }
        }
    }
}
