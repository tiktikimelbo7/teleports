#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QJsonObject>
#include <QDebug>
#include <QQmlContext>
#include <QuickFlux>
#include <qtdlib/quick/plugin.h>
#include <QtQuickControls2/QQuickStyle>
#include "messagedelegatemap.h"
#include "messagecontentdelegatemap.h"
#include <libintl.h>
#include <locale.h>
#include <utils/i18n.h>

#define QUICK_FLUX_DISABLE_AUTO_QML_REGISTER

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Suru");

    QCoreApplication::setApplicationName(QStringLiteral("teleports.ubports"));
    QCoreApplication::setOrganizationName(QStringLiteral("teleports.ubports"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("teleports.ubports"));
    QCoreApplication::setApplicationVersion(QStringLiteral("0.4.0"));

    registerQuickFluxQmlTypes();
    QTdLib::registerQmlTypes();

    setlocale(LC_ALL, "");
    bindtextdomain("teleports.ubports", "/opt/click.ubuntu.com/teleports.ubports/current/share/locale");
    bind_textdomain_codeset("teleports.ubports", "UTF-8");
    textdomain(GETTEXT_DOMAIN.toStdString().c_str());

    MessageDelegateMap delegateMap;
    MessageContentDelegateMap contentDelegateMap;

    QQmlApplicationEngine engine;

    engine.addImportPath(QStringLiteral("qrc:/plugins"));
    engine.rootContext()->setContextProperty("applicationDirPath", QGuiApplication::applicationDirPath());
    engine.rootContext()->setContextProperty(QStringLiteral("delegateMap"), &delegateMap);
    engine.rootContext()->setContextProperty(QStringLiteral("contentDelegateMap"), &contentDelegateMap);

    engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));

    return app.exec();
}
