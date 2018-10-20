import QtQuick 2.4
import Ubuntu.Components 1.3
import "../actions"

// TODO: Use qqc2-suru
Page {
    id: waitPassword

    header: PageHeader {
        title: "Enter Password:"
    }

    TextField {
        id: passwordField
        anchors.centerIn: parent

    }

    Label {
        text: "Password hint: "
    }

    Button {
        text: "Send"
        anchors {
            top: passwordField.bottom
            topMargin: units.gu(1)
            horizontalCenter: parent.horizontalCenter
        }
        onClicked: AppActions.auth.setPassword(passwordField.text)
    }
}
