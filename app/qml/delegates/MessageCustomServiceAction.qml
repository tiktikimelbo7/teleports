import QtQuick 2.9
import "../actions"

MessageActionItem {
    text: "%1 %2".arg(
              message.isCollapsed
              ? ""
              : message.sender.firstName
                ? message.sender.firstName
                : message.sender.username)
                .arg(message.content.text)
}
