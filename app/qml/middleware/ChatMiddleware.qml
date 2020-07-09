import QtQuick 2.9
import QuickFlux 1.1
import Ubuntu.Components 1.3 as UITK
import Ubuntu.Components.Popups 1.3
import "../actions"
import "../components"

Middleware {
    filterFunctionEnabled: true

    Component {
        id: confirmationDlg
        PopupDialog {
            confirmButtonColor: theme.palette.normal.negative
            onConfirmed: next(action, message)
        }
    }

    Component {
        id: inviteLinkDialog
        PopupDialog {
            confirmButtonColor: theme.palette.normal.activity
            onConfirmed: next(action, message)
        }
    }

    function deleteChatHistory(message) {
        var dlg = PopupUtils.open(confirmationDlg, root, {
                        title: i18n.tr("Are you sure you want to clear the history?"),
                        confirmButtonText: i18n.tr("Clear history"),
                        })
        dlg.confirmed.connect(function(){
            next(ChatKey.deleteChatHistory, message)
        })
    }

    function leaveChat(message) {
        var dlg = PopupUtils.open(confirmationDlg, root, {
                        title: i18n.tr("Are you sure you want to leave this chat?"),
                        confirmButtonText: i18n.tr("Leave"),
                        })
        dlg.confirmed.connect(function(){
            next(ChatKey.leaveChat, message)
        })
    }

    function showChatInviteLinkInfo(message) {
        console.log(message.info.title)
        var dlg = PopupUtils.open(inviteLinkDialog, null, {
                        title: message.info.title,
                        text: i18n.tr("%1 member", "%1 members", message.info.memberCount).arg(message.info.memberCount),
                        confirmButtonText: i18n.tr("Join"),
                        })
        dlg.confirmed.connect(function(){
            next(ChatKey.joinChatByInviteLink, message)
        })
    }
}
