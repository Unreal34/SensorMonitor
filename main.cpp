#include "MainWindow.hpp"
#include "TranslationManager.hpp"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    const QString locale = QLocale::system().name();
    TranslationManager trManager;
    trManager.setLanguage("en_US");

    MainWindow window;
    window.show();

    return app.exec();
}
