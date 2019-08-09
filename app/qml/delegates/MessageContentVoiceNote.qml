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

MessageContentBase {
    property QTdMessageVoiceNote voiceNote: message.content
    property QTdLocalFile voiceLocal: voiceNote.voiceNote.voice.local
    property url localFileSource: voiceNote && voiceLocal.path ? Qt.resolvedUrl("file://" + voiceLocal.path) : ""

    Item {
        id: voiceNoteContainer
        width: maximumAvailableContentWidth
        height:Suru.units.gu(3)
        Connections {
            target: voiceNote.voiceNote
            onFileChanged: {
                // do something when media is loaded/changed
            }
        }
        Component.onCompleted: {
            if (voiceLocal.canBeDownloaded && !voiceLocal.isDownloadingCompleted) {
                voiceNote.voiceNote.voice.downloadFile();
            }
        }
        Item {
            id: fileIcon
            height:audioIcon.height
            width: height
            anchors.rightMargin: Suru.units.gu(2)
            UITK.Icon {
                id: audioIcon
                visible: voiceLocal.isDownloadingCompleted
                source: voiceLocal.isDownloadingCompleted ? "qrc:/qml/icons/playMedia.svg" : ""
                anchors {
                    top: parent.top
                    left: parent.left
                    bottomMargin: Suru.units.gu(0.5)
                }
                width: height
            }
            BusyIndicator {
                visible: !voiceLocal.isDownloadingCompleted
                anchors.fill: parent
                running: !voiceLocal.isDownloadingCompleted
            }
        }

        TextEdit {
            id:infoLabel
            height: contentHeight
            width: parent.width - audioIcon.width
            wrapMode: TextEdit.WrapAtWordBoundaryOrAnywhere
            anchors{
                left: fileIcon.right
                right: parent.right
                top: parent.top
                topMargin: Suru.units.gu(1)
                leftMargin: Suru.units.gu(2)
            }
            text: i18n.tr("Voice note") + " " + voiceNote.voiceNote.qmlDuration
            color: Suru.foregroundColor
        }
    }

    FormattedText {
        anchors {
            top: voiceNoteContainer.bottom
            topMargin: message.isOutgoing ? Suru.units.dp(10) : Suru.units.dp(5)
        }

        formattedText: voiceNote.caption
        maximumWidth: maximumAvailableContentWidth
        isPreview: message.isCollapsed
        visible: voiceNote.caption
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
          console.log(message.content.voiceNote.voice.local);
            if(voiceLocal.isDownloadingCompleted){
                console.log("voice note clicked "+ voiceLocal.path)
                AppActions.view.pushToStack("qrc:///pages/PreviewPage.qml", {
                                                "fileName": voiceNote.voiceNote.voice.fileName,
                                                "audioPreviewSource": localFileSource
                                            })
            }
        }
    }
}
