import QtQuick 2.4
import QtQuick.Controls 2.2
import Ubuntu.Components 1.3 as UITK
import QtQuick.Controls.Suru 2.2
import QuickFlux 1.1
import "../actions"

// TODO: Use qqc2-suru
Page {
    id: waitCodePage

    property bool registered
    property color hf: Suru.foregroundColor
    property color hb: Suru.backgroundColor
    property color hd: Suru.neutralColor
    header: UITK.PageHeader {
        title: i18n.tr('Enter Code')

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

          Item {
              id: firstNameFieldContainer

              width: (!registered) ? firstNameField.implicitWidth : 0
              height: (!registered) ? firstNameField.implicitHeight : 0

              anchors {
                  centerIn: parent
                  horizontalCenter: parent.horizontalCenter
              }

              TextField {
                  id: firstNameField
                  visible: !registered
  //                 visible: false
                  placeholderText: i18n.tr("First Name")
              }
          }

          Item {
              id: lastNameFieldContainer

              width: (!registered) ? lastNameField.implicitWidth : 0
              height: (!registered) ? lastNameField.implicitHeight : 0

              anchors {
                  top: firstNameFieldContainer.bottom
                  topMargin: units.gu(1)
                  horizontalCenter: parent.horizontalCenter
              }

              TextField {
                  id: lastNameField
                  visible: !registered
  //                 visible: false
                  placeholderText: i18n.tr("Last Name")
              }
          }

          TextField {
              id: codeField
              anchors {
                  top: lastNameFieldContainer.bottom
                  topMargin: units.gu(1)
                  horizontalCenter: parent.horizontalCenter
              }
              placeholderText: i18n.tr("Code")
              inputMethodHints: Qt.ImhFormattedNumbersOnly
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
    }

    AppScript {
       id: sendCode
       script: {
           // Enter number including dial code
           AppActions.auth.setCode(message.code, message.firstname, message.lastname);

           once(AuthKey.authCodeError, function(message) {
               errorLabel.text = message.error;
               exit(1);
           })

           once(AuthKey.authCodeAccepted, exit.bind(this,0))
       }
    }
}
