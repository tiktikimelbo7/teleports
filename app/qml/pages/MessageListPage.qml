import QtQuick 2.8
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK
import QTelegram 1.0
import "../actions"
import "../delegates"
import "../stores"

Page {
    property color hf: Suru.foregroundColor
    property color hb: Suru.backgroundColor
    property color hd: Suru.neutralColor
    header: UITK.PageHeader {
        title: Telegram.chats.currentChat.title
        UITK.StyleHints {
            foregroundColor: hf
            backgroundColor: hb
            dividerColor: hd
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
    }

    Image {
        id: background_img
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
        horizontalAlignment: Image.AlignLeft
        verticalAlignment: Image.AlignTop
        sourceSize: Qt.size(width,height)
        source: {
            //return Theme.name == "Ubuntu.Components.Themes.SuruDark" ? "qrc:/qml/files/telegram_background_dark.png" : "qrc:/qml/files/telegram_background.png"
            return "qrc:/qml/icons/telegram_background.png"
        }
        opacity: 0.7
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
            anchors.fill: parent
            model: Telegram.chats.messageList
            verticalLayoutDirection: ListView.BottomToTop
            delegate: Component {
                Loader {
                    id: loader
                    width: parent.width
                    height: childrenRect.height
                    Component.onCompleted: setSource(delegateMap.findComponent(modelData.content.type), {message: modelData})
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
                placeholderText: i18n.tr("Send a message")

                Layout.fillHeight: true
                Layout.fillWidth: true

                function send() {
                    Qt.inputMethod.commit();
                    AppActions.chat.sendMessage(entry.text.trim());
                    entry.text = "";
                }

                Keys.onReturnPressed: {
                    if (event.modifiers & Qt.ShiftModifier) {
                        entry.send();
                        return;
                    }
                    event.accepted = false;
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
