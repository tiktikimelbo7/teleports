import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import QTelegram 1.0
import QuickFlux 1.1
import "../components"

MessageContentBase {
    property QTdMessageSticker stickerContent: message.content
    property QTdFile file: stickerContent.sticker.sticker
    // TODO: Consider thumbnail
    // property QTdFile thumbnail: stickerContent.sticker.thumbnail
    property QTdLocalFile localFile: file.local

    Image {
        id: image

        property url localFileSource: localFile.path !== "" && !stickerContent.sticker.isAnimated
                                      ? Qt.resolvedUrl("file://" + localFile.path)
                                      : Qt.resolvedUrl("")

        function reload() {
            image.source = Qt.resolvedUrl();
            image.source = localFileSource;
        }

        width: Math.min(d.maxStickerSize, maximumAvailableContentWidth)
        height: width * (stickerContent.sticker.height/stickerContent.sticker.width)
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
        readonly property int maxStickerSize: message.isCollapsed ? Suru.units.gu(20/5) : Suru.units.gu(20)
    }

    Component.onCompleted: {
        if (localFile.canBeDownloaded && !localFile.isDownloadingCompleted) {
            file.downloadFile();
        }
    }
}
