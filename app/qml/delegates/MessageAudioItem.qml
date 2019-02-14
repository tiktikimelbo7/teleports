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

MessageItemBase {
    property QTdMessageAudio audio: message.content
    property QTdLocalFile audioLocal: audio.audio.audio.local
    property QTdPhotoSize thumbnail: audio.audio.albumCoverThumbnail
    property QTdLocalFile thumbnailLocal: thumbnail.local ? thumbnail.local : null
    property url localFileSource: audio && audioLocal.path ? Qt.resolvedUrl("file://" + audioLocal.path) : ""
    Item {
        id: audioContainer
        // Image {
        //   id: thumbnailImg
        //   // visible:!media_video.isPlaying
        //   anchors.fill: parent
        //   source:audio && thumbnailLocal.path? Qt.resolvedUrl("file://" + thumbnailLocal.path) : ""
        // }


        // BusyIndicator {
        //     anchors.centerIn: parent
        //     running: media_video.status === VideoOutput.Loading
        //              || media_video.status === VideoOutput.Null
        // }
        // width: Math.min(maximumAvailableContentWidth,audioIcon.width+fileNameLabel.contentWidth+Suru.units.gu(2))
        width: maximumAvailableContentWidth
        height:Math.max(fileNameLabel.height,fileIcon.height)
        Connections {
            target: audio.audio.audio
            onFileChanged: {
                // do something when media is loaded/changed
            }
        }
        Component.onCompleted: {
            if (audioLocal.canBeDownloaded && !audioLocal.isDownloadingCompleted) {
                audio.audio.audio.downloadFile();
            }
            if (thumbnailLocal && thumbnailLocal.canBeDownloaded && !thumbnailLocal.isDownloadingCompleted) {
                thumbnail.downloadFile();
            }
        }
        Item {
            id: fileIcon
            height:audioIcon.height
            width: height
            anchors.rightMargin: Suru.units.gu(2)
            UITK.Icon {
                id: audioIcon
                visible: audioLocal.isDownloadingCompleted
                source: audioLocal.isDownloadingCompleted ? "qrc:/qml/icons/playMedia.svg" : ""
                anchors {
                    top: parent.top
                    left: parent.left
                    bottomMargin: Suru.units.gu(0.5)
                }
                width: height
            }
            BusyIndicator {
                visible: !audioLocal.isDownloadingCompleted
                anchors.fill: parent
                running: !audioLocal.isDownloadingCompleted
            }
        }

        TextEdit {
            id: fileNameLabel
            height: contentHeight
            width: parent.width - audioIcon.width
            wrapMode: TextEdit.WrapAtWordBoundaryOrAnywhere
            anchors{
                left: fileIcon.right
                right: parent.right
                top: parent.top
                leftMargin: Suru.units.gu(2)
            }
            text: audio.audio.fileName
            color: Suru.foregroundColor
        }
    }


    Column {
        anchors {
            top: audioContainer.bottom
            topMargin: message.isOutgoing ? Suru.units.dp(10) : Suru.units.dp(5)
        }
        spacing: Suru.units.gu(2)

        width: textEdit.width

        TextEdit {
            id: textEdit

            height: contentHeight
            width: Math.min(maximumAvailableContentWidth, dummyTextEdit.contentWidth)
            readOnly: true
            text: audio.caption.text
            color: Suru.foregroundColor
            selectedTextColor: Suru.highlightColor
            wrapMode: TextEdit.WrapAtWordBoundaryOrAnywhere
            onLinkActivated: {
                console.log("Link activated: ", link)
                Qt.openUrlExternally(link)
            }
        }

        TextEdit {
            id: dummyTextEdit
            visible: false
            height: contentHeight
            text: audio.caption.text
        }

    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            if(audioLocal.isDownloadingCompleted){
                console.log("audio clicked "+ audioLocal.path)
                AppActions.view.pushToStack("qrc:///pages/PreviewPage.qml", {
                                                "fileName": audio.audio.fileName,
                                                "audioPreviewSource": localFileSource
                                            })
            }
        }
    }
}
