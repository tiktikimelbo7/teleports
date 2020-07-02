import QtQuick 2.4
import QuickFlux 1.1
import QTelegram 1.0
import Ubuntu.Content 1.1 as ContentHub
import Ubuntu.Components 1.3 as UITK
import "../actions"

Store {

    /**
     * ChatList model
     *
     * Model containing all chats for the configured account.
     * the model is not sorted in anyway but is exposed to
     * be able to access the undelying QTdChatListModel->model
     * property and it's QList like API from qml.
     *
     * If you want a sorted list use SortedChatList below.
     */
    property alias listMode: chatList.listMode
    property alias list: chatList.model
    property alias currentChat: chatList.currentChat
    property alias forwardChatId: chatList.forwardedFromChat
    property bool uriHaveBeenProcessed: false

    ChatList {
        id: chatList
        onCurrentChatChanged: {
            if (chatList.currentChatValid) {
                AppActions.view.pushToStack("qrc:/pages/MessageListPage.qml", {})
            } else {
                AppActions.view.popFromStack()
            }
        }
        onPositionInfoReceived: {
            AppActions.chat.stopWaitLocation();
            AppActions.chat.sendLocation(latitude, longitude, 0);
        }
        onPositionInfoTimeout: {
            AppActions.chat.stopWaitLocation();
            AppActions.view.showError(i18n.tr("Error"), i18n.tr("No valid location received after 180 seconds!"), "");
        }
        onInvalidChatUsername: {
            AppActions.view.showError(i18n.tr("Error"), i18n.tr("Username <b>@%1</b> not found").arg(username), "");
        }
        onModelPolulatedCompleted: {
            // habdle URIs only the first time this signal is emitted
            if (uriHaveBeenProcessed) {
                return
            }
            if (Qt.application.arguments && Qt.application.arguments.length > 0) {
                for (var i = 1; i < Qt.application.arguments.length; i++) {
                    processUri(Qt.application.arguments[i]);
                }
            }
            uriHaveBeenProcessed = true
        }
    }

    /**
     * SortedChatList model
     *
     * This model sorts chats based on a few different
     * criteria. Firstly pinned chats will always appear
     * at the top. Followed by the most recently updated
     * chats based on QTdChat::lastMessage()->date().
     *
     * This should actually be done on QTdChat::order()
     * but that seems to return an unknown order (i.e 0)
     * for anything other than supergroups/channels so
     * isn't ideal to actually be used right now.
     *
     * Filters can be applied to the model to only return
     * chat's of a certian type. chatFilters accepts OR'd flags
     *
     * e.g
     *    chatFilters: SortedChatList.PrivateChats | SortedChatList.SecretChats | SortedChatList.SuperGroups
     *
     * The above will only show chats of those three types.
     * see qtdchatlistsortfiltermodel.h for all filter types.
     */
    property alias sortedList: sortedChatList
    SortedChatList {
        id: sortedChatList
        model: chatList
        chatFilters: SortedChatList.CurrentChats
    }

    /**
     *
     */
    property alias messageList: messageList.model
    property alias chat: chatList.currentChat
    MessageList {
        id: messageList
        chat: chatList.currentChat
        // TODO: Implement UI for error handling
        onError: console.error(errorMessage)
    }

    Filter {
        type: ChatKey.setCurrentChat
        onDispatched: {
            if (message.chat) {
                console.log("Opening new chat...")
                if (chatList.currentChatValid  && message.chat.id !== chatList.currentChat.id) {
                    console.log("Wrong chat open...")
                    return
                }
                chatList.currentChat = message.chat
            }
        }
    }

    Filter {
        type: ChatKey.createOrOpenPrivateChat
        onDispatched: {
            if (message.user) {
                AppActions.view.popAllButOneFromStack()
                chatList.createOrOpenPrivateChat(message.user.id)
            }
        }
    }

    Filter {
        type: ChatKey.createOrOpenSavedMessages
        onDispatched: {
            AppActions.view.popAllButOneFromStack()
            chatList.createOrOpenSavedMessages()
        }
    }

    Filter {
        type: ChatKey.createOrOpenSecretChat
        onDispatched: {
            if (message.user) {
                AppActions.view.popAllButOneFromStack()
                chatList.createOrOpenSecretChat(message.user.id)
            }
        }
    }


    Filter {
        type: ChatKey.setCurrentChatById
        onDispatched: {
            var chatById = chatList.model.get(message.chatId)
            if (chatById) {
                if (chatList.currentChat) {
                    AppActions.chat.closeCurrentChat()
                }
                AppActions.chat.setCurrentChat(chatById)
            } else
                console.log("Could not find chat by id")
        }
    }


    Filter {
        type: ChatKey.setCurrentChatByUsername
        onDispatched: {
            chatList.setCurrentChatByUsername(message.username)
        }
    }

    Filter {
        type: ChatKey.closeCurrentChat
        onDispatched: {
            if (chatList.currentChat) {
                console.log("Closing current chat...")
                chatList.currentChat.closeChat()
                chatList.clearCurrentChat()
            } else
                console.log("No chat open, ignoring close request")
        }
    }

    Filter {
        type: ChatKey.loadOlderMessages
        onDispatched: {
            if (d.canLoadMoreMessages) {
                messageList.loadOlder()
                d.canLoadMoreMessages = false
                enableLoadTimer.start()
            }
        }
    }
    Filter {
        type: ChatKey.loadNewerMessages
        onDispatched: {
            if (d.canLoadMoreMessages) {
                messageList.loadNewer()
                d.canLoadMoreMessages = false
                enableLoadTimer.start()
            }
        }
    }

    Filter {
        type: ChatKey.jumpToMessage
        onDispatched: {
            messageList.jumpToMessage(message.id)
        }
    }

    Filter {
        type: ChatKey.sendEditMessageText
        onDispatched: {
            messageList.editMessageText(message.id, message.text);
        }
    }

    Filter {
        type: ChatKey.sendEditMessageCaption
        onDispatched: {
            messageList.editMessageCaption(message.id, message.text);
        }
    }

    Filter {
        type: ChatKey.sendMessage
        onDispatched: {
            messageList.sendMessage(message.text, false);
        }
    }

    Filter {
        type: ChatKey.sendSticker
        onDispatched: {
            messageList.sendSticker(message.sticker, message.replyId);
        }
    }

    Filter {
       type: ChatKey.forwardMessage
       onDispatched: {
          chatList.forwardedFromChat = chatList.currentChat
          var messageIds = [];
          messageIds.push(message.id)
          chatList.forwardingMessages = messageIds;
          chatList.listMode = ChatList.ForwardingMessages
          chatList.currentChat.closeChat()
          chatList.clearCurrentChat()
       }
   }

    Filter {
        type: ChatKey.sendForwardMessage
        onDispatched: {
          chatList.listMode = ChatList.Idle
          AppActions.chat.setCurrentChat(message.chat);
          chatList.sendForwardMessage(chatList.forwardingMessages,
                                           message.chat.id,
                                           chatList.forwardedFromChat.id,
                                           message.text);
        }
    }
    Filter {
        type: ChatKey.cancelForwardMessage
        onDispatched: {
          AppActions.chat.setCurrentChat(chatList.forwardedFromChat)
          chatList.listMode = ChatList.Idle
        }
    }

    property var importedFiles: []
    property var importedFileType

    Filter {
       type: ChatKey.importFromContentHub
       onDispatched: {
          chatList.forwardedFromChat = chatList.currentChat
          chatList.listMode = ChatList.ImportingAttachments
          importedFileType = message.contentType
          importedFiles = message.filePaths
          if(chatList.currentChat) {
            chatList.currentChat.closeChat()
            chatList.clearCurrentChat()
          }
          AppActions.view.popAllButOneFromStack()
       }
   }

    Filter {
        type: ChatKey.sendImportData
        onDispatched: {
            chatList.listMode = ChatList.Idle;
            AppActions.chat.setCurrentChat(message.chat);
            if (message.text.length > 0 && importedFiles.length > 1) {
                AppActions.chat.sendMessage(message.text);
                message.text = ""
            }
            for (var i = 0; i < importedFiles.length; i++) {
                switch(importedFileType) {
                    case ContentHub.ContentType.Pictures:
                        AppActions.chat.sendPhoto(importedFiles[i], message.text);
                    break;
                    case ContentHub.ContentType.Videos:
                        AppActions.chat.sendVideo(importedFiles[i], message.text);
                    break;
                    case ContentHub.ContentType.Music:
                        AppActions.chat.sendAudio(importedFiles[i], message.text);
                    break;
                    case ContentHub.ContentType.Contacts:
                        AppActions.chat.sendContact(importedFiles[i], message.text);
                    break;
                    default:
                        AppActions.chat.sendDocument(importedFiles[i], message.text);
                }
            }
            importedFiles = []
            importedFileType = ""
        }
    }

    Filter {
        type: ChatKey.sendPhoto
        onDispatched: {
            console.log("Send photo %1".arg(message.photoUrl));
            messageList.sendPhoto(message.photoUrl, message.text, 0);
        }
    }

    Filter {
        type: ChatKey.sendAudio
        onDispatched: {
            console.log("Send audio %1".arg(message.photoUrl));
            messageList.sendAudio(message.audioUrl, message.text, 0);
        }
    }
    Filter {
        type: ChatKey.sendVideo
        onDispatched: {
            console.log("Send video %1".arg(message.photoUrl));
            messageList.sendVideo(message.videoUrl, message.text, 0);
        }
    }
    Filter {
        type: ChatKey.sendContact
        onDispatched: {
            console.log("Send contact %1".arg(message.photoUrl));
            messageList.sendContact(message.contactUrl, message.text, 0);
        }
    }
    Filter {
        type: ChatKey.sendDocument
        onDispatched: {
            console.log("Send document %1".arg(message.photoUrl));
            messageList.sendDocument(message.documentUrl, message.text, 0);
        }
    }

    //Location requesting and sending
    Filter {
        type: ChatKey.requestLocation
        onDispatched: {
            chatList.requestPositionInfo();
        }
    }

    Filter {
        type: ChatKey.sendLocation
        onDispatched: {
            messageList.sendLocation(message.latitude, message.longitude, message.livePeriod);
        }
    }

    Filter {
        type: ChatKey.cancelLocation
        onDispatched: {
            chatList.cancelPositionInfo();
        }
    }

    Filter {
        type: ChatKey.sendReplyToMessage
        onDispatched: {
            messageList.sendReplyToMessage(message.id, message.text);
        }
    }

    Filter {
        type: ChatKey.deleteMessage
        onDispatched: {
            messageList.deleteMessage(message.id)
        }
    }

    Filter {
        type: ChatKey.sendChatAction
        onDispatched: {
            chatList.currentChat.sendChatAction(true);
        }
    }

    Filter {
        type: ChatKey.sendVoiceNote
        onDispatched: {
            console.log("Send voice note");
            messageList.sendVoiceNote(message.filename, 0);
        }
    }

    Filter {
        type: ChatKey.leaveChat
        onDispatched: {
            var chat = chatList.model.get(message.chatId)
            if (chat) {
                chat.leaveChat()
            }
        }
    }

    Filter {
        type: ChatKey.deleteChatHistory
        onDispatched: {
            var chat = chatList.model.get(message.chatId)
            if (chat) {
                chat.deleteChatHistory()
            }
        }
    }

    Filter {
        type: ChatKey.setChatDraftMessage
        onDispatched: {
          chatList.setChatDraftMessage(message.draftText,
                                       message.replyToMessageId,
                                       chatList.currentChat.id);
        }
    }

    Filter {
        type: ChatKey.muteChat
        onDispatched: {
            message.chat.mute(message.duration)
        }
    }

    Filter {
        type: ChatKey.registerVoiceNote
        onDispatched: {
            messageList.registerVoiceNote(message.filename)
        }
    }
    Filter {
        type: ChatKey.stopVoiceNote
        onDispatched: {
            messageList.stopVoiceNote()
        }
    }
    Filter {
        type: ChatKey.deleteVoiceNote
        onDispatched: {
            messageList.deleteVoiceNote(message.filename)
        }
    }


    Timer {
        id: enableLoadTimer
        interval: 1000
        triggeredOnStart: false
        onTriggered: d.canLoadMoreMessages = true
    }

    QtObject {
        id: d
        property bool canLoadMoreMessages: true
    }


    Connections {
        target: UITK.UriHandler
        onOpened: {
            console.log('Open from UriHandler: '+uris)
            processUri(uris[0]);
        }
    }
    function processUri(uri) {
        if (typeof uri === "undefined") {
            return;
        }
        var protocol = uri.split(":")[0]
        switch (protocol) {
        case "teleports":
            // User clicked a notification
            var commands = uri.split("://")[1].split("/");
            if (commands) {
                switch(commands[0].toLowerCase()) {
                case "chat":
                    var chatId = parseInt(commands[1]);
                    if (isNaN(chatId) || chatId === 0) {
                        console.warn("Cannot parse chat id to open!");
                    } else {
                        console.info("Opening chat: " + chatId);
                        AppActions.chat.setCurrentChatById(chatId)
                    }
                    break;
                case "launch":
                    //userTapBackHome = true;
                    // Nothing to do.
                    break
                default:
                    console.warn("Unmanaged URI: " + commands)
                }
            }
            break
        case "https":
            // convert https URI into tg format
            uri = "tg://resolve?domain=" + uri.split("https://t.me/")[1]
            console.log("new uri "+uri)
        case "tg":
            // User opened a deep link to a chat or something
            var command
            if (uri.startsWith("tg://")) {
                command = uri.substr(5)
            } else if (uri.startsWith("tg:")) {
                command = uri.substr(3)
            } else {
                return
            }
            command = command.split("?")
            switch (command[0]) {
            case "resolve":
                var args = command[1].split("&")
                for (var i = 0; i < args.length; i++) {
                    var param = args[i].split("=")[0]
                    var value = args[i].split("=")[1]
                    console.log(param)
                    switch(param) {
                    case "domain":
                        AppActions.chat.setCurrentChatByUsername(value)
                        break;
                    default:
                        console.log("undhandled argument: " + args[i])
                        break
                    }
                }
                break
            // case "join":
            //     // tg:join?invite=XXXXXXXXXX
            //     break
            default:
                console.log("Unhandled command: " + command)
            }
            break
        default:
            console.log("Unhandled protocol: "+protocol+"\nPayload: "+uri.split(":")[1])
        }
    }
}
