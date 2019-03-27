import QtQuick 2.9
import "../actions"

MessageActionItem {
    text: i18n.tr("%1 joined by invite link").arg(
              message.isCollapsed
              ? ""
              : message.sender.firstName
                ? message.sender.firstName
                : message.sender.username)
    onClicked: AppActions.user.showUserInfo(message.sender)
}
