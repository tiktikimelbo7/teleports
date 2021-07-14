import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3 as UITK
import Ubuntu.Components.Popups 1.3 as UITK_Popups
import Ubuntu.Content 1.1 as ContentHub
import QuickFlux 1.1
import QTelegram 1.0
import QtFeedback 5.0
import "../components"
import "../actions"
import "../delegates"
import "../stores"

Page {
    id: messageListPage
    property QTdChat currentChat: Telegram.chats.currentChat
    property var draftReplyId: currentChat.draftMessage ? currentChat.draftMessage.replyToMessageId : 0
    property int currentMessageIndex: currentChat.currentMessageIndex
    property var locationWaitDialog: null
    Suru.highlightType: Suru.PositiveHighlight
    StackView.onRemoved: AppActions.chat.closeCurrentChat()

    header: UITK.PageHeader {
        title: Telegram.chats && currentChat ? currentChat.title : ""
        Suru.highlightType: Suru.PositiveHighlight
        subtitle: {
            if (Telegram.chats && currentChat) {
                if (currentChat.action != "")
                    return currentChat.action;
                if (currentChat.isGroup || currentChat.isChannel) {
                    var memberCountInfo = i18n.tr("%1 member", "%1 members", currentChat.memberCount).arg(currentChat.memberCount);
                    if (currentChat.onlineMemberCount > 0) {
                        memberCountInfo += i18n.tr(", %1 online", ", %1 online", currentChat.onlineMemberCount).arg(currentChat.onlineMemberCount);
                    }
                    return memberCountInfo;
                }
                return currentChat.chatType.user.status.string
            }
            return "";
        }
        leadingActionBar.actions: [
            BackAction {
                enableShortcut: pageStack.currentItem == messageListPage
                onTriggered: {
                    entry.saveDraft()
                    AppActions.view.popFromStack()
                }
            }
        ]
        contents: Item {
            anchors.fill: parent

            GenericPhoto {
                id: avatar

                anchors {
                    verticalCenter: parent.verticalCenter
                    leftMargin: units.dp(4)
                }
                height: units.gu(5)
                width: height
                property QTdLocalFile localFile: Telegram.chats.currentChat.chatPhoto.small.local
                photoPath: {
                    if (localFile && localFile.canBeDownloaded && !localFile.isDownloadingCompleted) {
                        Telegram.chats.currentChat.chatPhoto.small.downloadFile();
                    }
                    return Telegram.chats.currentChat && localFile.path ? localFile.path : ""
                }
                initials: Telegram.chats.currentChat ? Telegram.chats.currentChat.initials : "N/A"
                avatarColor: Telegram.chats.currentChat.avatarColor(Telegram.chats.currentChat ? Telegram.chats.currentChat.rawId : 0)
                myself: Telegram.chats.currentChat ? Telegram.chats.currentChat.isMyself : false
            }

            Rectangle {
                id: connectingIndicator
                anchors.fill: avatar
                visible: false
                color: theme.palette.normal.background
                UITK.Icon {
                    name: "sync-updating" // telegram.busy? "sync-updating" : isConnecting? "sync-error" : "sync-paused"
                    anchors.fill: parent
                    color: Theme.palette.normal.backgroundSecondaryText
                }
                SequentialAnimation {
                    running: visible
                    loops: Animation.Infinite
                    PropertyAnimation {
                        target: connectingIndicator;property: "opacity";to: 1;duration: 500
                    }
                    PropertyAnimation {
                        duration: 300
                    }
                    PropertyAnimation {
                        target: connectingIndicator;property: "opacity";to: 0.0;duration: 800
                    }
                    PropertyAnimation {
                        duration: 300
                    }
                }
            }

            UITK.Icon {
                id: secret_chat_icon
                visible: Telegram.chats.currentChat.isSecret
                name: "network-secure"
                anchors {
                    top: parent.top
                    left: avatar.right
                    topMargin: units.dp(5)
                    bottomMargin: units.dp(5)
                    bottom: titleText.bottom
                }
                width: height
            }
            Label {
                id: titleText
                anchors {
                    top: parent.top
                    left: secret_chat_icon.visible ? secret_chat_icon.right : avatar.right
                    leftMargin: units.gu(1)
                    right: parent.right
                }
                verticalAlignment: Text.AlignVCenter

                font.pixelSize: FontUtils.sizeToPixels("large")
                elide: Text.ElideRight
                wrapMode: Text.WrapAnywhere
                maximumLineCount: 1
                text: header.title.length === 0 ? i18n.tr("Telegram") : header.title

                state: header.subtitle.length > 0 ? "subtitle" : "default"
                states: [
                    State {
                        name: "default"
                        AnchorChanges {
                            target: titleText
                            anchors.verticalCenter: titleText.parent.verticalCenter
                        }
                        PropertyChanges {
                            target: titleText
                            height: titleText.implicitHeight
                            anchors.topMargin: units.gu(0.7)
                        }
                    },
                    State {
                        name: "subtitle"
                        PropertyChanges {
                            target: titleText
                            height: titleText.parent.height / 2
                            anchors.topMargin: units.gu(0.35)
                        }
                    }
                ]

                transitions: [
                    Transition {
                        AnchorAnimation {
                            duration: UITK.UbuntuAnimation.FastDuration
                        }
                    }
                ]
            }

            Label {
                id: subtitleText
                anchors {
                    left: avatar.right
                    leftMargin: units.gu(1)
                    bottom: parent.bottom
                    bottomMargin: units.gu(0.15)
                }
                verticalAlignment: Text.AlignVCenter
                height: parent.height / 2
                width: parent.width

                elide: Text.ElideRight
                wrapMode: Text.WordWrap
                maximumLineCount: 1
                text: header.subtitle
                color: header.isOnline ? theme.palette.normal.focus : theme.palette.normal.backgroundTertiaryText

                Connections {
                    target: header
                    onSubtitleChanged: {
                        subtitleText.opacity = 0;
                        subtitleText.text = "";
                        subtitleTextTimer.start();
                    }
                }

                Timer {
                    id: subtitleTextTimer
                    interval: UITK.UbuntuAnimation.FastDuration
                    onTriggered: {
                        subtitleText.text = header.subtitle;
                        subtitleText.opacity = 1;
                    }
                }

                Behavior on opacity {
                    NumberAnimation {
                        duration: UITK.UbuntuAnimation.FastDuration
                    }
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    mouse.accepted = true;
                    currentChat.isPrivate ? AppActions.user.showUserInfo(currentChat.chatType.user, currentChat) : AppActions.chat.viewGroupInfo(currentChat)
                }
            }
        }
        UITK.StyleHints {
            backgroundColor: dumb_color.color
        }
        Item {
            id: dumb_color
            Suru.highlightType: Suru.PositiveHighlight
            property color color: currentChat.isSecret ? Qt.tint(Suru.backgroundColor, Qt.rgba(Suru.highlightColor.r, Suru.highlightColor.g, Suru.highlightColor.b, 0.4)) : "transparent"
        }
    }

    WaitingBar {
        id: waitingBar
        connectionState: Telegram.connections.connectionState
        z: 10
    }

    FlyingButton {
        id: scrollDownButton
        onClicked: {
            AppActions.chat.jumpToMessage(Telegram.chats.currentChat.lastMessage.id)
        }
        iconName: "toolkit_chevron-down_3gu"
        anchors.bottom: infoBox.enabled ? infoBox.top : msgListScrollView.bottom
        visibleState: msgList.visibleArea.yPosition < 1.0 - msgList.visibleArea.heightRatio - 0.01 && !attach_panel_object.visible
    }

    ScrollView {
        id: msgListScrollView
        anchors {
            left: parent.left
            top: parent.top
            right: parent.right
            bottom: writableChatInfo.visible? writableChatInfo.top : input.top
        }

        ListView {
            id: msgList
            clip: true
            anchors {
                fill: parent
                bottomMargin: Suru.units.gu(1)
            }

            // WORKAROUND: Fix for wrong grid unit size
            Component.onCompleted: root.flickable_responsive_scroll_fix(msgList)

            //highlightRangeMode: ListView.StrictlyEnforceRange
            highlightRangeMode: ListView.ApplyRange

            model: Telegram.chats.messageList
            verticalLayoutDirection: ListView.BottomToTop
            delegate: Component {
                Loader {
                    id: loader
                    width: parent.width
                    height: childrenRect.height
                    Component.onCompleted: setSource(delegateMap.findComponent(modelData.content.type), {
                        message: modelData,
                        chat: Telegram.chats.chat,
                        indicatorsEnabled: Telegram.settings.indicators
                    })
                    asynchronous: false
                }
            }

            visibleArea.onYPositionChanged: {
                if (visibleArea.yPosition < 0.05 && model.count > 10) {
                    AppActions.chat.loadOlderMessages()
                }

                if (visibleArea.yPosition > 1 - visibleArea.heightRatio - 0.05 && model.count > 10) {
                    AppActions.chat.loadNewerMessages()
                }
            }

            onMovingChanged: {
                if (moving && highlightRangeMode != ListView.NoHighlightRange) {
                    // this would smooth scrolling maybe, but must not be set when jumping to messages
                    // highlightRangeMode = ListView.NoHighlightRange;
                }
            }
        }
    }

    InputInfoBox {
        id: infoBox
        // Convenience properties
        readonly property bool editingMessage: d.chatState === ChatState.EditingMessage
        readonly property bool replyingToMessage: d.chatState === ChatState.ReplyingToMessage

        anchors {
            left: parent.left
            right: parent.right
            bottom: writableChatInfo.visible? writableChatInfo.top : input.top
        }
        enabled: editingMessage || replyingToMessage
        editing: editingMessage
        message: d.messageOfInterest
        onCloseRequested: {
            if (editingMessage) {
                entry.text = "";
            }
            d.chatState = ChatState.Default
            d.messageOfInterest = null
        }
    }

    UITK.Label {
        id: writableChatInfo
        anchors {
            left: parent.left
            right: parent.right
            bottom: showKeyboardLoader.top
        }
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        height: entry.height + Suru.units.gu(2)
        visible: !currentChat.isWritable
        text: {
            if (currentChat.isChannel) {
                return i18n.tr("You are not allowed to post in this channel");
            }
            else if (currentChat.isSecret) {
                if (currentChat.isPending) {
                    return i18n.tr("Waiting for other party to accept the secret chat...");
                } else if (currentChat.isClosed) {
                    return i18n.tr("Secret chat has been closed");
                }
            }
            else if (currentChat.isGroup) {
                switch (currentChat.status.type) {
                case QTdObject.CHAT_MEMBER_STATUS_CREATOR:
                    // if you are the creator and isWritable == false is because you left the group
                case QTdObject.CHAT_MEMBER_STATUS_LEFT:
                    return i18n.tr("You left this group")
                    break
                case QTdObject.CHAT_MEMBER_STATUS_BANNED:
                    return i18n.tr("You have been banned")
                    break
                default:
                    return i18n.tr("You are not allowed to post in this group")
                    break
                }
            }
            return i18n.tr("You can't write here. Reason unkown")
        }
    }

    Button {
        id: joinChatButton
        anchors {
            fill: input
            margins: Suru.units.gu(1)
        }
        visible: {
            if (currentChat.isGroup || currentChat.isChannel) {
                switch (currentChat.status.type) {
                case QTdObject.CHAT_MEMBER_STATUS_CREATOR:
                    return !currentChat.status.isMember
                case QTdObject.CHAT_MEMBER_STATUS_LEFT:
                    return true
                default:
                    return false
                }
            }
            return false
        }
        text: i18n.tr("Join")
        onClicked: AppActions.chat.joinChat(currentChat.id)
    }

    AttachPanel {
        id: attach_panel_object
        anchors {
            right: parent.right
            left: parent.left
            bottom: input.top
        }
        function requestMedia(mediaType) {
            Qt.inputMethod.hide();
            mediaImporter.contentType = mediaType;
            mediaImporter.requestMedia();
        }

        function requestLocation() {
            UITK_Popups.PopupUtils.open(sendLocationConfirmationDialog)
        }
        onPhotoRequested: requestMedia(ContentHub.ContentType.Pictures)
        onDocumentRequested: requestMedia(ContentHub.ContentType.Documents)
        onVideoRequested: requestMedia(ContentHub.ContentType.Videos)
        onAudioRequested: requestMedia(ContentHub.ContentType.Music)
        onContactRequested: requestMedia(ContentHub.ContentType.Contacts)
        onLocationRequested: requestLocation()
    }

    StickerPanel {
        id: sticker_panel_object

        anchors {
            right: parent.right
            left: parent.left
            bottom: input.top
        }

        onSendStickerRequested: {
            if (infoBox.replyingToMessage) {
                AppActions.chat.sendSticker(sticker, d.messageOfInterest.id);
                d.chatState = ChatState.Default;
            } else {
                AppActions.chat.sendSticker(sticker, 0);
            }
            sticker_panel_object.close();
        }
    }

    Rectangle {
        id: input
        anchors {
            left: parent.left
            right: parent.right
            bottom: showKeyboardLoader.top
        }
        height: entry.height + Suru.units.gu(2)
        color: Suru.backgroundColor
        visible: currentChat.isWritable
        Rectangle {
            anchors {
                top: parent.top
                right: parent.right
                left: parent.left
            }
            height: Suru.units.dp(1)
            color: Suru.neutralColor
        }

        MediaImport {
            id: mediaImporter

            onMediaReceived: {
                var fileNames = [ ]
                for ( var i = 0; i < importedFiles.length; i++ ) {
                    var filePath = String(importedFiles[i].url).replace('file://', '')
                    var fileName = filePath.split("/").pop();
                    fileNames.push(filePath)
                }
                for (var i = 0; i < fileNames.length; i++) {
                    switch(contentType) {
                        case ContentHub.ContentType.Pictures:
                            AppActions.chat.sendPhoto(fileNames[i], "");
                        break;
                        case ContentHub.ContentType.Videos:
                            AppActions.chat.sendVideo(fileNames[i], "");
                        break;
                        case ContentHub.ContentType.Music:
                            AppActions.chat.sendAudio(fileNames[i], "");
                        break;
                        case ContentHub.ContentType.Contacts:
                            AppActions.chat.sendContact(fileNames[i], "");
                        break;
                        default:
                            AppActions.chat.sendDocument(fileNames[i], "");
                    }
                }
            }
        }

        RowLayout {
            anchors.fill: parent
            anchors.margins: Suru.units.gu(1)
            spacing: Suru.units.gu(1)

            UITK.StyledItem {
                height: entry.implicitHeight
                width: height

                UITK.Icon {
                    anchors.fill: parent
                    visible: !sticker_panel_object.visible
                    source: "qrc:/qml/icons/msg_panel_stickers.svg"
                    color: Suru.foregroundColor
                }

                UITK.Icon {
                    anchors.fill: parent
                    visible: sticker_panel_object.visible
                    name: "down"
                    color: Suru.foregroundColor
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (attach_panel_object.visible) {
                            attach_panel_object.visible = false;
                        }
                        sticker_panel_object.visible = !sticker_panel_object.visible;
                    }
                }
            }

            UITK.TextArea {
                id: entry
                activeFocusOnPress: true
                autoSize: true
                selectByMouse: true
                mouseSelectionMode: TextEdit.SelectWords
                wrapMode: TextEdit.WrapAtWordBoundaryOrAnywhere
                placeholderText: i18n.tr("Type a message...")
                text: currentChat.draftMessage.inputMessageText.text
                Layout.fillHeight: true
                Layout.fillWidth: true

                function send() {
                    Qt.inputMethod.commit();

                    if (d.chatState === ChatState.EditingMessage) {
                        switch (d.messageOfInterest.content.type) {
                            case QTdObject.MESSAGE_TEXT:
                                if(text.trim() == "")
                                    UITK_Popups.PopupUtils.open("qrc:/components/DeleteDialog.qml", entry, {message:d.messageOfInterest})
                                else
                                    AppActions.chat.sendEditMessageText(d.messageOfInterest.id, text.trim())
                                break;
                            case QTdObject.MESSAGE_ANIMATION:
                            case QTdObject.MESSAGE_AUDIO:
                            case QTdObject.MESSAGE_DOCUMENT:
                            case QTdObject.MESSAGE_PHOTO:
                            case QTdObject.MESSAGE_VIDEO:
                            case QTdObject.MESSAGE_VOICE_NOTE:
                                AppActions.chat.sendEditMessageCaption(d.messageOfInterest.id, text.trim())
                                break;
                            default:
                                console.warn("Unsupported edit message type");
                        }
                    } else if (d.chatState === ChatState.ReplyingToMessage) {
                        AppActions.chat.sendReplyToMessage(d.messageOfInterest.id, text.trim());
                    } else {
                        AppActions.chat.sendMessage(text.trim(), true);
                    }

                    text = "";
                    d.chatState = ChatState.Default;
                }
                function sendIfEnter(event) {
                    if (event.modifiers & Qt.ShiftModifier) {
                        send();
                        return;
                    }
                    event.accepted = false;
                }
                function saveDraft() {
                    Qt.inputMethod.commit()
                    switch (d.chatState) {
                    case ChatState.Default:
                        if (currentChat.draftMessage.inputMessageText.text != entry.text || draftReplyId != 0) {
                            AppActions.chat.setChatDraftMessage(entry.text, 0)
                        }
                        break
                    case ChatState.ReplyingToMessage:
                        var replyId = d.messageOfInterest ? d.messageOfInterest.id : 0
                        if (currentChat.draftMessage.inputMessageText.text != entry.text || draftReplyId != replyId) {
                            AppActions.chat.setChatDraftMessage(entry.text, replyId)
                        }
                        break
                    }
                }

                Keys.onEnterPressed: sendIfEnter(event)
                Keys.onReturnPressed: sendIfEnter(event)

                onLengthChanged: {
                    if (!typing_timer.running) {
                        typing_timer.start()
                        AppActions.chat.sendChatAction();
                    }
                    draft_timer.restart()
                }

                Timer {
                    id: typing_timer
                    interval: 5000
                }
                Timer {
                    id: draft_timer
                    interval: 15000
                    onTriggered: entry.saveDraft()
                }

                Connections {
                    target: Qt.application
                    onStateChanged: if (Qt.application.state != Qt.ApplicationActive) {entry.saveDraft()}
                }
            }

            UITK.StyledItem {
                height: entry.implicitHeight
                width: height

                UITK.Icon {
                    anchors.fill: parent
                    name: attach_panel_object.visible ? "down" : "attachment"
                    color: Suru.foregroundColor
                    Suru.textStyle: Suru.SecondaryText

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (sticker_panel_object.visible) {
                                sticker_panel_object.visible = false;
                            }
                            attach_panel_object.visible = !attach_panel_object.visible;
                        }
                    }
                }
            }

            UITK.StyledItem {
                visible: showKeyboardLoader.active
                height: entry.implicitHeight
                width: height
                UITK.Icon {
                    anchors.fill: parent
                    name: showKeyboardLoader.active ? (showKeyboardLoader.item.anchors.bottomMargin < 0 ? "input-dialpad-symbolic" : "down") : ""
                    color: Suru.foregroundColor
                    Suru.textStyle: Suru.SecondaryText
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        hoverEnabled: false
                        onClicked: {
                            if (UbuntuApplication.inputMethod.visible) {
                                UbuntuApplication.inputMethod.hide()
                                showKeyboardLoader.item.hidden = false
                            } else {
                                showKeyboardLoader.item.hidden = !showKeyboardLoader.item.hidden
                            }
                        }
                    }
                }
            }

            UITK.StyledItem {
                id: voiceNoteItem
                property bool recording: false
                property var filename: ""
                height: entry.implicitHeight
                width: height
                visible: !sendIcon.visible
                UITK.Icon {
                    anchors.fill: parent
                    name: "audio-input-microphone-symbolic"
                    color: Suru.foregroundColor
                    Suru.textStyle: Suru.SecondaryText
                    MouseArea {
                        id: voiceNoteMouseArea
                        anchors.fill: parent
                        hoverEnabled: false
                        property var mouseXstart
                        onPressed: {
                            var now = new Date()
                            voiceNoteItem.filename = now.getTime()
                            AppActions.chat.registerVoiceNote(voiceNoteItem.filename)
                            voiceNoteItem.recording = true
                            mouseXstart = mouse.x
                            pressEffect.start()
                        }
                        onReleased: {
                            if (voiceNoteItem.recording) {
                                AppActions.chat.stopVoiceNote()
                                voiceNoteItem.recording = false
                                pressEffect.start()
                                AppActions.chat.sendVoiceNote(voiceNoteItem.filename)
                            }
                        }
                        onCanceled: {
                            AppActions.chat.stopVoiceNote()
                            AppActions.chat.deleteVoiceNote(voiceNoteItem.filename)
                            voiceNoteItem.recording = false
                            pressEffect.start()
                        }
                        onPositionChanged: {
                            if (voiceNoteItem.recording)
                                if (Math.abs(mouse.x - mouseXstart) > input.width * 0.4)
                                    voiceNoteMouseArea.canceled()
                        }
                        HapticsEffect {
                            id: pressEffect
                            attackIntensity: 0.0
                            attackTime: 50
                            intensity: 1.0
                            duration: 10
                            fadeTime: 50
                            fadeIntensity: 0.0
                        }
                    }
                }
            }

            Image {
                id: sendIcon
                visible: entry.displayText.trim() !== "" || d.chatState == ChatState.EditingMessage
                sourceSize.height: height
                source: "qrc:/qml/icons/send.png"
                height: entry.implicitHeight
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        entry.send();
                    }
                }
            }
        }
        Rectangle {
            id: voiceNoteLabelRect
            anchors.fill: parent
            anchors.margins: Suru.units.gu(1)
            color: Suru.backgroundColor
            property var offset: width/4
            visible: opacity > 0.0
            opacity: 0.0
            Behavior on opacity { NumberAnimation {duration: UITK.UbuntuAnimation.FastDuration} }
            Label {
                id: voiceNoteLabel
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                text: i18n.tr("<<< Swipe to cancel")
                x: (parent.width - width) / 2 + parent.offset
                Behavior on x { NumberAnimation {duration: UITK.UbuntuAnimation.FastDuration} }
            }
            Rectangle {
                id: blueCircle
                radius: width/2
                anchors.centerIn: voiceNoteWhite
                width: 0
                height: width
                color: Suru.highlightColor
                visible: voiceNoteItem.recording
                Behavior on width { NumberAnimation {duration: UITK.UbuntuAnimation.FastDuration} }
            }
            UITK.Icon {
                id: voiceNoteWhite
                height: entry.implicitHeight
                width: height
                anchors.right: parent.right
                name: "audio-input-microphone-symbolic"
                color: Suru.backgroundColor
                Suru.textStyle: Suru.SecondaryText
            }
            Item {
                anchors {
                    top: parent.top
                    left: parent.left
                    bottom: parent.bottom
                    topMargin: Suru.units.dp(1)-Suru.units.gu(1)
                    margins: -Suru.units.gu(1)
                }
                width: 2 * recRow.width
                clip: true
                Rectangle {
                    rotation: 90
                    anchors.centerIn: parent
                    width: Math.max(parent.width, parent.height)
                    height: width
                    gradient: Gradient {
                        GradientStop { position: 1.0; color: voiceNoteLabelRect.color }
                        GradientStop { position: 0.4; color: voiceNoteLabelRect.color }
                        GradientStop { position: 0.0; color: "transparent" }
                    }
                }
            }
            Row {
                id: recRow
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                spacing: Suru.units.gu(1)
                Rectangle {
                    id: recDot
                    height: voiceNoteStopWatch.height * 2 / 3
                    width: height
                    radius: height/2
                    anchors.verticalCenter: parent.verticalCenter
                    color: Suru.highlightColor
                    Suru.highlightType: Suru.NegativeHighlight
                    onVisibleChanged: visible ? recAnim.start() : recAnim.stop()
                    SequentialAnimation {
                        id: recAnim
                        loops: Animation.Infinite
                        PropertyAnimation { to: 0.0; target: recDot; property: "opacity"; duration: 400 }
                        PropertyAnimation { to: 1.0; target: recDot; property: "opacity"; duration: 400 }
                    }
                }
                Label {
                    id: voiceNoteStopWatch
                    text: elapsedTimer.minutes+":"+(elapsedTimer.seconds < 10 ? "0" : "")+elapsedTimer.seconds
                    Suru.textLevel: Suru.HeadingTwo
                    Timer {
                        id: elapsedTimer
                        property date startTime: new Date()
                        property date snapshot: startTime
                        property int seconds: elapsed - 60 * minutes
                        property int minutes: elapsed / 60
                        property int elapsed: (snapshot - startTime) / 1000
                        interval: 500
                        repeat: true
                        running: voiceNoteStopWatch.visible
                        triggeredOnStart: true
                        onRunningChanged: startTime = new Date()
                        onTriggered: snapshot = new Date()
                    }
                }
            }
        }

        states: [
        State {
            name: "recording"
            when: voiceNoteItem.recording
            PropertyChanges {target: voiceNoteLabelRect; opacity: 1.0}
            PropertyChanges {target: voiceNoteLabelRect; offset: voiceNoteMouseArea.mouseX/3;}
            PropertyChanges {target: voiceNoteLabel; opacity: 1+(voiceNoteMouseArea.mouseX / input.width)}
            PropertyChanges {target: blueCircle; width: entry.implicitHeight * 2}
        },
        State {
            name: "default"
            when: !voiceNoteItem.recording
            PropertyChanges {target: voiceNoteLabelRect; offset: voiceNoteLabelRect.width/4}
            PropertyChanges {target: voiceNoteLabelRect; opacity: 0.0}
            PropertyChanges {target: blueCircle; width: 0}
        }
        ]
    }

    Component {
        id: sendLocationConfirmationDialog
        PopupDialog {
            text: i18n.tr("Do you want to share your location with %1?").arg(currentChat.title)
            confirmButtonColor: theme.palette.normal.positive
            confirmButtonText: i18n.tr("Send")
            onConfirmed: {
                messageListPage.locationWaitDialog = UITK_Popups.PopupUtils.open(locationWaitDialog)
                AppActions.chat.requestLocation();
            }
        }
    }

    Component {
        id: locationWaitDialog
        PopupWaitCancel {
            text: i18n.tr("Requesting location from OS...")
            onFinished: {
                AppActions.chat.sendLocation();
            }
            onCancelled: {
                AppActions.chat.cancelLocation();
            }
        }
    }

    AppListener {
        Filter {
            type: ChatKey.stopWaitLocation
            onDispatched: {
                UITK_Popups.PopupUtils.close(messageListPage.locationWaitDialog);
            }
        }
    }

    Loader {
        id: showKeyboardLoader
        active: Telegram.chats.currentChat.replyMarkupMessageId != 0 && Telegram.chats.currentChat.replyMarkupMessage.replyMarkup.type == QTdObject.REPLY_MARKUP_SHOW_KEYBOARD
        anchors.bottomMargin: active ? item.anchors.bottomMargin : 0
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        sourceComponent: ReplyMarkupShowKeyboard {
            replyMarkup: Telegram.chats.currentChat.replyMarkupMessage.replyMarkup
        }
    }

    QtObject {
        id: d

        property int chatState: draftReplyId == 0 ? ChatState.Default : ChatState.ReplyingToMessage
        property var messageOfInterest: draftReplyId != 0 ? Telegram.chats.messageList.get(draftReplyId) : null
    }

    AppListener {
        Filter {
            type: ChatKey.requestEditMessage
            onDispatched: {
                d.chatState = ChatState.EditingMessage;
                d.messageOfInterest = message.message;
                switch (message.message.content.type) {
                    case QTdObject.MESSAGE_TEXT:
                        entry.text = message.message.content.text.text;
                        break;
                    case QTdObject.MESSAGE_ANIMATION:
                    case QTdObject.MESSAGE_AUDIO:
                    case QTdObject.MESSAGE_DOCUMENT:
                    case QTdObject.MESSAGE_PHOTO:
                    case QTdObject.MESSAGE_VIDEO:
                    case QTdObject.MESSAGE_VOICE_NOTE:
                        entry.text = message.message.content.caption.text;
                        break;
                }
            }
        }

        Filter {
            type: ChatKey.requestReplyToMessage
            onDispatched: {
                entry.forceActiveFocus()
                d.chatState = ChatState.ReplyingToMessage
                d.messageOfInterest = message.message;
            }
        }

    }

    onCurrentMessageIndexChanged: {
        msgList.currentIndex = currentMessageIndex
        msgList.positionViewAtIndex(currentMessageIndex, ListView.Center);
    }
}
