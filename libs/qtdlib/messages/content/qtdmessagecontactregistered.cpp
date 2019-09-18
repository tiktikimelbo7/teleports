#include "qtdmessagecontactregistered.h"
#include "utils/await.h"

QTdMessageContactRegistered::QTdMessageContactRegistered(QObject *parent)
    : QTdMessageContent(parent)
{
    setType(MESSAGE_CONTACT_REGISTERED);
}

