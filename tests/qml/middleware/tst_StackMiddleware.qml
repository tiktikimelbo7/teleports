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
            property string page: ""
            property bool pushCalled: false
            property bool popCalled: false
            property bool clearCalled: false
            property int depth: 0
            function push(c, p) {
                page = c
                depth = depth + 1
                pushCalled = true
            }

            function pop() {
                depth = depth - 1
                popCalled = true
            }

            function clear(c, p) {
                page = ""
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
            pstack.page = ""
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

        function test_replace_on_stack() {
            compare(pstack.depth, 0)
            AppActions.view.pushToStack("x", {})
            compare(pstack.depth, 1)
            compare(pstack.page, "x")
            AppActions.view.replaceOnStack("y", {})
            compare(pstack.depth, 1)
            compare(pstack.page, "y")
        }

        function test_pop_from_stack() {
            compare(pstack.depth, 0)
            AppActions.view.pushToStack("x", {})
            compare(pstack.depth, 1)
            AppActions.view.popFromStack()
            compare(pstack.depth, 0)
        }

        function test_doesnt_propogate_stack_event_after_handling() {
            var msg = "no change"
            listener.on(ViewKey.pushToStack, function(message) {
                msg = message
            }).on(ViewKey.popFromStack, function (message) {
                msg = message
            }).on(ViewKey.replaceOnStack, function (message) {
                msg = message
            })
            AppActions.view.pushToStack("x", {})
            AppActions.view.replaceOnStack("y", {})
            AppActions.view.popFromStack()
            compare(msg, "no change")
        }
    }
}
