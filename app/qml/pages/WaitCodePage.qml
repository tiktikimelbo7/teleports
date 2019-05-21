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
        id: header
        title: i18n.tr('Enter Code')

        UITK.StyleHints {
            foregroundColor: hf
            backgroundColor: hb
            dividerColor: hd
        }
    }
    
    Column {
        width: Math.min( Suru.units.gu(45), parent.width - units.gu(4) )
        spacing: Suru.units.gu(2)
        
        anchors.top: header.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        Rectangle {
            width: parent.width
            height: 1
        }
        
        TextField {
            id: firstNameField
            width: parent.width
            
            visible: !registered
            placeholderText: i18n.tr('First Name')
        }

        TextField {
            id: lastNameField
            width: parent.width
            
            visible: !registered
            placeholderText: i18n.tr('Last Name')
        }

        UITK.TextField {
            id: codeField
            width: parent.width

            placeholderText: i18n.tr('Code')
            inputMethodHints: Qt.ImhFormattedNumbersOnly
            inputMask: "99999"
            maximumLength: 5
              
            onDisplayTextChanged: {
              if( length === maximumLength ) {
                sendCode.run({ firstname: firstNameField.text, lastname: lastNameField.text, code: text })
              }
            }
        }

        Label {
            width: parent.width
            wrapMode: Text.Wrap
            text: i18n.tr("We've send a code via telegram to your device. Please enter it here.")
        }
        
        Label {
            id: errorLabel
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
              
            visible: text != ''
            color: "red"
        }
    }

    AppListener {
      Filter {
        type: AuthKey.authCodeError
          onDispatched: {
            errorLabel.text = message.error
        }
      }
    }

    AppScript {
       id: sendCode
       script: {
           // Enter number including dial code
           AppActions.auth.setCode(message.code, message.firstname, message.lastname);
           once(AuthKey.authCodeAccepted, exit.bind(this,0))
       }
    }
}
