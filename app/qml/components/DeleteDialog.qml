import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import "../actions"

PopupDialog {
    property var message
    text: message.canBeDeletedForAllUsers ?
    i18n.tr("The message will be deleted for all users in the chat. Do you really want to delete it?") :
    i18n.tr("The message will be deleted only for you. Do you really want to delete it?")
    confirmButtonColor: theme.palette.normal.negative
    confirmButtonText: i18n.tr("Delete")
    onConfirmed: AppActions.chat.deleteMessage(message.id)
}
