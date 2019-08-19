#include "common/qabstracttdobject.h"
#include "qtdkeyboardbuttontype.h"

QTdKeyboardButtonType::QTdKeyboardButtonType(QObject *parent)
    : QTdObject(parent)
{
}

QTdKeyboardButtonTypeRequestLocation::QTdKeyboardButtonTypeRequestLocation(QObject *parent)
    : QTdKeyboardButtonType(parent)
{
}

QTdKeyboardButtonTypeRequestPhoneNumber::QTdKeyboardButtonTypeRequestPhoneNumber(QObject *parent)
    : QTdKeyboardButtonType(parent)
{
}

QTdKeyboardButtonTypeText::QTdKeyboardButtonTypeText(QObject *parent)
    : QTdKeyboardButtonType(parent)
{
}
