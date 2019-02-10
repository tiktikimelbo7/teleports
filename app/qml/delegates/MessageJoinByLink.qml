import QtQuick 2.9

MessageActionItem {
    text: i18n.tr("%1 joined by invite link").arg(
              message.sender.firstName
              ? message.sender.firstName
              : message.sender.username)
}
