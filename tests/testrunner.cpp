#include "testrunner.h"


static TestRunner *s_instance = 0;

TestRunner::TestRunner() {
    if (s_instance == 0) {
        s_instance = this;
    }
    m_hook = 0;
}

TestRunner::~TestRunner() {
    for (auto test : m_tests) {
        auto *t = test.value<QObject*>();
        if (t) {
            delete t;
        }
    }
    m_tests.clear();
}

TestRunner *TestRunner::instance() {
    return s_instance;
}

void TestRunner::add(QObject *testSuite) {
    m_tests.append(QVariant::fromValue<QObject*>(testSuite));
}

void TestRunner::add(const QString &qmlTestSuite) {
    add(QVariant(qmlTestSuite));
}

bool TestRunner::exec(QStringList arguments) {
    m_arguments = arguments;
    bool err = false;
    for (auto testObj : m_tests) {
        QObject *test = testObj.value<QObject*>();
        if (test) {
            err |= run(test, m_arguments);
        } else if (testObj.type() == (int)QMetaType::QString) {
            err |= run(testObj.toString(), m_arguments);
        }
    }
    return err;
}

void TestRunner::addImportPath(const QString &path) {
    m_importPaths << path;
}

int TestRunner::count() const {
    return m_tests.count();
}

QStringList TestRunner::arguments() const {
    return m_arguments;
}

void TestRunner::setEngineHook(std::function<void (QQmlEngine *)> hook) {
    m_hook = hook;
}

void TestRunner::execHook(QQmlEngine *engine) {
    if (m_hook) {
        m_hook(engine);
    }
}

void TestRunner::add(QVariant testSuite) {
    m_tests << testSuite;
}

bool TestRunner::run(QObject *test, const QStringList &args) {
    return true;
}

bool TestRunner::run(const QString &testPath, const QStringList &arguments) {
    QStringList args(arguments);
    QString executable = args.takeAt(0);
    QStringList testcases = arguments.filter(QRegExp("::"));

    QStringList nonOptionArgs; // Filter all "-" parameter
    for (QString arg : args) {
        if (arg.indexOf("-") != 0) {
            nonOptionArgs << arg;
        }
    }

    if (nonOptionArgs.size() !=0 && testcases.size() == 0) {
        // If you non-option args, but not a quick test case. Return
        return false;
    }

    QStringList paths;
    paths << testPath;
    paths << m_importPaths;

    char **s = (char**) malloc(sizeof(char*) * (10 + args.size() + paths.size() * 2));
    int idx = 0;
    s[idx++] = executable.toUtf8().data();

    for(QString p : paths) {
        s[idx++] = strdup("-import");
        s[idx++] = strdup(p.toUtf8().data());
    }

    for(QString arg : args) {
        s[idx++] = strdup(arg.toUtf8().data());
    }
    s[idx++] = 0;

    const char *name = "TeleportsQuickTests";
    const char *source = strdup(testPath.toUtf8().data());


    bool error = quick_test_main( idx-1, s, name, source);
    free(s);

    return error;
}

void HookRunner::execHook(QQmlEngine *engine) {
    TestRunner::execHook(engine);
}

TestWrapper::TestWrapper(QObject *parent) : QObject(parent) {}

bool TestWrapper::init() const {
    return true;
}

QObject *testRunnerProvider(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(scriptEngine);

    auto *runner = TestRunner::instance();
    if (runner) {
        HookRunner *hook = static_cast<HookRunner*>(runner);
        hook->execHook(engine);
    }
    TestWrapper *obj = new TestWrapper;
    return obj;
}

static void registerTestRunnerTypes() {
    qmlRegisterSingletonType<TestWrapper>("Teleports.Testing", 1, 0, "TestRunner", testRunnerProvider);
}

Q_COREAPP_STARTUP_FUNCTION(registerTestRunnerTypes)
