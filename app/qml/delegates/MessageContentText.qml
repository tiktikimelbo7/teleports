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
        anchors {
            top: parent.top
        }

        isPreview: message.isCollapsed
        maximumWidth: maximumAvailableContentWidth
        formattedText: textContent.text

        Loader {
            visible: textContent.webPage && !message.isCollapsed
            active: visible
            asynchronous: true
            height: childrenRect.height
            width: parent.width
            sourceComponent: Component {
                WebPagePreview {
                    preview: textContent.webPage
                }
            }
        }
    }
}
