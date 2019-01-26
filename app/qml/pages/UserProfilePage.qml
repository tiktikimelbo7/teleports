import QtQuick 2.4
import QtQuick.Controls 2.2
import Ubuntu.Components 1.3 as UITK
import QtQuick.Controls.Suru 2.2
import QtQuick.Layouts 1.1
import QTelegram 1.0
import "../actions"
import "../components"

Page {
    id: userProfilePage
    property color hf: Suru.foregroundColor
    property color hb: Suru.backgroundColor
    property color hd: Suru.neutralColor
    header: UITK.PageHeader {
        title: i18n.tr('Profile')

        UITK.StyleHints {
            foregroundColor: hf
            backgroundColor: hb
            dividerColor: hd
        }

        leadingActionBar.actions: [
            UITK.Action {
                iconName: "back"
                text: i18n.tr('Back')
                onTriggered: {
                  AppActions.user.clearCurrentUser()
                }
            }
        ]
    }

    Flickable {
        id: userProfileFlickable
        anchors.fill: parent
        anchors.margins: units.gu(2)
        contentHeight: userProfileColumn.height

        ColumnLayout {
            id: userProfileColumn
            anchors {
                fill: parent
            }

            UserProfile {}
        }
    }
}
