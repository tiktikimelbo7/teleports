import QtQuick 2.9
import QtQuick.Controls 2.2
import Ubuntu.Components 1.3 as UITK
import Ubuntu.Components.ListItems 1.3 as UITK_ListItem
import Ubuntu.Components.Popups 1.3 as UITK_Popups
import QtQuick.Controls.Suru 2.2
import QtQuick.Layouts 1.1
import QTelegram 1.0
import "../actions"
import "../components"
import "../stores"

Page {
    id: settingsPage
    property QTdUser me: Telegram.users.me

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
                  AppActions.view.popFromStack()
                }
            }
        ]
    }

    Flickable {
        id: settingsFlickable
        anchors.fill: parent
        anchors.margins: units.gu(2)
        contentHeight: settingsColumn.height

        ColumnLayout {
            id: settingsColumn
            width: parent.width
            spacing: 0

            UserProfile {
                Layout.fillWidth: true
                user: me
                parentMargins: settingsFlickable.anchors.margins
                isSettings: true
                itemList: [
                {
                    iconName: "system-log-out",
                    titleText: i18n.tr('Logout'),
                    delegateState: "popup",
                    popup: logoutConfirmationDialog
                },
                {
                    iconName: "delete",
                    titleText: i18n.tr("Delete account"),
                    delegateState: "popup",
                    popup: deleteAccountConfirmationDialog
                },
                {
                    iconName: "info",
                    titleText: i18n.tr("Connectivity status"),
                    delegateState: "new-page",
                    newPage: "qrc:/pages/ConnectivityPage.qml",
                    newPageParams: {connectionManager: Telegram.connections}
                }
                ]
            }
        }
    }

    Component {
        id: logoutConfirmationDialog
        PopupDialog {
            text: i18n.tr("Warning: Logging out will delete all local data from this device, including secret chats. Are you still sure you want to log out?")
            confirmButtonColor: theme.palette.normal.negative
            confirmButtonText: i18n.tr("Logout")
            onConfirmed: AppActions.auth.logOut()
        }
    }

    Component {
        id: deleteAccountConfirmationDialog
        PopupDialog {
            text: i18n.tr("Warning: Deleting the account will delete all the data you ever received or send using telegram except for data you have explicitly saved outside the telegram cloud. Are you really really sure you want to delete your telegram account?")
            confirmButtonColor: theme.palette.normal.negative
            confirmButtonText: i18n.tr("Delete")
            onConfirmed: AppActions.auth.deleteAccount()
        }
    }
}
