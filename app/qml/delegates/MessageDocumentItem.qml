import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK
import QtMultimedia 5.8
import QTelegram 1.0
import QuickFlux 1.1
import Ubuntu.Content 1.1
import "../components"

MessageItemBase {
    property QTdMessageDocument document: message.content
    property QTdLocalFile documentLocal: document.document.document.local
    property QTdPhotoSize thumbnail: document.document.thumbnail
    property QTdLocalFile thumbnailLocal: thumbnail.local ? thumbnail.local : null
    property url localFileSource: document && documentLocal.path ? Qt.resolvedUrl("file://" + documentLocal.path) : ""

    Item {
      id: documentContainer
      width: maximumAvailableContentWidth
      // width:  documentIcon.width+fileNameLabel.width+Suru.units.gu(2))
      height:Math.max(fileNameLabel.height,documentIcon.height)

      Component.onCompleted: {
          // console.log("c_reg",this,"\n")
          if (documentLocal.canBeDownloaded && !documentLocal.isDownloadingCompleted) {
              document.document.document.downloadFile();
          }
          if (thumbnailLocal && thumbnailLocal.canBeDownloaded && !thumbnailLocal.isDownloadingCompleted) {
              thumbnail.downloadFile();
          }
      }
      UITK.Icon {
          id: documentIcon
          source: "qrc:/qml/icons/download.svg"
          anchors {
            top: parent.top
            left: parent.left
            bottomMargin: Suru.units.gu(0.5)
          }
          width: height
      }
      Label {
          id: fileNameLabel
          height: contentHeight
          wrapMode: TextEdit.WrapAtWordBoundaryOrAnywhere
          anchors{
            left: documentIcon.right
            right: parent.right
            top: parent.top
            leftMargin: Suru.units.gu(2)
          }
          text: document.document.fileName
          color: Suru.foregroundColor

      }
    }
    Column {
        anchors {
            top: documentContainer.bottom
            topMargin: textEdit.text ? Suru.units.dp(5) : Suru.units.dp(0)
        }
        spacing: Suru.units.gu(2)

        width: textEdit.width

        TextEdit {
            id: textEdit

            height: contentHeight
            width: Math.min(maximumAvailableContentWidth, dummyTextEdit.contentWidth)
            readOnly: true
            text: document.caption.text
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
            text: document.caption.text
        }

    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
          console.log("document clicked")
          var properties;
          pageStack.push("qrc:///pages/PickerPage.qml", {
              "url": localFileSource,
              "handler": ContentHandler.Destination,
              "contentType": ContentType.Links
          });
        }
    }
}
