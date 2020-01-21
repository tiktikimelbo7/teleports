import QtQuick 2.4
import QtQuick.Controls 2.2
import Ubuntu.Components 1.3 as UITK
import Ubuntu.Components.Popups 1.3 as UITK_Popups
import QtQuick.Controls.Suru 2.2
import QtQuick.Layouts 1.1
import QTelegram 1.0
import "../actions"
import "../components"

Page {
    id: userProfilePage

    property QTdUser user: null
    property QTdChat chat: null

    property color hf: Suru.foregroundColor
    property color hb: Suru.backgroundColor
    property color hd: Suru.neutralColor
    header: UITK.PageHeader {
        title: i18n.tr('Profile')

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
                    AppActions.view.popFromStack()
                }
            }
        ]
        trailingActionBar.actions: [
            UITK.Action {
                iconName: "edit"
                text: i18n.tr('Edit')
                onTriggered: {
                    UITK_Popups.PopupUtils.open(editUserDialog)
                }
            }
        ]
    }
    Component {
        id: editUserDialog
        PopupDialog {
            text: i18n.tr("Edit user data and press Save")
            confirmButtonColor: theme.palette.normal.positive
            confirmButtonText: i18n.tr("Save")
            onConfirmed: AppActions.user.addUser(userName.text, firstName.text, lastName.text)
            UITK.TextField {
                anchors {
                    left: parent.left
                    right: parent.right
                }
                id: userName
                text: '+' + user.phoneNumber
                readOnly: true
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                placeholderText: i18n.tr("Phone no")
            }
            UITK.TextField {
                anchors {
                    left: parent.left
                    right: parent.right
                }
                id: firstName
                text: user.firstName
                placeholderText: i18n.tr("First name")
            }
            UITK.TextField {
                anchors {
                    left: parent.left
                    right: parent.right
                }
                id: lastName
                text: user.lastName
                placeholderText: i18n.tr("Last name")
            }
        }
    }

    Flickable {
        id: userProfileFlickable
        anchors.fill: parent
        anchors.margins: units.gu(2)
        contentHeight: userProfile.height
        UserProfile {
            id: userProfile
            user: userProfilePage.user
            chat: userProfilePage.chat
        }
    }
}
