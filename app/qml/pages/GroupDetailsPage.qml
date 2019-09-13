import QtQuick 2.4
import QtQuick.Controls 2.2
import Ubuntu.Components 1.3 as UITK
import QtQuick.Controls.Suru 2.2
import QtQuick.Layouts 1.1
import QTelegram 1.0
import "../actions"
import "../components"
import "../stores"


Page {
    id: groupDetailsPage

    property QTdChat chat: null

    property color hf: Suru.foregroundColor
    property color hb: Suru.backgroundColor
    property color hd: Suru.neutralColor
    header: UITK.PageHeader {
        title: i18n.tr('Group Details')

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
    }

    Flickable {
        id: groupDetailsFlickable
        anchors.fill: parent
        anchors.margins: units.gu(2)
        contentHeight: groupDetailsColumn.height

        ColumnLayout {
            id: groupDetailsColumn
            anchors.fill: parent

            RowLayout {
                anchors.fill: parent
                id: profile
                spacing: units.gu(1)

                GenericPhoto {
                    id: avatar

                    /*anchors {
                        top: parent.top
                        topMargin: units.dp(4)
                        left: parent.left
                        leftMargin: units.gu(1)
                        bottom: parent.bottom
                        bottomMargin: units.dp(4)
                        rightMargin: units.gu(2)
                    }*/

                    height: units.gu(13)
                    width: height
                    photoPath: chat.chatPhoto.small.local.path ?
                                    chat.chatPhoto.small.local.path : ""
                    initials: chat.initials ? chat.initials : "N/A"
                    avatarColor: chat.avatarColor(chat.rawId)
                    myself: false
                }

                Column {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.maximumHeight: units.gu(13)

                    Item {
                        height: titleAndMemberCount.height
                        width: parent.width
                        UITK.ListItemLayout {
                            id: titleAndMemberCount
                            width: parent.width

                            padding {
                                top: units.gu(0.25)
                                bottom: units.gu(2)
                            }
                            title {
                                text: chat.title
                                font.pixelSize: units.gu(2.5)
                                wrapMode: Text.Wrap
                                maximumLineCount: 3
                            }
                            subtitle.text: i18n.tr("Members: %1").arg(parseInt(chat.memberCount))
                            subtitle.font.pixelSize: units.gu(1.5)
                        }
                    }

                    Item {
                        height: chatInviteLinkLayout.height
                        width: parent.width
                        visible: chat.inviteLink

                        UITK.ListItemLayout {
                            id: chatInviteLinkLayout
                            width: parent.width

                            padding {
                                top: 0
                                bottom: units.gu(1)
                            }

                            UITK.Icon {
                                height: parent.title.font.pixelSize
                                width: height
                                name: "insert-link"
                                UITK.SlotsLayout.position: UITK.SlotsLayout.Leading
                                anchors.verticalCenter: parent.verticalCenter
                            }

                            title {
                                text: chat.inviteLink
                                wrapMode: Text.Wrap
                                maximumLineCount: 3
                            }
                        }
                    }

                    Item {
                        height: chatDescriptionLayout.height
                        width: parent.width

                        UITK.ListItemLayout {
                            id: chatDescriptionLayout
                            width: parent.width

                            padding {
                                top: 0
                                bottom: 0
                            }

                            /*UITK.Icon {
                                height: parent.title.height
                                width: parent.title.height
                                name: "call-start"
                                UITK.SlotsLayout.position: UITK.SlotsLayout.Leading
                            }*/

                            title {
                                text: chat.description
                                wrapMode: Text.Wrap
                                maximumLineCount: 15
                            }
                        }
                    }
                }
            }
        }
    }
}
