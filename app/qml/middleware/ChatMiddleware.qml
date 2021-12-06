import QtQuick 2.9
import QuickFlux 1.1
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
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
        }
    }

    Component {
        id: inviteLinkDialog
        GroupPreviewDialog {}
    }

    Component {
        id: setTTLDialog
        SetTTLDialog {}
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
        var dlg = PopupUtils.open(inviteLinkDialog, null, {
                        confirmButtonText: i18n.tr("Join group"),
                        inviteLinkInfo: message.info,
                        })
        dlg.confirmed.connect(function(){
            next(ChatKey.joinChatByInviteLink, message)
        })
    }

    function setTTL(message) {
        var dlg = PopupUtils.open(setTTLDialog, root, {secretChat: message.chat})
    }
}
