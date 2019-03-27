import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import QTelegram 1.0
import QTelegramStyles 1.0


Column {
    property var isPreview: false
    property var maximumWidth: 0
    property QTdFormattedText formattedText: null

    spacing: Suru.units.gu(2)

    width: textEdit.width

    TextEdit {
        id: textEdit

        height: contentHeight
        width: Math.min(maximumWidth, dummyTextEdit.contentWidth)
        readOnly: true
        text: isPreview ? elidedTextMetrics.elidedText : formattedText.text
        color: Suru.foregroundColor
        Suru.textStyle: Suru.PrimaryText
        selectedTextColor: Suru.highlightColor
        wrapMode: TextEdit.WrapAtWordBoundaryOrAnywhere
        onLinkActivated: {
            console.log("Link activated: ", link)
            Qt.openUrlExternally(link)
        }
    }

    TextMetrics {
        id: elidedTextMetrics
        text: formattedText.text
        elide: Text.ElideRight
        elideWidth: Math.max(maximumWidth, textEdit.width) - Suru.units.gu(2)
    }

    TextEdit {
        id: dummyTextEdit
        visible: false
        height: contentHeight
        text: formattedText.text
    }

    // Only load the formatter if there are
    // entities to be formatted.
    Loader {
        id: formatter
        active: !isPreview && formattedText.entities.count > 0
        asynchronous: true
        sourceComponent: Component {
            Item {
                TextFormatter {
                    id: textFormatter
                    codeColor: Suru.foregroundColor
                    linkColor: Suru.highlightColor
                    textDocument: textEdit.textDocument
                    content: formattedText
                    Suru.theme: root.Suru.theme
                    Suru.textStyle: Suru.TertiaryText
                }

                TextFormatter {
                    id: dummyTextFormatter
                    textDocument: dummyTextEdit.textDocument
                    content: formattedText
                }
            }
        }
    }
}

