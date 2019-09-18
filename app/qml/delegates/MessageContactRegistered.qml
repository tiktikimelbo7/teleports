import QtQuick 2.9
import "../actions"

MessageActionItem {
    text: i18n.tr("%1 has joined Telegram!".arg(
              message.isCollapsed
              ? ""
              : getAddingUserString()))

    function getAddingUserString() {
        if (message.isCollapsed) {
            return ""
        }
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
