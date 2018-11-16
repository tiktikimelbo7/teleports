import QtQuick 2.9
import QtQuick.Window 2.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK
import QuickFlux 1.1
import QTelegram 1.0
import "./stores"

Window {

    /**
     * Only show the window once we have reached a state
     * that has something to display.
     * TODO: Unless we want to show a progress spinner or something
     * during the AuthStateWaitTdParams and AuthStateWaitEncryptionKey
     * state???
     */
    visible: Telegram.auth.state > AuthState.WaitEncryptionKey
    width: Suru.units.gu(45)
    height: Suru.units.gu(75)
    
    StackView {
        id: pageStack
        anchors {
            fill: parent
            bottomMargin: UbuntuApplication.inputMethod.visible ? UbuntuApplication.inputMethod.keyboardRectangle.height / Screen.devicePixelRatio : 0
            Behavior on bottomMargin {
                NumberAnimation {
                    duration: 175
                    easing.type: Easing.OutQuad
                }
            }
            onBottomMarginChanged: console.debug("TODO: implement scrool to correct position")
        }
    }

    AppListener {
        Filter {
            type: "pushToStack"
            onDispatched: {
                pageStack.push(message.view, message.properties)
            }
        }

        Filter {
            type: "replaceOnStack"
            onDispatched: {
                if (pageStack.depth) {
                    pageStack.clear()
                }
                pageStack.push(message.view, message.properties)
            }
        }


        Filter {
            type: "popFromStack"
            onDispatched: {
                pageStack.pop()
            }
        }
    }
}
