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
          if (usersStore.currentUser) {
              AppDispatcher.dispatch("replaceOnStack", {view: "qrc:/pages/UserProfilePage.qml"})
          }
          else
            AppDispatcher.dispatch("replaceOnStack", {view: "qrc:/pages/ChatListPage.qml"})
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
                    AppDispatcher.dispatch("replaceOnStack", {view: "qrc:/pages/SettingsPage.qml"})
                }
                else {
                    AppDispatcher.dispatch("replaceOnStack", {view: "qrc:/pages/UserProfilePage.qml"})
                }
            }
        }
    }
    Filter {
        type: UserStateKey.clearCurrentUser
        onDispatched: {
            usersStore.clearCurrentUser();
        }
    }
}
