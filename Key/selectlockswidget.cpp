#include "selectlockswidget.h"
#include "ui_selectlockswidget.h"
#include <QDebug>
#include <QList>
#include <QAbstractButton>
#include <QSignalMapper>
#include <QVector>
#include "lockcabinetwidget.h"
#include "kcbutils.h"
#include "kcbcommon.h"

SelectLocksWidget::SelectLocksWidget(QWidget *parent, Role role) :
    QWidget(parent),
    m_role(role),
    m_cancel_open(false),
    m_lock_cab(* new LockCabinetWidget(this)),
    m_lock_list{},
    ui(new Ui::SelectLocksWidget)
{
    ui->setupUi(this);

    ui->vloLockCabinet->addWidget(&m_lock_cab);

    connect(&m_lock_cab, SIGNAL(NotifyLockSelected(QString, bool)), this, SLOT(OnNotifyLockSelected(QString, bool)));

    if (m_role == USER)
    {
        ui->btnCancelOpen->setEnabled(true);
    }
}

SelectLocksWidget::~SelectLocksWidget()
{
    Kcb::Utils::DestructorMsg(this);
    delete ui;
}

void SelectLocksWidget::setLocks(QString locks)
{
    Q_ASSERT(m_role == USER);
    Q_ASSERT(locks != "");

    // qDebug() << "Locks:" << locks;

    m_lock_cab.disableAllLocks();
    m_lock_cab.setEnabledLocks(locks);

    ui->lstSelectedLocks->clear();

    ui->btnOpenSelected->setEnabled(locks.length() > 0 ? true : false);
}

QString SelectLocksWidget::getLocks()
{
    QStringList locks;
    QString lock;
    QString str;
    QString cab;
    int num_items = ui->lstSelectedLocks->count();

    for (int ii = 0; ii < num_items; ++ii)
    {
        str = ui->lstSelectedLocks->item(ii)->text();
        getCabinetLockFromStr(str, cab, lock);
        locks.append(QString::number(lock.toInt()));
    }

    return locks.join(",");
}

void SelectLocksWidget::createLockListStr(QString cab, QString lock, QString& str)
{
    str = QString(tr("%1 - Lock %2")).arg(cab, 3, '0').arg(lock, 3, '0');
}

void SelectLocksWidget::addLockToList(QString lock)
{
    QString item_str;

    createLockListStr(m_lock_cab.getSelectedCabinet(), lock, item_str);

    m_lock_list.append(item_str);
    m_lock_list.removeDuplicates();
    m_lock_list.sort();

    qDebug() << m_lock_list;

    ui->lstSelectedLocks->clear();
    ui->lstSelectedLocks->addItems(m_lock_list);
}

void SelectLocksWidget::addLocksToList(QString locks)
{
    if (locks.contains(","))
    {
        QStringList sl = locks.split(",");

        foreach (auto s, sl)
        {
            addLockToList(s);
        }
    }
    else
    {
        addLockToList(locks);
    }
}

void SelectLocksWidget::removeLockFromList(QString lock)
{
    QString item_str;

    createLockListStr(m_lock_cab.getSelectedCabinet(), lock, item_str);

    int index = m_lock_list.indexOf(item_str);
    if (index >= 0)
    {
        m_lock_list.removeAt(index);
        m_lock_list.sort();

        ui->lstSelectedLocks->clear();
        ui->lstSelectedLocks->addItems(m_lock_list);
    }
}

void SelectLocksWidget::OnNotifyLockSelected(QString lock, bool is_selected)
{
    if (is_selected)
    {
        addLockToList(lock);
    }
    else
    {
        removeLockFromList(lock);
    }

    ui->btnOpenSelected->setEnabled(ui->lstSelectedLocks->count() > 0 ? true : false);
}

void SelectLocksWidget::getCabinetLockFromStr(QString& str, QString& cab, QString& lock)
{
    // mmmm - Lock yyy
    //  0   1  2    3
    QStringList cab_lock = str.split(' ', QString::SkipEmptyParts);
    KCB_DEBUG_TRACE("CabLockList" << cab_lock);
    QVector<QString> cab_lock_vtr = cab_lock.toVector();
    KCB_DEBUG_TRACE("CabLockVtr" << cab_lock_vtr);
    cab = cab_lock_vtr[0];
    lock = cab_lock_vtr[3];
}

void SelectLocksWidget::openDoorTimer()
{
    QString cab;
    QString lock;
    QString str;

    str = ui->lstSelectedLocks->item(0)->text();
    delete ui->lstSelectedLocks->item(0);
    KCB_DEBUG_TRACE("LockStr:" << str);
    getCabinetLockFromStr(str, cab, lock);

    m_lock_cab.setSelectedCabinet(cab, lock);
    m_lock_cab.clrSelectedLocks(lock);
    emit NotifyRequestLockOpen(QString::number(lock.toInt()), false);

    if (m_cancel_open || ui->lstSelectedLocks->count() == 0)
    {
        ui->btnOpenSelected->setEnabled(ui->lstSelectedLocks->count() > 0 ? true : false);
        ui->btnCancelOpen->setEnabled(false);
    }
    else
    {
        QTimer::singleShot(500, this, SLOT(openDoorTimer()));
    }
}


void SelectLocksWidget::on_btnCancelOpen_clicked()
{
    if (m_role == ADMIN)
    {
        m_cancel_open = true;
    }
    else // m_role == USER
    {
        emit NotifyClose();
    }
}

void SelectLocksWidget::on_btnOpenSelected_clicked()
{
    if (m_role == ADMIN)
    {
        m_cancel_open = false;
        ui->btnOpenSelected->setEnabled(false);
        ui->btnCancelOpen->setEnabled(true);
        openDoorTimer();
    }
    else // m_role == USER
    {
        emit NotifyOpen();
    }
}
