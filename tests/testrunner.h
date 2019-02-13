#ifndef TESTRUNNER_H
#define TESTRUNNER_H

#include <QObject>
#include <QCommandLineParser>
#include <QMetaObject>
#include <QPointer>
#include <QQmlEngine>
#include <QTest>
#include <QtQuickTest/quicktest.h>
#include <functional>


class TestRunner
{
public:
    explicit TestRunner();
    ~TestRunner();

    static TestRunner *instance();

    void add(QObject *testSuite);

    void add(const QString &qmlTestSuite);

    template <typename T>
    void add();

    bool exec(QStringList arguments);

    void addImportPath(const QString &path);

    int count() const;

    QStringList arguments() const;

    void setEngineHook(std::function<void(QQmlEngine*)> hook);

protected:
    virtual void execHook(QQmlEngine *engine);

private:
    void add(QVariant testSuite);

    bool run(QObject *test, const QStringList &args);

    bool run(const QString &testPath, const QStringList &arguments);

    QVariantList m_tests;
    QStringList m_importPaths;
    QStringList m_arguments;
    std::function<void(QQmlEngine*)> m_hook;

};

template <typename T>
void TestRunner::add()
{
    add(new T());
}

class HookRunner : public TestRunner
{
public:
    virtual void execHook(QQmlEngine *engine);
};

class TestWrapper : public QObject
{
  Q_OBJECT
public:
    explicit TestWrapper(QObject *parent = 0);

public slots:
    bool init() const;
};

#endif // TESTRUNNER_H
