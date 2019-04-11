#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chaingenerator.h"
#include "chainrecognizer.h"
#include "grammarconverter.h"
#include "grammarviewer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void grammar_slot();
    void chainGenerator_slot();
    void converter_slot();

private slots:
    void on_changeGrammar_clicked();

    void on_generateChains_clicked();

    void on_recognizeChains_clicked();

    void on_konvertGrammar_clicked();

    void on_rulesList_clicked();

    void on_saveGrammar_triggered();

    void on_savechains_triggered();

    void on_savechainrec_triggered();

    void on_actionAuthor_triggered();

    void on_actionTheme_triggered();

private:
    Ui::MainWindow *ui;

    Grammar grammar;
    GrammarConverter converter;
    GrammarViewer viewer;
    ChainGenerator chain_generator;
    ChainRecognizer chain_recognizer;
};

#endif // MAINWINDOW_H
