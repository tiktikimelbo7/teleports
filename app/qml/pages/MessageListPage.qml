import QtQuick 2.8
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK
import QTelegram 1.0
import QTelegramStyles 1.0
import "../components"
import "../actions"
import "../delegates"
import "../stores"

Page {
    header: UITK.PageHeader {
        property QTdChat currentChat: Telegram.chats.currentChat
        title: Telegram.chats && Telegram.chats.currentChat ? Telegram.chats.currentChat.title : ""
        subtitle: {
            if (Telegram.chats && currentChat)
            {
                if (currentChat.action != "")
                    return currentChat.action;
                if (currentChat.isGroup || currentChat.isChannel)
                {
                    return i18n.tr("%1 member", "%1 members", currentChat.memberCount).arg(currentChat.memberCount);
                }
            }
            return "";
        }
        leadingActionBar.actions: [
            UITK.Action {
                iconName: "back"
                text: "Back"
                onTriggered: {
                    AppActions.chat.closeCurrentChat()
                }
            }
        ]
        contents: Item {
            anchors.fill: parent

            GenericPhoto {
                    id: avatar

                    anchors {
                        verticalCenter: parent.verticalCenter
                        leftMargin: units.dp(4)
                    }
                    height: units.gu(5)
                    width: height
                    property QTdLocalFile localFile : Telegram.chats.currentChat.chatPhoto.small.local
                    photoPath: {
                        if (localFile && localFile.canBeDownloaded && !localFile.isDownloadingCompleted) {
                            Telegram.chats.currentChat.chatPhoto.small.downloadFile();
                        }
                        return Telegram.chats.currentChat && localFile.path ? localFile.path : ""
                    }
                    initials: Telegram.chats.currentChat ? Telegram.chats.currentChat.initials : "N/A"
                    avatarColor: Telegram.chats.currentChat.avatarColor(Telegram.chats.currentChat ? Telegram.chats.currentChat.rawId : 0)
                    myself: Telegram.chats.currentChat ? Telegram.chats.currentChat.isMyself : false
                }

            Rectangle {
                id: connectingIndicator
                anchors.fill: avatar
                visible: false
                color: theme.palette.normal.background
                UITK.Icon {
                    name: "sync-updating"// telegram.busy? "sync-updating" : isConnecting? "sync-error" : "sync-paused"
                    anchors.fill: parent
                    color: Theme.palette.normal.backgroundSecondaryText
                }
                SequentialAnimation {
                    running: visible
                    loops: Animation.Infinite
                    PropertyAnimation { target: connectingIndicator; property: "opacity"; to: 1; duration: 500 }
                    PropertyAnimation { duration: 300 }
                    PropertyAnimation { target: connectingIndicator; property: "opacity"; to: 0.0; duration: 800 }
                    PropertyAnimation { duration: 300 }
                }
            }

            UITK.Icon {
            id: secret_chat_icon
            visible: Telegram.chats.currentChat.isSecret
            name: "network-secure"
            anchors {
                top: parent.top
                left: avatar.right
                topMargin: units.dp(5)
                bottomMargin: units.dp(5)
                bottom: titleText.bottom
            }
            width: height
            }
            Label {
                id: titleText
                anchors {
                    top: parent.top
                    left: secret_chat_icon.visible? secret_chat_icon.right : avatar.right
                    leftMargin: units.gu(1)
                    right: parent.right
                }
                verticalAlignment: Text.AlignVCenter

                font.pixelSize: FontUtils.sizeToPixels("large")
                elide: Text.ElideRight
                wrapMode: Text.WrapAnywhere
                maximumLineCount: 1
                text: header.title.length === 0 ? i18n.tr("Telegram") : header.title

                state: header.subtitle.length > 0 ? "subtitle" : "default"
                states: [
                    State {
                        name: "default"
                        AnchorChanges {
                            target: titleText
                            anchors.verticalCenter: titleText.parent.verticalCenter
                        }
                        PropertyChanges {
                            target: titleText
                            height: titleText.implicitHeight
                            anchors.topMargin: units.gu(0.7)
                        }
                    },
                    State {
                        name: "subtitle"
                        PropertyChanges {
                            target: titleText
                            height: titleText.parent.height / 2
                            anchors.topMargin: units.gu(0.35)
                        }
                    }
                ]

                transitions: [
                    Transition {
                        AnchorAnimation {
                            duration: UITK.UbuntuAnimation.FastDuration
                        }
                    }
                ]
            }

            Label {
                id: subtitleText
                anchors {
                    left: avatar.right
                    leftMargin: units.gu(1)
                    bottom: parent.bottom
                    bottomMargin: units.gu(0.15)
                }
                verticalAlignment: Text.AlignVCenter
                height: parent.height / 2
                width: parent.width

                elide: Text.ElideRight
                wrapMode: Text.WordWrap
                maximumLineCount: 1
                text: header.subtitle
                color: header.isOnline ? UbuntuColors.blue : theme.palette.normal.backgroundTertiaryText

                Connections {
                    target: header
                    onSubtitleChanged: {
                        subtitleText.opacity = 0;
                        subtitleText.text = "";
                        subtitleTextTimer.start();
                    }
                }

                Timer {
                    id: subtitleTextTimer
                    interval: UITK.UbuntuAnimation.FastDuration
                    onTriggered: {
                        subtitleText.text = header.subtitle;
                        subtitleText.opacity = 1;
                    }
                }

                Behavior on opacity {
                    NumberAnimation {
                        duration: UITK.UbuntuAnimation.FastDuration
                    }
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    mouse.accepted = true;
                    header.clicked();
                }
            }

        }
    }

    ScrollView {
        anchors {
            left: parent.left
            top: parent.top
            right: parent.right
            bottom: input.top
        }

        ListView {
            id: msgList
            anchors {
                fill: parent
                bottomMargin: Suru.units.gu(1)
            }
            model: Telegram.chats.messageList
            verticalLayoutDirection: ListView.BottomToTop
            delegate: Component {
                Loader {
                    id: loader
                    width: parent.width
                    height: childrenRect.height
                    Component.onCompleted: setSource(delegateMap.findComponent(modelData.content.type), {message: modelData, chat: Telegram.chats.chat })
                }
            }

            /**
             * When we are near the top of the listview
             * we want to load more messages.
             * the visible area positioning goes from
             * 1.0 to 0.0 because we start at y end
             * so when we get below 0.02 we trigger another load action
             */
            visibleArea.onYPositionChanged: {
                if (visibleArea.yPosition < 0.02 && model.count > 20) {
                    AppActions.chat.loadMoreMessages()
                }
            }
        }
    }

    Rectangle {
        id: input
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        height: entry.height + Suru.units.gu(2)
        color: Suru.backgroundColor

        Rectangle {
            anchors {
                top: parent.top
                right: parent.right
                left: parent.left
            }
            height: Suru.units.dp(1)
            color: Suru.neutralColor
        }

        RowLayout {
            anchors.fill: parent
            anchors.margins: Suru.units.gu(1)
            spacing: Suru.units.gu(1)

            UITK.TextArea {
                id: entry
                activeFocusOnPress: true
                autoSize: true
                selectByMouse: true
                mouseSelectionMode: TextEdit.SelectWords
                wrapMode: TextEdit.WrapAtWordBoundaryOrAnywhere
                placeholderText: i18n.tr("Type a message...")

                Layout.fillHeight: true
                Layout.fillWidth: true

                function send() {
                    Qt.inputMethod.commit();
                    AppActions.chat.sendMessage(text.trim());
                    text = "";
                }

                Keys.onReturnPressed: {
                    if (event.modifiers & Qt.ShiftModifier) {
                        send();
                        return;
                    }
                    event.accepted = false;
                }

                onLengthChanged: {
                    if (!typing_timer.running) {
                        typing_timer.start()
                        AppActions.chat.sendChatAction();
                    }
                }

                Timer {
                    id: typing_timer
                    interval: 5000
                }
            }

            Image {
                visible: entry.displayText.trim() !== ""
                sourceSize.height: height
                source: "qrc:/qml/icons/send.png"

                Layout.fillHeight: false
                Layout.fillWidth: false

                Component.onCompleted: {
                    height = parent.height * 0.75;
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        entry.send();
                    }
                }
            }
        }
    }
}
