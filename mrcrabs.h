#ifndef MRCRABS_H
#define MRCRABS_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MrCrabs; }
QT_END_NAMESPACE

class MrCrabs : public QMainWindow
{
    Q_OBJECT

public:
    MrCrabs(QWidget *parent = nullptr);
    ~MrCrabs();

private slots:
    void on_claimDaily_clicked();

    void on_newIncome_clicked();

    void on_reportUsage_clicked();

    void on_takeLoan_clicked();

    void on_payDebt_clicked();

private:
    Ui::MrCrabs *ui;
};
#endif // MRCRABS_H
