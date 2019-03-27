#include "common/qabstracttdobject.h"
#include "qtdkeyboardbutton.h"

QTdKeyboardButton::QTdKeyboardButton(QObject *parent) : QTdObject(parent),
    m_type(Q_NULLPTR)
{
}

QString QTdKeyboardButton::text() const
{
    return m_text;
}

void QTdKeyboardButton::unmarshalJson(const QJsonObject &json)
{
    m_text = json["text"].toString();

    if (m_type) {
        delete m_type;
        m_type = nullptr;
    }
    const QJsonObject typeObj = json["type"].toObject();
    const QString type = typeObj["@type"].toString();
    if (type == "keyboardButtonTypeRequestLocation") {
        m_type = new QTdKeyboardButtonTypeRequestLocation(this);
    } else if (type == "keyboardButtonTypeRequestPhoneNumber") {
        m_type = new QTdKeyboardButtonTypeRequestPhoneNumber(this);
    } else if (type == "keyboardButtonTypeText") {
        m_type = new QTdKeyboardButtonTypeText(this);
    }
}

QTdKeyboardButtonType *QTdKeyboardButton::type() const
{
    return m_type;
}

QTdKeyboardRow::QTdKeyboardRow(QObject *parent) : QTdObject(parent),
    m_row(Q_NULLPTR)
{
    m_row = new QQmlObjectListModel<QTdKeyboardButton>(this);
}

QObject *QTdKeyboardRow::row() const
{
    return m_row;
}

void QTdKeyboardRow::unmarshalJson(const QJsonValue &json)
{
    const QJsonArray row = json.toArray();
    for (const QJsonValue &v: row) {
        QTdKeyboardButton *button = new QTdKeyboardButton;
        button->unmarshalJson(v.toObject());
        m_row->append(button);
    }
}
