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
    property QTdMessageVideo video: message.content
    property QTdLocalFile videoLocal: video.video.video.local
    property QTdPhotoSize thumbnail: video.video.thumbnail
    property QTdLocalFile thumbnailLocal: thumbnail.photo.local
    property real uniqeId: Math.floor(Math.random() * Math.floor(10000));
    property real maximumMediaHeight: Suru.units.gu(24)
    property real minimumMediaHeight: Suru.units.gu(16)
    property real maximumMediaWidth: Suru.units.gu(30)
    property real maximumMediaRatio: maximumMediaWidth / maximumMediaHeight
    property real mediaWidth:video.video.width
    property real mediaHeight:video.video.height
    property url localFileSource: video && videoLocal.path ? Qt.resolvedUrl("file://" + videoLocal.path) : ""
    Item {
        id: videoContainer
        width: mediaWidth > mediaHeight?
                   Math.min(mediaWidth, maximumMediaWidth):
                   mediaWidth * Math.min(1, maximumMediaHeight / mediaHeight)
        height: mediaHeight >= mediaWidth?
                    Math.min(mediaHeight, maximumMediaHeight):
                    Math.max(mediaHeight * Math.min(1, maximumMediaWidth / mediaWidth), minimumMediaHeight)

        Image {
            id: thumbnailImg
            // visible:!media_video.isPlaying
            anchors.fill: parent
            source:video && thumbnailLocal.path? Qt.resolvedUrl("file://" + thumbnailLocal.path) : ""
        }
        Item {
            id: fileIcon
            width: units.gu(7)
            height: units.gu(7)
            anchors.centerIn: parent
            UITK.Icon {
                visible: videoLocal.isDownloadingCompleted
                source: "qrc:/qml/icons/playMedia.svg"
                anchors.fill: parent
            }

            BusyIndicator {
                visible: !videoLocal.isDownloadingCompleted
                anchors.centerIn: parent
                running: !videoLocal.isDownloadingCompleted
            }
        }
        Connections {
            target: video.video.video
            // onFileChanged: {
            //     media_video.reload();
            // }
        }
        Component.onCompleted: {
            // console.log("c_reg",this,"\n")
            if (videoLocal.canBeDownloaded && !videoLocal.isDownloadingCompleted) {
                video.video.video.downloadFile();
            }
            if (thumbnailLocal.canBeDownloaded && !thumbnailLocal.isDownloadingCompleted) {
                thumbnail.downloadFile();
            }
        }
    }
    Column {
        anchors {
            top: videoContainer.bottom
            topMargin: message.isOutgoing ? Suru.units.dp(10) : Suru.units.dp(5)
        }
        spacing: Suru.units.gu(2)

        width: textEdit.width

        TextEdit {
            id: textEdit

            height: contentHeight
            width: Math.min(maximumAvailableContentWidth, dummyTextEdit.contentWidth)
            readOnly: true
            text: video.caption.text
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
            text: video.caption.text
        }

    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            if(videoLocal.isDownloadingCompleted){
                console.log("video clicked")
                //TODO crashes the app sometimes or dbus ;)
                // if(media_video.isPlaying)media_video.pause()
                // else media_video.play()
                AppActions.view.pushToStack("qrc:///pages/PreviewPage.qml", {
                                                "fileName": video.video.fileName,
                                                "videoPreviewSource": localFileSource
                                            });
            }
        }
    }
}
