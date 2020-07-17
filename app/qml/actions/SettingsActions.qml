import QtQuick 2.4
import QuickFlux 1.1
import "./"

ActionCreator {
    signal setTheme(int theme)
    signal toggleIndicators(bool indicators)

    function viewConnectivity(connectionManager) {
        view.pushToStack("qrc:/pages/ConnectivityPage.qml", {
                         connectionManager: connectionManager
                     })
    }

    function viewUserList() {
        view.pushToStack("qrc:/pages/UserListPage.qml", { })
    }
}
