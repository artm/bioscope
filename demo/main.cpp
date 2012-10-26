#include "PlayerShell.hpp"
#include "BioscopeTestSuite.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QStringList args = QCoreApplication::arguments();
    bool testing = args.contains("--test");

    if (testing) {
        BioscopeTestSuite suite(args);
        return suite.runTests();
    } else {
        PlayerShell w;
        w.show();

        if (argc > 1) {
            w.open(argv[1]);
        }

        return a.exec();
    }
}
