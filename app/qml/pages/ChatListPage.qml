import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Ubuntu.Components 1.3 as UITK
import Ubuntu.Components.Popups 1.3 as UITK_Popups
import QtQuick.Controls.Suru 2.2
import QTelegram 1.0
import "../components"
import "../actions"
import "../stores"

Page {
    id: chatListPage
    property color hf: Suru.foregroundColor
    property color hb: Suru.backgroundColor
    property color hd: Suru.neutralColor
    header: UITK.PageHeader {
        title: i18n.tr("Telegram Plus")

        UITK.StyleHints {
            foregroundColor: hf
            backgroundColor: hb
            dividerColor: hd
        }

        trailingActionBar.actions: [
            UITK.Action {
                id: settingsIcon
                objectName: "settingsIcon"
                text: i18n.tr("Settings")
                iconName: "settings"
                onTriggered: {
                    AppActions.user.setCurrentUser(Telegram.users.me)
                }
            }
        ]
    }

    ScrollView {
        anchors.fill: parent
        ScrollBar.vertical.policy: ScrollBar.AlwaysOff
        ListView {
            anchors{
                fill: parent
            }
            model: Telegram.chats.sortedList
            delegate: UITK.ListItem {

                readonly property QTdChat chat: modelData

                width: parent.width
                height: layout.implicitHeight
                color: chat.isSecret ? "lightgreen" : "transparent"

                onClicked: {
                    AppActions.chat.setCurrentChat(chat)
                }

                leadingActions: UITK.ListItemActions {
                    actions: [
                        UITK.Action {
                            iconName: "system-log-out"
                            text: i18n.tr("Leave chat")
                            onTriggered: UITK_Popups.PopupUtils.open(leaveChatConfirmationDialog)
                        },
                        UITK.Action {
                            iconName: "edit-clear"
                            text: i18n.tr("Clear history")
                            visible: chat.isPrivate
                            onTriggered: UITK_Popups.PopupUtils.open(clearHistoryConfirmationDialog)
                        }
                    ]
                }

                trailingActions: UITK.ListItemActions {
                    actions: [
                        UITK.Action {
                            iconName: "info"
                            text: i18n.tr("Info")
                            onTriggered: chat.isPrivate ? AppActions.user.setCurrentUserById(chat.chatType.userId) : AppActions.chat.viewInDetail(chat)
                        }
                    ]
                }

                UITK.SlotsLayout {
                    id: layout

                    GenericPhoto {
                        id: avatar
                        height: units.gu(6)
                        width: height
                        photoPath: chat && chat.chatPhoto.small.local.path ? chat.chatPhoto.small.local.path : ""
                        initials: chat ? chat.initials : "N/A"
                        avatarColor: chat.avatarColor(chat ? chat.rawId : 0)
                        myself: chat ? chat.isMyself : false
                        UITK.SlotsLayout.position: UITK.SlotsLayout.Leading
                        UITK.SlotsLayout.padding.trailing: 0
                    }

                    mainSlot: Column {
                        spacing: units.gu(0.5)

                        RowLayout {
                            width: parent.width

                            UITK.Icon {
                                id: secret_chat_icon
                                visible: chat.isSecret //if chat is encrypted
                                name: "network-secure"
                                height: units.gu(2)
                                width: height
                            }

                            UITK.Icon {
                                id: contact_group_icon
                                visible: chat.isGroup //if chat is a real chat or a supergroup, but not channel
                                name: "contact-group"
                                height: units.gu(2)
                                width: height
                            }

                            UITK.Icon {
                                id: contact_channel_icon
                                visible: chat.isChannel //if chat is channel but not supergroup
                                source: "qrc:/qml/icons/broadcast.svg"
                                height: units.gu(2)
                                width: height
                            }

                            UITK.Icon {
                                id: audio_volume_muted_icon
                                visible: chat.isMuted
                                name: "audio-volume-muted"
                                height: units.gu(2)
                                width: height
                            }

                            Text {
                                elide: Text.ElideRight
                                wrapMode: Text.WrapAnywhere
                                maximumLineCount: 1
                                font.weight: Font.DemiBold
                                font.pixelSize: units.dp(17)
                                text: chat.title
                                Layout.fillWidth: true
                            }
                        }

                        RowLayout {
                            height: units.gu(2)
                            width: parent.width
                            Text {
                                elide: Text.ElideRight
                                wrapMode: Text.WrapAnywhere
                                maximumLineCount: 1
                                font.pixelSize: units.dp(15)
                                color: theme.palette.normal.backgroundTertiaryText
                                text: chat.summary
                                Layout.fillWidth: true
                            }
                        }
                    }

                    Item {
                        width: Math.max(time_text.width, icon_col.width)
                        height: time_text.height + icon_col.height
                        UITK.SlotsLayout.position: UITK.SlotsLayout.Trailing
                        UITK.SlotsLayout.overrideVerticalPositioning: true
                        UITK.SlotsLayout.padding.leading: 0

                        UITK.Label {
                            id: time_text
                            anchors {
                                top: parent.top
                                topMargin: units.gu(1)
                                right: parent.right
                            }

                            visible: chat.lastMessage
                            font.pixelSize: units.dp(12)
                            color: theme.palette.normal.backgroundTertiaryText
                            text: chat.lastMessage ? chat.formatDate(chat.lastMessage.date) : ""
                        }

                        ColumnLayout {
                            id: icon_col
                            height: icons.height
                            width: icons.width
                            anchors {
                                top: time_text.bottom
                                topMargin: units.gu(1)
                                right: parent.right
                            }

                            RowLayout {
                                id: icons
                                height: childrenRect.height
                                layoutDirection: Qt.RightToLeft
                                Layout.alignment: Qt.AlignRight

                                Rectangle {
                                    id: unread_rect
                                    width: Math.min(height, units.gu(4))
                                    height: units.gu(2.8)
                                    radius: width*0.5
                                    color: chat.isMuted ? "grey" : "#5ec245"
                                    visible: chat.hasUnreadMessages

                                    Text {
                                        id: unread_text
                                        anchors {
                                            centerIn: parent
                                            margins: 0
                                        }
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                        font.weight: Font.DemiBold
                                        font.pixelSize: FontUtils.sizeToPixels("small")
                                        color: "white"
                                        text: chat.unreadCount < 999 ? chat.unreadCount : ":D"; // no-i18n
                                    }
                                }

                                UITK.Icon {
                                    id: pinned_icon
                                    visible: chat.isPinned
                                    source: "qrc:/qml/icons/attach.svg"
                                    height: units.gu(2)
                                    width: height
                                }
                            }
                        }
                    }
                }

                Component {
                    id: leaveChatConfirmationDialog
                    PopupDialog {
                        text: i18n.tr("Are you sure you want to leave this chat?")
                        confirmButtonColor: UITK.UbuntuColors.red
                        confirmButtonText: i18n.tr("Leave")
                        cancelButtonColor: UITK.UbuntuColors.green
                        onConfirmed: AppActions.chat.leaveChat(chat.rawId)
                    }
                }

                Component {
                    id: clearHistoryConfirmationDialog
                    PopupDialog {
                        text: i18n.tr("Are you sure you want to clear the history?")
                        confirmButtonColor: UITK.UbuntuColors.red
                        confirmButtonText: i18n.tr("Clear history")
                        cancelButtonColor: UITK.UbuntuColors.green
                        onConfirmed: AppActions.chat.deleteChatHistory(chat.rawId)
                    }
                }
            }
        }
    }
}
