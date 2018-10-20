import QtQuick 2.4
import Ubuntu.Components 1.3
import QuickFlux 1.1
import "../actions"

Page {
    id: waitPasswordPage

    header: PageHeader {
        title: i18n.tr("Enter Password")
    }

    TextField {
        id: passwordField
        anchors.centerIn: parent
        text: ""
        echoMode: TextInput.Password // TODO Change to PasswordEchoOnEdit
    }

    Label {
        id: hintLabel
        text: i18n.tr("Password hint: ")
        anchors {
            top: passwordField.bottom
            topMargin: units.gu(1)
            horizontalCenter: parent.horizontalCenter
        }
    }

    Button {
        anchors {
            top: hintLabel.bottom
            topMargin: units.gu(1)
            horizontalCenter: parent.horizontalCenter
        }
        onClicked: sendPassword.run({password: passwordField.text})
    }

    AppScript {
       id: sendPassword
       script: {

           AppActions.auth.setPassword(message.password);

           once(AuthKey.authPasswordError, function(message) {
               errorLabel.text = message.error;
               exit(1);
           })

           once(AuthKey.authPasswordAccepted, exit.bind(this,0))
       }
    }
}
