import QtQuick 2.4
import QuickFlux 1.1
import "./"

ActionCreator {
    signal setTheme(int theme)

    function viewConnectivity(connectionManager) {
    view.pushToStack("qrc:/pages/ConnectivityPage.qml", {
                         connectionManager: connectionManager
                     })
    }
}