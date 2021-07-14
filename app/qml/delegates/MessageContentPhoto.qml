import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK
import QTelegram 1.0
import QuickFlux 1.1
import "../actions"
import "../components"
import QtGraphicalEffects 1.0

MessageContentBase {
    property QTdMessagePhoto photoContent: message.content
    property QTdPhotoSize size: photoContent.photo.sizes.size() > 1 ? photoContent.photo.sizes.get(1) : photoContent.photo.sizes.get(0)
    property QTdFile photo: size.photo
    property QTdLocalFile photoLocal: photo.local
    property bool contentBeforeImage
    property QTdRemoteFile photoRemote: photo.remote

    property real maximumMediaHeight: message.isCollapsed ? Suru.units.gu(24/5) : Suru.units.gu(24)
    property real minimumMediaHeight: 0//message.isCollapsed ? Suru.units.gu(16/5) : Suru.units.gu(16)  //TODO
    property real maximumMediaWidth: message.isCollapsed ? Suru.units.gu(24/5) : Suru.units.gu(30)
    property real maximumMediaRatio: maximumMediaWidth / maximumMediaHeight
    property real mediaWidth:size.width
    property real mediaHeight:size.height

    Item {
        id: imgContainer
        width: {
            if(size) {
                var photoWidth = mediaWidth > mediaHeight?
                    Math.min(mediaWidth, maximumMediaWidth):
                    mediaWidth * Math.min(1, maximumMediaHeight / mediaHeight);
                return Math.max(photoWidth, units.gu(7));
            }
            return units.gu(7);
        }
        height: {
            if(size) {
                var photoHeight = mediaHeight >= mediaWidth?
                    Math.min(mediaHeight, maximumMediaHeight):
                    Math.max(mediaHeight * Math.min(1, maximumMediaWidth / mediaWidth), minimumMediaHeight);
                return Math.max(photoHeight, units.gu(7));
            }
            return units.gu(7);
        }

        Image {
            id: media_img
            anchors.fill: parent
            fillMode: message.isCollapsed ? Image.PreserveAspectCrop : Image.PreserveAspectFit
            property string localFileSource: photo && photoLocal.path ? "file:///" + photoLocal.path : ""
            function reload() {
                media_img.source = "";
                media_img.source = localFileSource;
            }
            source: localFileSource
            visible: !radiusCornerMask.enabled

            Rectangle {
                color: "#77000000"
                radius: Suru.units.dp(4)
                width: message_status_comp.width + Suru.units.gu(1)
                height: message_status_comp.height + Suru.units.gu(1)
                visible: multimediaLayout && !message.isCollapsed
                anchors {
                    right: parent.right
                    bottom: parent.bottom
                    margins: Suru.units.dp(4)
                }
                MessageStatusRow {
                    id: message_status_comp
                    anchors.centerIn: parent
                    Suru.theme: Suru.Dark
                    textStyle: Suru.PrimaryText
                    opacity: 1
                }
            }

        }
        BusyPercentageIndicator {
            width: units.gu(7)
            height: units.gu(7)
            id: busyIndicator
            anchors.centerIn: parent
            visible: photoLocal.isDownloadingActive || photoRemote.isUploadingActive
            running: !photoLocal.isDownloadingCompleted || !photoRemote.isUploadingCompleted
            percentage: {
                if (photoLocal.isDownloadingActive) {
                    return parseInt(photoLocal.downloadedSize) / parseInt(photo.size) * 100;
                } else if (photoRemote.isUploadingActive) {
                    return parseInt(photoRemote.uploadedSize) / parseInt(photo.size) * 100;
                }
                return 0.0;
            }
        }

        OpacityMask {
            id: radiusCornerMask
            anchors.fill: parent
            source: media_img
            enabled: !contentBeforeImage || photoContent.caption.text == ""
            maskSource: Rectangle {
                width: media_img.width
                height: media_img.height
                radius: message.isCollapsed ? Suru.units.dp(3) : Suru.units.dp(4)
                visible: false // this also needs to be invisible or it will cover up the image
                Rectangle {
                    anchors {
                        left: parent.left
                        right: parent.right
                        top: contentBeforeImage && !message.isCollapsed ? parent.top : parent.verticalCenter
                        bottom: photoContent.caption.text != "" && !message.isCollapsed ? parent.bottom : parent.verticalCenter
                    }
                }
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
            if (message.isCollapsed) {
                imgContainer.width = Math.max(imgContainer.width, imgContainer.height)
                imgContainer.height = Math.max(imgContainer.width, imgContainer.height)
                //TODO
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                var largeSize = photoContent.photo.sizes.get(photoContent.photo.sizes.size() - 1);
                var largePhoto = largeSize.photo;

                if (largePhoto.canBeDownloaded && !largePhoto.isDownloadingCompleted) {
                    largePhoto.downloadFile();
                }
                AppActions.view.pushToStack("qrc:///pages/PreviewPage.qml", {
                                                "photoPreviewSource": Qt.resolvedUrl("file://" + largePhoto.local.path)
                                            });

            }
        }
    }


    FormattedText {
        id: captionText
        anchors {
            top: imgContainer.bottom
            topMargin: formattedText.text == "" ? 0 : Suru.units.dp(5)
            leftMargin: Suru.units.dp(5)
            left: parent.left
            rightMargin: Suru.units.dp(5)
            right: parent.right
        }

        formattedText: photoContent.caption
        maximumWidth: imgContainer.width - 2 * anchors.leftMargin
        isPreview: message.isCollapsed
    }
}
