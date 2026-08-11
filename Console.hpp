#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <QWidget>

#define CONSOLE_FLUSH_AFTER 10'000

class QTextEdit;

class Console : public QWidget
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
        Console(QWidget *parent = nullptr);

    public slots:
        void appendLog(const QString& message, Console::ELogType logType = Information);
        void clear();

    private:
        int mLinesCounter = 0;
        QTextEdit* mTextArea;
};

#endif // CONSOLE_HPP
