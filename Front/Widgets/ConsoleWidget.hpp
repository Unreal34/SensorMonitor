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
    void appendLog(const QString& message, ConsoleWidget::ELogType logType = Information);
    void clear();

private:
    int mLinesCounter = 0;
    QTextEdit* mTextArea;
};

#endif // CONSOLEWIDGET_H
