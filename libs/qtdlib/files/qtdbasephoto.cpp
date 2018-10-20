#include "qtdbasephoto.h"
#include "qtdlocalfile.h"
#include "qtdremotefile.h"

QTdBasePhoto::QTdBasePhoto(QObject *parent) : QTdObject(parent),
    m_small(new QTdFile), m_big(new QTdFile)
{
}

QTdFile *QTdBasePhoto::small() const
{
    return m_small.data();
}

QTdFile *QTdBasePhoto::big() const
{
    return m_big.data();
}

void QTdBasePhoto::unmarshalJson(const QJsonObject &json)
{
    m_small->unmarshalJson(json["small"].toObject());
    emit smallChanged(m_small.data());
    m_big->unmarshalJson(json["big"].toObject());
    emit bigChanged(m_big.data());
}
