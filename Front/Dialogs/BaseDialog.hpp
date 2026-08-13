#ifndef BASEDIALOG_HPP
#define BASEDIALOG_HPP

#include <QDialog>
#include <QObject>
#include <QPushButton>
#include <QVBoxLayout>

/**
 * @brief Base class for all dialogs in the application.
 * BaseDialog provides a common footer with standard dialog buttons,
 * such as Validate and Cancel.
 */
class BaseDialog : public QDialog
{
    Q_OBJECT
public:
    explicit BaseDialog(bool *ok = nullptr, const QString& validateLabel = tr("Validate"), const QString& cancelLabel = tr("Cancel"), QWidget* parent = nullptr);

public:
    QVBoxLayout* containLayout() const;
    QHBoxLayout* bottomLayout() const;

protected slots:
    virtual void validate();
    virtual void cancel();

protected:
    QPushButton* mValidate = nullptr;
    QPushButton* mCancel = nullptr;
    QVBoxLayout* mContainLayout = nullptr;
    QHBoxLayout* mBottomLayout = nullptr;
    bool* mOk;
};

#endif // BASEDIALOG_HPP
