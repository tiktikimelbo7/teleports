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
    property QTdMessageAnimation animation: message.content
    property QTdLocalFile animationLocal: animation.animation.animation.local
    property QTdPhotoSize thumbnail: animation.animation.thumbnail
    property QTdLocalFile thumbnailLocal: thumbnail.photo.local
    property real uniqeId: Math.floor(Math.random() * Math.floor(10000));
    property real maximumMediaHeight: Suru.units.gu(24)
    property real minimumMediaHeight: Suru.units.gu(16)
    property real maximumMediaWidth: Suru.units.gu(30)
    property real maximumMediaRatio: maximumMediaWidth / maximumMediaHeight
    property real mediaWidth:animation.animation.width
    property real mediaHeight:animation.animation.height
    property url localFileSource: animation && animationLocal.path ? Qt.resolvedUrl("file://" + animationLocal.path) : ""

    Item {
        id: animationContainer
        width: mediaWidth > mediaHeight?
                   Math.min(mediaWidth, maximumMediaWidth):
                   mediaWidth * Math.min(1, maximumMediaHeight / mediaHeight)
        height: mediaHeight >= mediaWidth?
                    Math.min(mediaHeight, maximumMediaHeight):
                    Math.max(mediaHeight * Math.min(1, maximumMediaWidth / mediaWidth), minimumMediaHeight)
        // VideoOutput {
        //     visible:media_video.isPlaying
        //     source: media_video
        //     anchors.fill: parent
        //     smooth: true
        // }
        // MediaPlayer {
        //     id: media_video
        //     loops: 1
        //     autoPlay: false
        //     autoLoad: false
        //     property bool isPlaying: playbackState === MediaPlayer.PlayingState
        //
        //     // fillMode: Video.PreserveAspectFit
        //     function reload() {
        //         console.log("reload triggerd")
        //         media_video.stop()
        //         // media_video.source = Qt.resolvedUrl();
        //         // media_video.source = localFileSource;
        //     }
        //     onError: {
        //         console.error("MediaPlayer: " + error + ":" + errorString)
        //     }
        //
        //     source: localFileSource
        //
        // }

        Image {
            id: thumbnailImg
            // visible:!media_video.isPlaying
            anchors.fill: parent
            source:animation && thumbnailLocal.path? Qt.resolvedUrl("file://" + thumbnailLocal.path) : ""
        }
        Item {
            id: fileIcon
            width: units.gu(7)
            height: units.gu(7)
            anchors.centerIn: parent
            UITK.Icon {
                visible: animationLocal.isDownloadingCompleted
                source: "qrc:/qml/icons/playMedia.svg"
                anchors.fill: parent
            }
            BusyIndicator {
                anchors.centerIn: parent
                visible: !animationLocal.isDownloadingCompleted
                running: !animationLocal.isDownloadingCompleted
            }
        }

        // BusyIndicator {
        //     anchors.centerIn: parent
        //     running: media_video.status === VideoOutput.Loading
        //              || media_video.status === VideoOutput.Null
        // }
        Connections {
            target: animation.animation.animation
            onFileChanged: {
                // media_video.reload();
            }
        }
        Component.onCompleted: {
            // console.log("c_reg",this,"\n")
            if (animationLocal.canBeDownloaded && !animationLocal.isDownloadingCompleted) {
                animation.animation.animation.downloadFile();
            }
            if (thumbnailLocal.canBeDownloaded && !thumbnailLocal.isDownloadingCompleted) {
                thumbnail.downloadFile();
            }
        }
    }

    FormattedText {
        anchors {
            top: animationContainer.bottom
            topMargin: message.isOutgoing ? Suru.units.dp(10) : Suru.units.dp(5)
        }

        isPreview: message.isCollapsed
        maximumWidth: maximumAvailableContentWidth
        formattedText: animation.caption
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if(animationLocal.isDownloadingCompleted){
                console.log("animated gif clicked")
                //TODO crashes the app sometimes or dbus ;)
                // if(media_video.isPlaying)media_video.pause()
                // else media_video.play()
                AppActions.view.pushToStack("qrc:///pages/PreviewPage.qml", {
                                                "fileName": animation.animation.fileName,
                                                "videoPreviewSource": localFileSource
                                            })
            }
        }
    }
}
