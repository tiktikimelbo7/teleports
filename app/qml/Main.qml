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

UITK.MainView {
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
    property bool landscape: width > height
    readonly property bool tablet: landscape ? width >= units.gu(90) : height >= units.gu(90)
    theme.name: Telegram.settings.uitkTheme
    anchorToKeyboard: true
    applicationName: "teleports.ubports"

    Item {
        id: rootItem
        Suru.theme: Telegram.settings.theme
        anchors.fill: parent

        StackView {
            id: pageStack
            anchors {
                fill: parent
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

    // Placed in MainView for convenience.
    function flickable_responsive_scroll_fix(flickable) {
        // WORKAROUND: Fix for wrong grid unit size
        flickable.flickDeceleration = 1500 * units.gridUnit / 8
        flickable.maximumFlickVelocity = 2500 * units.gridUnit / 8
    }
}
