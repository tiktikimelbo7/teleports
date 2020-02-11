import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK
import QtMultimedia 5.8
import QTelegram 1.0
import QuickFlux 1.1
import Ubuntu.Content 1.1
import "../actions"
import "../components"

MessageContentBase {
    property QTdMessageDocument documentContent: message.content
    property QTdFile tdFile: documentContent.document.document
    property QTdLocalFile localFile: tdFile.local
    property QTdRemoteFile remoteFile: tdFile.remote
    property QTdPhotoSize thumbnail: documentContent.document.thumbnail
    property QTdLocalFile thumbnailLocal: thumbnail.photo.local ? thumbnail.photo.local : null
    property url localFileSource: tdFile && localFile.path ? Qt.resolvedUrl("file://" + localFile.path) : ""

    Item {
        id: documentContainer
        width: maximumAvailableContentWidth
        // width:  documentIcon.width+fileNameLabel.width+Suru.units.gu(2))
        height:Math.max(fileNameLabel.height,fileIcon.height)

        Component.onCompleted: {
            if (thumbnailLocal && thumbnailLocal.canBeDownloaded && !thumbnailLocal.isDownloadingCompleted) {
                thumbnail.downloadFile();
            }
        }
        Item {
            id: fileIcon
            width: units.gu(7)
            height: units.gu(7)
            anchors.rightMargin: Suru.units.gu(2)
            UITK.Icon {
                visible: localFile.isDownloadingCompleted
                color: UITK.UbuntuColors.ash
                name: "document-open"
                anchors.fill: parent
            }
            UITK.Icon {
                visible: !localFile.isDownloadingCompleted && !localFile.isDownloadingActive
                source: "qrc:/qml/icons/download.svg"
                anchors.fill: parent
                color: UITK.UbuntuColors.ash
            }
            BusyPercentageIndicator {
                anchors.fill: parent
                visible: localFile.isDownloadingActive || remoteFile.isUploadingActive
                running: !localFile.isDownloadingCompleted || !remoteFile.isUploadingCompleted
                percentage: {
                    if (localFile.isDownloadingActive) {
                        return parseInt(localFile.downloadedSize) / parseInt(tdFile.size) * 100;
                    } else if (remoteFile.isUploadingActive) {
                        return parseInt(remoteFile.uploadedSize) / parseInt(tdFile.size) * 100;
                    }
                    return 0.0;
                }
            }
        }
        Label {
            id: fileNameLabel
            height: contentHeight
            wrapMode: TextEdit.WrapAtWordBoundaryOrAnywhere
            anchors{
                left: fileIcon.right
                right: parent.right
                top: parent.top
                leftMargin: Suru.units.gu(2)
            }
            text: documentContent.document.fileName
            color: Suru.foregroundColor

        }
    }

    FormattedText {
        anchors {
            top: documentContainer.bottom
        }

        formattedText: documentContent.caption
        maximumWidth: maximumAvailableContentWidth
        isPreview: message.isCollapsed
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (localFile.canBeDownloaded && !localFile.isDownloadingCompleted) {
                tdFile.downloadFile();
            }
            if(localFile.isDownloadingCompleted){
                AppActions.view.pushToStack("qrc:///pages/PickerPage.qml", {
                                                "url": localFileSource,
                                                "handler": ContentHandler.Destination,
                                                "contentType": ContentType.Documents
                                            });
            }
        }
    }
}
