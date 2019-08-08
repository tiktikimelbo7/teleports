import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK
import Ubuntu.Components.Popups 1.3 as UITK_Popups

UITK_Popups.Dialog {
    id: dialogue
    objectName: "dialogWaitCancel"

    property var progress: -1
    property var cancelButtonText: i18n.tr("Cancel")
    //Only main action should be colored. Cancel should be no color or ash
    property var cancelButtonColor: UITK.UbuntuColors.ash

    signal finished();
    signal cancelled();

    text: dialogue.text

    ProgressBar {
        value: progress
        indeterminate: progress < 0
    }

    UITK.Button {
        text: dialogue.cancelButtonText
        color: dialogue.cancelButtonColor
        onClicked: {
            dialogue.cancelled();
            UITK_Popups.PopupUtils.close(dialogue)
        }
    }
}
