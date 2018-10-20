import QtQuick 2.4
import Ubuntu.Components 1.3
import QuickFlux 1.1
import "../actions"

Page {
    id: waitUserProfilePage

    header: PageHeader {
        title: i18n.tr("Enter User Details")
    }

    Item {
        anchors.centerIn: parent

        TextField {
            id: firstNameField
            anchors.centerIn: parent
            placeholderText: i18n.tr("First Name")
        }

        TextField {
            id: lastNameField
            anchors {
                top: firstNameField.bottom
                topMargin: units.gu(1)
                horizontalCenter: parent.horizontalCenter
            }
            placeholderText: i18n.tr("Last Name")
        }

        TextField {
            id: codeField
            anchors {
                top: lastNameField.bottom
                topMargin: units.gu(1)
                horizontalCenter: parent.horizontalCenter
            }
            placeholderText: i18n.tr("Code")
        }

        Button {
            id: nextButton
            text: i18n.tr("Next...")
            anchors {
                top: codeField.bottom
                topMargin: units.gu(1)
                horizontalCenter: parent.horizontalCenter
            }
            onClicked: sendCode.run({ firstname: firstNameField.text, lastname: lastNameField.text, code: codeField.text })
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

    }

    AppScript {
       id: sendCode
       script: {

           console.log("sendCode Begin")
           AppActions.auth.setCode(message.code, message.firstname, message.lastname);

           once(AuthKey.authCodeError, function(message) {
               errorLabel.text = message.error;
               exit(1);
           })

           once(AuthKey.authCodeAccepted, exit.bind(this,0))
           console.log("sendCode End")
       }
    }
}
