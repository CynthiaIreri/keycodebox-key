#ifndef FRMUSERCODE_H
#define FRMUSERCODE_H

#include <QDialog>
#include <QTimer>
#include <QString>
#include <QMouseEvent>

#include "hidreader.h"
#include "magtekcardreader.h"
#include "dlgfingerprint.h"

namespace Ui {
class CFrmUserCode;
}

#define MAX_CODE_SIZE   15

class CFrmUserCode : public QDialog
{
    Q_OBJECT

    public:
        explicit CFrmUserCode(QWidget *parent = 0);
        ~CFrmUserCode();

        void SetDisplayFingerprintButton(bool state);
        void SetDisplayShowHideButton(bool state);
        void SetDisplayCodeEntryControls(bool state);
        void SetDisplayTakeReturnButtons(bool state);

        void show();

    private:
        Ui::CFrmUserCode *ui;

        QTimer  _dtTimer;

        bool m_fp_state;
        bool m_showhide_state;
        bool m_takereturn_state;


        void onButtonClick(char key);
        void onCodeEntered();   // enter key pressed
        void onBackSpace();

        void enableKeypad(bool bEnable);

        void initialize();

        void mousePressEvent(QMouseEvent* event);
        void EnterAdminControl();

    signals:
        void __KeyPressed(char key);
        void __CodeEntered(QString sCode);
        void __FingerprintCodeEntered(QString sCode);
        void __OnUserCodeCancel();

        void __onVerifyFingerprint();
        void __onVerifyFingerprintDialog();
        void __OnDisplayFingerprintButton(bool showFingerprint);
        void __OnDisplayShowHideButton(bool showShowHide);
        void __OnDisplayTakeReturnButtons(bool showTakeReturn);

    public slots:
        void OnEnableKeyboard(bool bEnable);
        void OnNewCodeMessage(QString sCodeMsg);
        void OnClearCodeDisplay();

        void OnSwipeCode(QString sCode);
        void OnDisplayFingerprintButton(bool state);
        void OnDisplayShowHideButton(bool state);
        void OnDisplayTakeReturnButtons(bool state);


    private slots:
        void on_btn_1_clicked();
        void on_btn_2_clicked();
        void on_btn_8_clicked();
        void on_btn_3_clicked();
        void on_btn_4_clicked();
        void on_btn_5_clicked();
        void on_btn_6_clicked();
        void on_btn_7_clicked();
        void on_btn_9_clicked();
        void on_btn_0_clicked();
        void on_btn_Return_clicked();
        void on_btn_Del_clicked();
        void on_btn_Clear_clicked();
        void on_btnShowHideCode_clicked(bool checked);
        void on_btn_Cancel_clicked();
        void on_btnIdentifyFingerPrint_clicked();
        void OnDateTimeTimerTimeout();
        void on_pbTake_clicked();
        void on_pbReturn_clicked();
};

#endif // FRMUSERCODE_H
