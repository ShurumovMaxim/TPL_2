#include "grammar.h"
#include "ui_grammar.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDebug>

Grammar::Grammar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Grammar)
{
    ui->setupUi(this);

    errorMessage = new QErrorMessage;
    ui->saveGrammar->setEnabled(false);
}


void Grammar::on_updateAlpha_clicked()
{
    QString ntermAlpha = ui->lineNonterm->text();
    QString termAlpha = ui->lineTerm->text();
    QList<QString> Chars;

    ui->saveGrammar->setEnabled(false);

    nonterminals.clear();
    terminals.clear();
    rules.clear();
    ui->rulesList->clear();

    empty_sym = ui->emptySym->text();
    start_nterm = ui->startNterm->text();

    if(!ntermAlpha.length() || !termAlpha.length() || !empty_sym.length() || !start_nterm.length()) {
        errorMessage->showMessage("Неполная конфигурация");
        return;
    }

    Chars = ntermAlpha.split(",");
    for (int i = 0; i < Chars.size(); ++i) {
        if (Chars.at(i).length() != 1 || Chars.at(i).at(0) == empty_sym || nonterminals.contains(Chars.at(i))
            || terminals.contains(Chars.at(i))) {
           errorMessage->showMessage("Неправильный нетерминальный символ");
           nonterminals.clear();
           return;
        } else {
            nonterminals.insert(Chars.at(i));
        }
    }

    if(!nonterminals.contains(start_nterm)) {
        errorMessage->showMessage("Стартовый символ не является нетерминалом");
        nonterminals.clear();
        return;
    }

    Chars = termAlpha.split(",");
    for (int i = 0; i < Chars.size(); ++i) {
        if (Chars.at(i).length() != 1 || Chars.at(i).at(0) == empty_sym || terminals.contains(Chars.at(i)) || nonterminals.contains(Chars.at(i))) {
           errorMessage->showMessage("Неправильный терминальный символ");
           terminals.clear();
           return;
        } else {
            terminals.insert(Chars.at(i));
        }
    }
    terminals.insert(empty_sym);
}

void Grammar::on_addRule_clicked()
{
    QString nterm = ui->fromNterm->text();
    QString toChain = ui->toChain->text();
    QString ntermAlpha = ui->lineNonterm->text();
    QString termAlpha = ui->lineTerm->text();
    QString startterm = ui->startNterm->text();
    QStringList rs = toChain.split("|");

    if(!toChain.length()) {
        errorMessage->showMessage("Неправильное правило");
        return;
    }

    for(int i = 0; i < rs.length(); ++i) {
        if(!rs.at(i).length()) {
            errorMessage->showMessage("Неправильное правило");
            return;
        }
    }

    for (int i = 0; i < toChain.length(); ++i) {
        if (toChain.at(i) == '|') continue;
        if (toChain.at(i) == ' ') {
            errorMessage->showMessage("Неправильное правило");
            return;
        }
        if (!terminals.contains(toChain.at(i)) && !nonterminals.contains(toChain.at(i))) {
           errorMessage->showMessage("Неправильное правило");
           return;
        }

    }

    if (nterm.length() != 1 || rules.contains(nterm) || !nonterminals.contains(nterm)) {
        errorMessage->showMessage("Ошибочное значение");
        return;
    }

    for(int k = 0; k < rs.length();k++)
    {
    for(int i = 0; i < rs.at(k).length(); i++)
    {
            if(rs.at(k).length() >= 3)
            {
                errorMessage->showMessage("Неправильная правая часть правила");
                return;
            }


    }

    }

    int count = 0;
    for(int k = 0; k < rs.length();k++)
    {
    for(int i = 0; i < rs.at(k).length(); i++)
    {
        for(int j = 0; j < termAlpha.length(); j++)
        {
            if(rs.at(k).at(i) == termAlpha[j])
                count++;
        }
    }
    if(count >= 2) {
        errorMessage->showMessage("В правой части правил может быть только один терминальный символ");
        return;
    }

    count = 0;
    }

    empty_sym = ui->emptySym->text();
    for(int k = 0; k < rs.length();k++)
    {
    for(int i = 0; i < rs.at(k).length(); i++)
    {
        for(int j = 0; j < termAlpha.length(); j++)
        {
            if(nterm[0] != startterm && rs.at(k).at(0) == empty_sym)
            {
             errorMessage->showMessage("Пустой символ может быть в правой части только у начального символа");
             return;
         }
            if(rs.at(k).at(i) == empty_sym && i != 0)
               {
                errorMessage->showMessage("Пустой символ не может быть в середине правила");
                return;
            }
        }
    }

    }

    for(int k = 0; k < rs.length();k++)
    {
    for(int i = 0; i < rs.at(k).length(); i++)
    {
        for(int j = 0; j < ntermAlpha.length(); j++)
        {
            if(rs.at(k).at(i) == ntermAlpha[j])
                count++;
        }
    }
    if(count >= 3 || count == 1) {
        errorMessage->showMessage("В правой части может быть только 2 нетерминала");
        return;
    }
    count = 0;
    }



    ui->rulesList->addItem(nterm + "->" + toChain);
    rules.insert(nterm, toChain);

    ui->toChain->clear();
    ui->fromNterm->clear();

    ui->saveGrammar->setEnabled(true);
}


