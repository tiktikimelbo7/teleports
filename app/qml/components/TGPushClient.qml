import QtQuick 2.0
import Ubuntu.Components 1.3
import Ubuntu.PushNotifications 0.1
import Qt.labs.settings 1.0
import Ubuntu.Components.Popups 1.3
import QTelegram 1.0
import QuickFlux 1.1
import "../actions"
import "../stores"


PushClient {
    id: pushClient

    property var errorReport: null
    onTokenChanged: console.log("👍 Registered new token=======================================================: '", token, "'")
    Component.onCompleted: {
        console.log("PUSHCOMPLETE=======================================================", token)
        onTokenChanged: console.log("👍 Registered new token (2)=======================================================: '", token, "'")
        error.connect(pusherror)
    }

    function pusherror ( reason ) {
        console.warn("👹 Error with pushclient=======================================================:",reason)
        if ( reason === "bad auth" ) {
            //PopupUtils.open( noUbuntuOneDialog )
        }
        else errorReport = reason
    }

    function enable () {
        if (errorReport === null) return
        console.log("👷 Try to enable push notifications with token: '%1'".arg(token))
        AppActions.notificiations.enableNotifications ( token )
        // TODO: Send request to the telegram server
    }

    // TODO: Why is this not working? (AppListener is not a type)
    // TODO: When the user is logged in -> Enable push notifications
    // TODO: When the user starts the app AND is logged in AND push notifications are not enabled -> Enable push notifications
    /*AppListener {
    Filter {
    type: AuthKey.authPasswordAccepted
    onDispatched: pushClient.enable ()
}
}*/

appId: "telegram-plus.dpniel_telegram-plus"
}
