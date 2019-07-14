import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK
import Ubuntu.Components.Popups 1.3 as UITK_Popups
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
        title: Telegram.chats.listMode != ChatList.Idle
            ? i18n.tr("Select destination or cancel...")
            : i18n.tr("TELEports")

        UITK.StyleHints {
            foregroundColor: hf
            backgroundColor: hb
            dividerColor: hd
        }

        leadingActionBar.actions: [
            UITK.Action {
                text: i18n.tr("Cancel")
                iconName: "cancel"
                onTriggered: AppActions.chat.cancelForwardMessage()
                visible: Telegram.chats.listMode != ChatList.Idle
            }
        ]
        trailingActionBar.actions: [
            UITK.Action {
                id: settingsIcon
                objectName: "settingsIcon"
                text: i18n.tr("Settings")
                iconName: "settings"
                onTriggered: AppActions.view.pushToStack("qrc:/pages/SettingsPage.qml", {})
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
                height: layout.height
                color: chat.isSecret ? "lightgreen" : "transparent"

                onClicked: (Telegram.chats.listMode == ChatList.ForwardingMessages)
                    ? UITK_Popups.PopupUtils.open(forwardConfirmationDialog)
                    : AppActions.chat.setCurrentChat(chat)

                leadingActions: UITK.ListItemActions {
                    actions: [
                        UITK.Action {
                            iconName: "system-log-out"
                            text: i18n.tr("Leave chat")
                            onTriggered: AppActions.chat.leaveChat(chat.id)
                        },
                        UITK.Action {
                            iconName: "edit-clear"
                            text: i18n.tr("Clear history")
                            visible: chat.isPrivate
                            onTriggered: AppActions.chat.deleteChatHistory(chat.id)
                        }
                    ]
                }

                trailingActions: UITK.ListItemActions {
                    actions: [
                        UITK.Action {
                            iconName: "info"
                            text: i18n.tr("Info")
                            onTriggered: chat.isPrivate ? AppActions.user.showUserInfo(chat.chatType.user) : AppActions.chat.viewGroupInfo(chat)
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

                    mainSlot: Item {
                        height: col.height

                        Column {
                            id: col
                            anchors {
                                left: parent.left
                                right: parent.right
                                top: parent.top
                            }
                            spacing: units.gu(1)

                            RowLayout {
                                height: units.dp(17)
                                width: parent.width
                                spacing: units.dp(2)

                                Row {
                                    spacing: units.dp(2)
                                    Layout.alignment: Qt.AlignTop
                                    Layout.topMargin: units.dp(3)

                                    UITK.Icon {
                                        id: secret_chat_icon
                                        visible: chat.isSecret //if chat is encrypted
                                        name: "network-secure"
                                        color: Suru.foregroundColor
                                        Suru.textStyle: Suru.TertiaryText
                                        height: units.gu(2)
                                        width: height
                                    }

                                    UITK.Icon {
                                        id: contact_group_icon
                                        visible: chat.isGroup //if chat is a real chat or a supergroup, but not channel
                                        name: "contact-group"
                                        color: Suru.foregroundColor
                                        Suru.textStyle: Suru.TertiaryText
                                        height: units.gu(2)
                                        width: height
                                    }

                                    UITK.Icon {
                                        id: contact_channel_icon
                                        visible: chat.isChannel //if chat is channel but not supergroup
                                        source: "qrc:/qml/icons/broadcast.svg"
                                        color: Suru.foregroundColor
                                        Suru.textStyle: Suru.TertiaryText
                                        height: units.gu(2)
                                        width: height
                                    }

                                    UITK.Icon {
                                        id: audio_volume_muted_icon
                                        visible: chat.isMuted
                                        name: "audio-volume-muted"
                                        color: Suru.foregroundColor
                                        Suru.textStyle: Suru.TertiaryText
                                        height: units.gu(2)
                                        width: height
                                    }
                                }

                                Label {
                                    id: title
                                    elide: Text.ElideRight
                                    wrapMode: Text.WrapAnywhere
                                    maximumLineCount: 1
                                    font.weight: Font.DemiBold
                                    font.pixelSize: units.dp(17)
                                    text: chat.title
                                    Layout.fillWidth: true
                                    Layout.alignment: Qt.AlignBottom
                                }
                            }

                            RowLayout {
                                height: units.gu(2)
                                width: parent.width
                                Label {
                                    elide: Text.ElideRight
                                    wrapMode: Text.WrapAnywhere
                                    maximumLineCount: 1
                                    font.pixelSize: units.dp(15)
                                    Suru.textStyle: Suru.TertiaryText
                                    text: chat.summary
                                    Layout.fillWidth: true
                                }
                            }
                        }
                    }

                    Item {
                        width: Math.max(time_text.width, icons.width)
                        height: time_text.height + icons.height
                        UITK.SlotsLayout.position: UITK.SlotsLayout.Trailing
                        UITK.SlotsLayout.overrideVerticalPositioning: true
                        UITK.SlotsLayout.padding.leading: 0

                        UITK.Label {
                            id: time_text
                            anchors {
                                top: parent.top
                                topMargin: units.gu(1.5)
                                right: parent.right
                            }

                            visible: chat.lastMessage
                            font.pixelSize: units.dp(12)
                            color: Suru.foregroundColor
                            Suru.textStyle: Suru.TertiaryText
                            text: chat.lastMessage ? chat.formatDate(chat.lastMessage.date) : ""
                        }

                        Row {
                            id: icons
                            height: unread_rect.implicitHeight
                            anchors {
                                top: time_text.bottom
                                topMargin: units.gu(0.5)
                                right: parent.right
                            }
                            layoutDirection: Qt.RightToLeft
                            spacing: units.dp(2)

                            Rectangle {
                                id: unread_rect
                                width: Math.min(height, units.gu(4))
                                height: units.gu(2.8)
                                radius: width*0.5
                                color: chat.isMuted ? Suru.neutralColor : Suru.highlightColor
                                visible: chat.hasUnreadMessages

                                Label {
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
                                anchors {
                                    top: parent.top
                                    topMargin: units.gu(0.5)
                                }

                                visible: chat.isPinned
                                source: "qrc:/qml/icons/attach.svg"
                                color: Suru.foregroundColor
                                Suru.textStyle: Suru.TertiaryText
                                height: units.gu(2)
                                width: height
                            }
                        }
                    }
                    Component {
                        id: forwardConfirmationDialog
                        PopupDialog {
                            text: i18n.tr("Do you want to forward the selected messages to %1?").arg(chat.title)
                            confirmButtonColor: UITK.UbuntuColors.blue
                            confirmButtonText: i18n.tr("Forward")
                            onConfirmed: {
                                var optionalMessageText = optionalMessage.text.trim();
                                console.log(chat, optionalMessageText)
                                AppActions.chat.sendForwardMessage(chat, "test");
                                 // AppActions.chat.sendForwardMessage(chat, optionalMessageText);
                            }
                            TextArea {
                                height: units.gu(10)
                                id: optionalMessage
                                placeholderText: i18n.tr("Enter optional message...")
                            }
                        }

                    }
                }

            }
        }
    }
}
