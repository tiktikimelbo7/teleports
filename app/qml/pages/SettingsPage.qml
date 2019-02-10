import QtQuick 2.4
import QtQuick.Controls 2.2
import Ubuntu.Connectivity 1.0
import Ubuntu.Components 1.3 as UITK
import Ubuntu.Components.ListItems 1.3 as UITK_ListItem
import Ubuntu.Components.Popups 1.3 as UITK_Popups
import QtQuick.Controls.Suru 2.2
import QtQuick.Layouts 1.1
import QTelegram 1.0
import QTelegramStyles 1.0
import "../actions"
import "../components"
import "../stores"

Page {
    id: settingsPage
    property QTdUser me: Telegram.users.me

    property color hf: Suru.foregroundColor
    property color hb: Suru.backgroundColor
    property color hd: Suru.neutralColor

    property var statusMap: [i18n.tr("Offline"), i18n.tr("Connecting"), i18n.tr("Online")]

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

        trailingActionBar.actions: [
            UITK.Action {
                id: infoIcon
                objectName: "infoIcon"
                text: i18n.tr("About")
                iconName: "info"
                onTriggered: {
                    AppActions.view.pushToStack("qrc:///pages/AboutPage.qml", {})
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
            anchors {
                fill: parent
            }

            //UITK_ListItem.Header {
            //    text: i18n.tr("Account")
            //    visible: Telegram.users.currentUser == Telegram.users.me
            //}

            UserProfile {
                user: me
            }

            UITK.ListItem {
               UITK.ListItemLayout {
                  UITK.Icon {
                     width: units.gu(2)
                     name: "night-mode"
                     UITK.SlotsLayout.position: UITK.SlotsLayout.Leading
                  }
                  title.text : i18n.tr('Night mode')
                  Switch {
                     id: theme_switch
                     checked: Telegram.settings.theme === Suru.Dark
                     Suru.highlightType: Suru.PositiveHighlight
                     UITK.SlotsLayout.position: UITK.SlotsLayout.Trailing
                     onCheckedChanged: AppActions.settings.setTheme(checked?Suru.Dark:Suru.Light)
                  }
               }

               onClicked: theme_switch.checked = !theme_switch.checked
            }

            UITK.ListItem {
                UITK.ListItemLayout {
                    UITK.Icon {
                        width: units.gu(2)
                        name: "system-log-out"
                        UITK.SlotsLayout.position: UITK.SlotsLayout.Leading
                    }
                    title.text : i18n.tr('Logout')
                }

                onClicked: UITK_Popups.PopupUtils.open(logoutConfirmationDialog)
            }

            UITK.ListItem {
                UITK.ListItemLayout {
                    UITK.Icon {
                        UITK.SlotsLayout.position: UITK.SlotsLayout.Leading
                        width: units.gu(2)
                        name: "delete"
                    }
                    title.text : i18n.tr("Delete account")
                }

                onClicked: UITK_Popups.PopupUtils.open(deleteAccountConfirmationDialog)
            }
            Label {
                text: "Connectivity Debug:"
                font.pixelSize: units.gu(1)
            }
            Label {
                // use the online property
                text: Connectivity.online ? i18n.tr("Online") : i18n.tr("Not online")
                font.pixelSize: units.gu(1)
            }
            Label {
                // use the status property
                text: i18n.tr("Status:") + " " + statusMap[Connectivity.status]
                font.pixelSize: units.gu(1)
            }
            Label {
                // use the limitedBandwith property
                text: Connectivity.limitedBandwith ? i18n.tr("Bandwith limited") : i18n.tr("Bandwith not limited")
                font.pixelSize: units.gu(1)
            }

//            UITK_ListItem.Header {
//                text: i18n.tr("Whatever")
//            }
        }
    }

    Component {
        id: logoutConfirmationDialog
        PopupDialog {
            text: i18n.tr("Warning: Logging out will delete all local data from this device, including secret chats. Are you still sure you want to log out?")
            confirmButtonColor: UITK.UbuntuColors.red
            confirmButtonText: i18n.tr("Logout")
            cancelButtonColor: UITK.UbuntuColors.green
            onConfirmed: AppActions.auth.logOut()
        }
    }

    Component {
        id: deleteAccountConfirmationDialog
        PopupDialog {
            text: i18n.tr("Warning: Deleting the account will delete all the data you ever received or send using telegram except for data you have explicitly saved outside the telegram cloud. Are you really really sure you want to delete your telegram account?")
            confirmButtonColor: UITK.UbuntuColors.red
            confirmButtonText: i18n.tr("Delete")
            cancelButtonColor: UITK.UbuntuColors.green
            onConfirmed: AppActions.auth.deleteAccount()
        }
    }
}
