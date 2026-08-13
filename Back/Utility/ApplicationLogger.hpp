#ifndef APPLICATIONLOGGER_HPP
#define APPLICATIONLOGGER_HPP

#include <QObject>

/**
 * @brief Static class used to append log to a file located next to the executable.
 */
class ApplicationLogger : public QObject
{
    Q_OBJECT

public:
    enum ELogType
    {
        Success,
        Warning,
        Error,
        Information,
        Debug,
    };
    Q_ENUM(ELogType)

public:
    explicit ApplicationLogger(QObject *parent = nullptr);

public:
    static void log(const QString& message, ELogType type = ELogType::Information, const QString& logFile = "Logs.txt");
};

#endif // APPLICATIONLOGGER_HPP
