import QtQuick 2.9
import QuickFlux 1.1
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.3
import "../actions"

Middleware {
    filterFunctionEnabled: true

    Component {
        id: errorDlg

        Dialog {
            id: dlg
            property string callback

            Button {
                width: parent.width
                text: i18n.tr("Close")
                onClicked: {
                    if (callback !== "") {
                        next(dlg.callback)
                    }
                    PopupUtils.close(dlg)
                }
            }
        }
    }

    function showError(message) {
        PopupUtils.open(errorDlg, rootItem, {
                        title: message.title,
                        text: message.description,
                        callback: message.callback ? message.callback : ""
                        })
    }
}
