import QtQuick 2.9
import "../actions"

MessageActionItem {
    text:   i18n.tr("%1 changed the chat title to <i>%2</i>").arg(message.isCollapsed
              ? ""
              : getAddingUserString()).arg(content.title)

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
