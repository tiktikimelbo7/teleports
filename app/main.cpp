#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QJsonObject>
#include <QDebug>
#include <QQmlContext>
#include <QuickFlux>
#include <qtdlib/quick/plugin.h>
#include <QtQuickControls2/QQuickStyle>
#include "messagedelegatemap.h"
#define QUICK_FLUX_DISABLE_AUTO_QML_REGISTER

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Suru");

    QCoreApplication::setApplicationName(QStringLiteral("telegram-plus.dpniel"));
    QCoreApplication::setApplicationVersion(QStringLiteral("0.0.1"));

    registerQuickFluxQmlTypes();
    QTdLib::registerQmlTypes();

    MessageDelegateMap delegateMap;

    QQmlApplicationEngine engine;
    engine.addImportPath(QStringLiteral("qrc:/imports"));
    engine.rootContext()->setContextProperty(QStringLiteral("delegateMap"), &delegateMap);

    engine.load("qrc:/Main.qml");

    return app.exec();
}
