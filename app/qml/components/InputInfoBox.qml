import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK

Rectangle {
    id: editBox

    property bool enabled
    property string title
    property string text

    signal closeRequested()

    visible: enabled
    height: units.gu(7)

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
        anchors.margins: Suru.units.gu(1)

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
                anchors.fill: parent
                name: "close"
            }

            onClicked: closeRequested()
        }
    }
}
