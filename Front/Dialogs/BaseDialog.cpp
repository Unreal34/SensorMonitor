#include "BaseDialog.hpp"
#include "Application.hpp"

BaseDialog::BaseDialog(bool *ok, const QString& validateLabel, const QString& cancelLabel, QWidget* parent) : QDialog(parent)
, mValidate(new QPushButton(QIcon("://Icons/Checked.png"), validateLabel, this))
, mCancel(new QPushButton(QIcon("://Icons/Cancel.png"), cancelLabel, this))
, mOk(ok)
{
    setWindowIcon(QIcon(APPLICATION_ICON));
    setWindowTitle(APPLICATION_NAME);

    resize(300, 200);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    setLayout(mainLayout);

    mContainLayout = new QVBoxLayout();
    mainLayout->addLayout(mContainLayout);
    mContainLayout->setContentsMargins(16, 16, 16, 16);

    mBottomLayout = new QHBoxLayout();
    mainLayout->addLayout(mBottomLayout);

    QWidget* spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    spacer->setFixedHeight(32);

    mBottomLayout->addWidget(spacer);
    mBottomLayout->addWidget(mCancel);
    mBottomLayout->addWidget(mValidate);
    mBottomLayout->setContentsMargins(16, 16, 16, 16);

    connect(mValidate, SIGNAL(clicked(bool)), this, SLOT(validate()));
    connect(mCancel, SIGNAL(clicked(bool)), this, SLOT(cancel()));

    if(mOk)
    {
        *mOk = false;
    }
}

QVBoxLayout *BaseDialog::containLayout() const
{
    return mContainLayout;
}

void BaseDialog::validate()
{
    if(mOk)
    {
        *mOk = true;
    }

    close();
}

void BaseDialog::cancel()
{
    close();
}

QHBoxLayout *BaseDialog::bottomLayout() const
{
    return mBottomLayout;
}
