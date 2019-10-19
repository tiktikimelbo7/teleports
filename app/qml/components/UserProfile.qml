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
    id: profile

    property QTdUser user: null

    anchors.fill: parent
    spacing: units.gu(1)

    ProfilePhoto {
        id: profilePhoto
        height: units.gu(13)
        width: height
        photo: user.profilePhoto
        initials: user.initials
        avatarColor: user.avatarColor

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
                title.text: user.firstName + " " + user.lastName
                title.font.pixelSize: units.gu(2.5)
                subtitle.text: user.status.string
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

                title.text: user.username ? "@" + user.username
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
                    bottom: units.gu(1)
                }

                UITK.Icon {
                    height: parent.title.height
                    width: parent.title.height
                    name: "call-start"
                    UITK.SlotsLayout.position: UITK.SlotsLayout.Leading
                }

                title.text: user.phoneNumber ? "<a href='#'>+" + user.phoneNumber + "</a>"
                                             : i18n.tr("not available")
                title.onLinkActivated: {
                    onClicked: Qt.openUrlExternally("tel:///+" + user.phoneNumber)
                }
            }
        }

        Item {
            height: bioLayout.height
            width: parent.width
            visible: user.fullInfo.bio != "" && !user.isMyself

            UITK.ListItemLayout {
                id: bioLayout
                width: parent.width
                padding {
                    top: 0
                    bottom: units.gu(1)
                }

                UITK.Icon {
                    height: parent.title.font.pixelSize
                    width: height
                    name: "note"
                    UITK.SlotsLayout.position: UITK.SlotsLayout.Leading
                }
                title {
                    text: user.fullInfo.bio
                    wrapMode: Text.Wrap
                    maximumLineCount: 10
                }
            }
        }

        Item {
            height: groupInCommonLayout.height
            width: groupInCommonLayout.width
            visible: !user.isMyself

            UITK.ListItemLayout {
                id: groupInCommonLayout

                padding {
                    top: 0
                    bottom: units.gu(1)
                }

                UITK.Icon {
                    height: parent.title.height
                    width: parent.title.height
                    name: "contact-group"
                    UITK.SlotsLayout.position: UITK.SlotsLayout.Leading
                }

                title.text: user.fullInfo.groupInCommonCount
            }
        }

        Item {
            height: shareInfoLayout.height
            width: parent.width
            visible: user.fullInfo.shareText != "" && !user.isMyself

            UITK.ListItemLayout {
                id: shareInfoLayout
                width: parent.width
                padding {
                    top: 0
                    bottom: 0
                }

                UITK.Icon {
                    height: parent.title.font.pixelSize
                    width: height
                    name: "info"
                    UITK.SlotsLayout.position: UITK.SlotsLayout.Leading
                }

                title {
                    text: user.fullInfo.shareText
                    wrapMode: Text.Wrap
                    maximumLineCount: 5
                }
            }
        }
    }
}
