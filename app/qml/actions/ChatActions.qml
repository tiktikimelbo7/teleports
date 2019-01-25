import QtQuick 2.4
import QuickFlux 1.1
import "./"

ActionCreator {
    signal setCurrentChat(var chat)
    signal closeCurrentChat()

    signal loadMoreMessages();
    signal sendMessage(string text);
    signal sendChatAction();

    signal leaveChat(var chatId);
    signal deleteChatHistory(var chatId);

    signal viewInDetail(var chat)
    signal leaveGroupDetails();

    signal deleteMessage(string id)
    signal forwardMessage(string id)
    signal editMessage(string id)
    signal replyToMessage(string id)
    signal showStickerPack(string stickerId)
}
