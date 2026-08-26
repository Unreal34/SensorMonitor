#ifndef TRANSLATIONMANAGER_HPP
#define TRANSLATIONMANAGER_HPP

#include <QObject>
#include <qtranslator.h>

class TranslationManager : public QObject
{
    Q_OBJECT
public:
    explicit TranslationManager(QObject* parent = nullptr);

public:
    bool setLanguage(const QString& locale);

private:
    QTranslator mTranslator;
};

#endif // TRANSLATIONMANAGER_HPP
