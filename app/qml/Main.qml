import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK
import QuickFlux 1.1
import QTelegram 1.0
import "./actions"
import "./components"
import "./middleware"
import "./stores"

ApplicationWindow {
    id: root
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
    Suru.theme: Telegram.settings.theme

    UITK.StyledItem {
        id: rootItem
        theme.name: Telegram.settings.uitkTheme
        anchors.fill: parent

        StackView {
            id: pageStack
            anchors {
                fill: parent
                bottomMargin: UbuntuApplication.inputMethod.visible ? UbuntuApplication.inputMethod.keyboardRectangle.height/(units.gridUnit / 8) : 0
                Behavior on bottomMargin {
                    NumberAnimation {
                        duration: 175
                        easing.type: Easing.OutQuad
                    }
                }
                onBottomMarginChanged: console.debug("TODO: implement scrool to correct position")
            }
        }

        MiddlewareList {
            applyTarget: AppDispatcher
            /**
             * View Middleware
             *
             * Used for showing errors, conrolling the stack etc
             */
            StackMiddleware {
                stack: pageStack
            }
            ErrorsMiddleware {}

            /**
             * Chat Middleware
             */
            ChatMiddleware{}
        }
    }
}
