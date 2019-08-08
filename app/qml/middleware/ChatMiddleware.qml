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
            confirmButtonColor: UITK.UbuntuColors.red
            onConfirmed: next(action, message)
        }
    }

    function deleteChatHistory(message) {
        var dlg = PopupUtils.open(confirmationDlg, rootItem, {
                        title: i18n.tr("Are you sure you want to clear the history?"),
                        confirmButtonText: i18n.tr("Clear history"),
                        })
        dlg.confirmed.connect(function(){
            next(ChatKey.deleteChatHistory, message)
        })
    }

    function leaveChat(message) {
        var dlg = PopupUtils.open(confirmationDlg, rootItem, {
                        title: i18n.tr("Are you sure you want to leave this chat?"),
                        confirmButtonText: i18n.tr("Leave"),
                        })
        dlg.confirmed.connect(function(){
            next(ChatKey.leaveChat, message)
        })
    }
}
