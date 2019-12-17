import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK
import Ubuntu.Components.Popups 1.3 as UITK_Popups

UITK_Popups.Dialog {
    id: dialogue
    objectName: "dialogPopup"

    //confirmButtonText should be a clear text
    //i.e. To the question: 'Do you want to delete this file?' Main action text should be 'Delete' instead of 'Okay' / 'Yes' or similar
    property var confirmButtonText: i18n.tr("Okay")
    property var cancelButtonText: i18n.tr("Cancel")
    //confirmButtonColor should be green for positive/go actions, blue for neutral actios and red for destructive actions
    property var confirmButtonColor: UITK.UbuntuColors.green
    //Only main action should be colored. Cancel should be no color or ash
    property var cancelButtonColor: UITK.UbuntuColors.ash
    //place new Items before the Buttons
    default property alias content: top_col.data

    signal confirmed();
    signal canceled();

    text: dialogue.text

    Column {
        id: top_col
        spacing: Suru.units.gu(2)
    }

    UITK.Button {
        text: dialogue.confirmButtonText
        color: dialogue.confirmButtonColor
        onClicked: {
            Qt.inputMethod.commit();
            dialogue.confirmed();
            UITK_Popups.PopupUtils.close(dialogue)
        }
    }

    UITK.Button {
        text: dialogue.cancelButtonText
        color: dialogue.cancelButtonColor
        onClicked: {
            dialogue.canceled();
            UITK_Popups.PopupUtils.close(dialogue)
        }
    }
}
