import QtQuick 2.4
import QuickFlux 1.1
import QTelegram 1.0
import "../actions"

Store {

    EnableNotifications {
        id: enableNotifications
    }

    Filter {
        type: NotificationsKey.enableNotifications
        onDispatched: {
            enableNotifications.enable ( message.token )
        }
    }

}
