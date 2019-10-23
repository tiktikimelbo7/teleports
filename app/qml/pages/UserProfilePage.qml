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
            confirmButtonColor: UITK.UbuntuColors.green
            confirmButtonText: i18n.tr("Save")
            onConfirmed: AppActions.user.addUser(userName.text, firstName.text, lastName.text)
            UITK.TextArea {
                height: units.gu(5)
                autoSize: true
                anchors {
                    left: parent.left
                    right: parent.right
                    topMargin: units.gu(0.1)
                }
                id: userName
                text: '+' + user.phoneNumber
                readOnly: true
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                placeholderText: i18n.tr("Phone no")
            }
            UITK.TextArea {
                height: units.gu(5)
                autoSize: true
                anchors {
                    left: parent.left
                    right: parent.right
                    topMargin: units.gu(0.1)
                }
                id: firstName
                text: user.firstName
                placeholderText: i18n.tr("First name")
            }
            UITK.TextArea {
                height: units.gu(5)
                autoSize: true
                anchors {
                    left: parent.left
                    right: parent.right
                    topMargin: units.gu(0.1)
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
        contentHeight: userProfileColumn.height

        ColumnLayout {
            id: userProfileColumn
            anchors {
                fill: parent
            }

            UserProfile {
                user: userProfilePage.user
            }
        }
    }
}
