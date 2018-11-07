import QtQuick 2.4
import QtQuick.Controls 2.2
import Ubuntu.Components 1.3 as UITK
import QtQuick.Controls.Suru 2.2
import QtQuick.Layouts 1.1
import QTelegram 1.0
import "../actions"
import "../components"
import "../stores"

RowLayout {
    anchors.fill: parent
    id: profile
    spacing: units.gu(2)

    ProfilePhoto {
        id: profilePhoto
        height: units.gu(13)
        width: height
        photo: Telegram.users.currentUser.profilePhoto
        initials: Telegram.users.currentUser.initials
        avatarColor: Telegram.users.currentUser.avatarColor

        MouseArea {
            anchors.fill: parent
            onClicked: {
                // popup to change profile picture
            }
        }
    }

    Column {
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.maximumHeight: units.gu(13)

        UITK.Captions {
            title.text: Telegram.users.currentUser.firstName + " " + Telegram.users.currentUser.lastName
            title.font.pixelSize: units.gu(2.5)

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    // popup to enter the real name
                }
            }

            Label {
                text: Telegram.users.currentUser.status.string
                font.pixelSize: units.gu(1.5)
            }

            Item { Layout.fillHeight: true }
        }

        Row {
            UITK.Icon {
                width: units.gu(2)
                name: "contact"
                UITK.SlotsLayout.position: UITK.SlotsLayout.Leading
            }

            Label {
                id: usernameLabel
                text: Telegram.users.currentUser.username ? " @" + Telegram.users.currentUser.username
                                                          : " " + i18n.tr("not available")

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        // popup to enter the username
                    }
                }
            }
        }

        Row {
            UITK.Icon {
                width: units.gu(2)
                name: "call-start"
                UITK.SlotsLayout.position: UITK.SlotsLayout.Leading
            }

            Label {
                text: Telegram.users.currentUser.phoneNumber ? " " + i18n.tr("+") + Telegram.users.currentUser.phoneNumber
                                                             : " " + i18n.tr("not available")
            }
        }
    }

    Item { Layout.fillWidth: true }
}
