import QtQuick 2.4
import QtQuick.Controls 2.2
import Ubuntu.Components 1.3 as UITK
import QtQuick.Controls.Suru 2.2
import "../actions"

// TODO: Use qqc2-suru
Page {
    id: settingsPage
    property color hf: Suru.foregroundColor
    property color hb: Suru.backgroundColor
    property color hd: Suru.neutralColor
    header: UITK.PageHeader {
        title: i18n.tr('Settings')

        UITK.StyleHints {
            foregroundColor: hf
            backgroundColor: hb
            dividerColor: hd
        }
        leadingActionBar.actions: [
            UITK.Action {
                iconName: "back"
                text: i18n.tr('Back')
                onTriggered: {
                    pageStack.pop()
                }
            }
        ]

    }
    Flickable {
        id: settingsFlickable
        anchors.fill: parent
        anchors.margins: units.gu(2)
        Column {
            id: settingsCloumn
            anchors.fill: parent
            spacing:units.dp(2)
            Button {
                id: logOutButton
                anchors.left: parent.left
                anchors.right: parent.right
                background: Rectangle {
                    implicitHeight: 40
                    color: logOutButton.down ? "#d6d6d6" : "#f6f6f6"
                    border.color: hf
                    border.width: 0
                    radius: 0
                }
                text: i18n.tr('Logout')
                anchors {
                    top: header.bottom
                }
                onClicked: AppActions.auth.logOut()
            }

            Button {
                id: deleteAccountButton
                anchors.left: parent.left
                anchors.right: parent.right
                background: Rectangle {
                    implicitHeight: 40
                    color: deleteAccountButton.down ? "#d6d6d6" : "#f6f6f6"
                    border.color: hf
                    border.width: 0
                    radius: 0
                }
                text: i18n.tr('Delete account') + " (disabled for safety)"
                anchors {
                    top: logOutButton.bottom
                }
                onClicked: AppActions.auth.deleteAccount()
                enabled: false
            }

        }

    }


}
