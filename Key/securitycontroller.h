#ifndef CSECURITYCONTROLLER_H
#define CSECURITYCONTROLLER_H

#include <QObject>
#include <QThread>
#include "securityadmin.h"
#include "securityuser.h"
#include "modelsecurity.h"


class CSecurityController : public QObject
{
    Q_OBJECT
public:
    explicit CSecurityController(QObject *parent = 0);

    void CheckAccessCodeOne(QString code1);
    void CheckAccessCodeTwo(QString code2); // User code #2 or Admin pw

    void CheckFingerprintAccessCodeOne(QString code1);
    void CheckFingerprintAccessCodeTwo(QString code2); // User code #2 or Admin pw

    void CheckAdminPassword(QString sPW);

    void initializeSignals();

    static QDateTime &roundDateTime(uint32_t res, QDateTime &datetime);

    void getAllCodes1(QStringList& codes1);
private:
    CSecurityUser       *_pSecurityUser;      // Default no access
    CModelSecurity      _modelSecurity;
    QString             _sCode1, _sCode2, _sAdminPW;
    QThread             _securityControlThread;
    CAdminRec           *_pAdminRec = 0;

public:
    QString             getLastCodeOne() { return _sCode1; }
    QString             getLastCodeTwo() { return _sCode2; }

signals:
    void    __OnSecurityCheckPassUser(CSecurityUser *pUser);
    void    __OnSecurityCheckPassAdmin(CSecurityUser *pUser);
    void    __OnSecurityCheckFail(QString code1, QString code2);

    // These are to signal upstream to the systemcontroller
    void    __OnRequireAdminPassword();
    void    __OnRequireCodeTwo();
    void    __OnAdminSecurityCheckOk(QString type);
    void    __OnAdminSecurityCheckFailed();
    void __OnSecurityCheckSuccess(QString);

    void    __OnSecurityCheckSuccessWithAnswers(QString lockNums, QString answer1, QString answer2, QString answer3);
    
    void    __OnSecurityCheckedFailed();
    void    __OnSecurityCheckTimedOut();

    // These are to connect downstream to the modelsecurity
    void    __VerifyCodeOne(QString code);
    void    __VerifyCodeTwo(QString code);

        // These are to connect downstream to the modelsecurity
    void    __VerifyFingerprintCodeOne(QString code);
    void    __VerifyFingerprintCodeTwo(QString code);

    void    __VerifyAdminPassword(QString sPW);

    void    __OnRequestCurrentAdmin();
    void    __OnRequestedCurrentAdmin(CAdminRec*);

    void    __UpdateCurrentAdmin(CAdminRec *adminInfo);
    void    __OnUpdatedCurrentAdmin(bool bSuccess);
    void    __OnCreateHistoryRecordFromLastSuccessfulLogin();

    void    __OnCreateHistoryRecordFromLastSuccessfulLoginWithAnswers(QString answer1, QString answer2, QString answer3);

    void    __OnUpdateCodeState(CLockState *rec);
    void    __OnUpdatedCodeState(bool bSuccess);
    
signals:
    void __OnReadLockSet(QString nLockNum, QDateTime start, QDateTime end);
    void __OnLockSet(CLockSet *pLockSet);
    
    void __TrigEnrollFingerprint(QString sCode);
    void __TrigEnrollFingerprintDialog(QString);
    
    void __TrigQuestionUserDialog(QString,QString,QString,QString);
    void __TrigQuestionUser(QString,QString,QString,QString);

    void __OnSuccessfulQuestionUsersAnswers(QString lockNum, QString answer1, QString answer2, QString answer3);
    void __OnQuestionUserCancelled();
    public slots:
  
  void OnEnrollFingerprint(QString sCode) { emit __TrigEnrollFingerprint(sCode); }
  void OnEnrollFingerprintDialog(QString sCode) { emit __TrigEnrollFingerprintDialog(sCode); }
  
