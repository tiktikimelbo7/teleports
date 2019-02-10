import QtQuick 2.9

MessageActionItem {
    text: message ? message.content.date.toLocaleDateString(Qt.locale(), i18n.tr("dd MMMM")) : "Some date missing"
}
