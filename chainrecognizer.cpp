#include "chainrecognizer.h"
#include "ui_chainrecognizer.h"
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

ChainRecognizer::ChainRecognizer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChainRecognizer)
{
    ui->setupUi(this);
    errorMessage = new QErrorMessage;
}


void ChainRecognizer::init(Grammar *g, QSet<QString> *c)
{
    grammar = g;
    chains = c;

    if(chains != nullptr) {
        QString str;
        QSetIterator<QString> *it = new QSetIterator<QString>(*chains);

        ui->chainsList->clear();

        while (it->hasNext()) {
            str = it->next();
            if(str == "") {
                str = grammar->empty_sym;
            }
            ui->chainsList->addItem(str);
        }
        current_chain = ui->chainsList->currentText();
        qDebug() << current_chain;
    }
}


void ChainRecognizer::on_recognize_clicked()
{
    int n = current_chain.length();
    if(n == 0)
        return;
    QStringList rs;
    int ind = 0;
    ui->tableT->setColumnCount(0);
    ui->tableT->setRowCount(0);
    rules.clear();
    ui->chainOutput->clear();

    foreach(QString from, grammar->rules.keys()) {
        rs = grammar->rules.value(from).split("|");
        foreach(QString toChain, rs) {
            rules.append(QPair<QString, QString>(from, toChain));
            ++ind;
        }
    }

    QStringList header;

    for (int i = 0; i < n; ++i) {
        ui->tableT->insertColumn(i);
        ui->tableT->insertRow(i);
        header.push_back(QString::number(i));
    }

    ui->tableT->setHorizontalHeaderLabels(header);
    ui->tableT->setVerticalHeaderLabels(header);

    for (int i = 0; i < n; ++i) {
        for (int j  = 0; j < n; ++j) {
            ui->tableT->setItem(j, i, new QTableWidgetItem(QString("-")));
        }
    }

    QString from;
    QStringList l;
    for(int i = 0; i < current_chain.length(); ++i) {
        if(grammar->nonterminals.contains(current_chain.at(i))) {
            errorMessage->showMessage("В цепочке присутствуют нетерминалы");
            return;
        }
        if(!grammar->terminals.contains(current_chain.at(i))) {
            errorMessage->showMessage("Не все символы в цепочке терминалы");
            return;
        }
        l.clear();
        from = createItem(current_chain.at(i), rules, l);
        ui->tableT->setItem(i, 0, new QTableWidgetItem(from));
    }

    for(int j = 1; j < n; ++j) {
        for(int i = 0; i < n - j; ++i) {
            QStringList right, left;
            for(int k = 0; k < j; ++k) {
                QStringList B = ui->tableT->item(i,k)->text().split(",");
                QStringList C = ui->tableT->item(i+k+1,j-k-1)->text().split(",");

                foreach (QString val1, B) {
                    foreach (QString val2, C) {
                        if (!right.contains(val1 + val2)) {
                            right.append(val1 + val2);
                        }
                    }
                }
            }
            from = "";
            QString tmp;
            left.clear();
            foreach (QString val, right) {
                tmp = createItem(val, rules, left);
                if(tmp == "") continue;
                if(from != "") from += "," + tmp;
                else from += tmp;
            }

            left.clear();
            right.clear();
            ui->tableT->setItem(i, j, new QTableWidgetItem(from));
        }
    }

    QStringList check_list = ui->tableT->item(0, n-1)->text().split(",");
    if(!check_list.contains(grammar->start_nterm)) {
        errorMessage->showMessage("Ошибка при распозновании цепочки");
        return;
    }

    chain_output.clear();

    R(0, n-1, grammar->start_nterm);

    QList<QPair<int, QString> > final_output;
    foreach(int val, chain_output) {
        if(final_output.size() == 0) {
            final_output.append(QPair<int, QString>(val, rules.at(val).second));
        } else {
            QString tmp = final_output.last().second;
            int ind = tmp.indexOf(rules.at(val).first);
            tmp.replace(ind, 1, "");
            tmp.insert(ind, rules.at(val).second);
            final_output.append(QPair<int, QString>(val,tmp));
        }
    }
    for (int i = 0; i < final_output.size(); ++i) {
        ui->chainOutput->addItem("(" + QString::number(final_output.at(i).first) + ")  - >  " + final_output.at(i).second);
    }
    if(final_output.last().second != current_chain) {
         errorMessage->showMessage("Ошибка при распозновании цепочки");
    }

}


QString ChainRecognizer::createItem(QString sym, QList< QPair<QString, QString> > rules, QStringList &left)
{
    QString item = "";

    for(int j = 0; j < rules.count(); ++j){
        if (rules.at(j).second == sym) {
            if(!left.contains(rules.at(j).first)) {
                left.append(rules.at(j).first);
                if(item != "") item += "," + rules.at(j).first;
                else item = rules.at(j).first;
            }
        }
    }

    return item;
}


void ChainRecognizer::R(int i, int j, QString nterm)
{
    int rule_id;
    QStringList right;

    if (j == 0) {
        rule_id = rules.indexOf(QPair<QString, QString>(nterm, current_chain.at(i)));
        if(rule_id != -1) {
            chain_output.push_back(rule_id);
            qDebug() << rule_id;
        }
        return;
    }

    for(int k = 0; k < j; ++k) {
        QStringList B = ui->tableT->item(i,k)->text().split(",");
        QStringList C = ui->tableT->item(i+k+1,j-k-1)->text().split(",");

        foreach (QString val1, B) {
            foreach (QString val2, C) {
                if (!right.contains(val1 + val2)) {
                    right.append(val1 + val2);
                    rule_id = rules.indexOf(QPair<QString, QString>(nterm, val1 + val2));
                    if(rule_id != -1) {
                        chain_output.push_back(rule_id);
                        qDebug() << rule_id;
                        R(i,k, val1);
                        R(i+k+1, j-k-1, val2);
                        return;
                    }
                }
            }
        }
    }

}


void ChainRecognizer::on_chainsList_activated(const QString &arg1)
{
    current_chain = arg1;
    qDebug() << current_chain;
}


void ChainRecognizer::saveChainRecognize()
{
    if(!chain_output.size()) {
        errorMessage->showMessage("Ни одна цепочка не распознана!");
        return;
    }

    QList<QPair<int, QString> > final_output;
    foreach(int val, chain_output) {
        if(final_output.size() == 0) {
            final_output.append(QPair<int, QString>(val, rules.at(val).second));
        } else {
            QString tmp = final_output.last().second;
            int ind = tmp.indexOf(rules.at(val).first);
            tmp.replace(ind, 1, "");
            tmp.insert(ind, rules.at(val).second);
            final_output.append(QPair<int, QString>(val,tmp));
        }
    }

    QString filename = QFileDialog::getSaveFileName(nullptr, "Сохранение вывода цепочки", "", "*.txt");
    QFile file(filename);

    if(file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);

        int n = current_chain.length();

        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < n; ++j) {
                stream << ui->tableT->takeItem(i,j)->text() + "\t";
            }
            stream << "\n";
        }

        for (int i = 0; i < final_output.size(); ++i) {
            stream << "(" + QString::number(final_output.at(i).first) + ")  - >  " + final_output.at(i).second + "\n";
        }

        file.close();
    }
}


ChainRecognizer::~ChainRecognizer()
{
    delete ui;
}
