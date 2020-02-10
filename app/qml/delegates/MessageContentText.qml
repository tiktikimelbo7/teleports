import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import QTelegram 1.0
import QTelegramStyles 1.0
import "../components"

MessageContentBase {
    id: item

    property QTdMessageText textContent: message.content

    FormattedText {
        id: text
        anchors {
            top: parent.top
        }

        isPreview: message.isCollapsed
        maximumWidth: maximumAvailableContentWidth
        width: webpage_loader.visible ? webpage_loader.width : implicitWidth
        formattedText: textContent.text

        // loading asynchronous with yet undefined height makes jumping
        // to the first unread message unpresice
        Loader {
            id: webpage_loader
            visible: textContent.webPage && !message.isCollapsed
            active: visible
            asynchronous: true
            height: childrenRect.height
            sourceComponent: Component {
                WebPagePreview {
                    width: Math.min(textContent.webPage.title != "" ? Math.max(Suru.units.gu(30), implicitWidth) : implicitWidth, maximumAvailableContentWidth)
                    preview: textContent.webPage
                }
            }
        }
    }
}
