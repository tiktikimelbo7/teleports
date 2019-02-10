import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK
import QTelegram 1.0
import QTelegramStyles 1.0
import "../components"

MessageItemBase {
    id: item

    property QTdMessageCall call: message.content

    Column {
        anchors {
            top: parent.top
        }

        width: l1.contentWidth + Suru.units.gu(6)

        RowLayout {
            id: row
            height: Math.max(Suru.units.gu(3), c.height)
            width: parent.width

            Column {
                id: c
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignTop
                Label {
                    id: l1
                    Suru.highlightType: {
                        switch(call.discardReason.type) {
                        case QTdObject.CALL_DISCARD_REASON_DECLINED:
                            return Suru.NegativeHighlight
                        case QTdObject.CALL_DISCARD_REASON_DISCONNECTED:
                        case QTdObject.CALL_DISCARD_REASON_EMPTY:
                            return Suru.WarningHighlight
                        case QTdObject.CALL_DISCARD_REASON_HUNG_UP:
                            return Suru.PositiveHighlight
                        case QTdObject.CALL_DISCARD_REASON_MISSED:
                            return Suru.InformationHighlight
                        }
                    }
                    Suru.textStyle: Suru.SecondaryText
                    color: Suru.highlightColor

                    text: {
                        switch(call.discardReason.type) {
                        case QTdObject.CALL_DISCARD_REASON_DECLINED:
                            return i18n.tr("Call Declined")
                        case QTdObject.CALL_DISCARD_REASON_DISCONNECTED:
                            return i18n.tr("Call Disconnected")
                        case QTdObject.CALL_DISCARD_REASON_EMPTY:
                        case QTdObject.CALL_DISCARD_REASON_HUNG_UP:
                            return i18n.tr("Call Ended")
                        case QTdObject.CALL_DISCARD_REASON_MISSED:
                            return message.isOutgoing ? i18n.tr("Cancelled Call") : i18n.tr("Missed Call")
                        default:
                            return ""
                        }
                    }
                }

                Label {
                    visible: call.duration > 0
                    readonly property int minutes: Math.floor(call.duration / 60)
                    readonly property string minutesString: (minutes < 10 ? "0%1" : "%1").arg(minutes)
                    readonly property int seconds: call.duration - minutes * 60
                    readonly property string secondsString: (seconds < 10 ? "0%1" : "%1").arg(seconds)
                    text: i18n.tr("Duration: %1.%2").arg(minutesString).arg(secondsString)
                    Suru.textLevel: Suru.Small
                    Suru.textStyle: Suru.TertiaryText
                }
            }

            UITK.StyledItem {
                height: units.gu(3)
                width: height
                Layout.alignment: Qt.AlignVCenter
                UITK.Icon {
                    width: parent.width
                    height: parent.height
                    name: "call-stop"
                }
            }
        }
    }
}
