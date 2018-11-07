import QtQuick 2.4
import QtQuick.Controls 2.2
import Ubuntu.Components 1.3 as UITK
import QtQuick.Controls.Suru 2.2

Page {
    id: aboutPage
    property color hf: Suru.foregroundColor
    property color hb: Suru.backgroundColor
    property color hd: Suru.neutralColor
    header: UITK.PageHeader {
        title: i18n.tr('About')

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
                    pageStack.pop()
                }
            }
        ]

    }

    Flickable {
        id: aboutFlickable
        anchors.fill: parent
        anchors.margins: units.gu(2)

        Column {
            id: aboutCloumn
            anchors.top: header.bottom
            spacing:units.dp(2)
            width:parent.width

            Image {
                width: 80
                height: 80
                mipmap: true
                anchors.horizontalCenter: parent.horizontalCenter
                source: "file:///" + applicationDirPath + "/assets/icon-large.png"
            }

            Label {
                width: parent.width
                font.pixelSize: units.gu(4)
                font.bold: true
                color: theme.palette.normal.backgroundText
                horizontalAlignment: Text.AlignHCenter
                text: i18n.tr("Telegram Plus")
            }

            Label {
                width: parent.width
                color: UITK.UbuntuColors.ash
                horizontalAlignment: Text.AlignHCenter
                text: i18n.tr("Version %1").arg("0.0.1")
            }
        }

        ListView {
            id: listViewAbout
            anchors {
                top: aboutCloumn.bottom
                bottom: parent.bottom
                left: parent.left
                right: parent.right
                topMargin: units.gu(2)
            }

            currentIndex: -1
            interactive: false

            model: [
                        { name: i18n.tr("Get the source"), url: "https://gitlab.com/Flohack74/tg-plus" },
                        { name: i18n.tr("Report issues"),  url: "https://gitlab.com/Flohack74/tg-plus/issues" },
                        { name: i18n.tr("Help translate"), url: "https://translate.ubports.com/projects/ubports/telegram-app" }
            ]

            delegate: UITK.ListItem {
                UITK.ListItemLayout {
                    title.text : modelData.name
                    UITK.Icon {
                        width:units.gu(2)
                        name:"go-next"
                    }
                }
                onClicked: Qt.openUrlExternally(modelData.url)
            }
        }
    }
}
