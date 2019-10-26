import QtQuick 2.9
import "../actions"

MessageActionItem {
     text: message.senderUserId === content.userId
            ? i18n.tr("%1 left the group").arg(message.sender.fullName)
            : i18n.tr("%1 removed %2").arg(message.sender.fullName).arg(content.user.fullName)
    onClicked: AppActions.user.showUserInfo(message.sender)
}
