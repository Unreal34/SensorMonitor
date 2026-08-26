#include "TranslationManager.hpp"
#include <QApplication>

TranslationManager::TranslationManager(QObject* parent) : QObject(parent)
{
}

bool TranslationManager::setLanguage(const QString& locale)
{
    qApp->removeTranslator(&mTranslator);

    // English is the source language.
    if(locale == "en_US")
    {
        return true;
    }

    const QString translationPath = QString(":/i18n/SensorMonitor_%1.qm").arg(locale);

    if(!mTranslator.load(translationPath))
    {
        return false;
    }

    qApp->installTranslator(&mTranslator);

    return true;
}
