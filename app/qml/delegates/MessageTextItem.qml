import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import QTelegram 1.0
import QTelegramStyles 1.0
import "../components"

MessageItemBase {
    id: item

    property QTdMessageText textContent: message.content

    Column {
        anchors {
            top: parent.top
        }
        spacing: Suru.units.gu(2)

        width: textEdit.width

        TextEdit {
            id: textEdit

            height: contentHeight
            width: Math.min(maximumAvailableContentWidth, dummyTextEdit.contentWidth)
            readOnly: true
            text: textContent.text.text
            color: ColorsBright.textColor
            selectedTextColor: Suru.highlightColor
            wrapMode: TextEdit.WrapAtWordBoundaryOrAnywhere
            onLinkActivated: {
                console.log("Link activated: ", link)
                Qt.openUrlExternally(link)
            }
        }

        TextEdit {
            id: dummyTextEdit
            visible: false
            height: contentHeight
            text: textContent.text.text
        }

        Loader {
            visible: textContent.webPage
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

    // Only load the formatter if there are
    // entities to be formatted.
    Loader {
        id: formatter
        active: textContent.text.entities.count > 0
        asynchronous: true
        sourceComponent: Component {
            Item {
                TextFormatter {
                    id: textFormatter
                    codeColor: ColorsBright.codeColor
                    linkColor: ColorsBright.linkColor
                    textDocument: textEdit.textDocument
                    content: textContent.text
                }

                TextFormatter {
                    id: dummyTextFormatter
                    textDocument: dummyTextEdit.textDocument
                    content: textContent.text
                }
            }
        }
    }
}
