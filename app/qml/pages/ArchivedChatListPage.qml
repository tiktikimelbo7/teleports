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
        title: i18n.tr("Archived chats")

        UITK.StyleHints {
            foregroundColor: hf
            backgroundColor: hb
            dividerColor: hd
        }

        leadingActionBar.actions: [
            UITK.Action {
                text: i18n.tr("Back")
                iconName: "back"
                onTriggered: {
                    AppActions.view.popFromStack()
                }
            }
        ]
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
            model: Telegram.chats.archivedSortedList
            delegate: ChatListDelegate {}
        }
    }
}
