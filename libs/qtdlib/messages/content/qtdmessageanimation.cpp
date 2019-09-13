#include "qtdmessageanimation.h"

QTdMessageAnimation::QTdMessageAnimation(QObject *parent)
    : QTdMessageContent(parent)
    , m_animation(new QTdAnimation)
    , m_caption(new QTdFormattedText)
{
    setType(MESSAGE_ANIMATION);
}

QTdAnimation *QTdMessageAnimation::animation() const
{
    return m_animation.data();
}
QTdFormattedText *QTdMessageAnimation::caption() const
{
    return m_caption.data();
}
void QTdMessageAnimation::unmarshalJson(const QJsonObject &json)
{
    m_animation->unmarshalJson(json["animation"].toObject());
    m_caption->unmarshalJson(json["caption"].toObject());
}
