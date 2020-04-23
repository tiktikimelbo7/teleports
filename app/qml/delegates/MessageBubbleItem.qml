import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK
import Ubuntu.Components.Popups 1.3 as UITK_Popups
import QTelegram 1.0

import "../components"
import "../actions"
import "../stores"

UITK.ListItem {
    id: base
    width: parent.width
    height: contentCol.height + row.anchors.topMargin + row.anchors.bottomMargin
    divider.visible: false

    property QTdMessage message: null
    property QTdChat chat: null
    property bool indicatorsEnabled: false
    property bool transparentBackground: false
    // multimediaLayout: this property is used to display the date/time of sending inside a photo/video/etc... instead as another row at the bottom of the message.
    property bool multimediaLayout: false
    property real mcMargins: Suru.units.dp(5)
    property real maximumAvailableContentWidth: maxAvailableWidthNoMargins - mc.horizontalMargins
    property real maxAvailableWidthNoMargins: Math.min(Suru.units.gu(45), width * (3/4))
    property alias senderLabelVisible: senderLabel.visible

    default property alias content: mainContent.data
    highlightColor: Qt.rgba(Suru.highlightColor.r, Suru.highlightColor.g, Suru.highlightColor.b, 0.4)
    property var textLastCharX: undefined
    property var inlineFitEnabled: false
    property var textFullWidth: undefined

    leadingActions: UITK.ListItemActions {
        actions: [
            UITK.Action {
                iconName: "delete"
                text: i18n.tr("Delete")
                onTriggered: UITK_Popups.PopupUtils.open("qrc:/components/DeleteDialog.qml", base, {message:base.message})

                visible: message.canBeDeletedOnlyForSelf || message.canBeDeletedForAllUsers
            }
        ]
    }

    trailingActions: UITK.ListItemActions {
        actions: [
            UITK.Action {
                iconName: "edit-copy"
                text: i18n.tr("Copy")
                visible: message.content.type == QTdObject.MESSAGE_TEXT
                onTriggered: {
                    var mimeData = UITK.Clipboard.newData();
                    mimeData.text = message.content.text.text;
                    UITK.Clipboard.push(mimeData);
                }
            },
            UITK.Action {
                iconName: "edit"
                text: i18n.tr("Edit")
                visible: message.canBeEdited
                onTriggered: {
                    AppActions.chat.requestEditMessage(message)
                }
            },
            UITK.Action {
                iconName: "mail-reply"
                text: i18n.tr("Reply")
                visible: !message.isChannelPost
                onTriggered: AppActions.chat.requestReplyToMessage(message)
            },
            UITK.Action {
                iconName: "info"
                text: i18n.tr("Sticker Pack info")
                visible: message.content.type === QTdObject.MESSAGE_STICKER
                onTriggered: AppActions.chat.showStickerPack(message.content.sticker.setId)
            },
            UITK.Action {
                iconName: "next"
                text: i18n.tr("Forward")
                visible: message.canBeForwarded
                onTriggered: AppActions.chat.forwardMessage([message.id])
            }
        ]
    }

    RowLayout {
        id: row
        anchors {
            fill: parent
            margins: Suru.units.gu(1)
            topMargin: message.sameUserAsNextMessage && !message.isLatest ? Suru.units.dp(2) : Suru.units.dp(3)
            bottomMargin: message.sameUserAsPreviousMessage && !message.isLatest ? Suru.units.dp(2) : Suru.units.dp(3)
        }

        Item {
            width: Suru.units.gu(4.5)
            height: Suru.units.gu(4.5)
            Layout.alignment: Qt.AlignBottom | Qt.AlignLeft
            visible: !(message.isOutgoing || chat.isPrivate || chat.isSecret || chat.isChannel)

            GenericPhoto {
                visible: !message.sameUserAsPreviousMessage
                anchors.fill: parent
                photoPath: message.sender && message.sender.profilePhoto ? message.sender.profilePhoto.small.local.path : ""
                initials: message.sender ? message.sender.initials : "N/A"
                avatarColor: message.sender ? message.sender.avatarColor(message.sender.id) : ""
            }
            MouseArea {
                anchors.fill: parent
                onClicked: AppActions.user.showUserInfo(message.sender, null)
            }
        }

        Rectangle {
            id: contentCol
            color: {
                if (transparentBackground) {
                    return "transparent"
                }
                if (message.isOutgoing) {
                    if (message.content.type === QTdObject.MESSAGE_CALL) {
                        return Suru.secondaryBackgroundColor
                    }
                    if (message.sendingState) {
                        if (message.sendingState.type===QTdObject.MESSAGE_SENDING_STATE_PENDING) { // unsent
                            return Suru.neutralColor
                        } else if (message.sendingState.type===QTdObject.MESSAGE_SENDING_STATE_FAILED) {  //failed
                            var failedcolor = Suru.theme == Suru.Dark ? Suru.lightNegative : Suru.darkNegative
                            return Qt.tint(Suru.backgroundColor, Qt.rgba(failedcolor.r, failedcolor.g, failedcolor.b, 0.5))
                        }
                    } else if (message.id > chat.lastReadOutboxMessageId) { //sent
                        var sentcolor = Suru.theme == Suru.Dark ? Suru.lightInformation : Suru.darkInformation
                        return Qt.tint(Suru.backgroundColor, Qt.rgba(sentcolor.r, sentcolor.g, sentcolor.b, 0.5))
                    } // read
                    var readcolor = Suru.theme == Suru.Dark ? Suru.lightPositive : Suru.darkPositive
                    return Qt.tint(Suru.backgroundColor, Qt.rgba(readcolor.r, readcolor.g, readcolor.b, 0.5))
                } //incoming
                return Suru.secondaryBackgroundColor
            }

            radius: Suru.units.dp(4)
            Layout.alignment: message.isOutgoing ? Qt.AlignRight : Qt.AlignLeft
            Layout.minimumWidth: Math.min(maxAvailableWidthNoMargins, mc.width + mc.horizontalMargins + (message_status_item.extend_bubble ? -message_status_item.anchors.rightMargin : 0))
            Layout.maximumWidth: Layout.minimumWidth
            Layout.preferredHeight: mc.height + 2*mcMargins - message_status_item.anchors.bottomMargin

            Rectangle {
               width: contentCol.width
               height: contentCol.height
               radius: contentCol.radius
               y: units.dp(1)
               z: -5
               visible: !transparentBackground
               color: Suru.neutralColor    // (i.e. Silk, #CDCDCD) (#e6e6e6) with opacity: 0.5
            }

            Column {
                id: mc

                anchors {
                    top: parent.top
                    topMargin: mcMargins
                    left: parent.left
                    leftMargin: mcMargins
                    rightMargin: mcMargins
                }

                property real horizontalMargins: mc.anchors.leftMargin + mc.anchors.rightMargin

                width: Math.min(maximumAvailableContentWidth,
                                Math.max(mainContent.width,
                                         senderLabel.contentWidth,
                                         message_status_comp.implicitWidth))
                height: childrenRect.height

                Item {
                    visible: !(message.isOutgoing || chat.isPrivate || chat.isSecret) && !message.sameUserAsNextMessage
                    width: parent.width
                    height: Suru.units.gu(mcMargins == 0 ? 3 : 2.5)

                    RowLayout {
                        id: topBar
                        anchors.fill: parent

                        Label {
                            id: senderLabel
                            text: message.sender ? "%1 %2".arg(message.sender.firstName).arg(message.sender.lastName) : ""
                            font.bold: false
                            color: message.sender ? message.sender.avatarColor(message.sender.id) : ""
                            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                            Layout.leftMargin: mcMargins == 0 ? Suru.units.dp(5) : 0
                        }
                    }
                }

                Item {
                    id: mainContent
                    height: children ? childrenRect.height : 0
                    width: childrenRect.width
                }
            }

            Item {
                id: message_status_item
                property bool inlineFit: message_status_comp.x > textLastCharX + Suru.units.gu(1) && inlineFitEnabled
                property real inlineLength: textLastCharX + Suru.units.gu(1) + message_status_comp.width
                property bool extend_bubble: inlineLength > textFullWidth && inlineLength < maximumAvailableContentWidth && inlineFitEnabled
                height: visible ? message_status_comp.height : 0
                visible: !multimediaLayout
                anchors {
                    bottom: mc.bottom
                    bottomMargin: inlineFit ? Suru.units.dp(-2) : -height
                    left: mc.left
                    right: mc.right
                    rightMargin: extend_bubble ? -message_status_comp.width - 2*mcMargins : 0
                }

                    MessageStatusRow {
                        id: message_status_comp
                        anchors {
                            right: parent.right
                            rightMargin: mcMargins == 0 ? Suru.units.dp(5) : 0
                        }

                        MessageStatusIndicator {
                            bgColor: contentCol.color
                            enabled: base.indicatorsEnabled

                            state: {
                                if(message.isOutgoing) {
                                    if(message.content.type === QTdObject.MESSAGE_CALL) {
                                        return "new";
                                    }
                                    if(message.sendingState) {
                                        return "outgoing";
                                    }
                                    if(message.id > chat.lastReadOutboxMessageId) {
                                        return "sent";
                                    }
                                    return "read";
                                }
                                return "incoming";
                            }
                        }
                    }
                }
            }
        }

        Item {
            Layout.fillWidth: !message.isOutgoing
        }
    }

    ColorAnimation {
        id: highlightAnimation
        target: base
        property: "color"
        easing.type: Easing.OutSine
        from: base.highlightColor
        to: "transparent"
        duration: 750
    }
    function highlight() {
        highlightAnimation.complete()
        highlightAnimation.start()
    }
}
