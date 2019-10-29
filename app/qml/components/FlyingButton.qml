import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK

UITK.UbuntuShape {

    id: flyingButtonItem
    property var iconName: ""
    signal clicked()
    property var visibleState: false

    width: units.gu(6)
    height: width
    anchors.bottom: parent.bottom
    anchors.right: parent.right
    anchors.margins: width / 2
    anchors.rightMargin: -(scrollDownButton.width * 2)
    backgroundColor: Suru.secondaryBackgroundColor
    aspect: UITK.UbuntuShape.DropShadow
    radius: "large"
    opacity: 0.9
    Behavior on backgroundColor {
        ColorAnimation { duration: 200 }
    }

    z: 14
    MouseArea {
        id: mouseArea
        onPressed: parent.backgroundColor = Qt.darker(Suru.secondaryBackgroundColor, 1.3)
        onReleased: parent.backgroundColor = Suru.secondaryBackgroundColor
        anchors.fill: parent
        enabled: parent.visible
        onClicked: parent.clicked()
    }
    UITK.Icon {
        name: iconName
        width: units.gu(3.5)
        height: width
        anchors.topMargin: height / 2
        anchors.centerIn: parent
        z: 14
        color: theme.palette.normal.baseText
    }

    transitions: Transition {
        SpringAnimation {
            spring: 2
            damping: 0.2
            properties: "anchors.rightMargin"
        }
    }
    states: State {
        name: "visible"
        when: visibleState
        PropertyChanges {
            target: scrollDownButton
            anchors.rightMargin: scrollDownButton.width / 2
        }
    }

}
