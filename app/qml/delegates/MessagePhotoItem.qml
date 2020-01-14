import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import QTelegram 1.0
import QuickFlux 1.1

MessageBubbleItem {
    id: bubble
    multimediaLayout: message.content.caption.text == ""
    mcMargins: 0
    MessageContentPhoto {
        message: bubble.message
        contentBeforeImage: senderLabelVisible || contentBeforeMain
    }
}
