#include "chaingenerator.h"
#include "ui_chaingenerator.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>


ChainGenerator::ChainGenerator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChainGenerator)
{
    ui->setupUi(this);
    max_cnt = min_len = max_len = 0;
    chains = nullptr;
    errorMessage = new QErrorMessage;
}


void ChainGenerator::setGrammar(Grammar *g) {
    grammar = g;

    foreach(QString toChain, grammar->rules) {
        QList<QString> rs = toChain.split("|");

        for (int i = 0; i < rs.size(); ++i) {
            if (max_cnt < count_nterm(rs.at(i))) {
                max_cnt = count_nterm(rs.at(i));
            }
        }
    }
}


int ChainGenerator::count_term(QString str)
{
    int n = 0;
    for (int i = 0; i < str.length(); i++){
        if (grammar->terminals.contains(QString(str[i])) && str.at(i) != "_") n++;
    }
    return n;
}


int ChainGenerator::count_nterm(QString str)
{
    int n = 0;
    for (int i = 0; i < str.length(); i++){
        if (grammar->nonterminals.contains(QString(str[i]))) n++;
    }
    return n;
}


bool ChainGenerator::is_valid(QString str)
{
    int len = count_nterm(str) + count_term(str);

    if(len <= max_len && count_term(str) <= ui->maxLen->value()) {
        return true;
    }

    return false;
}


void ChainGenerator::on_generate_clicked()
{
    QString str;
    QSet<QString> *set = new QSet<QString>();
    set->insert(grammar->start_nterm);

    if(chains == nullptr) {
        chains = new QSet<QString>();
    } else {
        chains->clear();
    }
    QSetIterator<QString> *it = new QSetIterator<QString>(*set);
    QList<QString> *repeats = new QList<QString>;


    while(it->hasNext()){

        str = it->next();

        for (int i = 0; i < str.length(); i++) {
            if (grammar->rules.contains(QString(str[i]))) {

                QList<QString> list = grammar->rules.value(QString(str[i])).split("|");

                for (int j = 0; j < list.size(); j++) {
                    QString newStr = str;
                    newStr.replace(i, 1, list.at(j));
                    newStr.replace("_", "");

                    if (!is_valid(newStr)) {
                        continue;
                    }

                    if(!count_nterm(newStr) && newStr.length() >= min_len && !chains->contains(newStr)) {
                        chains->insert(newStr);
                        continue;
                    }

                    if(!repeats->contains(newStr)) {
                        repeats->append(newStr);
                        set->insert(newStr);
                    }

                }
            }
        }

        set->remove(str);
        repeats->append(str);
        delete it;
        it = new QSetIterator<QString>(*set);
    }

    ui->chainList->clear();
    delete it;

    it = new QSetIterator<QString>(*chains);
    while (it->hasNext()) {
        ui->chainList->addItem(it->next());
    }

    delete repeats;
    delete it;
    delete set;
}


void ChainGenerator::on_minLen_valueChanged(int arg1)
{
    min_len = arg1;
}


void ChainGenerator::on_maxLen_valueChanged(int arg1)
{
    max_len = arg1 + max_cnt;
}


void ChainGenerator::closeEvent(QCloseEvent *e)
{
    emit chainsReady();
    QWidget::closeEvent(e);
}


void ChainGenerator::saveChains()
{
    if(chains == nullptr) {
        errorMessage->showMessage("Цепочки не сгенерированы!");
        return;
    }
    QString filename = QFileDialog::getSaveFileName(nullptr, "Сохранение цепочек", "", "*.txt");
    QFile file(filename);

    if(file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);

        stream << chains->toList().join("\n");

        file.close();
    }
}


ChainGenerator::~ChainGenerator()
{
    delete chains;
    delete ui;
}

void ChainGenerator::on_saveChains_clicked()
{
    emit chainsReady();
    ChainGenerator::close();
}
