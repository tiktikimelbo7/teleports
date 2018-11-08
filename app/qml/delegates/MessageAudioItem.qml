import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK
import QtMultimedia 5.8
import QTelegram 1.0
import QuickFlux 1.1
import "../components"

MessageItemBase {
    property QTdMessageAudio audio: message.content
    property QTdLocalFile audioLocal: audio.audio.audio.local
    property QTdPhotoSize thumbnail: audio.audio.albumCoverThumbnail
    property QTdLocalFile thumbnailLocal: thumbnail.local
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
      Connections {
          target: audio.audio.audio
          onFileChanged: {
              media_video.reload();
          }
      }
      Component.onCompleted: {
          // console.log("c_reg",this,"\n")
          if (audioLocal.canBeDownloaded && !audioLocal.isDownloadingCompleted) {
              audio.audio.audio.downloadFile();
          }
          if (thumbnailLocal.canBeDownloaded && !thumbnailLocal.isDownloadingCompleted) {
              thumbnail.downloadFile();
          }
      }
    }
    UITK.Icon {
        id: audioIcon
        source: "qrc:/qml/icons/playMedia.svg"
        anchors {
            left: parent.left
            topMargin: units.dp(4)
            bottomMargin: units.dp(4)
            rightMargin: units.dp(1)
        }
        width: height
    }
    TextEdit {
        id: fileNameLabel
        height: contentHeight
        anchors{
          left: audioIcon.right
        }
        text: audio.audio.fileName
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
            color: message.isOutgoing ? "white" : Suru.foregroundColor
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
          console.log("audio clicked")
          //TODO crashes the app sometimes or dbus ;)
          // if(media_video.isPlaying)media_video.pause()
          // else media_video.play()
          var properties;
          properties = {
              "senderName": message.sender.username,
              "audioPreviewSource": localFileSource
          };
          pageStack.push("qrc:///pages/PreviewPage.qml", properties);


         }
    }
}
