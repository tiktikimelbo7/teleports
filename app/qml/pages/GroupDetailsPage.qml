import QtQuick 2.4
import QtQuick.Controls 2.2
import Ubuntu.Components 1.3 as UITK
import QtQuick.Controls.Suru 2.2
import QtQuick.Layouts 1.1
import QTelegram 1.0
import "../actions"
import "../components"
import "../stores"


Page {
    id: groupDetailsPage
    property color hf: Suru.foregroundColor
    property color hb: Suru.backgroundColor
    property color hd: Suru.neutralColor
    header: UITK.PageHeader {
        title: i18n.tr('Group Details')

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
                    AppActions.chat.leaveGroupDetails()
                }
            }
        ]
    }

    Flickable {
        id: groupDetailsFlickable
        anchors.fill: parent
        anchors.margins: units.gu(2)
        contentHeight: groupDetailsColumn.height

        ColumnLayout {
            id: groupDetailsColumn
            anchors {
                fill: parent
            }

            RowLayout {
                anchors.fill: parent
                id: profile
                spacing: units.gu(1)

                GenericPhoto {
                    id: avatar

                    /*anchors {
                        top: parent.top
                        topMargin: units.dp(4)
                        left: parent.left
                        leftMargin: units.gu(1)
                        bottom: parent.bottom
                        bottomMargin: units.dp(4)
                        rightMargin: units.gu(2)
                    }*/

                    height: units.gu(13)
                    width: height
                    photoPath: Telegram.chats.viewedInDetailGroup.chatPhoto.small.local.path ?
                                    Telegram.chats.viewedInDetailGroup.chatPhoto.small.local.path : ""
                    initials: Telegram.chats.viewedInDetailGroup.initials ? Telegram.chats.viewedInDetailGroup.initials : "N/A"
                    avatarColor: Telegram.chats.viewedInDetailGroup.avatarColor(Telegram.chats.viewedInDetailGroup.rawId)
                    myself: false
                }

                Column {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.maximumHeight: units.gu(13)

                    Item {
                        height: titleAndMemberCount.height
                        width: titleAndMemberCount.width
                        UITK.ListItemLayout {
                            id: titleAndMemberCount

                            padding {
                                top: units.gu(0.25)
                                bottom: units.gu(2)
                            }
                            title.text: Telegram.chats.viewedInDetailGroup.title
                            title.font.pixelSize: units.gu(2.5)
                            subtitle.text: Telegram.chats.viewedInDetailGroup.memberCount
                            subtitle.font.pixelSize: units.gu(1.5)
                        }
                    }

                    Item {
                        height: chatInviteLinkLayout.height
                        width: chatInviteLinkLayout.width
                        visible: Telegram.chats.viewedInDetailGroup.inviteLink

                        UITK.ListItemLayout {
                            id: chatInviteLinkLayout

                            padding {
                                top: 0
                                bottom: units.gu(1)
                            }

                            UITK.Icon {
                                height: parent.title.height
                                width: parent.title.height
                                name: "insert-link"
                                UITK.SlotsLayout.position: UITK.SlotsLayout.Leading
                            }

                            title.text: Telegram.chats.viewedInDetailGroup.inviteLink
                        }
                    }

                    Item {
                        height: chatDescriptionLayout.height
                        width: chatDescriptionLayout.width

                        UITK.ListItemLayout {
                            id: chatDescriptionLayout

                            padding {
                                top: 0
                                bottom: 0
                            }
             
                            /*UITK.Icon {
                                height: parent.title.height
                                width: parent.title.height
                                name: "call-start"
                                UITK.SlotsLayout.position: UITK.SlotsLayout.Leading
                            }*/

                            //title.text.wrapMode: Text.WordWrap
                            title.text: Telegram.chats.viewedInDetailGroup.description
                        }
                    }
                }
            }
        }
    }   
}
