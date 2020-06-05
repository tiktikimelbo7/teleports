import QtQuick 2.9

MessageActionItem {
    text: message ? message.content.dateString : "Some date missing"
}
