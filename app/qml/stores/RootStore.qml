import QtQuick 2.4
import QuickFlux 1.1

Store {
    property alias auth: auth
    AuthStateStore {
        id: auth
    }

    property alias users: usersStore
    UsersStateStore {
        id: usersStore
    }

    property alias chats: chatsStore
    ChatStateStore {
        id: chatsStore
    }

    property alias settings: settingsStore
    SettingsStore {
        id: settingsStore
    }
}
