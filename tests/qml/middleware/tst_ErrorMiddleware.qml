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
            ErrorsMiddleware {}
        }
        AppListener {id: listener}
    }


    TestCase {
        name: "ErrorMiddleware"
        when: windowShown

        function init() {
            TestRunner.init()
        }

        function cleanup() {
            var dlg = findChild(root, "errorDialog")
            if (dlg) {
                PopupUtils.close(dlg)
            }
        }

        function test_showError_opens_dialog() {
            AppActions.view.showError("x", "y", "")
            var dlg = findChild(root, "errorDialog")
            compare(dlg.title, "x")
            compare(dlg.text, "y")
        }

        function test_doesnt_propogate_event_after_handling() {
            var msg = "no change"
            listener.on(ViewKey.showError, function(message) {
                msg = message;
            })
            AppActions.view.showError("x", "y", "")
            compare(msg, "no change")
        }

        function test_close_dialog_dispatches_callback() {
            var called = false
            listener.on("testCallback", function(message) {
                called = true;
            })
            AppActions.view.showError("x", "y", "testCallback")
            var btn = findChild(root, "errorDialogClose")
            mouseClick(btn)
            compare(called,true, "Close callback not received")
        }
    }
}
