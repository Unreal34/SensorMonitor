#include "ApplicationLogger.hpp"

#include <QFile>
#include <QCoreApplication>
#include <QDateTime>
#include <QMetaEnum>

ApplicationLogger::ApplicationLogger(QObject *parent) : QObject{parent}
{}

void ApplicationLogger::log(const QString &message, ELogType type, const QString& logFile)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<ELogType>();

    QFile file(QCoreApplication::applicationDirPath() + "/" + logFile);

    if (file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QTextStream out(&file);
        QString log = QString("- [%1]").arg(QString::fromUtf8(metaEnum.valueToKey(type)).toUpper()) + " [" + QDateTime::currentDateTime().toString() + "] " + message + "\n";
        out << log;
    }

    file.close();
}
