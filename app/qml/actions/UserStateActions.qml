import QtQuick 2.4
import QuickFlux 1.1
import "./"

ActionCreator {


    function showUserInfo(user) {
        view.pushToStack("qrc:/pages/UserProfilePage.qml", {user: user})
    }
}
