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
    spacing: units.gu(1)

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
                // popup to change profile picture?
            }
        }
    }

    Column {
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.maximumHeight: units.gu(13)

        Item {
            height: nameAndStatusLayout.height
            width: nameAndStatusLayout.width
            UITK.ListItemLayout {
                id: nameAndStatusLayout

                padding {
                    top: units.gu(0.25)
                    bottom: units.gu(2)
                }
                title.text: Telegram.users.currentUser.firstName + " " + Telegram.users.currentUser.lastName
                title.font.pixelSize: units.gu(2.5)
                subtitle.text: Telegram.users.currentUser.status.string 
                subtitle.font.pixelSize: units.gu(1.5)
            }
        }

        Item {
            height: usernameLayout.height
            width: usernameLayout.width

            UITK.ListItemLayout {
                id: usernameLayout

                padding {
                    top: 0
                    bottom: units.gu(1)
                }

                UITK.Icon {
                    height: parent.title.height
                    width: parent.title.height
                    name: "contact"
                    UITK.SlotsLayout.position: UITK.SlotsLayout.Leading
                }

                title.text: Telegram.users.currentUser.username ? "@" + Telegram.users.currentUser.username
                                                                : i18n.tr("not available")
            }
        }

        Item {
            height: phoneNumberLayout.height
            width: phoneNumberLayout.width

            UITK.ListItemLayout {
                id: phoneNumberLayout

                padding {
                    top: 0
                    bottom: 0
                }
 
                UITK.Icon {
                    height: parent.title.height
                    width: parent.title.height
                    name: "call-start"
                    UITK.SlotsLayout.position: UITK.SlotsLayout.Leading
                }

                title.text: Telegram.users.currentUser.phoneNumber ? "<a href='#'>+" + Telegram.users.currentUser.phoneNumber + "</a>"
                                                                   : i18n.tr("not available")
                title.onLinkActivated: {
                    onClicked: Qt.openUrlExternally("tel:///+" + Telegram.users.currentUser.phoneNumber)
                }
            }
        }
    }
}
