import QtQuick 2.4
import QuickFlux 1.1
import "./"

ActionCreator {
    signal setCurrentChat(var chat)
    signal createOrOpenPrivateChat(var user)
    signal createOrOpenSecretChat(var user)
    signal createOrOpenSavedMessages()
    signal setCurrentChatById(string chatId)
    signal setCurrentChatByUsername(string username)
    signal closeCurrentChat()

    signal loadNewerMessages();
    signal loadOlderMessages();

    signal sendMessage(string text);
    signal sendPhoto(string photoUrl, string text);
    signal sendVideo(string videoUrl, string text);
    signal sendAudio(string audioUrl, string text);
    signal sendContact(string contactUrl, string text);
    signal sendDocument(string documentUrl, string text);
    signal sendSticker(var sticker, string replyId);
    signal sendVoiceNote(string filename);
    signal sendLocation(double latitude, double longitude, int livePeriod);
    signal requestLocation();
    signal cancelLocation();
    signal stopWaitLocation();
    signal sendChatAction();

    signal leaveChat(string chatId);
    signal deleteChatHistory(string chatId);

    signal deleteMessage(string id)
    signal forwardMessage(string id)
    signal sendForwardMessage(var chat, string text)
    signal importFromContentHub(var contentType, var filePaths)
    signal sendImportData(var chat, string text)
    signal cancelForwardMessage()
    signal requestEditMessage(var message)
    signal sendEditMessageText(string id, string text)
    signal sendEditMessageCaption(string id, string text)
    signal requestReplyToMessage(var message)
    signal sendReplyToMessage(string id, string text)
    signal showStickerPack(string stickerId)
    signal setChatDraftMessage(string draftText, string replyToMessageId)

    signal muteChat(var chat, int duration)
    signal registerVoiceNote(string filename)
    signal stopVoiceNote()
    signal deleteVoiceNote(string filename)
    signal joinChat()

    signal jumpToMessage(string id)

    function viewGroupInfo(chat) {
        view.pushToStack("qrc:/pages/ChatInfoPage.qml", {
                             chat: chat,
                             isGroup: true
                         })
    }
}
