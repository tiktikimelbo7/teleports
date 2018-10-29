import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import QTelegram 1.0
import QuickFlux 1.1
import "../components"

MessageItemBase {
    property QTdMessagePhoto photoContent: message.content
    property QTdFile file: photoContent.photo.small

    // TODO: Consider thumbnail
    // property QTdFile thumbnail: photoContent.photo.thumbnail
    property QTdLocalFile localFile: file.local

    transparentBackground: true

    Image {
        id: image

        property url localFileSource: localFile.path !== ""
                                      ? Qt.resolvedUrl("file://" + localFile.path)
                                      : Qt.resolvedUrl("")

        function reload() {
            image.source = Qt.resolvedUrl();
            image.source = localFileSource;
        }

        width: Math.min(d.maxPhotoSize, maximumAvailableContentWidth)
        height: width
        source: localFileSource

        // TODO: Handle Image.Error

        BusyIndicator {
            anchors.centerIn: parent
            running: image.status === Image.Loading
                     || image.status === Image.Null
        }
    }

    Connections {
        target: file
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
        if (localFile.canBeDownloaded && !localFile.isDownloadingCompleted) {
            file.downloadFile();
        }
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
          console.log("photo clicked", Telegram.users.me)
          onCompleted: {
            console.log("photo",photoContent.photo)

          }
         }
    }
}
