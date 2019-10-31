import QtQuick 2.9
import QTelegram 1.0
import "../components"
import "../actions"
import "../stores"

MessageActionItem {

    text: i18n.tr("%1 pinned %2").arg(getAddingUserString()).arg(message.content.message)

    function getAddingUserString() {
        if (message.isCollapsed) {
            return ""
        }
        if(!message.sender)
            return "";
        if (message.sender.firstName !== "") {
            var fullName = message.sender.firstName
            if (message.sender.lastName !== "")
                fullName = fullName + " " + message.sender.lastName
            return fullName
        } else {
            return message.sender.username  
        }
    }
    Component.onCompleted: {
        message.content.chatId = message.chatId;
    }
}
