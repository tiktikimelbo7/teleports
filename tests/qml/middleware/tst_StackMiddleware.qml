import QtQuick 2.9
import QtTest 1.0
import QuickFlux 1.1
import Teleports.Testing 1.0
import actions 0.1
import middleware 0.1

Item {
    id: root
    width: 400
    height: width

    Item {
        id: rootItem
        anchors.fill: parent
        Item {
            id: pstack
            property bool pushCalled: false
            property bool popCalled: false
            property bool clearCalled: false
            property int depth: 0
            function push(c, p) {
                depth = depth + 1
                pushCalled = true
            }

            function pop() {
                depth = depth - 1
                popCalled = true
            }

            function clear(c, p) {
                depth = 0
                clearCalled = true
            }
        }

        MiddlewareList {
            applyTarget: AppDispatcher
            StackMiddleware {
                stack: pstack
            }
        }
        AppListener {id: listener}
    }


    TestCase {
        name: "StackMiddleware"

        function init() {
            TestRunner.init()
        }

        function cleanup() {
            pstack.pushCalled = false
            pstack.popCalled = false
            pstack.clearCalled = false
            pstack.depth = 0
        }

        function test_push_to_stack() {
            compare(pstack.depth, 0)
            AppActions.view.pushToStack("x", {})
            compare(pstack.depth, 1)
        }

        function test_doesnt_propogate_push_event_after_handling() {
            var msg = "no change"
            listener.on(ViewKey.pushToStack, function(message) {
                msg = message;
            })
            AppActions.view.pushToStack("x", {})
            compare(msg, "no change")
        }
    }
}
