import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK
import Ubuntu.Components.Popups 1.3 as UITK_Popups

UITK_Popups.Dialog {
    id: dialogue

    property var question: ""
    property var confirmButtonText: i18n.tr("Okay")
    property var cancelButtonText: i18n.tr("Cancel")
    property var confirmButtonColor: UITK.UbuntuColors.green
    property var cancelButtonColor: UITK.UbuntuColors.red
    
    signal confirmed();
    signal canceled();

    text: dialogue.text

    UITK.Button {
        objectName: "confirmBtn"
        text: dialogue.confirmButtonText
        color: dialogue.confirmButtonColor
        onClicked: {
            dialogue.confirmed();
            UITK_Popups.PopupUtils.close(dialogue)
        }
    }

    UITK.Button {
        objectName: "cancelBtn"
        text: dialogue.cancelButtonText
        color: dialogue.cancelButtonColor
        onClicked: {
            dialogue.canceled();
            UITK_Popups.PopupUtils.close(dialogue)
        }
    }
}
