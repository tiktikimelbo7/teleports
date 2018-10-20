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
        onClicked: sendCode.run({code: codeField.text})
    }

    Label {
        id: errorLabel
        visible: text != ""
        color: "red"
    }

    AppScript {
       id: sendCode
       script: {
           // Enter number including dial code
           AppActions.auth.setCode(message.code, "", "");

           once(AuthKey.authCodeError, function(message) {
               errorLabel.text = message.error;
               exit(1);
           })

           once(AuthKey.authCodeAccepted, exit.bind(this,0))
       }
    }
}
