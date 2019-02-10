import QtQuick 2.9
import QuickFlux 1.1
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.3
import "../actions"

Middleware {
    filterFunctionEnabled: true

    property var stack

    function pushToStack(message) {
        stack.push(message.view, message.properties)
    }

    function replaceOnStack(message) {
        if (stack.depth) {
            stack.clear()
        }
        stack.push(message.view, message.properties)
    }

    function popFromStack(message) {
        stack.pop()
    }
}
