import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import QTelegram 1.0
import QTelegramStyles 1.0
import "../components"

MessageContentBase {
    id: item

    property QTdContact contact: message.content.contact
    property QTdUser user: contact.user

    RowLayout {
        GenericPhoto {
            id: avatar
            height: units.gu(13)
            width: height
            photoPath: user.profilePhoto.small.local.path ?
                    user.profilePhoto.small.local.path : ""
            initials: user.initials ? user.initials : "N/A"
            myself: false
            visible: contact.user_id != "0"
        }        
        ColumnLayout {
            anchors {
                top: parent.top
                topMargin: message.isOutgoing ? Suru.units.dp(10) : Suru.units.dp(5)
            }
            Text {

                text: "<b>%1 %2</b>".arg(contact.first_name).arg(contact.last_name)
            }
            Text {

                text: contact.phone_number ? contact.phone_number
                                        : ""
            }
            Text {

                text: user.username ? "@" + user.username
                                          : ""
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            //TODO
        }
    }
}

