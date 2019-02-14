import QtQuick 2.9
import QtTest 1.0
import QuickFlux 1.1
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.3
import Teleports.Testing 1.0
import actions 0.1
import middleware 0.1

Item {
    id: root
    width: 400
    height: width

    StyledItem {
        id: rootItem
        anchors.fill: parent
        MiddlewareList {
            applyTarget: AppDispatcher
            ChatMiddleware {}
        }
        AppListener {id: listener}
    }


    TestCase {
        name: "ChatMiddleware"
        when: windowShown

        property QtObject msg: QtObject{}

        function init() {
            TestRunner.init()
        }

        function cleanup() {
            var dlg = findChild(root, "chatConfirmationDialog")
            if (dlg) {
                PopupUtils.close(dlg)
            }
        }

        function test_show_delete_chat_history_dialog() {
            AppActions.chat.deleteChatHistory(msg)
            var dlg = findChild(root, "chatConfirmationDialog")
            compare(dlg !== null, true)
            compare(dlg.title !== "", true)
            compare(dlg.confirmButtonText !== "", true)
        }

        function test_delete_chat_history_confirm_propogates_event() {
            var called = false
            listener.on(ChatKey.deleteChatHistory, function(message) {
                called = true;
            })
            AppActions.chat.deleteChatHistory(msg)
            compare(called, false)
            var dlg = findChild(root, "chatConfirmationDialog")
            var btn = findChild(dlg, "confirmBtn")
            mouseClick(btn)
            compare(called, true, "Delete chat not confirmed")
        }

        function test_cancel_delete_chat_history_doesnt_propogate_event() {
            var called = false
            listener.on(ChatKey.deleteChatHistory, function(message) {
                called = true;
            })
            AppActions.chat.deleteChatHistory(msg)
            compare(called, false)
            var dlg = findChild(root, "chatConfirmationDialog")
            var btn = findChild(dlg, "cancelBtn")
            mouseClick(btn)
            compare(called, false, "Delete chat was propogated")
        }

        function test_show_leave_chat_dialog() {
            AppActions.chat.leaveChat(msg)
            var dlg = findChild(root, "chatConfirmationDialog")
            compare(dlg !== null, true)
            compare(dlg.title !== "", true)
            compare(dlg.confirmButtonText !== "", true)
        }

        function test_leave_chat_confirm_propogates_event() {
            var called = false
            listener.on(ChatKey.leaveChat, function(message) {
                called = true;
            })
            AppActions.chat.leaveChat(msg)
            compare(called, false)
            var dlg = findChild(root, "chatConfirmationDialog")
            var btn = findChild(dlg, "confirmBtn")
            mouseClick(btn)
            compare(called, true, "Leave chat not confirmed")
        }

        function test_cancel_leave_chat_doesnt_propogate_event() {
            var called = false
            listener.on(ChatKey.leaveChat, function(message) {
                called = true;
            })
            AppActions.chat.leaveChat(msg)
            compare(called, false)
            var dlg = findChild(root, "chatConfirmationDialog")
            var btn = findChild(dlg, "cancelBtn")
            mouseClick(btn)
            compare(called, false, "Leave chat was propogated")
        }
    }
}
