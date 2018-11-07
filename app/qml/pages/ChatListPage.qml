import QtQuick 2.4
import QtQuick.Controls 2.2
import Ubuntu.Components 1.3 as UITK
import Ubuntu.Components.Popups 1.3
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
                width: parent.width
                readonly property QTdChat chat: modelData
                onClicked: {
                    AppActions.chat.setCurrentChat(chat)
                }
                leadingActions: UITK.ListItemActions {
                    actions: [
                        UITK.Action {
                            iconName: "system-log-out"
                            text: i18n.tr("Leave chat")

                            onTriggered: {
                                PopupUtils.open(Qt.resolvedUrl("qrc:/qml/ui/dialogs/ConfirmationDialog.qml"),
                                                null, {
                                                    text: i18n.tr("Are you sure you want to leave this chat?"),
                                                    onAccept: function() {
                                                        pageStack.clear()
                                                        //telegram.messagesDeleteHistory(dialogId, true)
                                                    }
                                                }
                                                );
                            }
                        },
                        UITK.Action {
                            iconName: "edit-clear"
                            text: i18n.tr("Clear history")

                            onTriggered: {
                                PopupUtils.open(Qt.resolvedUrl("qrc:/qml/ui/dialogs/ConfirmationDialog.qml"),
                                                null, {
                                                    text: i18n.tr("Are you sure you want to clear the history?"),
                                                    onAccept: function() {
                                                        //telegram.messagesDeleteHistory(dialogId, false)
                                                    }
                                                }
                                                );
                            }
                        }
                    ]
                }

                trailingActions: UITK.ListItemActions {
                    actions: [
                        UITK.Action {
                            iconName: "info"
                            text: i18n.tr("Info")
                            onTriggered: {
                                //                                    pageStack.addPageToNextColumn(account_page, profile_page_component, {
                                //                                            telegram: list_item.telegram,
                                //                                            dialog: list_item.dialog
                                //                                    });
                            }
                        }
                    ]
                }

                //Rectangle will show greenish color if this chat is a secret chat
                Rectangle {
                    anchors {
                        fill: parent;
                        margins: units.dp(4)
                    }
                    color: "lightgreen"
                    visible: chat.isSecret
                }

                GenericPhoto {
                    id: avatar

                    anchors {
                        top: parent.top
                        topMargin: units.dp(4)
                        left: parent.left
                        leftMargin: units.gu(1)
                        bottom: parent.bottom
                        bottomMargin: units.dp(4)
                        rightMargin: units.gu(2)
                    }
                    width: height
                    photoPath: chat && chat.chatPhoto.small.local.path ? chat.chatPhoto.small.local.path : ""
                    initials: chat ? chat.initials : "N/A"
                    avatarColor: chat.avatarColor(chat ? chat.rawId : 0)
                    myself: chat ? chat.isMyself : false
                }

                Row {
                    anchors {
                        top: parent.top
                        left: avatar.right
                        leftMargin: units.dp(8)
                        bottom: parent.verticalCenter
                        right: time_text.left
                        margins: units.dp(4)
                    }
                    spacing: units.dp(4)

                    UITK.Icon {
                        id: secret_chat_icon
                        visible: chat.isSecret //if chat is encrypted
                        name: "network-secure"
                        anchors {
                            top: parent.top
                            bottom: parent.bottom
                            topMargin: units.dp(4)
                            bottomMargin: units.dp(4)
                        }
                        width: height
                    }

                    UITK.Icon {
                        id: contact_group_icon
                        visible: chat.isGroup //if chat is a real chat or a supergroup, but not channel
                        name: "contact-group"
                        anchors {
                            top: parent.top
                            bottom: parent.bottom
                            topMargin: units.dp(2)
                            bottomMargin: units.dp(2)
                        }
                        width: height
                    }

                    UITK.Icon {
                        id: contact_channel_icon
                        visible: chat.isChannel //if chat is channel but not supergroup
                        source: "qrc:/qml/icons/broadcast.svg"
                        anchors {
                            top: parent.top
                            bottom: parent.bottom
                            topMargin: units.dp(2)
                            bottomMargin: units.dp(2)
                        }
                        width: height
                    }

                    UITK.Icon {
                        id: audio_volume_muted_icon
                        visible: chat.isMuted
                        name: "audio-volume-muted"
                        anchors {
                            top: parent.top
                            bottom: parent.bottom
                            topMargin: units.dp(4)
                            bottomMargin: units.dp(4)
                        }
                        width: height
                    }

                    Text {
                        id: title_text
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                        wrapMode: Text.WrapAnywhere
                        maximumLineCount: 1
                        font.weight: Font.DemiBold
                        font.pixelSize: units.dp(17)
                        text: chat.title
                        width: parent.width - (secret_chat_icon.visible? secret_chat_icon.width : 0.0)
                               - (contact_group_icon.visible? contact_group_icon.width : 0.0)
                               - (audio_volume_muted_icon.visible? audio_volume_muted_icon.width : 0.0)
                               - (pinned_icon.visible? pinned_icon.width : 0.0)
                        color: theme.palette.normal.backgroundText
                    }

                    UITK.Icon {
                        id: pinned_icon
                        visible: chat.isPinned
                        source: "qrc:/qml/icons/attach.svg"
                        anchors {
                            top: parent.top
                            bottom: parent.bottom
                            topMargin: units.dp(4)
                            bottomMargin: units.dp(4)
                            rightMargin: units.dp(1)
                        }
                        width: height
                    }
                }

                Row {
                    anchors {
                        top: parent.verticalCenter
                        bottom: parent.bottom
                        left: avatar.right
                        leftMargin: units.dp(8)
                        right: parent.right
                        margins: units.dp(4)
                        topMargin: 0
                    }

                    //TODO: Determine the various details from chat and last message
                    Text {
                        id: message_author
                        //                        visible: message && (message.out || isChat || isChannel) && dialog.typingUsers.length === 0 && (message.message != "" || message.action.classType == MessageAction.Empty)
                        maximumLineCount: 1
                        font.pixelSize: units.dp(15)//FontUtils.sizeToPixels("smaller")
                        color: theme.palette.normal.activityText
                        text: {
                            //                            if (!message || dialog.typingUsers.length > 0) return '';
                            //                            if (message.out) return i18n.tr("You: ");
                            //                            if (isChat || isChannel) return telegramObject.user(message.fromId).firstName + ': ';
                            return '';
                        }
                    }

                    Text {
                        id: message_text
                        elide: Text.ElideRight
                        wrapMode: Text.WrapAnywhere
                        maximumLineCount: 1
                        font.pixelSize: units.dp(15)
                        color: theme.palette.normal.backgroundTertiaryText
                        width: parent.width - (message_author.visible? message_author.width : 0.0) - (unread_rect.visible ? unread_rect.width : 0.0)
                        text: {

                            //                            var list = dialog.typingUsers;
                            //                            if (list.length > 0) {
                            //                                // TRANSLATORS: Indicates in a subtitle of a dialog list item that someone is typing.
                            //                                return i18n.tr("typing...")
                            //                            } else {
                            //                                if (!message)
                            //                                    return "";
                            //                                if (message.message == "") {
                            //                                    var res = "";
                            //                                    var user = telegramObject.user(message.action.userId)
                            //                                    var fromUser = telegramObject.user(message.fromId)
                            //                                    var userName = user.firstName + " " + user.lastName
                            //                                    var fromUserName = fromUser.firstName + " " + fromUser.lastName
                            //                                    userName = userName.trim()
                            //                                    fromUserName = fromUserName.trim()

                            //                                    switch(message.action.messageActionEnum) {
                            //                                        case MessageAction.ChatEditTitle:
                            //                                            if (fromUserName != "") {
                            //                                                if(fromUser.id == telegramObject.me)
                            //                                                    res = i18n.tr("You changed the group title to %1").arg(message.action.title)
                            //                                                else
                            //                                                    res = i18n.tr("%1 changed the group title to %2").arg(fromUserName).arg(message.action.title)
                            //                                            }
                            //                                            break

                            //                                        case MessageAction.Empty:
                            //                                            if (fromUserName != "") {
                            //                                                if (isAudio)
                            //                                                    res = i18n.tr("Voice message")
                            //                                                else if (isSticker)
                            //                                                    res = i18n.tr("Sticker")
                            //                                                else {
                            //                                                    res = file_handler.fileName
                            //                                                    if (res == "")
                            //                                                        res = i18n.tr("Photo")
                            //                                                }
                            //                                            }
                            //                                            break

                            //                                        case MessageAction.ChatCreate:
                            //                                            if (message.action.title == "Secret Chat") {
                            //                                                if (user.id == telegramObject.me)
                            //                                                    res = i18n.tr("%1 joined your secret chat.").arg(fromUserName)
                            //                                                else
                            //                                                    res = i18n.tr("You joined the secret chat.")
                            //                                            } else {
                            //                                                if (fromUser.id == telegramObject.me)
                            //                                                    res = i18n.tr("You created the group")
                            //                                                else
                            //                                                    res = i18n.tr("%1 created the group").arg(fromUserName)
                            //                                            }
                            //                                            break

                            //                                        case MessageAction.ChatAddUser:
                            //                                            if (fromUser.id == telegramObject.me)
                            //                                                res = i18n.tr("You added %1").arg(userName)
                            //                                            else if (user.id == telegramObject.me)
                            //                                                res = i18n.tr("%1 added you").arg(fromUserName)
                            //                                            else if (userName=="")
                            //                                                res = i18n.tr("%1 joined the group").arg(fromUserName)
                            //                                            else
                            //                                                res = i18n.tr("%1 added %2").arg(fromUserName).arg(userName)
                            //                                            break

                            //                                        case MessageAction.ChatDeleteUser:
                            //                                            if(user.id == fromUser.id) {
                            //                                                res = i18n.tr("%1 left the group").arg(userName)
                            //                                            } else {
                            //                                                if (fromUser.id == telegramObject.me)
                            //                                                    res = i18n.tr("You removed %1").arg(userName)
                            //                                                else if (user.id == telegramObject.me)
                            //                                                    res = i18n.tr("%1 removed you").arg(fromUserName)
                            //                                                else
                            //                                                    res = i18n.tr("%1 removed %2").arg(fromUserName).arg(userName)
                            //                                            }
                            //                                            break

                            //                                        case MessageAction.ChatJoinedByLink:
                            //                                            if(fromUser.id == telegramObject.me)
                            //                                                res = i18n.tr("You joined the group via invite link")
                            //                                            else
                            //                                                res = i18n.tr("%1 joined the group via invite link").arg(fromUserName)
                            //                                            break

                            //                                        default:
                            //                                            break
                            //                                    }
                            //                                    return "<font color=\"" + theme.palette.normal.activityText + "\">" + res + "</font>"
                            //                                }
                            return chat.summary
                        }
                    }
                }
                UITK.Label {
                    id: time_text
                    anchors {
                        top: parent.top
                        bottom: parent.verticalCenter
                        right: parent.right
                        rightMargin: units.gu(1)
                    }
                    verticalAlignment: Text.AlignVCenter
                    visible: chat.lastMessage
                    font.pixelSize: units.dp(12)
                    color: theme.palette.normal.backgroundTertiaryText
                    text: chat.lastMessage ? chat.formatDate(chat.lastMessage.date) : ""
                }

                Rectangle {
                    id: unread_rect
                    anchors {
                        top: parent.verticalCenter
                        topMargin: 0
                        right: time_text.right
                    }
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
            }
        }
    }
}
