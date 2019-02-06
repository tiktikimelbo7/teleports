import QtQuick 2.0
import Ubuntu.Components 1.3
import Ubuntu.PushNotifications 0.1
import Qt.labs.settings 1.0
import Ubuntu.Components.Popups 1.3
import QTelegram 1.0
import QuickFlux 1.1
import "../actions"
import "../stores"


Item {

    property alias token: pushClient.token

    Component {
        id: dialog

        Dialog {
            id: dialogue
            title: i18n.tr("No Ubuntu One")
            Rectangle {
                height: units.gu(0.2)
                width: parent.width
                color: settings.mainColor
            }
            Label {
                text: i18n.tr("Please connect to Ubuntu One to receive push notifications.")
                width: parent.width
                wrapMode: Text.Wrap
            }
            Button {
                width: parent.width
                text: i18n.tr("Close")
                onClicked: {
                    saveSettings.onceDialog = true
                    PopupUtils.close(dialogue)
                }
            }
        }
    }

    Settings {
        id: saveSettings
        property var onceDialog: false
    }

    PushClient {
        id: pushClient

        property var errorReport: null

        onTokenChanged: {
            console.log("👍 Registered new token: '", token, "'")
        if ( token !== "" ) {
                AppActions.notifications.enableNotifications(token);
            }
        }

        Component.onCompleted: {
            error.connect(pusherror)
            //notificationsChanged.connect(newNotification)
        }

        /* Use for debugging:
        function newNotification ( message ) {
            console.log("NEW NOTIFICATION:", JSON.stringify(message))
        }*/

        function pusherror ( reason ) {
            if ( reason === "bad auth" && !saveSettings.onceDialog ) {
                PopupUtils.open(dialog)
            }
            else console.warn("👹 Error with pushclient:",reason)
        }

        appId: "teleports.ubports_teleports"
    }

}
