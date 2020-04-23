import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import QTelegram 1.0
import QuickFlux 1.1

MessageBubbleItem {
    id: bubble
    textLastCharX: msgAudio.caption.textLastCharX
    textFullWidth: msgAudio.caption.width
    inlineFitEnabled: message.content.caption.text != ""
    MessageContentAudio {
        id: msgAudio
        message: bubble.message
    }
}
