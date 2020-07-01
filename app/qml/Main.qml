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
    Connections {
        target: UITK.UriHandler
        onOpened: {
            console.log('Open from UriHandler\n'+uris[0])
            processUri(uris[0]);
        }
    }
    Component.onCompleted: {
        if (Qt.application.arguments && Qt.application.arguments.length > 0) {
            console.log(Qt.application.arguments)
            for (var i = 0; i < Qt.application.arguments.length; i++) {
                if (Qt.application.arguments[i].match(/^tg/)) {
                    processUri(Qt.application.arguments[i]);
                }
            }
        }
    }
    function processUri(uri) {
        if (typeof uri === "undefined") {
            return;
        }
        var command
        if (uri.startsWith("tg://")) {
            command = uri.substr(5)
        } else if (uri.startsWith("tg:")) {
            command = uri.substr(3)
        } else {
            return
        }
        command = command.split("?")
        switch (command[0]) {
        case "resolve":
            var args = command[1].split("&")
            for (var i = 0; i < args.length; i++) {
                var param = args[i].split("=")[0]
                var value = args[i].split("=")[1]
                console.log(param)
                switch(param) {
                case "domain":
                    AppActions.chat.setCurrentChatByUsername(value)
                    break;
                default: console.log("undhandled argument: " + args[i]); break;
                }
            }
            break
        case "join":
            // tg:join?invite=XXXXXXXXXX
            break
        default: console.log("undhandled command: " + command); break;
        }
    }
}
