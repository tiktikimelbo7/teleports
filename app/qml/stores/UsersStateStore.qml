import QtQuick 2.4
import QuickFlux 1.1
import QTelegram 1.0
import "../actions"

Store {

    property alias me: usersStore.me
    property alias model: usersStore.model
    property alias currentUser: usersStore.currentUser



    Users {
        id: usersStore
        onCurrentUserChanged:{
          console.log("CURRENT USER CHANGED")
        }
    }
    Filter {
        type: UserStateKey.setCurrentUser
        onDispatched: {
            if (message.user) {
                if (usersStore.currentUser && message.user.id !== usersStore.currentUser.id) {
                    return
                }
                usersStore.currentUser = message.user
                if (message.user == me) {
                    AppDispatcher.dispatch("pushToStack", {view: "qrc:/pages/SettingsPage.qml"})
                }
                else {
                    AppDispatcher.dispatch("pushToStack", {view: "qrc:/pages/UserProfilePage.qml"})
                }
            }
        }
    }
    Filter {
        type: UserStateKey.clearCurrentUser
        onDispatched: {
            AppDispatcher.dispatch("popFromStack")
            usersStore.clearCurrentUser();
        }
    }
    Filter {
        type: UserStateKey.setCurrentUserById
        onDispatched: {
            if (!message.userId) {
                return
            }
            usersStore.currentUser = usersStore.model.get(message.userId)
            AppDispatcher.dispatch("pushToStack", {view: "qrc:/pages/UserProfilePage.qml"})
        }
    }
 
}
