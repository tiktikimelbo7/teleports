import QtQuick 2.4
import QuickFlux 1.1
import QTelegram 1.0
import "../actions"

Store {

    /**
     * ChatList model
     *
     * Model containing all chats for the configured account.
     * the model is not sorted in anyway but is exposed to
     * be able to access the undelying QTdChatListModel->model
     * property and it's QList like API from qml.
     *
     * If you want a sorted list use SortedChatList below.
     */
    property alias list: chatList.model
    property alias currentChat: chatList.currentChat
    ChatList {
        id: chatList
        onCurrentChatChanged: {
            if (chatList.currentChat) {
                messageList.loadMore()
                AppActions.view.pushToStack("qrc:/pages/MessageListPage.qml", {})
            } else {
                AppActions.view.popFromStack()
            }
        }
    }

    /**
     * SortedChatList model
     *
     * This model sorts chats based on a few different
     * criteria. Firstly pinned chats will always appear
     * at the top. Followed by the most recently updated
     * chats based on QTdChat::lastMessage()->date().
     *
     * This should actually be done on QTdChat::order()
     * but that seems to return an unknown order (i.e 0)
     * for anything other than supergroups/channels so
     * isn't ideal to actually be used right now.
     *
     * Filters can be applied to the model to only return
     * chat's of a certian type. chatFilters accepts OR'd flags
     *
     * e.g
     *    chatFilters: SortedChatList.PrivateChats | SortedChatList.SecretChats | SortedChatList.SuperGroups
     *
     * The above will only show chats of those three types.
     * see qtdchatlistsortfiltermodel.h for all filter types.
     */
    property alias sortedList: sortedChatList
    SortedChatList {
        id: sortedChatList
        model: chatList
        chatFilters: SortedChatList.CurrentChats
    }

    /**
     *
     */
    property alias messageList: messageList.model
    property alias chat: chatList.currentChat
    MessageList {
        id: messageList
        chat: chatList.currentChat
        // TODO: Implement UI for error handling
        onError: console.error(errorMessage)
    }

    Filter {
        type: ChatKey.setCurrentChat
        onDispatched: {
            if (message.chat) {
                if (chatList.currentChat  && message.chat.id !== chatList.currentChat.id) {
                    return
                }
                chatList.currentChat = message.chat
            }
        }
    }

    Filter {
        type: ChatKey.closeCurrentChat
        onDispatched: {
            if (chatList.currentChat) {
                chatList.currentChat.closeChat()
                chatList.clearCurrentChat()

            }
        }
    }

    Filter {
        type: ChatKey.loadMoreMessages
        onDispatched: {
            if (d.canLoadMoreMessages) {
                messageList.loadMore()
                d.canLoadMoreMessages = false
                enableLoadTimer.start()
            }
        }
    }

    Filter {
        type: ChatKey.sendEditMessageText
        onDispatched: {
            messageList.editMessageText(message.id, message.text);
        }
    }

    Filter {
        type: ChatKey.sendEditMessageCaption
        onDispatched: {
            messageList.editMessageCaption(message.id, message.text);
        }
    }

    Filter {
        type: ChatKey.sendMessage
        onDispatched: {
            messageList.sendMessage(message.text);
        }
    }
    Filter {
        type: ChatKey.sendPhoto
        onDispatched: {
            messageList.sendPhoto(message.photoUrl, message.text, 0);
        }
    }
    Filter {
        type: ChatKey.sendReplyToMessage
        onDispatched: {
            messageList.sendReplyToMessage(message.id, message.text);
        }
    }

    Filter {
        type: ChatKey.sendChatAction
        onDispatched: {
            chatList.currentChat.sendChatAction(true);
        }
    }

    Filter {
        type: ChatKey.leaveChat
        onDispatched: {
            var chat = chatList.model.get(message.chatId)
            if (chat) {
                chat.leaveChat()
            }
        }
    }

    Filter {
        type: ChatKey.deleteChatHistory
        onDispatched: {
            var chat = chatList.model.get(message.chatId)
            if (chat) {
                chat.deleteChatHistory()
            }
        }
    }


    Timer {
        id: enableLoadTimer
        interval: 1000
        triggeredOnStart: false
        onTriggered: d.canLoadMoreMessages = true
    }

    QtObject {
        id: d
        property bool canLoadMoreMessages: true
    }
}
