import QtQuick 2.4
import QtQuick.Controls 2.2
import Ubuntu.Components 1.3 as UITK
import QtQuick.Controls.Suru 2.2
import QuickFlux 1.1
import "../actions"

Page {
    id: waitPasswordPage

    property string hint
    header: UITK.PageHeader {
        title: i18n.tr('Enter Password')

        UITK.StyleHints {
            foregroundColor: hf
            backgroundColor: hb
            dividerColor: hd
        }

    }
    Flickable {
        id: waitCodeFlickable
        anchors.fill: parent
        anchors.margins: units.gu(2)
        Item {
          anchors.centerIn: parent
          width: parent.width
          TextField {
              id: passwordField
              anchors.centerIn: parent
              placeholderText: i18n.tr("Password")
              text: ""
              echoMode: TextInput.Password // TODO Change to PasswordEchoOnEdit
          }

          Label {
              id: hintLabel
              text: qsTr("Password hint: %1").arg(hint)
              anchors {
                  top: passwordField.bottom
                  topMargin: units.gu(1)
                  horizontalCenter: parent.horizontalCenter
              }
          }

          Button {
              text: i18n.tr("Next...")
              anchors {
                  top: hintLabel.bottom
                  topMargin: units.gu(1)
                  horizontalCenter: parent.horizontalCenter
              }
              onClicked: sendPassword.run({password: passwordField.text})
          }
        }
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
