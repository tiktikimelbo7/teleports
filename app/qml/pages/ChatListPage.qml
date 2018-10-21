import QtQuick 2.4
import QtQuick.Controls 2.2
import Ubuntu.Components 1.3 as UITK
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

        leadingActionBar {
          id: menu
          actions : [
              UITK.Action {
                  id: settingsIcon
                  objectName: "settingsIcon"
                  text: i18n.tr("Settings")
                  iconName: "settings"
                  onTriggered: {
                      pageStack.push("qrc:///pages/SettingsPage.qml")
                  }
              },
              UITK.Action {
                  id: infoIcon
                  objectName: "infoIcon"
                  text: i18n.tr("About")
                  iconName: "info"
                  onTriggered: {
                      pageStack.push("qrc:///pages/AboutPage.qml")
                  }
              }
          ]
          numberOfSlots: 1
      }
    }

    ScrollView {
        anchors.fill: parent

        ListView {
            anchors{
                fill: parent
            }
            model: Telegram.chats.sortedList
            delegate: ItemDelegate {
                height: layout.implicitHeight
                width: parent.width
                readonly property QTdChat chat: modelData
                onClicked: {
                    AppActions.chat.setCurrentChat(chat)
                    pageStack.push("qrc:///pages/MessageListPage.qml")
                }

                // TODO: Move to a context action
                //            onClicked: chat.isPinned ? chat.unpinChat() : chat.pinChat()
                UITK.ListItemLayout {
                    id: layout
                    title.text: chat.isPinned ? chat.title + " (pinned)" : chat.title
                    title.color: Suru.foregroundColor
                    subtitle.text: chat.summary
                    subtitle.color: Suru.foregroundColor
                    summary.color: Suru.foregroundColor

                    Item {
                        width: Suru.units.gu(5)
                        height: width
                        UITK.SlotsLayout.position: UITK.SlotsLayout.Leading
                        
                        GenericPhoto {
                            anchors.fill: parent
                            photoPath: chat && chat.chatPhoto.small.local.path ? chat.chatPhoto.small.local.path : ""
                            initials: chat && chat.initials ? chat.initials : "N/A"
                            avatarColor: chat && chat.avatarColor
                        }
                    }

                    UITK.Label {
                        text: chat.lastMessage ? chat.formatDate(chat.lastMessage.date) : ""
                        anchors.top: parent
                        anchors.topMargin: Suru.units.gu(1)
                    }

                    UITK.UbuntuShape {
                        height: Suru.units.gu(2)
                        width: Suru.units.gu(2)
                        color: Suru.darkPositive
                        visible: chat.hasUnreadMessages
                        UITK.Label {
                            anchors.centerIn: parent
                            text: chat.unreadCount
                            color: "white"
                            textSize: UITK.Label.Small
                        }
                    }
                    UITK.UbuntuShape {
                        height: Suru.units.gu(2)
                        width: Suru.units.gu(2)
                        color: Suru.darkPositive
                        visible: chat.hasUnreadMentions
                        UITK.Label {
                            anchors.centerIn: parent
                            text: "@" + chat.unreadMentionCount
                            color: "white"
                            textSize: UITK.Label.Small
                        }
                        UITK.SlotsLayout.position: UITK.SlotsLayout.Last
                    }
                }
            }
        }
    }
}
