import QtQuick 2.4
import QuickFlux 1.1
import "./"

ActionCreator {
    signal setCurrentChat(var chat)
    signal closeCurrentChat()

    signal loadMoreMessages();

    signal sendMessage(string text);
    signal sendPhoto(string photoUrl, string text);
    signal sendDocument(string documentUrl, string text);
    signal sendChatAction();

    signal leaveChat(string chatId);
    signal deleteChatHistory(string chatId);

    signal deleteMessage(string id)
    signal forwardMessage(string id)
    signal requestEditMessage(var message)
    signal sendEditMessageText(string id, string text)
    signal sendEditMessageCaption(string id, string text)
    signal requestReplyToMessage(var message)
    signal sendReplyToMessage(string id, string text)
    signal showStickerPack(string stickerId)


    function viewGroupInfo(chat) {
        view.pushToStack("qrc:/pages/GroupDetailsPage.qml", {
                             chat: chat
                         })
    }
}
