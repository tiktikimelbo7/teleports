pragma Singleton
import QtQuick 2.8

Item {

    property alias auth: auth
    AuthAction {
        id: auth
    }

    property alias chat: chat
    ChatActions {
        id: chat
    }
    property alias user: user
    UserStateActions {
        id: user
    }

    property alias notifications: notifications
    NotificationsAction {
        id: notifications
    }

    property alias settings: settings
    SettingsActions {
        id: settings
    }
}
