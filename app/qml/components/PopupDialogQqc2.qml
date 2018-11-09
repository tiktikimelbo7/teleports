import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK

Popup {
    id: dialogue
    objectName: "dialogPopup"
    focus: true
    margins: units.gu(2)

    property var text: ""
    property var confirmButtonText: i18n.tr("Okay")
    property var cancelButtonText: i18n.tr("Cancel")
    property var confirmButtonColor: UITK.UbuntuColors.green
    property var cancelButtonColor: UITK.UbuntuColors.red
    
    signal confirmed();
    signal canceled();

    ColumnLayout {
        anchors.fill: parent
        //anchors.horizontalCenter: parent.horizontalCenter
        //anchors.verticalCenter: parent.verticalCenter

        Label {
            text: dialogue.text
            wrapMode: Text.WordWrap
            maximumLineCount: 5
            elide: Text.ElideRight
        }

        RowLayout {
            anchors.fill: parent
            anchors.horizontalCenter: parent.horizontalCenter

            Button {
                text: confirmButtonText
                //color: confirmButtonColor
                onClicked: {
                    dialogue.confirmed();
                    dialogue.close()
                }
            }

            Button {
                text: cancelButtonText
                //color: cancelButtonColor
                onClicked: {
                    dialogue.canceled();
                    dialogue.close()
                }
            }
        }
    }
}
