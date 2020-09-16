import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Ubuntu.Content 1.3
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
            },
            UITK.Action {
                text: i18n.tr("Settings")
                iconName: "navigation-menu"
                onTriggered: {
                    mainMenuPanel.visible ? mainMenuPanel.close() : mainMenuPanel.open()
                }
                visible: Telegram.chats.listMode == ChatList.Idle && Telegram.chats.chatList_type == SortedChatList.Main
            },
            UITK.Action {
                text: i18n.tr("Back")
                iconName: "back"
                onTriggered: {
                    AppActions.chat.toggleArchivedChats()
                    chatListListView.positionViewAtBeginning()
                }
                visible: Telegram.chats.chatList_type == SortedChatList.Archive
            }
        ]
    }

    Menu {
        id: mainMenuPanel
        width: parent.width * 0.75
        Suru.theme: rootItem.Suru.theme
        MenuPanelItem {
            icon: "inbox"
            label: i18n.tr("Saved Messages")
            onTriggered: AppActions.chat.createOrOpenSavedMessages()
        }
        MenuPanelItem {
            icon: "address-book-app-symbolic"
            label: i18n.tr("Contacts")
        onTriggered: AppActions.settings.viewUserList()
        }
        MenuPanelItem {
            icon: "settings"
            label: i18n.tr("Settings")
        onTriggered: AppActions.view.pushToStack("qrc:/pages/SettingsPage.qml", {})
        }
        MenuPanelItem {
            id: nightModeMenuItem
            icon: "night-mode"
            label: i18n.tr("Night mode")
            Switch {
                id: theme_switch
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                checked: Telegram.settings.theme === Suru.Dark
                Suru.highlightType: Suru.PositiveHighlight
                onCheckedChanged: AppActions.settings.setTheme(checked ? Suru.Dark : Suru.Light)
            }
            onTriggered: AppActions.settings.setTheme(theme_switch.checked ? Suru.Light : Suru.Dark)
        }
        MenuPanelItem {
            icon: "help"
            label: i18n.tr("About")
            onTriggered: AppActions.view.pushToStack("qrc:///pages/AboutPage.qml", {})
        }
    }

    WaitingBar {
        id: waitingBar
        connectionState: Telegram.connections.connectionState
        z: 10
    }

    ScrollView {
        id: chatListScrollView
        anchors.fill: parent
        ListView {
            id: chatListListView
            anchors{
                fill: parent
            }

            // WORKAROUND: Fix for wrong grid unit size
            Component.onCompleted: root.flickable_responsive_scroll_fix(chatList)

            model: Telegram.chats.sortedList
            header: UITK.ListItem {
                height: visible ? archivedLayout.height : 0
                UITK.ListItemLayout {
                    id: archivedLayout
                    title {
                        text: i18n.tr("Archived chats")
                        font.weight: Font.DemiBold
                    }
                }
                onClicked: AppActions.chat.toggleArchivedChats()
                visible: Telegram.chats.chatList_type == SortedChatList.Main
            }
            delegate: ChatListDelegate {}
        }
    }

    property string incomingTextFromContentHub: ""
    function processContentHubIncoming(transfer) {
        if (transfer.contentType === ContentType.Links || transfer.contentType === ContentType.Text) {
            var message = ""
            for (var i = 0; i < transfer.items.length; i++) {
                if (String(transfer.items[i].text).length > 0) {
                    message += String(transfer.items[i].text)
                    message += "\n"
                }
                if (String(transfer.items[i].url).length > 0) {
                    message += String(transfer.items[i].url)
                    message += "\n"
                }
            }
            incomingTextFromContentHub = message.trim()
            AppActions.chat.importFromContentHub(transfer.contentType, [""]) // dummy filename list with length = 1
        } else {
            var fileNames = [ ]
            for ( var i = 0; i < transfer.items.length; i++ ) {
                var filePath = String(transfer.items[i].url).replace('file://', '')
                var fileName = filePath.split("/").pop();
                fileNames.push(filePath)
            }
            AppActions.chat.importFromContentHub(transfer.contentType, fileNames)
        }
    }

    Connections {
        target: ContentHub
        onImportRequested: processContentHubIncoming(transfer)
        onShareRequested: processContentHubIncoming(transfer)
    }
}
