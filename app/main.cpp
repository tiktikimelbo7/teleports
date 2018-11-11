#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QJsonObject>
#include <QDebug>
#include <QQmlContext>
#include <QuickFlux>
#include <qtdlib/quick/plugin.h>
#include <QtQuickControls2/QQuickStyle>
#include "messagedelegatemap.h"
#include <libintl.h>
#include <locale.h>
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

    bindtextdomain ("telegram-plus.dpniel", "/opt/click.ubuntu.com/telegram-plus.dpniel/current/share/locale");
    bind_textdomain_codeset ("telegram-plus.dpniel", "UTF-8");
    textdomain ("telegram-plus.dpniel");

    MessageDelegateMap delegateMap;

    QQmlApplicationEngine engine;

    engine.addImportPath(QStringLiteral("qrc:/plugins"));
    engine.rootContext()->setContextProperty("applicationDirPath", QGuiApplication::applicationDirPath());
    engine.rootContext()->setContextProperty(QStringLiteral("delegateMap"), &delegateMap);

    engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));

    return app.exec();
}
