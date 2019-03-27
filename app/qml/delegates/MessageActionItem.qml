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

    signal clicked()

    height: message.isCollapsed ? childrenRect.height : Suru.units.gu(5)
    width: childrenRect.width

    Label {
        visible: message.isCollapsed
        id: label
        anchors {
            top: parent.top
        }
        width: contentWidth
    }

    RowLayout {
        visible: !message.isCollapsed
        anchors {
            topMargin: Suru.units.gu(1)
            fill: parent
        }
        Rectangle {
            color: Suru.overlayColor
            opacity: 0.8
            radius: 4
            width: dl.contentWidth + Suru.units.gu(2)
            height: units.gu(3)
            AbstractButton {
                height: parent.height
                width: dl.contentWidth
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: action.clicked()
                Label {
                    id: dl
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        top: parent.top
                        topMargin: Suru.units.dp(4)
                    }
                    text: label.text
                    opacity: 1
                    color: "white"
                }
            }
            Layout.alignment: Qt.AlignCenter
        }
    }
}
