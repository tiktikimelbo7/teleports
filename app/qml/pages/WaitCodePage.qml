import QtQuick 2.4
import Ubuntu.Components 1.3
import "../actions"

// TODO: Use qqc2-suru
Page {
    id: waitCodePage

    header: PageHeader {
        title: i18n.tr("Enter Code")
    }

    TextField {
        id: codeField
        anchors.centerIn: parent

    }
    Button {
        text: i18n.tr("Next...")
        anchors {
            top: codeField.bottom
            topMargin: units.gu(1)
            horizontalCenter: parent.horizontalCenter
        }
        onClicked: AppActions.auth.setCode(codeField.text)
    }
}
