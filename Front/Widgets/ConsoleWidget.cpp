#include "ConsoleWidget.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QIcon>
#include <QTextEdit>
#include <QDateTime>
#include <QFile>
#include <QCoreApplication>

ConsoleWidget::ConsoleWidget(QWidget *parent) : QWidget(parent)
, mTextArea(new QTextEdit(this))
{
    QHBoxLayout* hbox = new QHBoxLayout(this);
    hbox->addWidget(mTextArea);

    setLayout(hbox);

    mTextArea->setReadOnly(true);
}

void ConsoleWidget::appendLog(const QString &message, ELogType logType)
{
    QString type;

    switch (logType) {
    case ELogType::Error:
        type = "<span style='color:#ff0000'>(Error)</span>";
        break;

    case ELogType::Success:
        type = "<span style='color:#00ff00'>(Success)</span>";
        break;

    case ELogType::Warning:
        type = "<span style='color:#ff9933'>(Warning)</span>";
        break;

    case ELogType::Information:
        type = "<span style='color:#00ffff'>(Info)</span>";
        break;

    case ELogType::Debug:
        type = "<span style='color:#e933ff'>(Debug)</span>";
        break;

    default:
        type = "<span style='color:#ff0000'>(Invalid Log Type)</span>";
        break;
    }

    QString log = "[" + QDateTime::currentDateTime().toString() + "] " + type + " - " + message;

    mTextArea->append(log);
    mLinesCounter++;

    if(mLinesCounter > CONSOLE_FLUSH_AFTER)
    {
        mLinesCounter = 0;
        mTextArea->clear();
    }

    QFile logFile(QCoreApplication::applicationDirPath() + "/Log_Session.txt");

    if (logFile.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QTextStream out(&logFile);
        QString log = "[" + QDateTime::currentDateTime().toString() + "]" + " - " + message + "\n";
        out << log;
    }
}

void ConsoleWidget::clear()
{
    mTextArea->setPlainText("");
}
