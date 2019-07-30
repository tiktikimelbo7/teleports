import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK
import "./"
import "../actions"
Item {
    id: attach_panel

    visible: false

    signal photoRequested()
    signal documentRequested()
    signal videoRequested()
    signal audioRequested()
    signal contactRequested()
    signal locationRequested()
    signal close()

    anchors {
        right: parent.right
        left: parent.left
        bottom: parent.bottom
    }
    clip: false

    onClose: attach_panel.visible = false

    Rectangle {
        id: attach_box
        width: parent.width
        height: units.gu(18)
        y: -height

        Rectangle {
            anchors.fill: parent
            color: Suru.backgroundColor
        }
        Grid {
            id: attachment_grid
            anchors {
                top: parent.top
                topMargin: units.gu(1)
                horizontalCenter: parent.horizontalCenter
            }
            columns: 5
            rows: 1
            horizontalItemAlignment: Grid.AlignHCenter
            verticalItemAlignment: Grid.AlignVCenter
            spacing: units.gu(2.8)

            AttachPanelItem {
                id: attach_photo_item
                // TRANSLATORS: Used in attach menu, when sending a photo to the conversation.
                text: i18n.tr("Image")
                image: Qt.resolvedUrl("qrc:/qml/icons/attach_gallery.png")
                onClicked: {
                    attach_panel.photoRequested()
                    attach_panel.close()
                }
            }
            AttachPanelItem {
                // TRANSLATORS: Used in attach menu, when sending a document to the conversation.
                text: i18n.tr("File")
                image: Qt.resolvedUrl("qrc:/qml/icons/attach_document.png")
                onClicked: {
                    attach_panel.documentRequested()
                    attach_panel.close()
                }
            }
            // AttachPanelItem {
            //     // TRANSLATORS: Used in attach menu, when sending a video to the conversation.
            //     text: i18n.tr("Video")
            //     image: Qt.resolvedUrl("qrc:/qml/icons/attach_video.png")
            //     onClicked: {
            //         attach_panel.videoRequested()
            //         attach_panel.close()
            //     }
            // }
            //
            // AttachPanelItem {
            //     // TRANSLATORS: Used in attach menu, when sending audio to the conversation.
            //     text: i18n.tr("Audio")
            //     image: Qt.resolvedUrl("qrc:/qml/icons/attach_audio.png")
            //     onClicked: {
            //         attach_panel.audioRequested()
            //         attach_panel.close()
            //     }
            // }
            //
            // AttachPanelItem {
            //     // TRANSLATORS: Used in attach menu, when sending a contact to the conversation.
            //     text: i18n.tr("Contact")
            //     image: Qt.resolvedUrl("qrc:/qml/icons/attach_contact.png")
            //     onClicked: {
            //         attach_panel.contactRequested()
            //         attach_panel.close()
            //     }
            // }

/*             AttachPanelItem {
                // TRANSLATORS: Used in attach menu, when sending a location to the conversation.
                text: i18n.tr("Location")
                icon:"location-idle"
                onClicked: {
                    attach_panel.locationRequested()
                    attach_panel.close()
                }
            } */

            AttachPanelItem {
                height: attach_photo_item.height
                // TRANSLATORS: Used in attach menu, when sending a file to the conversation.
                image: Qt.resolvedUrl("qrc:/qml/icons/attach_hide1.png")
                showTick: true
                onClicked: {
                    attach_panel.close()
                }
            }

        }
    }

}
