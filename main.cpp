#include "MainWindow.hpp"
#include "TranslationManager.hpp"

#include <QApplication>
#include <QFile>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // app will uses english as main language.
    const QString locale = QLocale::system().name();
    TranslationManager trManager;
    trManager.setLanguage("en_EN");

    // used to set the application style.
    QFile styleFile("://Styles/Dark.qss");

    if(styleFile.open(QFile::ReadOnly))
    {
        app.setStyleSheet(styleFile.readAll());
    }

    MainWindow window;
    window.show();

    return app.exec();
}
