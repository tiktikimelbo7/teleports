import QtQuick 2.9
import QTelegram 1.0

MessageActionItem {

    property QTdMessageChatAddMembers content: message.content

    text: content.members.count ? i18n.tr("%1 joined the group").arg(getUsersString()) : "Unknown joined group"

    function getUsersString() {

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
