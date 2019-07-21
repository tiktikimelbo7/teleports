import QtLocation 5.8
import QtPositioning 5.8
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK
import QtMultimedia 5.8
import QTelegram 1.0
import QuickFlux 1.1
import "../actions"
import "../components"

MessageContentBase {
    property QTdMessageLocation location: message.content

    height: Math.max(errorText.height, qtMap.height)
    width: maximumAvailableContentWidth

    Label {
        id: errorText
        anchors.fill: parent
        text: qtMap.errorString
        visible: qtMap.error != Map.NoError
    }

    Plugin {
        id: mapPlugin
        name: "osm"
    }

    Map {
        id: qtMap
        anchors.top: parent.top
        height: maximumAvailableContentWidth
        width: maximumAvailableContentWidth
        plugin: mapPlugin
        center: QtPositioning.coordinate(location.location.latitude, location.location.longitude)
        zoomLevel: 17
        copyrightsVisible: false
        Component.onCompleted: {
            var positionCircle = Qt.createQmlObject('import QtLocation 5.8; MapCircle {}', qtMap)
            positionCircle.center = qtMap.center
            positionCircle.radius = 10.0
            positionCircle.border.width = 3
            qtMap.addMapItem(positionCircle)
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            Qt.openUrlExternally("geo:" + location.location.latitude + "," + location.location.longitude)
        }
    }
}
