import QtQuick 2.4
import QuickFlux 1.1
import QTelegram 1.0
import "../actions"

Store {

    property alias me: usersStore.me
    property alias model: usersStore.model
    property alias contactsmodel: contactsStore.model

    Users {
        id: usersStore
    }

    Contacts {
        id: contactsStore
    }

    Filter {
        type: UserStateKey.deleteUser
        onDispatched: {
            console.log("Deleting user with id " + message.userId)
            usersStore.deleteUser(message.userId)
        }
    }

        Filter {
        type: UserStateKey.addUser
        onDispatched: {
            console.log("Adding user with name " + message.userName)
            usersStore.addUser(message.userName, message.firstName, message.lastName)
        }
    }
}
