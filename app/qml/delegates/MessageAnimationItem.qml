import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import QTelegram 1.0
import QuickFlux 1.1

MessageBubbleItem {
    id: bubble
    textLastCharX: msgAnim.caption.textLastCharX
    textFullWidth: msgAnim.caption.width
    inlineFitEnabled: message.content.caption.text != ""
    inlineFitCanEnlargeBubble: false
    MessageContentAnimation {
        id: msgAnim
        message: bubble.message
    }
}
