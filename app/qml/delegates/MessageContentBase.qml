import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK
import QTelegram 1.0
import QTelegramStyles 1.0

import "../components"
import "../actions"
import "../stores"

Item {
    id: content

    default property alias content: mainContent.data
    property QTdMessage message: null

    width: contentColumn.width
    height: contentColumn.height
    
    Component {
        id: citation
        Citation {
            messageCited: message.messageRepliedTo
        }
    }

    // Ensure that the message replied to is loaded always
    // Not sure why we have to do that... Thanks to @TimDev for the hint!
    Connections {
        target: message
        onMessageRepliedToChanged: {
            citationLoader.sourceComponent = null
            citationLoader.sourceComponent = citation
        }
    }

    Column {
        id: contentColumn

        width: Math.max(citationLoader.width, mainContent.width)
        // Show an icon on the right to expand/collapse the citation?
        Loader {
            id: citationLoader
            active: message.isReply && !message.isCollapsed
            sourceComponent: citation
        }

        Item {
            id: mainContent
            height: childrenRect.height
            width: childrenRect.width
        }
    }
}
