import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK
import Ubuntu.Components.Popups 1.3 as UITK_Popups
import QTelegram 1.0
import "../components"
import "../actions"
import "../stores"

Page {
    id: userListPage
    property color hf: Suru.foregroundColor
    property color hb: Suru.backgroundColor
    property color hd: Suru.neutralColor
    header: UITK.PageHeader {
        title: i18n.tr("Contacts")

        UITK.StyleHints {
            foregroundColor: hf
            backgroundColor: hb
            dividerColor: hd
        }

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
            iconName: "add"
            text: i18n.tr('Add Contact')
            onTriggered: UITK_Popups.PopupUtils.open(addDialog)
        }
        ]
    }

    Component {
        id: addDialog
        PopupDialog {
            text: i18n.tr("The contact will be added. First and last name are optional")
            confirmButtonColor: theme.palette.normal.positive
            confirmButtonText: i18n.tr("Add")
            onConfirmed: AppActions.user.addUser(userName.text, firstName.text, lastName.text)
            UITK.TextField {
                anchors {
                    left: parent.left
                    right: parent.right
                }
                id: userName
                text: "+"
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                placeholderText: i18n.tr("Phone no")
            }
            UITK.TextField {
                anchors {
                    left: parent.left
                    right: parent.right
                }
                id: firstName
                placeholderText: i18n.tr("First name")
            }
            UITK.TextField {
                anchors {
                    left: parent.left
                    right: parent.right
                }
                id: lastName
                placeholderText: i18n.tr("Last name")
            }
        }
    }

    ListView {
        id: userListView
        anchors{
            fill: parent
        }
        clip: true
        currentIndex: -1
        model: Telegram.users.contactsmodel
        delegate: UITK.ListItem {

            readonly property QTdUser user: modelData

            width: parent.width
            height: layout.height
            color: "transparent"

            onClicked: {
                AppActions.chat.createOrOpenPrivateChat(user)
            }

            //disables the FastScroll when a list item is swiped
            onSwipedChanged: {
                if (swiped === false && userListView.height >= fastScroll.minimumHeight) {
                    fastScroll.enabled = true
                } else {
                    fastScroll.enabled = false
                }
            }

            leadingActions: UITK.ListItemActions {
                actions: [
                UITK.Action {
                    iconName: "edit-clear"
                    text: i18n.tr("Delete")
                    onTriggered: UITK_Popups.PopupUtils.open(deleteConfirmationDialog)
                }
                ]
            }

            trailingActions: UITK.ListItemActions {
                actions: [
                UITK.Action {
                    iconName: "info"
                    text: i18n.tr("Info")
                    onTriggered: AppActions.user.showUserInfo(user)
                },
                UITK.Action {
                    iconName: "network-secure"
                    text: i18n.tr("Secret Chat")
                    onTriggered: {
                        AppActions.chat.createOrOpenSecretChat(user)
                    }
                }
                ]
            }

            UITK.SlotsLayout {
                id: layout

                GenericPhoto {
                    id: avatar
                    height: units.gu(6)
                    width: height
                    photoPath: user && user.profilePhoto.small.local.path ? user.profilePhoto.small.local.path : ""
                    initials: user ? user.initials : "N/A"
                    avatarColor: user.avatarColor(user ? user.id : 0)
                    myself:  false
                    UITK.SlotsLayout.position: UITK.SlotsLayout.Leading
                    UITK.SlotsLayout.padding.trailing: 0
                }

                mainSlot: Item {
                    height: col.height

                    Column {
                        id: col
                        anchors {
                            left: parent.left
                            right: parent.right
                            top: parent.top
                        }
                        spacing: units.gu(1)

                        RowLayout {
                            height: units.dp(17)
                            width: parent.width
                            spacing: units.dp(2)

                            Row {
                                spacing: units.dp(2)
                                Layout.alignment: Qt.AlignTop
                                Layout.topMargin: units.dp(3)
                            }

                            Label {
                                id: name
                                elide: Text.ElideRight
                                wrapMode: Text.WrapAnywhere
                                maximumLineCount: 1
                                font.weight: Font.DemiBold
                                font.pixelSize: units.dp(17)
                                text: user.firstName + " " + user.lastName
                                Layout.fillWidth: true
                                Layout.alignment: Qt.AlignBottom
                            }
                        }

                        RowLayout {
                            height: units.gu(2)
                            width: parent.width
                            Label {
                                elide: Text.ElideRight
                                wrapMode: Text.WrapAnywhere
                                maximumLineCount: 1
                                font.pixelSize: units.dp(15)
                                Suru.textStyle: Suru.TertiaryText
                                text: user.status.string
                                Layout.fillWidth: true
                            }
                        }
                    }
                }
                Component {
                    id: deleteConfirmationDialog
                    PopupDialog {
                        text: i18n.tr("The contact will be deleted. Are you sure?")
                        confirmButtonColor: UITK.UbuntuColors.red
                        confirmButtonText: i18n.tr("Delete")
                        onConfirmed: AppActions.user.deleteUser(user.id)
                    }
                }
            }

        }
        function getSectionText(index) {
            return Telegram.users.contactsmodel.get(index).firstName.substring(0,1)
        }
        section {
            property: "firstName"
            criteria: ViewSection.FirstCharacter
            labelPositioning: ViewSection.InlineLabels
            delegate: UITK.ListItem {
                height: Suru.units.gu(4)
                Label {
                    text: section
                    anchors.fill: parent
                    anchors.leftMargin: Suru.units.gu(2)
                    verticalAlignment: Text.AlignVCenter
                    height: Suru.units.gu(3)
                }
            }
        }
    }

    FastScroll {
        id: fastScroll
        listView: userListView
        enabled: userListView.height >= minimumHeight
        anchors {
            right: parent.right
            verticalCenter: parent.verticalCenter
        }
    }
}
