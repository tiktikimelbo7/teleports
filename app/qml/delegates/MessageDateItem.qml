import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import QTelegram 1.0
import QTelegramStyles 1.0

Item {

    property QTdMessage message: null
    property QTdChat chat: null

    height: Suru.units.gu(5)
    width: parent.width

    RowLayout {
        anchors {
            topMargin: Suru.units.gu(1)
            fill: parent
        }
        Rectangle {
            color: ColorsBright.messageActionBackground
            radius: 4
            width: dl.contentWidth + Suru.units.gu(2)
            height: units.gu(3)
            Item {
                height: parent.height
                width: dl.contentWidth
                anchors.horizontalCenter: parent.horizontalCenter
                Label {
                    id: dl
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        top: parent.top
                        topMargin: Suru.units.dp(4)
                    }
                    opacity: 1
                    color: "white"
                    // TRANSLATORS: A short date format, consisting only of the day number and the month spelled out
                    text: message ? message.content.date.toLocaleDateString(Qt.locale(), i18n.tr("dd MMM")) : "Some date missing"
                }
            }
            Layout.alignment: Qt.AlignCenter
        }
    }
}
