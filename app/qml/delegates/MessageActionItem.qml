import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import QTelegram 1.0

Item {
    id: action
    property QTdMessage message: null
    property QTdChat chat: null
    property alias text: label.text
    property real maximumAvailableContentWidth: Math.min(units.gu(45), parent.width * (3/4)) - units.dp(10)

    signal clicked()

    height: message.isCollapsed ? childrenRect.height : container.height + Suru.units.gu(2)
    width: childrenRect.width

    Label {
        visible: message.isCollapsed
        id: label
        anchors {
            top: parent.top
        }
        width: Math.min(maximumAvailableContentWidth, contentWidth)
    }

    RowLayout {
        visible: !message.isCollapsed
        anchors {
            topMargin: Suru.units.gu(1)
            bottomMargin: Suru.units.gu(1)
            fill: parent
        }
        Rectangle {
            id: container
            color: Suru.overlayColor
            opacity: 0.8
            radius: 4
            width: containerButton.width + Suru.units.gu(2)
            height: containerButton.height
            anchors.horizontalCenter: parent.horizontalCenter

            AbstractButton {
                id: containerButton
                height: dl.height + Suru.units.dp(8)
                width: dl.contentWidth
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: action.clicked()
                Label {
                    id: dl
                    width: Math.min(maximumAvailableContentWidth, dl.contentWidth)
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        top: parent.top
                        topMargin: Suru.units.dp(4)
                    }
                    text: label.text
                    horizontalAlignment: Label.AlignHCenter
                    wrapMode: Label.WordWrap
                    opacity: 1
                    color: "white"
                }
            }
            Layout.alignment: Qt.AlignCenter
        }
    }
}