  void OnQuestionUserDialog(QString lockNum, QString question1, QString question2, QString question3)
  {
    emit __TrigQuestionUser(lockNum,question1,question2,question3);
    emit __TrigQuestionUserDialog(lockNum,question1,question2,question3);
  }

  void OnQuestionUserAnswers(QString lockNum, QString answer1, QString answer2, QString answer3)
  {
    emit __OnSuccessfulQuestionUsersAnswers(lockNum,answer1,answer2,answer3);
  }

  void OnQuestionUserCancel()
  {
    emit __OnQuestionUserCancelled();
  }
  
  void OnReadLockSet(QString nLockNum, QDateTime start, QDateTime end) { 
      qDebug() << "SLOT: SecurityController -> OnReadLockSet";
      emit __OnReadLockSet(nLockNum, start, end); 
  }
  void OnLockSet(CLockSet *pSet) { 
      qDebug() << "SLOT: SecurityController -> OnLockSet";
      emit __OnLockSet(pSet); 
  }

signals:
    void __OnReadLockHistorySet(QString nLockNum, QDateTime start, QDateTime end);
    void __OnLockHistorySet(CLockHistorySet *pLockSet);
public slots:
    void OnReadLockHistorySet(QString nLockNum, QDateTime start, QDateTime end) { emit __OnReadLockHistorySet(nLockNum, start, end); }
    void OnLockHistorySet(CLockHistorySet *pSet) { emit __OnLockHistorySet(pSet); }

signals:
    void __RequestLastSuccessfulLogin(QString locknums, QString answer1, QString answer2, QString answer3);
    void __OnLastSuccessfulLogin(CLockHistoryRec *);
public slots:
    void RequestLastSuccessfulLogin(QString locknums);
    void RequestLastSuccessfulLoginWithAnswers(QString locknums, QString answer1, QString answer2, QString answer3);
    void OnLastSuccessfulLoginRequest(CLockHistoryRec *pLockHistory);

signals:
    void __RequestCodeHistoryForDateRange(QDateTime dtStart, QDateTime dtEnd);
    void __OnCodeHistoryForDateRange(CLockHistorySet *pLockHistorySet);
public slots:
    void OnRequestCodeHistoryForDateRange(QDateTime dtStart, QDateTime dtEnd);
    void OnCodeHistoryForDateRange(CLockHistorySet *pLockHistorySet);

public slots:
    void    OnUpdateCurrentAdmin(CAdminRec *adminInfo) {
        qDebug() << "CSecurityController::OnUpdateCurrentAdmin(adminInfo)";
        _pAdminRec = adminInfo;
        emit __UpdateCurrentAdmin(adminInfo);
    }
    void    OnUpdatedCurrentAdmin(bool bSuccess) { emit __OnUpdatedCurrentAdmin(bSuccess); }

    void    OnUpdateCodeState(CLockState *rec);
    void    OnUpdatedCodeState(bool bSuccess) { emit __OnUpdatedCodeState(bSuccess); }

public slots:
//    void OnSecurityChecked(bool bIsOk, int accessLevel, QString type);

    // CModelSecurity signals
    void OnRequireAdminPassword();
    void OnRequireCodeTwo();
    void OnAdminSecurityCheckOk(QString type);
    void OnAdminSecurityCheckFailed();
    void OnSecurityCheckSuccess(QString lockNums);
    void OnSecurityCheckSuccessWithAnswers(QString lockNums, QString answer1, QString answer2, QString answer3);
    void OnSecurityCheckedFailed();
    void OnSecurityCheckTimedOut();

    void OnRequestCurrentAdmin();
    void OnRequestedCurrentAdmin(CAdminRec*);

    void CheckPredictiveAccessCode(QString code);

    std::string GetPredictiveAccessCode(QString code, int nLockNum);

private slots:
};

#endif // CSECURITYCONTROLLER_H
