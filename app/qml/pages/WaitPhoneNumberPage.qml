import QtQuick 2.4
import QtQuick.Controls 2.2
import Ubuntu.Components 1.3 as UITK
import QtQuick.Controls.Suru 2.2
import QuickFlux 1.1
import "../actions"

Page {
    id: waitPhoneNumberPage

    header: UITK.PageHeader {
        title: i18n.tr("Enter Phone Number")
    }

    TextField {
        id: textfield
        anchors.centerIn: parent
        text: ""
        inputMethodHints: Qt.ImhFormattedNumbersOnly
        placeholderText: "+<intl><phonenumber>"
    }

    Button {
        id: nextButton
        text: i18n.tr("Next...")
        anchors {
            top: textfield.bottom
            topMargin: units.gu(1)
            horizontalCenter: parent.horizontalCenter
        }
        onClicked: sendNumber.run({number: textfield.text})
    }

    Label {
        id: errorLabel
        anchors {
            top: nextButton.bottom
            topMargin: units.gu(1)
            horizontalCenter: parent.horizontalCenter
        }
        visible: text != ""
        color: "red"
    }

    AppScript {
       id: sendNumber
       script: {
           // Enter number including dial code
           AppActions.auth.setPhoneNumber("", message.number);

           once(AuthKey.authPhoneNumberError, function(message) {
               errorLabel.text = message.error;
               exit(1);
           })

           once(AuthKey.authPhoneNumberAccepted, exit.bind(this,0))
       }
    }
}
