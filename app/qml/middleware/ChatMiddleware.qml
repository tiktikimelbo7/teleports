import QtQuick 2.9
import QuickFlux 1.1
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK
import Ubuntu.Components.Popups 1.3
import "../actions"
import "../components"

Middleware {
    filterFunctionEnabled: true

    Component {
        id: confirmationDlg
        PopupDialog {
            confirmButtonColor: theme.palette.normal.negative
            onConfirmed: next(action, message)
        }
    }

    Component {
        id: inviteLinkDialog
        PopupDialog {
            property var inviteLinkInfo
            GenericPhoto {
                anchors.horizontalCenter: parent.horizontalCenter
                photoPath: inviteLinkInfo.photo.small.local.path
                initials: inviteLinkInfo.initials
                avatarColor: inviteLinkInfo.avatarColor
                height: Suru.units.gu(10)
                width: height
            }
            UITK.Label {
                Suru.textLevel: Suru.HeadingTwo
                anchors.horizontalCenter: parent.horizontalCenter
                text: inviteLinkInfo.title
                horizontalAlignment: Text.AlignHCenter
                width: parent.width
                maximumLineCount: 2
                elide: Text.ElideRight
                wrapMode: Text.Wrap
            }
            UITK.Label {
                Suru.textStyle: Suru.TertiaryText
                anchors.horizontalCenter: parent.horizontalCenter
                text: {
                    if (inviteLinkInfo.memberCount == inviteLinkInfo.highlightedMemberCount) {
                        i18n.tr("%1 member", "%1 members", inviteLinkInfo.memberCount).arg(inviteLinkInfo.memberCount)
                    } else {
                        i18n.tr("%1 members, among them:").arg(inviteLinkInfo.memberCount)
                    }
                }
            }
            Flickable {
                width: Math.min(parent.width, usersRow.width)
                height: usersRow.height
                contentWidth: usersRow.width
                anchors.horizontalCenter: parent.horizontalCenter
                Row {
                    id: usersRow
                    spacing: Suru.units.gu(2)
                    Repeater {
                        model: inviteLinkInfo.memberUsers
                        delegate: Column {
                            width: Suru.units.gu(8)
                            spacing: Suru.units.gu(0.5)
                            GenericPhoto {
                                anchors.horizontalCenter: parent.horizontalCenter
                                photoPath: modelData.profilePhoto.small.local.path
                                initials: modelData.initials
                                avatarColor: modelData.avatarColor(modelData.id)
                                height: Suru.units.gu(6)
                                width: height
                            }
                            UITK.Label {
                                width: parent.width
                                elide: Text.ElideRight
                                horizontalAlignment: Text.AlignHCenter
                                anchors.horizontalCenter: parent.horizontalCenter
                                text: modelData.firstName
                                Suru.textStyle: Suru.SecondaryText
                            }
                        }
                    }
                }
            }
            confirmButtonColor: theme.palette.normal.activity
            onConfirmed: next(action, message)
        }
    }

    function deleteChatHistory(message) {
        var dlg = PopupUtils.open(confirmationDlg, root, {
                        title: i18n.tr("Are you sure you want to clear the history?"),
                        confirmButtonText: i18n.tr("Clear history"),
                        })
        dlg.confirmed.connect(function(){
            next(ChatKey.deleteChatHistory, message)
        })
    }

    function leaveChat(message) {
        var dlg = PopupUtils.open(confirmationDlg, root, {
                        title: i18n.tr("Are you sure you want to leave this chat?"),
                        confirmButtonText: i18n.tr("Leave"),
                        })
        dlg.confirmed.connect(function(){
            next(ChatKey.leaveChat, message)
        })
    }

    function showChatInviteLinkInfo(message) {
        var dlg = PopupUtils.open(inviteLinkDialog, null, {
                        confirmButtonText: i18n.tr("Join group"),
                        inviteLinkInfo: message.info,
                        })
        dlg.confirmed.connect(function(){
            next(ChatKey.joinChatByInviteLink, message)
        })
    }
}
