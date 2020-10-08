import QtQuick 2.9
import "../actions"

MessageActionItem {
    text:  message.isChannelPost
        ? i18n.tr("Channel title has been changed to <b>%1</b>").arg(content.title)
        : i18n.tr("%1 changed the chat title to <b>%2</b>").arg(getAddingUserString()).arg(content.title).trim()

    function getAddingUserString() {
        if (message.isCollapsed) {
            return ""
        }
        if (!message.sender)
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
