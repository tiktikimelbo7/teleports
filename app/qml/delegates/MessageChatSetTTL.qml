import QtQuick 2.9
import "../actions"

MessageActionItem {
    text: i18n.tr("The message time-to-live has been set to <b>%1</b> seconds!").arg(content.ttl)
}
