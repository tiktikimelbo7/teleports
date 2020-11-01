import QtQuick 2.9
import "../actions"

MessageActionItem {
    text:  message.isChannelPost
        ? i18n.tr("%1 pinned a message").arg(currentChat.title).trim()
        : i18n.tr("%1 pinned a message").arg(getAddingUserString()).trim()

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
}
