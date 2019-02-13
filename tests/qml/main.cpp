#include <QtCore>
#include <QDebug>
#include <QGuiApplication>
#include <QQmlEngine>
#include <QQmlContext>
#include <QuickFlux>
#include <qtdlib/quick/plugin.h>
#include "testrunner.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc,argv);

    registerQuickFluxQmlTypes();
    QTdLib::registerQmlTypes();

    TestRunner runner;

//    // Run Qt Quick Test in SRCDIR. It will scan all qml file begin with tst_
    const QString testSuiteBase = QString(SRCDIR);
    qDebug() << "testSuiteBase" << testSuiteBase;
    runner.add(QString("%1/middleware").arg(testSuiteBase));

    runner.addImportPath("qrc:///");
    runner.addImportPath("qrc:/");
    runner.setEngineHook([](QQmlEngine* engine) {
        Q_UNUSED(engine);
    });

    bool error = runner.exec(app.arguments());

    if (!error) {
        qWarning() << "All test cases passed!";
    }

    return error;
}
