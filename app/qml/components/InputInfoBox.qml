import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import QTelegram 1.0
import Ubuntu.Components 1.3 as UITK

Item {
    id: editBox

    property bool enabled
    property string title
    property var message
    readonly property string text: message.content.infoText

    signal closeRequested()

    height: units.gu(7)
    clip: true

    MouseArea {
        // only for blocking mouse and touch events
        anchors.fill: parent
        enabled: editBox.enabled
    }

    Rectangle {
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            topMargin: editBox.enabled ? 0 : height
        }

        visible: anchors.topMargin != height
        height: parent.height
        color: Suru.backgroundColor

        Rectangle {
            anchors {
                top: parent.top
                right: parent.right
                left: parent.left
            }
            height: Suru.units.dp(1)
            color: Suru.neutralColor
        }

        RowLayout {
            anchors.fill: parent
            anchors.rightMargin: Suru.units.gu(2)
            anchors.margins: Suru.units.gu(1)

            spacing: units.gu(1)

            UITK.UbuntuShape {
                Layout.maximumWidth: units.gu(5)
                Layout.maximumHeight: units.gu(5)
                visible: message.content.infoImageUrl != ""
                aspect: UITK.UbuntuShape.Flat
                backgroundColor: avatarColor
                source: Image {
                    asynchronous: true
                    source: message.content.infoImageUrl != "" ? message.content.infoImageUrl: ""
                }
            }

            Column {
                Layout.fillWidth: true
                Layout.fillHeight: true

                Label {
                    text: editBox.title
                    font.bold: true
                    color: Suru.color(Suru.Blue)
                }

                Label {
                    Layout.fillWidth: true
                    text: editBox.text
                    wrapMode: Text.NoWrap
                    elide: Text.ElideRight
                    maximumLineCount: 1
                }
            }

            AbstractButton {
                Layout.fillHeight: true
                implicitWidth: Suru.units.gu(2)

                UITK.Icon {
                    anchors.centerIn: parent
                    name: "close"
                    color: Suru.foregroundColor
                    width: units.gu(3)
                    height: units.gu(3)
                }

                onClicked: closeRequested()
            }
        }

        Behavior on anchors.topMargin {
            NumberAnimation {
                duration: 100
                easing.type: Easing.InQuad
            }
        }
    }
}
