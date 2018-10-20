import QtQuick 2.4
import QtQuick.Controls 2.2
import Ubuntu.Components 1.3 as UITK
import QtQuick.Controls.Suru 2.2
import "../actions"

// TODO: Use qqc2-suru
Page {
    id: settingsPage
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

    ListModel {
       id: infoModel
    }

    Component.onCompleted: {
        infoModel.append({ name: i18n.tr("Get the source"), url: "https://gitlab.com/Flohack74/tg-plus" })
        infoModel.append({ name: i18n.tr("Report issues"), url: "https://gitlab.com/Flohack74/tg-plus/issues" })
        infoModel.append({ name: i18n.tr("Help translate"), url: "https://gitlab.com/Flohack74/tg-plus/issues" })
    }

    Flickable {
        id: aboutFlickable
        anchors.fill: parent
        anchors.margins: units.gu(2)
        // contentHeight: columnLayout.height

      Column {
          id: aboutCloumn
          // anchors.top: header.bottom
          spacing:units.dp(2)
          width:parent.width

          Image {
            width: 80
            height: 80
            anchors.horizontalCenter: parent.horizontalCenter
            source: "file:///" + applicationDirPath + "/assets/logo.svg"
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

      UITK.UbuntuListView {
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

           model :infoModel
           delegate: UITK.ListItem {
              UITK.ListItemLayout {
               title.text : model.name
               UITK.Icon {
                   width:units.gu(2)
                   name:"go-to"
               }
              }
              onClicked: Qt.openUrlExternally(model.url)
           }
         }
      }
}
