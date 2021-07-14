import QtQuick 2.9
import Ubuntu.Components 1.3 as UITK

UITK.Action {
    id: backAction

    property bool enableShortcut: false

    iconName: "back"
    text: i18n.tr('Back')
    shortcut: enableShortcut ? StandardKey.Cancel : undefined
}
