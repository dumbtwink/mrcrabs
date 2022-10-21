#include "mrcrabs.h"
#include "./ui_mrcrabs.h"
#include <bits/stdc++.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include <string>
#include <QMessageBox>

int income, savings, debt, allowance, inhand;
QString currency = " kr";
std::string localVarDir = std::string(getenv("HOME")) + "/.local/share/MrCrabs";
std::ifstream incomeFile(localVarDir + "/income"),
              savingsFile(localVarDir + "/savings"),
              debtFile(localVarDir + "/debt"),
              allowanceFile(localVarDir + "/allowance"),
              inhandFile(localVarDir + "/inhand");


MrCrabs::MrCrabs(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MrCrabs)
{
    ui->setupUi(this);
    mkdir(localVarDir.c_str(), 0777);
    while(savingsFile >> savings);
    ui->savingsDisplay->setText(QString::number(savings).append(currency));
    while(debtFile >> debt);
    ui->debtDisplay->setText(QString::number(debt).append(currency));
    while(allowanceFile >> allowance);
    ui->allowanceDisplay->setText(QString::number(allowance).append(currency));
    while(incomeFile >> income);
    ui->incomeDisplay->setText(QString::number(income).append(currency));
    while(inhandFile >> inhand);
    ui->inhandDisplay->setPlainText(QString::number(inhand).append(currency));
    if(debt==0){
        ui->payDebt->setEnabled(false);
    }
}

MrCrabs::~MrCrabs()
{
    incomeFile.close();
    savingsFile.close();
    debtFile.close();
    allowanceFile.close();
    inhandFile.close();
    std::ofstream incomeFile(localVarDir + "/income"),
                  savingsFile(localVarDir + "/savings"),
                  debtFile(localVarDir + "/debt"),
                  allowanceFile(localVarDir + "/allowance"),
                  inhandFile(localVarDir + "/inhand");
    incomeFile << income;
    savingsFile << savings;
    debtFile << debt;
    allowanceFile << allowance;
    inhandFile << inhand;
    incomeFile.close();
    savingsFile.close();
    debtFile.close();
    allowanceFile.close();
    inhandFile.close();
    delete ui;
}

// Claim daily
void MrCrabs::on_claimDaily_clicked()
{
    savings += allowance;
    ui->savingsDisplay->setPlainText(QString::number(savings).append(currency));
    allowance = (income - debt)/30;
    inhand -= allowance;
    ui->inhandDisplay->setPlainText(QString::number(inhand).append(currency));
    ui->allowanceDisplay->setPlainText(QString::number(allowance).append(currency));
    ui->claimDaily->setEnabled(false);
    QMessageBox messageBox;
    messageBox.information(0, "Daily Allowance", QString::number(income/30) + " kr has been put in your allowance.\nYesterday's allowance has been put into your savings.");
    messageBox.setFixedSize(500,200);
}

// New Income button
void MrCrabs::on_newIncome_clicked()
{
    int buffer = ui->valueInput->toPlainText().toInt();
    savings += income;
    ui->savingsDisplay->setPlainText(QString::number(savings).append(currency));
    income = buffer;
    inhand = buffer;
    debt = 0;
    ui->incomeDisplay->setPlainText(QString::number(income + debt).append(currency));
    ui->inhandDisplay->setPlainText(QString::number(inhand).append(currency));
    ui->newIncome->setEnabled(false);
    QMessageBox messageBox;
    messageBox.information(0, "Income", "Your income has been added.");
    messageBox.setFixedSize(500,200);
    ui->valueInput->setText("");
}


void MrCrabs::on_reportUsage_clicked()
{
    int buffer = ui->valueInput->toPlainText().toInt();
    if(0<=(allowance-buffer)){
        allowance -= buffer;
        ui->allowanceDisplay->setPlainText(QString::number(allowance).append(currency));
        QMessageBox messageBox;
        messageBox.information(0, "Allowance", QString::number(buffer) + " kr has been spent from your allowance");
        messageBox.setFixedSize(500,200);
        ui->valueInput->setText("");
    } else {
        std::cout << allowance << "   " << allowance - buffer << std::endl;
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Insufficient funds!");
        messageBox.setFixedSize(500,200);
        ui->valueInput->setText("");
    }
}


void MrCrabs::on_takeLoan_clicked()
{
    int buffer = ui->valueInput->toPlainText().toInt();
    debt += buffer;
    allowance += buffer;
    inhand -= debt;
    ui->inhandDisplay->setPlainText(QString::number(inhand).append(currency));
    ui->debtDisplay->setPlainText(QString::number(debt).append(currency));
    ui->allowanceDisplay->setPlainText(QString::number(allowance).append(currency));
    QMessageBox messageBox;
    messageBox.information(0, "Loan", QString::number(buffer) + " kr has been put in your allowance. You're now " + QString::number(debt) + " kr in debt.\nYour allowence is now " + QString::number((income - debt)/30) + " kr/day.");
    messageBox.setFixedSize(500,200);
    ui->valueInput->setText("");
    ui->payDebt->setEnabled(true);
}


void MrCrabs::on_payDebt_clicked()
{
    if(debt-allowance>0){
        debt -= allowance;
        inhand += debt;
        allowance = 0;
        ui->inhandDisplay->setPlainText(QString::number(inhand).append(currency));
        QMessageBox messageBox;
        ui->debtDisplay->setPlainText(QString::number(debt).append(currency));
        ui->allowanceDisplay->setPlainText(QString::number(allowance).append(currency));
        messageBox.information(0, "Debt", "You partially payed off your debt! But you still\n have " + QString::number(debt) + " kr left in debt.");
        messageBox.setFixedSize(500,200);
    } else {
        allowance -= debt;
        inhand += debt;
        debt = 0;
        ui->debtDisplay->setPlainText(QString::number(debt).append(currency));
        ui->allowanceDisplay->setPlainText(QString::number(allowance).append(currency));
        ui->payDebt->setEnabled(false);
        ui->inhandDisplay->setPlainText(QString::number(inhand).append(currency));
        QMessageBox messageBox;
        messageBox.information(0, "Debt", "You payed off your debt!");
        messageBox.setFixedSize(500,200);
    }
}

