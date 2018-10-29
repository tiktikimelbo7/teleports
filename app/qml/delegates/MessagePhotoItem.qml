import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK
import QTelegram 1.0
import QuickFlux 1.1
import "../components"

MessageItemBase {
    property QTdMessagePhoto photoContent: message.content
    property QTdPhotoSize size: photoContent.photo.sizes.get(1)
    property QTdFile photo: size.photo
    property QTdLocalFile photoLocal: photo.local

    property real maximumMediaHeight: Suru.units.gu(24)
    property real minimumMediaHeight: Suru.units.gu(16)
    property real maximumMediaWidth: Suru.units.gu(30)
    property real maximumMediaRatio: maximumMediaWidth / maximumMediaHeight
    property real mediaWidth:size.width
    property real mediaHeight:size.height
    Item {
      id: imgContainer
      width: mediaWidth > mediaHeight?
                        Math.min(mediaWidth, maximumMediaWidth):
                        mediaWidth * Math.min(1, maximumMediaHeight / mediaHeight)
      height: mediaHeight >= mediaWidth?
                        Math.min(mediaHeight, maximumMediaHeight):
                        Math.max(mediaHeight * Math.min(1, maximumMediaWidth / mediaWidth), minimumMediaHeight)
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
              photo.downloadFile();
          }
      }
    }


    Column {
        anchors {
            top: imgContainer.bottom
            topMargin: message.isOutgoing ? Suru.units.dp(10) : Suru.units.dp(5)
        }
        spacing: Suru.units.gu(2)

        width: textEdit.width

        TextEdit {
            id: textEdit

            height: contentHeight
            width: Math.min(maximumAvailableContentWidth, dummyTextEdit.contentWidth)
            readOnly: true
            text: photoContent.caption.text
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
            text: photoContent.caption.text
        }

    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
          console.log("photo clicked",photoContent.photo.small.local.canBeDownloaded
                                      )
         }
    }
}
