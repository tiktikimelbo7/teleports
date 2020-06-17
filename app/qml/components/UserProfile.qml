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

Column {
    id: profileColumn
    property bool isSettings: false
    property bool isGroup: false
    property QTdUser user: isSettings ? me : null
    property QTdChat chat: null
    // this component is usually used inside a Flickable which has anchors.margins > 0
    // the margins value is used to extend dividers to full width
    property var parentMargins: 0
    property var itemList: []
    spacing: 0
    RowLayout {
        id: profile
        spacing: Suru.units.gu(1)
        width: parent.width

        GenericPhoto {
            id: avatar
            Layout.preferredHeight: Suru.units.gu(10)
            Layout.preferredWidth: Layout.preferredHeight
            photoPath: isGroup ? chat.chatPhoto.small.local.path : user.profilePhoto.small.local.path
            initials: isGroup ? chat.initials : user.initials
            avatarColor: isGroup ? chat.avatarColor(chat.rawId) : user.avatarColor(user.id)
            myself: false
            Layout.alignment: Qt.AlignVCenter
            Component.onCompleted: {
                var bigPhoto = isGroup ? chat.chatPhoto.big : user.profilePhoto.big
                if (bigPhoto.local.canBeDownloaded && !bigPhoto.local.isDownloadingCompleted) {
                    console.log("downloading...")
                    bigPhoto.downloadFile();
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    var smallPhoto = isGroup ? chat.chatPhoto.small : user.profilePhoto.small
                    var bigPhoto = isGroup ? chat.chatPhoto.big : user.profilePhoto.big
                    // wait for the photo be downloaded? or pass to the PreviewPage the photo object s.t. it can show the photo as soon as the download finished?
                    AppActions.view.pushToStack("qrc:///pages/PreviewPage.qml", {
                        "photoPreviewSource": Qt.resolvedUrl("file://" + (bigPhoto.local.isDownloadingCompleted ? bigPhoto.local.path : smallPhoto.local.path))
                    });
                }
            }
        }
        Item {
            Layout.preferredHeight: chatTitle.height
            Layout.maximumHeight: avatar.height
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter

            UITK.ListItemLayout {
                id: chatTitle
                anchors.verticalCenter: parent.verticalCenter

                title {
                    text: isGroup ? chat.title : user.fullName
                    font.pixelSize: Suru.units.gu(2.5)
                    wrapMode: Text.Wrap
                    maximumLineCount: 3
                }
                subtitle.text: isGroup ? i18n.tr("Members: %1").arg(parseInt(chat.memberCount)) : user.status.string
                subtitle.font.pixelSize: Suru.units.gu(1.5)
            }
        }
    }

    Item {
        width: parent.width
        height: Suru.units.gu(2)
    }

    Rectangle {
        height: Suru.units.dp(1)
        anchors {
            left: parent.left
            right: parent.right
            leftMargin: -parentMargins
            rightMargin: -parentMargins
        }
        color: Suru.neutralColor
    }
    Component {
        id: dumbComp
        Item{}
    }
    ListModel {
        id: userProfileModel
        Component.onCompleted: {
            // dumb element with all of the properties used in this model.
            // apparently you can't introduce new properties on later elements
            // and  the type of the properties must be consistend, as dynamicRoles is false.
            // setting it to true would slower the performance somewhere between 4-6x (from Qt docs)
            // and makes destoying this item segfault the app
            append({
                iconName: "",
                titleText: "",
                maxLine: 1,
                fullDivider: false,
                delegateState: "dumb",
                switchChecked: false,
                newPage: "",
                newPageParams: {},
                popup: dumbComp
            })
            if (isGroup ? false : user.phoneNumber) {
                append({
                    iconName: "call-start",
                    titleText: "<a href='tel:///+"+user.phoneNumber+"'>+" + user.phoneNumber + "</a>"
                })
            }
            if (isGroup ? chat.userName != ""  : false) {
                append({
                    iconName: "stock_link",
                    titleText: "https://t.me/" + chat.userName,
                    maxLine: 4
                })
            }
            if (isGroup ? chat.inviteLink != ""  : false) {
                append({
                    iconName: "insert-link",
                    titleText: chat.inviteLink,
                    maxLine: 4
                })
            }
            if (isGroup ? chat.description != undefined && chat.description != "" : (user.fullInfo.bio + user.fullInfo.shareText) != "") {
                append({
                    iconName: "info",
                    titleText: isGroup ? chat.description : user.fullInfo.bio + user.fullInfo.shareText,
                    maxLine: 15
                })
            }
            if (isGroup ? false : user.username != "") {
                append({
                    iconName: "contact",
                    titleText: "@" + user.username
                })
            }
            if (count>0) {setProperty(count-1, "fullDivider", true)}
            if (profileColumn.itemList) {
                for (var i = 0; i < profileColumn.itemList.length; i++) {
                    append(profileColumn.itemList[i])
                }
            }
        }
    }
    Repeater {
        model: userProfileModel
        delegate: UITK.ListItem {
            id: itemDelegate
            width: parent.width
            height: layoutDelegate.height
            state: delegateState ? delegateState : ""
            divider {
                anchors {
                    leftMargin: fullDivider  ? -parentMargins : 0
                    rightMargin: fullDivider ? -parentMargins : 0
                }
            }
            UITK.ListItemLayout {
                id: layoutDelegate
                UITK.Icon {
                    height: parent.title.font.pixelSize * 2
                    visible: parent.width > Suru.units.gu(20)
                    width: height
                    name: iconName
                    UITK.SlotsLayout.position: UITK.SlotsLayout.Leading
                }
                title {
                    text: titleText
                    wrapMode: Text.Wrap
                    maximumLineCount: maxLine
                    elide: Text.ElideRight
                    linkColor: Suru.highlightColor
                    onLinkActivated: Qt.openUrlExternally(link)
                }
                Switch {
                    id: switchDelegate
                    visible: false
                    UITK.SlotsLayout.position: UITK.SlotsLayout.Trailing
                    Suru.highlightType: Suru.PositiveHighlight
                    onCheckedChanged: AppActions.chat.muteChat(chat, checked ? 0 : 574410023)
                }
                UITK.ProgressionSlot {id: progressionSlot; visible: false}
            }
            states: [
            State {
                name: "switch"
                PropertyChanges {target: switchDelegate; visible: true; checked: switchChecked}
                PropertyChanges {target: itemDelegate; onClicked: switchDelegate.checked = !switchDelegate.checked}
            },
            State {
                name: "new-page"
                PropertyChanges {target: progressionSlot; visible: true}
                PropertyChanges {
                    target: itemDelegate
                    onClicked: AppActions.view.pushToStack(newPage, newPageParams)
                }
            },
            State {
                name: "popup"
                PropertyChanges {
                    target: itemDelegate
                    onClicked: {
                        console.log(popup)
                        UITK_Popups.PopupUtils.open(popup)
                    }
                }
            },
            State {
                name: "dumb"
                PropertyChanges {target: itemDelegate; visible: false; height: 0}
            }
            ]
        }
    }
}
