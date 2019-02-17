import QtQuick 2.9
import QtTest 1.0
import QuickFlux 1.1
import Teleports.Testing 1.0
import actions 0.1

Item {
    id: root
    width: 400
    height: width

    AppListener{id: listener}

    TestCase {
        name: "ChatAction"

        function init() {
            TestRunner.init()
        }

        function test_set_current_chat() {
            var chat = testutils.getChat()
            var match = false
            listener.on(ChatKey.setCurrentChat, function (message) {
                match = message.chat.id === chat.id
            })
            AppActions.chat.setCurrentChat(chat)
            verify(match)
        }

        function test_close_current_chat() {
            var seen = false
            listener.on(ChatKey.closeCurrentChat, function (message) {
                seen = true
            })
            AppActions.chat.closeCurrentChat()
            verify(seen)
        }

        function test_load_more_messages() {
            var seen = false
            listener.on(ChatKey.loadMoreMessages, function (message) {
                seen = true
            })
            AppActions.chat.loadMoreMessages()
            verify(seen)
        }

        function test_send_message() {
            var match = false
            listener.on(ChatKey.sendMessage, function (message) {
                match = message.text === "message"
            })
            AppActions.chat.sendMessage("message")
            verify(match)
        }

        function test_send_chat_action() {
            var seen = false
            listener.on(ChatKey.sendChatAction, function (message) {
                seen = true
            })
            AppActions.chat.sendChatAction()
            verify(seen)
        }

        function test_leave_chat() {
            var chatid = ""
            listener.on(ChatKey.leaveChat, function (message) {
                chatid = message.chatId
            })
            AppActions.chat.leaveChat("12345")
            compare(chatid, "12345")
        }

        function test_delete_chat_history() {
            var chatid = ""
            listener.on(ChatKey.deleteChatHistory, function (message) {
                chatid = message.chatId
            })
            AppActions.chat.deleteChatHistory("12345")
            compare(chatid, "12345")
        }

        function test_delete_message() {
            var msgid = ""
            listener.on(ChatKey.deleteMessage, function (message) {
                msgid = message.id
            })
            AppActions.chat.deleteMessage("123456")
            compare(msgid, "123456")
        }

        function test_forward_message() {
            var msgid = ""
            listener.on(ChatKey.forwardMessage, function (message) {
                msgid = message.id
            })
            AppActions.chat.forwardMessage("123456")
            compare(msgid, "123456")
        }

        function test_request_edit_message() {

        }

        function test_send_edit_message_text() {

        }

        function test_send_edit_message_caption() {

        }

        function test_request_reply_to_message() {

        }

        function test_send_reply_to_message() {

        }

        function test_show_stickerpack() {

        }

        function test_view_group_info() {
            var chat = testutils.getChat()
            var seen = false
            var pushed = false
            var chatId = ""
            listener.on(ChatKey.viewGroupInfo, function (message) {
                seen = true
            }).on(ViewKey.pushToStack, function (message) {
                pushed = true
                chatId = message.properties.chat.id
            })
            AppActions.chat.viewGroupInfo(chat)
            // viewGroupInfo shouldn't dispatch the key
            verify(!seen)
            // but should dispatch a pagestack push
            verify(pushed)
            compare(chatId, chat.id)
        }
    }
}
