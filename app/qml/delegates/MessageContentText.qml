import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import QTelegram 1.0
import "../components"

MessageContentBase {
    id: item

    property QTdMessageText textContent: message.content

    ColumnLayout {
        FormattedText {
            id: text
            isPreview: message.isCollapsed
            maximumWidth: maximumAvailableContentWidth
            Layout.fillWidth: true
            formattedText: textContent.text
        }
        // loading asynchronous with yet undefined height makes jumping
        // to the first unread message unpresice
        Loader {
            id: webpage_loader
            visible: textContent.webPage && !message.isCollapsed
            active: visible
            asynchronous: true
            Layout.preferredHeight: childrenRect.height
            Layout.maximumWidth: maximumAvailableContentWidth
            Layout.fillWidth: true
            Layout.minimumWidth: Suru.units.gu(30)
            sourceComponent: Component {
                WebPagePreview {
                    id: webPagePreview
                    preview: textContent.webPage
                }
            }
        }
    }
}
