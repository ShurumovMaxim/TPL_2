#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&grammar, SIGNAL(grammarReady(void)), this, SLOT(grammar_slot(void)));
    connect(&chain_generator, SIGNAL(chainsReady(void)), this, SLOT(chainGenerator_slot(void)));
    connect(&converter, SIGNAL(conversionComplete(void)), this, SLOT(converter_slot(void)));
}


void MainWindow::grammar_slot()
{
    statusBar()->setStatusTip("Grammar is ready to use!");
    ui->generateChains->setEnabled(true);
}

void MainWindow::chainGenerator_slot()
{
    statusBar()->setStatusTip("Chains is ready to use!");
}

void MainWindow::converter_slot()
{
    statusBar()->setStatusTip("Grammar converted into Chomsky form!");
    ui->recognizeChains->setEnabled(true);
}


void MainWindow::on_changeGrammar_clicked()
{
    grammar.updateGUI();
    grammar.show();
}


void MainWindow::on_generateChains_clicked()
{
    chain_generator.setGrammar(&grammar);
    chain_generator.show();
}

void MainWindow::on_recognizeChains_clicked()
{
    chain_recognizer.init(&grammar, chain_generator.chains);
    chain_recognizer.show();
}

void MainWindow::on_konvertGrammar_clicked()
{
    converter.setGrammar(&grammar);
    converter.show();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_rulesList_clicked()
{
    viewer.close();
    viewer.init(&grammar);
    viewer.show();
}

void MainWindow::on_saveGrammar_triggered()
{
    grammar.saveGrammar();
}

void MainWindow::on_savechains_triggered()
{
    chain_generator.saveChains();
}

void MainWindow::on_savechainrec_triggered()
{
    chain_recognizer.saveChainRecognize();
}

void MainWindow::on_actionAuthor_triggered()
{
    QMessageBox::information(nullptr, "Об авторе", "Студент: Шурумов М.С. Группа: Ип-511. Вариант: 20\n Написать программу, которая для языка, заданного контекстносвободной грамматикой в требуемой форме (проверить корректность задания и при отрицательном результате выдать соответствующее сообщение), построит модель табличного распознавателя (алгоритм Кока-Янгера-Касами). Программа должна сгенерировать по исходной грамматике несколько цепочек в указанном диапазоне длин и проверить их допустимость, построить цепочку вывода. Полученные цепочки и проверку их выводимости (включая построение промежуточной таблицы Т) отображать на экране. Предусмотреть возможность проверки цепочки, введённой пользователем.");
}

void MainWindow::on_actionTheme_triggered()
{
    QMessageBox::information(nullptr,"Тема", "Mодель табличного распознавателя (алгоритм Кока-Янгера-Касами).");
}
