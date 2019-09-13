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
    property QTdMessageVideoNote videoNote: message.content
    property QTdLocalFile videoNoteLocal: videoNote.videoNote.video.local
    property QTdPhotoSize thumbnail: videoNote.videoNote.thumbnail
    property QTdLocalFile thumbnailLocal: thumbnail.photo.local
    property real uniqeId: Math.floor(Math.random() * Math.floor(10000));
    property url localFileSource: videoNote && videoNoteLocal.path ? Qt.resolvedUrl("file://" + videoNoteLocal.path) : ""
    Item {
        id: videoNoteContainer
        width: Suru.units.gu(20)
        height: Suru.units.gu(21)
        Image {
            id: thumbnailImg
            // visible:!media_video.isPlaying
            anchors.fill: parent
            source:videoNote && thumbnailLocal.path? Qt.resolvedUrl("file://" + thumbnailLocal.path) : ""
        }
        Item {
            id: fileIcon
            width: units.gu(7)
            height: units.gu(7)
            anchors.centerIn: parent
            UITK.Icon {
                visible: videoNoteLocal.isDownloadingCompleted
                source: "qrc:/qml/icons/playMedia.svg"
                anchors.fill: parent
            }

            BusyIndicator {
                visible: !videoNoteLocal.isDownloadingCompleted
                anchors.centerIn: parent
                running: !videoNoteLocal.isDownloadingCompleted
            }
        }
        Connections {
            target: videoNote.videoNote.video
            // onFileChanged: {
            //     media_video.reload();
            // }
        }
        Component.onCompleted: {
            // console.log("c_reg",this,"\n")
            if (videoNoteLocal.canBeDownloaded && !videoNoteLocal.isDownloadingCompleted) {
                videoNote.videoNote.video.downloadFile();
            }
            if (thumbnailLocal.canBeDownloaded && !thumbnailLocal.isDownloadingCompleted) {
                thumbnail.downloadFile();
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if(videoNoteLocal.isDownloadingCompleted){
                console.log("video note clicked")
                //TODO crashes the app sometimes or dbus ;)
                // if(media_video.isPlaying)media_video.pause()
                // else media_video.play()
                AppActions.view.pushToStack("qrc:///pages/PreviewPage.qml", {
                                                "fileName": videoNote.videoNote.fileName,
                                                "videoPreviewSource": localFileSource
                                            });
            }
        }
    }
}
