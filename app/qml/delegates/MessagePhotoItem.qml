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
    Column {
      id: imageCol
      Image {
          id: image

          property url localFileSource: photo && photoLocal.path ? Qt.resolvedUrl("file://" + photo.local.path) : ""
          function reload() {
              image.source = Qt.resolvedUrl();
              image.source = localFileSource;
          }

          // TODO: Handle scaling
          width: size.width
          height:size.height
          source: localFileSource

          BusyIndicator {
              anchors.centerIn: parent
              running: image.status === Image.Loading
                       || image.status === Image.Null
          }
      }

      Connections {
          target: photo
          onFileChanged: {
              image.reload();
          }
      }

      QtObject {
          id: d

          // TODO: dynamically adjust
          readonly property int maxPhotoSize: Suru.units.gu(20)
      }
      Component.onCompleted: {
          if (photo.canBeDownloaded && !photo.isDownloadingCompleted) {
              photo.downloadFile();
          }
      }
    }
    Column {
        anchors {
            top: imageCol.bottom
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
          console.log("photo clicked")
         }
    }
}
