import QtQuick 2.4
import QtQuick.Controls 2.2
import Ubuntu.Components 1.3 as UITK
import Ubuntu.Components.ListItems 1.3 as UITK_ListItem
import QtQuick.Controls.Suru 2.2
import QtQuick.Layouts 1.1
import QTelegram 1.0
import "../actions"
import "../components"
import "../stores"

Page {
    id: accountPage
    property color hf: Suru.foregroundColor
    property color hb: Suru.backgroundColor
    property color hd: Suru.neutralColor
    header: UITK.PageHeader {
        title: i18n.tr('Account')

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
                  AppActions.user.clearCurrentUser()
                }
            }
        ]

    }

    Flickable {
        id: accountFlickable
        anchors.fill: parent
        anchors.margins: units.gu(2)
        contentHeight: accountColumn.height

        ColumnLayout {
            id: accountColumn
            anchors {
                fill: parent
            }

            RowLayout {
                anchors.fill: parent
                id: profile

                ProfilePhoto {
                    id: profilePhoto
                    width: units.gu(13)
                    height: units.gu(13)
                    photo: Telegram.users.me.profilePhoto
                    initials: Telegram.users.me.initials
                    avatarColor: Telegram.users.me.avatarColor
                }

                Column {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    Label {
                        text: Telegram.users.me.firstName + " " + Telegram.users.me.lastName
                    }

                    Label {
                        text: Telegram.users.me.username
                    }

                    Label {
                        text: i18n.tr("+") + Telegram.users.me.phoneNumber
                    }
                }
            }

            //UITK_ListItem.Header {
            //    text: i18n.tr("Account Settings")
            //}

            UITK_ListItem.Header {
                text: i18n.tr("Account Actions")
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

                onClicked: AppActions.auth.logOut()
            }

            UITK.ListItem {
                UITK.ListItemLayout {
                    UITK.Icon {
                        UITK.SlotsLayout.position: UITK.SlotsLayout.Leading
                        width: units.gu(2)
                        name: "delete"
                    }
                    title.text : i18n.tr('Delete account') + " (TODO: add confirmation dialog)"
                }

                //onClicked: AppActions.auth.deleteAccount()
            }
        }
    }
}
