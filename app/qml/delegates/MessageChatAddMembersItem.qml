import QtQuick 2.9
import QTelegram 1.0
import "../actions"

MessageActionItem {

    property QTdMessageChatAddMembers content: message.content

    text: content.members.count ? i18n.tr("%1 joined the group").arg(getUsersString()) : "Unknown joined group"

    onClicked: {
        // 99 times out of 100 members only contains a single
        // user as they have joined the group them selves rather
        // than being batch added so just display the first user for now.
        // TODO DESIGN: Need design input on how to show multiple users
        // ... dialog or page?? etc
        if (content.members.count > 0) {
            AppActions.user.showUserInfo(content.members.get(0))
        }
    }

    function getUsersString() {

        if (message.isCollapsed && content.members.count == 1) {
            return ""
        }
       
        // If more than 3 users display a string like
        // "5 users joined the group"
        if (content.members.count > 3) {
            return i18n.tr("%1 users").arg(content.members.count)
        }
        // Else get their firstname or username for display
        var users = []
        for (var i = 0; i < content.members.count; i++) {
            var user = content.members.get(i)
            if (user.firstName !== "") {
                users.push(user.firstName)
            } else {
                users.push(user.username)
            }
        }
        return users.toString()
    }

}
