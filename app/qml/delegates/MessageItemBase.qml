import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import QTelegram 1.0
import "../components"
import "../actions"

ItemDelegate {
    id: base
    width: parent.width
    height: contentCol.height + Suru.units.gu(2)

    property QTdMessage message: null

    default property alias content: mainContent.data

    background: Item {}

    RowLayout {
        anchors {
            fill: parent
            margins: Suru.units.gu(1)
        }

        Item {
            width: Suru.units.gu(4.5)
            height: Suru.units.gu(4.5)
            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
            
            GenericPhoto {
                visible: !message.isOutgoing
                anchors.fill: parent
                photoPath: message.sender && message.sender.profilePhoto ? message.sender.profilePhoto.small.local.path : ""
                initials: message.sender ? message.sender.initials : "N/A"
                avatarColor: message.sender ? message.sender.avatarColor(message.sender.id) : ""
            }
        }

        Rectangle {
            id: contentCol
            color: {
                if (message.isOutgoing) {
                    if (message.sendingState) {
                        return "lightblue"
                    }
                    return Suru.darkPositive
                }
                return "lightgrey"
            }
            radius: 4
            Layout.fillWidth: true
            Layout.alignment: message.isOutgoing ? Qt.AlignRight : Qt.AlignLeft
            Layout.maximumWidth: Suru.units.gu(45)
            Layout.preferredHeight: message.isOutgoing ? mc.height + Suru.units.dp(5) : mc.height

            Column {
                id: mc
                anchors {
                    left: parent.left
                    leftMargin: Suru.units.dp(5)
                    top: parent.top
                    topMargin: message.isOutgoing ? Suru.units.dp(10) : Suru.units.dp(5)
                    right: parent.right
                    rightMargin: Suru.units.dp(5)
                }

                Item {
                    visible: !message.isOutgoing
                    width: parent.width
                    height: Suru.units.gu(3)
                    RowLayout {
                        id: topBar
                        anchors.fill: parent
                        Label {
                            text: message.sender ? "%1 %2".arg(message.sender.firstName).arg(message.sender.lastName) : ""
                            font.bold: true
                            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                        }
                    }
                }

                Item {
                    id: mainContent
                    width: parent.width
                    height: children ? childrenRect.height : 0
                }

                Item {
                    width: parent.width
                    height: Suru.units.gu(3)
                    RowLayout {
                        id: bottomBar
                        anchors.fill: parent
                        Label {
                            text: message.formatDate(message.date)
                            color: message.isOutgoing ? "white" : Suru.foregroundColor
                            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                            Suru.textLevel: Suru.Small
                            Suru.textStyle: Suru.TertiaryText
                        }
                    }
                }
            }
        }
        Item {
            width: Suru.units.gu(1)
        }
    }
}
