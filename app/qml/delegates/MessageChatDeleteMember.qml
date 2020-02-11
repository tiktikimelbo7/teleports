import QtQuick 2.9
import "../actions"

MessageActionItem {
    text: message.senderUserId == content.userId
            ? i18n.tr("%1 left the group").arg(getUsersString())
            : i18n.tr("%1 removed %2").arg(getRemovingUserString()).arg(getUserString())

    onClicked: AppActions.user.showUserInfo(message.sender, null)

        function getRemovingUserString() {
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

    function getUserString() {

        if (message.isCollapsed) {
            return ""
        }
          var user = content.user
          if (user.firstName !== "") {
            var fullName = user.firstName
            if (user.lastName !== "")
              fullName = fullName + " " + user.lastName
            return fullName
          } else {
            return user.username
          }
    }
}
