#include "common/qabstracttdobject.h"
#include <QJsonValue>
#include "qtdreplymarkup.h"
#include "qtdkeyboardbutton.h"

QTdReplyMarkup::QTdReplyMarkup(QObject *parent) : QTdObject(parent)
{
}

QTdReplyMarkupForceReply::QTdReplyMarkupForceReply(QObject *parent) : QTdReplyMarkup(parent),
    m_isPersonal(false)
{
    setType(REPLY_MARKUP_FORCE_REPLY);
}

bool QTdReplyMarkupForceReply::isPersonal() const
{
    return m_isPersonal;
}

void QTdReplyMarkupForceReply::unmarshalJson(const QJsonObject &json)
{
    if (json.isEmpty()) {
        return;
    }
    m_isPersonal = json["is_personal"].toBool();
    emit isPersonalChanged();
}

QTdReplyMarkupInlineKeyboard::QTdReplyMarkupInlineKeyboard(QObject *parent) : QTdReplyMarkup(parent),
    m_rows(Q_NULLPTR)
{
    setType(REPLY_MARKUP_INLINE_KEYBOARD);
    m_rows = new QQmlObjectListModel<QTdKeyboardRow>(this);
}

QObject *QTdReplyMarkupInlineKeyboard::rows()
{
    return m_rows;
}

void QTdReplyMarkupInlineKeyboard::unmarshalJson(const QJsonObject &json)
{
    if (json.isEmpty()) {
        return;
    }
    const QJsonArray rows = json["rows"].toArray();
    for (const QJsonValue &item: rows) {
        QTdKeyboardRow *keyboardRow = new QTdKeyboardRow;
        keyboardRow->unmarshalJson(item);
        m_rows->append(keyboardRow);
    }
    emit rowsChanged();
}

QTdReplyMarkupRemoveKeyboard::QTdReplyMarkupRemoveKeyboard(QObject *parent) : QTdReplyMarkup(parent),
    m_isPersonal(false)
{
    setType(REPLY_MARKUP_REMOVE_KEYBOARD);
}

bool QTdReplyMarkupRemoveKeyboard::isPersonal() const
{
    return m_isPersonal;
}

void QTdReplyMarkupRemoveKeyboard::unmarshalJson(const QJsonObject &json)
{
    if (json.isEmpty()) {
        return;
    }
    m_isPersonal = json["is_personal"].toBool();
    emit isPersonalChanged();
}

QTdReplyMarkupShowKeyboard::QTdReplyMarkupShowKeyboard(QObject *parent) : QTdReplyMarkup(parent),
    m_rows(Q_NULLPTR), m_resizeKeyboard(false), m_oneTime(false), m_isPersonal(false)
{
    setType(REPLY_MARKUP_SHOW_KEYBOARD);
    m_rows = new QQmlObjectListModel<QTdKeyboardRow>(this);
}
QObject *QTdReplyMarkupShowKeyboard::rows() const
{
    return m_rows;
}

bool QTdReplyMarkupShowKeyboard::resizeKeyboard() const
{
    return m_resizeKeyboard;
}

bool QTdReplyMarkupShowKeyboard::oneTime() const
{
    return m_oneTime;
}

bool QTdReplyMarkupShowKeyboard::isPersonal() const
{
    return m_isPersonal;
}

void QTdReplyMarkupShowKeyboard::unmarshalJson(const QJsonObject &json)
{
    if (json.isEmpty()) {
        return;
    }
    const QJsonArray rows = json["rows"].toArray();
    for (const auto &item: rows) {
        QTdKeyboardRow *keyboardRow = new QTdKeyboardRow;
        keyboardRow->unmarshalJson(item);
        m_rows->append(keyboardRow);
    }
    m_resizeKeyboard = json["resize_keyboard"].toBool();
    m_oneTime = json["one_time"].toBool();
    m_isPersonal = json["is_personal"].toBool();
    emit markupChanged();
}
