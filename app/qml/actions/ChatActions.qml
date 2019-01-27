import QtQuick 2.4
import QuickFlux 1.1
import "./"

ActionCreator {
    signal setCurrentChat(var chat)
    signal closeCurrentChat()

    signal loadMoreMessages();
    signal sendMessage(string text);
    signal sendChatAction();

    signal leaveChat(string chatId);
    signal deleteChatHistory(string chatId);

    signal viewInDetail(var chat)
    signal leaveGroupDetails();

    signal deleteMessage(string id)
    signal forwardMessage(string id)
    signal requestEditMessage(string id, string text)
    signal sendEditMessage(string id, string text)
    signal replyToMessage(string id)
    signal showStickerPack(string stickerId)
}
