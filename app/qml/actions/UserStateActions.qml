import QtQuick 2.4
import QuickFlux 1.1
import "./"

ActionCreator {
    signal addUser(string userName, string firstName, string lastName)
    signal deleteUser(int userId)

    function showUserInfo(user, chat) {
        view.pushToStack("qrc:/pages/ChatInfoPage.qml", {user: user, chat: chat})
    }

}
