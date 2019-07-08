import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import QTelegram 1.0
import QTelegramStyles 1.0
import "../components"

MessageContentBase {
    id: item

    property QTdMessageContact contact: message.content

    Text {
        anchors {
            top: parent.top
            topMargin: message.isOutgoing ? Suru.units.dp(10) : Suru.units.dp(5)
        }

        text: i18n.tr("Contact") + ":<br>"
           + "<b>%1 %2</b><br>".arg(contact.contact.first_name).arg(contact.contact.last_name)
           + contact.contact.phone_number
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            //TODO
        }
    }
}