void Grammar::on_saveGrammar_clicked()
{
    Grammar::close();
    emit grammarReady();
}


void Grammar::on_rulesList_itemDoubleClicked(QListWidgetItem *item)
{
    QList<QString> lr = item->text().split("->");
    rules.remove(lr.at(0));
    delete item;
}


void Grammar::closeEvent(QCloseEvent *e)
{
    emit grammarReady();
    QWidget::closeEvent(e);
}


Grammar::~Grammar()
{
    delete ui;
}

void Grammar::on_loadGrammar_clicked()
{
    QString filename = QFileDialog::getOpenFileName(nullptr, "Выбор файла грамматики", "", "*.txt");
    QFile file(filename);

    if(file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        QString VT, VN, SS, ES, tmp;

        if(!stream.atEnd()) {
            VT = stream.readLine();
            if(VT.left(3).compare("VT:") != 0) {
                errorMessage->showMessage("Invalid format VT!");
                return;
            }
            VT.replace(0, 3, "");
        } else {
            errorMessage->showMessage("Invalid format VT!");
            return;
        }

        if(!stream.atEnd()) {
            VN = stream.readLine();
            if(VN.left(3).compare("VN:") != 0) {
                errorMessage->showMessage("Invalid format VN!");
                return;
            }
            VN.replace(0, 3, "");
        } else {
            errorMessage->showMessage("Invalid format VN!");
            return;
        }

        if(!stream.atEnd()) {
            SS = stream.readLine();
            if(SS.left(3).compare("SS:") != 0 || SS.length() > 4) {
                errorMessage->showMessage("Invalid format SS!");
                return;
            }
            SS.replace(0, 3, "");
        } else {
            errorMessage->showMessage("Invalid format SS!");
            return;
        }

        if(!stream.atEnd()) {
            ES = stream.readLine();
            if(ES.left(3).compare("ES:") != 0 || ES.length() > 4) {
                errorMessage->showMessage("Invalid format ES!");
                return;
            }
            ES.replace(0, 3, "");
        } else {
            errorMessage->showMessage("Invalid format ES!");
            return;
        }


        ui->lineTerm->setText(VT);
        ui->lineNonterm->setText(VN);
        ui->startNterm->setText(SS);
        ui->emptySym->setText(ES);

        ui->updateAlpha->click();

        QString P;
        while(!stream.atEnd()) {
            P = stream.readLine();
            QStringList rule = P.split("->");

            if(rule.size() > 2 || rule.at(0).length() > 1) {
                errorMessage->showMessage("Invalid format P!");
                return;
            }

            ui->fromNterm->setText(rule.at(0));
            ui->toChain->setText(rule.at(1));

            ui->addRule->click();
        }

        file.close();
    }
}


void Grammar::saveGrammar()
{
    if(!terminals.size() || !rules.size()) {
        errorMessage->showMessage("Грамматика не задана!");
        return;
    }

    QString filename = QFileDialog::getSaveFileName(nullptr, "Сохранение грамматики", "", "*.txt");
    QFile file(filename);

    if(file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);

        terminals.remove(empty_sym);
        stream << "VT:" + terminals.toList().join(",") + "\n";
        terminals.insert(empty_sym);

        stream << "VN:" + nonterminals.toList().join(",") + "\n";
        stream << "SS:" + start_nterm + "\n";
        stream << "ES:" + empty_sym + "\n";

        foreach (QString key, rules.keys()) {
            stream << key + "->" + rules.value(key) + "\n";
        }

        file.close();
    }
}

void Grammar::updateGUI()
{

    foreach(QString from, rules.keys()) {
        ui->rulesList->addItem(from + " -> " + rules.value(from));
    }

    ui->startNterm->setText(start_nterm);
    QSetIterator<QString> it(terminals);
    QString terms = "";
    while (it.hasNext()) {
        if(terms.length()) {
            terms += "," + it.next();
        } else {
            terms += it.next();
        }
    }

    it = QSetIterator<QString>(nonterminals);
    QString nterms = "";
    while (it.hasNext()) {
        if(nterms.length()) {
            nterms += "," + it.next();
        } else {
            nterms += it.next();
        }
    }

    ui->lineTerm->setText(terms);
    ui->lineNonterm->setText(nterms);
    ui->startNterm->setText(start_nterm);
}
