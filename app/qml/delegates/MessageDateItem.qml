import QtQuick 2.9

MessageActionItem {
    text: message ? content.dateString : i18n.tr("Some date missing")
}
