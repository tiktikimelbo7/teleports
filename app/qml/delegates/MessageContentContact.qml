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

    width: Math.max(maximumAvailableContentWidth / 2, renderedContact.width)

    RowLayout {
        id: renderedContact

        GenericPhoto {
            id: avatar
            height: units.gu(8)
            width: height
            photoPath: user && user.profilePhoto.small.local.path ?
                    user.profilePhoto.small.local.path : ""
            avatarColor: user? user.avatarColor(user.id) : "#7ec455"
            initials: user? user.initials : getInitials()
            myself: false
            function getInitials() {
                if(contact.first_name != "") {
                    if(contact.last_name != "") {
                        return (contact.first_name.substr(0,1) + contact.last_name.substr(0,1)).toUpperCase();
                    }
                    return contact.first_name.substr(0,2).toUpperCase();
                }
                if(contact.last_name != "") {
                    return contact.last_name.substr(0,2).toUpperCase();
                }
                if(user.username != "") {
                    return user.username.substr(0,2).toUpperCase();
                }
                return "N/A";
            }
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

                text: user? "@" + user.username
                                          : ""
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            
        }
    }
}

