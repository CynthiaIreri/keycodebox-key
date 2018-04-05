#include "dlgfingerprintverify.h"
#include "ui_dlgfingerprintverify.h"

#include <QDebug>

#include "dlgfullkeyboard.h"
#include "kcbcommon.h"

CDlgFingerprintVerify::CDlgFingerprintVerify(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDlgFingerprintVerify)
{
    ui->setupUi(this);
    //  CDlgFingerprintVerify::showFullScreen();
    setAttribute(Qt::WA_AcceptTouchEvents);
}

CDlgFingerprintVerify::~CDlgFingerprintVerify()
{
}

void CDlgFingerprintVerify::shutdown()
{
}

//void CDlgFingerprintVerify::on_buttonBoxCancel_clicked()
//{
//    KCB_DEBUG_ENTRY;
//    emit __onVerifyFingerprintDialogCancel();
//    KCB_DEBUG_EXIT;
//}

void CDlgFingerprintVerify::on_bbClose_clicked()
{
    KCB_DEBUG_ENTRY;
    emit __onVerifyFingerprintDialogCancel();
    KCB_DEBUG_EXIT;
}

void CDlgFingerprintVerify::setMessage(QString message)
{
  ui->lblFPrintMessage2->setText(message);
}

void CDlgFingerprintVerify::OnUpdateVerifyFingerprintDialog(bool result, QString message)
{
    KCB_DEBUG_ENTRY;
    qDebug() << result;
    qDebug() << message;
    setMessage(message);
    KCB_DEBUG_EXIT;
}
