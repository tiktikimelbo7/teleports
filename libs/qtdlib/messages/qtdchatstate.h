#ifndef QTDCHATSTATE
#define QTDCHATSTATE

#include <QObject>

class QTdChatState
{
    Q_GADGET
public:
    enum ChatState {
        Default,
        EditingMessage,
        ReplyingToMessage,
    };

    Q_ENUMS(ChatState)
private:
    explicit QTdChatState();
};

#endif // QTDCHATSTATE
