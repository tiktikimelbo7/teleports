#include <QtCore>
#include <QDebug>
#include <QGuiApplication>
#include <QQmlEngine>
#include <QQmlContext>
#include <QuickFlux>
#include <qtdlib/quick/plugin.h>
#include "testrunner.h"
#include "testutils.h"
#include "clientwrapper.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc,argv);

    registerQuickFluxQmlTypes();
    QTdLib::registerQmlTypes();

    TestRunner runner;

    ClientWrapper client;
    client.init();

    QTdClient::setInstance(&client);


//    // Run Qt Quick Test in SRCDIR. It will scan all qml file begin with tst_
    const QString testSuiteBase = QString(SRCDIR);
    qDebug() << "testSuiteBase" << testSuiteBase;
    runner.add(QString("%1/actions").arg(testSuiteBase));
    runner.add(QString("%1/middleware").arg(testSuiteBase));

    runner.addImportPath("qrc:///");
    runner.addImportPath("qrc:/");
    runner.setEngineHook([](QQmlEngine* engine) {
        TestUtils *utils = new TestUtils(engine);
        engine->rootContext()->setContextProperty("testutils", utils);
    });

    bool error = runner.exec(app.arguments());

    if (!error) {
        qWarning() << "All test cases passed!";
    }

    return error;
}
