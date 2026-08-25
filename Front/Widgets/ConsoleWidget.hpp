#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H

#include <QWidget>

#define CONSOLE_FLUSH_AFTER 10'000

class QTextEdit;

class ConsoleWidget : public QWidget
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
    ConsoleWidget(QWidget *parent = nullptr);

public slots:

    /**
     * @brief Append a new log to console.
     * @note The log type must be selected from the input enumerator.
     * @param message
     * @param logType
     */
    void appendLog(const QString& message, ConsoleWidget::ELogType logType = Information);

    /**
    * @brief Clears all logs currently displayed in the console.
    */
    void clear();

private:
    int mLinesCounter = 0;
    QTextEdit* mTextArea;
};

#endif // CONSOLEWIDGET_H
