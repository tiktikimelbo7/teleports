import QtQuick 2.9
import QtQuick.Controls 2.2
import Ubuntu.Components 1.3 as UITK
import Ubuntu.Components.Popups 1.3 as UITK_Popups
import QtQuick.Controls.Suru 2.2
import QtQuick.Layouts 1.1
import QTelegram 1.0
import "../actions"
import "../components"
import "../stores"


Page {
    id: chatInfoPage

    property bool isGroup: false
    property QTdUser user: null
    property QTdChat chat: null

    header: UITK.PageHeader {
        title: chatInfoPage.isGroup ? i18n.tr('Group Details') : i18n.tr('Profile')

        leadingActionBar.actions: [
        UITK.Action {
            iconName: "back"
            text: i18n.tr('Back')
            onTriggered: {
                AppActions.view.popFromStack()
            }
        }
        ]
        trailingActionBar.actions: [
        UITK.Action {
            iconName: "edit"
            text: i18n.tr('Edit')
            visible: !isGroup
            onTriggered: UITK_Popups.PopupUtils.open(editUserDialog)
        },
        UITK.Action {
            iconName: "message"
            text: i18n.tr('Send message')
            visible: !isGroup
            onTriggered: {
                AppActions.chat.closeCurrentChat()
                AppActions.chat.createOrOpenPrivateChat(user)
            }
        }
        ]
        UITK.StyleHints {
            backgroundColor: dumb_color.color
        }
        Item {
            id: dumb_color
            Suru.highlightType: Suru.PositiveHighlight
            property color color: chat && chat.isSecret ? Qt.tint(Suru.backgroundColor, Qt.rgba(Suru.highlightColor.r, Suru.highlightColor.g, Suru.highlightColor.b, 0.4)) : Suru.backgroundColor
        }
    }
    Component {
        id: editUserDialog
        PopupDialog {
            text: i18n.tr("Edit user data and press Save")
            confirmButtonColor: theme.palette.normal.positive
            confirmButtonText: i18n.tr("Save")
            onConfirmed: AppActions.user.addUser(userName.text, firstName.text, lastName.text)
            UITK.TextField {
                anchors {
                    left: parent.left
                    right: parent.right
                }
                id: userName
                text: '+' + user.phoneNumber
                readOnly: true
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                placeholderText: i18n.tr("Phone no")
            }
            UITK.TextField {
                anchors {
                    left: parent.left
                    right: parent.right
                }
                id: firstName
                text: user.firstName
                placeholderText: i18n.tr("First name")
            }
            UITK.TextField {
                anchors {
                    left: parent.left
                    right: parent.right
                }
                id: lastName
                text: user.lastName
                placeholderText: i18n.tr("Last name")
            }
        }
    }


    Flickable {
        id: chatInfoFlickable
        anchors.fill: parent
        anchors.margins: Suru.units.gu(2)
        contentHeight: chatInfoColumn.height

        ColumnLayout {
            id: chatInfoColumn
            width: parent.width
            spacing: 0

            UserProfile {
                Layout.fillWidth: true
                user: chatInfoPage.user
                chat: chatInfoPage.chat
                isGroup: chatInfoPage.isGroup
                parentMargins: chatInfoFlickable.anchors.margins
                Component.onCompleted: {
                    if (chat && !chat.isMyself) {
                        itemList.push({
                            iconName: "notification",
                            titleText: i18n.tr("Notifications"),
                            maxLine: 2,
                            fullDivider: false,
                            delegateState: "switch",
                            switchChecked: !chat.isMuted,
                            newPage: "",
                            newPageParams: {}
                        })
                    }
                    if (!isGroup && (chat ? !chat.isMyself : true)) {
                        itemList.push({
                            iconName: "contact-group",
                            titleText: i18n.tr("%1 groups in common").arg(user.fullInfo.groupInCommonCount),
                            maxLine: 2,
                            fullDivider: false,
                            delegateState: "",
                            switchChecked: false,
                            newPage: "",
                            newPageParams: {}
                        })
                    }
                    if (chat && chat.isSecret) {
                        itemList.push({
                            iconName: "system-lock-screen",
                            titleText: i18n.tr("Encryption Key"),
                            maxLine: 1,
                            fullDivider: false,
                            delegateState: "new-page",
                            switchChecked: false,
                            newPage: "qrc:///pages/SecretChatKeyHashPage.qml",
                            newPageParams: {
                                "keyHashMap": chat.keyHashMap,
                                "keyHashString": chat.keyHashString,
                                "userFirstName": user.firstName
                            }
                        })
                    }
                }
            }
        }
    }
}
