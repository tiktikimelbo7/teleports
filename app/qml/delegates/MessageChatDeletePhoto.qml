import QtQuick 2.9
import "../actions"

MessageActionItem {
    text:  message.isChannelPost
        ? i18n.tr("Channel photo has been removed")
        : i18n.tr("%1 deleted the chat photo").arg(message.isCollapsed
              ? ""
              : getAddingUserString())

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
