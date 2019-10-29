import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK
import QTelegram 1.0
import QTelegramStyles 1.0
import "../stores"
import "../actions"

Item {
    property QTdMessage messageCited: null

    height: column.height + Suru.units.gu(0.5)
    width: Math.min(maximumAvailableContentWidth, childrenRect.width)

    Rectangle {
        id: colorBlock
        anchors {
            left: parent.left
            top: parent.top
        }
        width: Suru.units.dp(3)
        height: parent.height - Suru.units.gu(0.5)
        color: Suru.foregroundColor
        opacity: 0.8
    }

    Column {
        id: column

        spacing: 0

        anchors {
            left: colorBlock.right
            leftMargin: Suru.units.gu(1)
            top: parent.top
        }

        Label {
            id: senderLabel
            height: Suru.units.gu(2.5)
            width: contentWidth
            text: messageCited.sender ? "%1 %2".arg(messageCited.sender.firstName).arg(messageCited.sender.lastName) : ""
            font.bold: false
            color: messageCited.sender ? messageCited.sender.avatarColor(messageCited.sender.id) : ""
        }

        TextMetrics {
            id: senderNameMetrics
            text: senderLabel.text
        }

        Loader {
            id: loader
            active: (messageCited != null)
            Component.onCompleted: (messageCited && messageCited.content) ?
                                        setSource(contentDelegateMap.findComponent(messageCited.content.type), {message: messageCited}) :
                                        setSource("qrc:///delegates/MessageUnavailable.qml")
        }
    }

    MouseArea {
        id: citationControl
        anchors.fill: parent
        enabled: parent.visible
        onClicked: {
            // Disable for 0.6.0, finish and reenable it for 0.7.0
            //AppActions.chat.jumpToMessage(message.messageRepliedTo.id)
            //message.messageRepliedTo.highlight()    //FIXME - this doesn't work
        }
    }
}
