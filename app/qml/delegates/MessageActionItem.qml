import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import QTelegram 1.0
import "../components"
Item {

    width: parent.width
    height: Suru.units.gu(3)

    Label {
        anchors.centerIn: parent
        text: "User joined"
    }
}
