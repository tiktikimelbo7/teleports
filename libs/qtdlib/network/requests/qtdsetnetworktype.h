#ifndef QTDSETNETWORKTYPEREQUEST_H
#define QTDSETNETWORKTYPEREQUEST_H

#include <QObject>
#include "common/qtdrequest.h"
#include "network/qtdnetworktype.h"

class QTdSetNetworkTypeRequest : public QTdRequest
{
    Q_OBJECT
public:
  explicit QTdSetNetworkTypeRequest(QObject *parent = nullptr);

  void setNetworkType(QTdNetworkType *networkType);

  QJsonObject marshalJson() Q_DECL_FINAL;

private:
  Q_DISABLE_COPY(QTdSetNetworkTypeRequest)
  QTdNetworkType *m_networkType;
};

#endif // QTDSETNETWORKTYPEREQUEST_H
