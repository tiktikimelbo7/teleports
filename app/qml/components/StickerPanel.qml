import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import QtQuick.Layouts 1.0
import Ubuntu.Components 1.3 as UITK
import "./"
import "../actions"

Item {
    id: sticker_panel

    visible: false

    property int bottomMargin: 0

    signal sendStickerRequested(var sticker)
    signal open()
    signal close()

    anchors {
        right: parent.right
        left: parent.left
        bottom: parent.bottom
    }

    clip: false

    onClose: sticker_panel.visible = false
    onOpen: sticker_panel.visible = true
    onVisibleChanged: {
        if (visible) {
            stickerLoader.load();
            sticker_panel.forceActiveFocus();
        }
    }

    Rectangle {
        id: sticker_box
        width: parent.width
        height: units.gu(24) + bottomMargin
        y: -height
        radius: 4
        color: Suru.backgroundColor
        border.color: Suru.neutralColor
        border.width: 2

        Item {
            anchors {
                fill: parent
                bottomMargin: bottomMargin
            }

            BusyIndicator {
                anchors.centerIn: parent
                running: stickerLoader.status !== Loader.Ready
            }

            Loader {
                id: stickerLoader

                anchors.fill: parent
                asynchronous: true

                Component {
                    id: stickerSetBoxComponent

                    StickerSetsBox {
                        anchors.fill: parent
                        onSendStickerRequested: {
                            sticker_panel.sendStickerRequested(sticker)
                        }
                    }
                }

                function load() {
                    if (sourceComponent === null) {
                        sourceComponent = stickerSetBoxComponent;
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: Suru.neutralColor
        }
    }
}
