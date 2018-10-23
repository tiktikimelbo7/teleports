import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import QTelegram 1.0
import QuickFlux 1.1
import "../components"

MessageItemBase {
    property QTdMessageSticker stickerContent: message.content
    property QTdFile file: stickerContent.sticker.sticker
    // TODO: Consider thumbnail
    // property QTdFile thumbnail: stickerContent.sticker.thumbnail
    property QTdLocalFile localFile: file.local

    transparentBackground: true

    Item {
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        height: childrenRect.height

        Image {
            id: image

            anchors {
                left: !message.isOutgoing ? parent.left: null
                right: message.isOutgoing ? parent.right: null
            }

            property url localFileSource: localFile.path !== ""
                                          ? Qt.resolvedUrl("file://" + localFile.path)
                                          : Qt.resolvedUrl("")

            function reload() {
                image.source = Qt.resolvedUrl();
                image.source = localFileSource;
            }

            width: Math.min(d.maxStickerSize, parent.width)
            height: width
            source: localFileSource

            // TODO: Handle Image.Error

            BusyIndicator {
                anchors.centerIn: parent
                running: image.status === Image.Loading
                         || image.status === Image.Null
            }
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
        readonly property int maxStickerSize: Suru.units.gu(20)
    }

    Component.onCompleted: {
        if (localFile.canBeDownloaded && !localFile.isDownloadingCompleted) {
            file.downloadFile();
        }
    }
}
