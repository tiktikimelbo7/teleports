import QtQuick 2.9

MessageActionItem {
    text: i18n.tr("Message time-to-live has been set to <b>%1</b> seconds").arg(message.content.ttl)
}
