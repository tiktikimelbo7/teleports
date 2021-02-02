import QtQuick 2.4
import QtQuick.Controls 2.2
import Ubuntu.Components 1.3 as UITK
import QtQuick.Controls.Suru 2.2
import QuickFlux 1.1
import "../actions"

Page {
    id: waitCodePage

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
        width: Math.min(Suru.units.gu(45), parent.width - units.gu(4))
        spacing: Suru.units.gu(2)

        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            topMargin: Suru.units.gu(2)
        }
        UITK.TextField {
            id: codeField
            width: parent.width

            placeholderText: i18n.tr('Code')
            inputMethodHints: Qt.ImhFormattedNumbersOnly
            inputMask: "99999"
            maximumLength: 5
            focus: true
            onDisplayTextChanged: {
              if( text.length === maximumLength ) {
                sendCode.run({ code: text })
              }
            }
        }

        Label {
            width: parent.width
            wrapMode: Text.Wrap
            text: i18n.tr("A code was sent via Telegram to your other devices. Please enter it here.")
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
                if(!message.isLengthWarning)
                    codeField.text = ""
            }
        }
    }

    AppScript {
       id: sendCode
       script: {
           // Enter number including dial code
           AppActions.auth.setCode(message.code);
            once(AuthKey.authCodeError, function(message) {
               exit(1);
           })
           once(AuthKey.authCodeAccepted, exit.bind(this,0))
       }
    }
}
