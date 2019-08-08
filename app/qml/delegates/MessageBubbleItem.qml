import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK
import Ubuntu.Components.Popups 1.3 as UITK_Popups
import QTelegram 1.0
import QTelegramStyles 1.0

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
    property bool transparentBackground: false
    property real maximumAvailableContentWidth: Math.min(Suru.units.gu(45), width * (3/4))
                                                - (mc.anchors.leftMargin + mc.anchors.rightMargin)

    default property alias content: mainContent.data
    highlightColor: Qt.rgba(Suru.highlightColor.r, Suru.highlightColor.g, Suru.highlightColor.b, 0.4)

    leadingActions: UITK.ListItemActions {
        actions: [
            UITK.Action {
                iconName: "delete"
                text: i18n.tr("Delete")
                onTriggered: UITK_Popups.PopupUtils.open(deleteConfirmationDialog)
                visible: message.canBeDeletedOnlyForSelf || message.canBeDeletedForAllUsers
            }
        ]
    }

    Component {
        id: deleteConfirmationDialog
        PopupDialog {
            text: message.canBeDeletedForAllUsers ?
                    i18n.tr("The message will be deleted for all users in the chat. Do you really want to delete it?") :
                    i18n.tr("The message will be deleted only for you. Do you really want to delete it?")
            confirmButtonColor: UITK.UbuntuColors.red
            confirmButtonText: i18n.tr("Delete")
            onConfirmed: AppActions.chat.deleteMessage(message.id)
        }
    }

    trailingActions: UITK.ListItemActions {
        actions: [
            UITK.Action {
                iconName: "edit-copy"
                text: i18n.tr("Copy")
                visible: false
                onTriggered: { /* TODO: Implement copy */ }
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
                onClicked: AppActions.user.showUserInfo(message.sender)
            }
        }

        Rectangle {
            id: contentCol
            color: {
                if (transparentBackground) {
                    return "transparent";
                }
                if (message.isOutgoing) {
                    if (message.content.type === QTdObject.MESSAGE_CALL) {
                        return Suru.secondaryBackgroundColor
                    }

                    if (message.sendingState) { // unsent
                        return Suru.neutralColor
                    } else if (message.id > chat.lastReadOutboxMessageId) { //sent
                        var sentcolor = Suru.theme == Suru.Dark ? Suru.lightInformation : Suru.darkInformation
                        return Qt.tint(Suru.backgroundColor, Qt.rgba(sentcolor.r, sentcolor.g, sentcolor.b, 0.5))
                    } // read
                    var readcolor = Suru.theme == Suru.Dark ? Suru.lightPositive : Suru.darkPositive
                    return Qt.tint(Suru.backgroundColor, Qt.rgba(readcolor.r, readcolor.g, readcolor.b, 0.5))
                } //incoming
                return Suru.secondaryBackgroundColor
            }

            radius: 4
            Layout.alignment: message.isOutgoing ? Qt.AlignRight : Qt.AlignLeft
            Layout.minimumWidth: Math.min(Math.min(Suru.units.gu(45), base.width * (3/4)), mc.width + mc.horizontalMargins)
            Layout.maximumWidth: Layout.minimumWidth
            Layout.preferredHeight: mc.height + Suru.units.dp(5)

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
                    topMargin: Suru.units.dp(5)
                    left: !message.isOutgoing ? parent.left : undefined
                    leftMargin: Suru.units.dp(5)
                    right: message.isOutgoing ? parent.right: undefined
                    rightMargin: Suru.units.dp(5)
                }

                property real horizontalMargins: mc.anchors.leftMargin + mc.anchors.rightMargin

                width: Math.min(Suru.units.gu(45) - mc.horizontalMargins,
                                Math.max(mainContent.width,
                                         senderLabel.contentWidth,
                                         dateLabel.implicitWidth))
                height: childrenRect.height

                Item {
                    visible: !(message.isOutgoing || chat.isPrivate || chat.isSecret) && !message.sameUserAsNextMessage
                    width: parent.width
                    height: Suru.units.gu(2.5)

                    RowLayout {
                        id: topBar
                        anchors.fill: parent

                        Label {
                            id: senderLabel
                            text: message.sender ? "%1 %2".arg(message.sender.firstName).arg(message.sender.lastName) : ""
                            font.bold: false
                            color: message.sender ? message.sender.avatarColor(message.sender.id) : ""
                            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                        }
                    }
                }

                Item {
                    id: mainContent
                    height: children ? childrenRect.height : 0
                    width: childrenRect.width
                }

                Item {
                    height: Suru.units.gu(2.5)
                    width: parent.width

                    RowLayout {
                        id: bottomBar
                        anchors.fill: parent

                        Row {
                            id: channel_views
                            visible: message.isChannelPost
                            anchors.verticalCenter: parent.verticalCenter
                            opacity: message.isOutgoing ? 1 : 0.6
                            UITK.Icon {
                                anchors.verticalCenter: parent.verticalCenter
                                width: units.gu(2)
                                height: width
                                source: Qt.resolvedUrl("qrc:/qml/icons/eye.svg")
                                color: Suru.foregroundColor
                            }

                            Label {
                                anchors.verticalCenter: parent.verticalCenter
                                text: message.views
                                Suru.textLevel: Suru.Small
                                Suru.textStyle: Suru.TertiaryText
                            }
                        }

                        Label {
                            id: editedLabel
                            text: i18n.tr("Edited");
                            visible: message.isEdited;
                            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                            Suru.textLevel: Suru.Small
                            Suru.textStyle: Suru.TertiaryText
                            opacity: message.isOutgoing ? 1 : 0.6
                        }

                        Label {
                            id: dateLabel
                            text: message.formatDate(message.date)
                            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                            Suru.textLevel: Suru.Small
                            Suru.textStyle: Suru.TertiaryText
                        }
                    }
                }
            }
        }

        Item {
            Layout.fillWidth: !message.isOutgoing
        }
    }
}
