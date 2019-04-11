#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <QWidget>
#include <QSet>
#include <QMap>
#include <QErrorMessage>
#include <QListWidgetItem>

namespace Ui {
class Grammar;
}

class Grammar : public QWidget
{
    Q_OBJECT

public:
    explicit Grammar(QWidget *parent = nullptr);
    ~Grammar();

    void saveGrammar();
    void updateGUI();

    QSet<QString> terminals;
    QSet<QString> nonterminals;
    QString empty_sym;
    QString start_nterm;
    QMap<QString, QString> rules;

signals:
    void grammarReady();

private slots:
    void on_updateAlpha_clicked();

    void on_addRule_clicked();

    void on_saveGrammar_clicked();

    void on_rulesList_itemDoubleClicked(QListWidgetItem *item);

    void on_loadGrammar_clicked();

private:
    Ui::Grammar *ui;
    QErrorMessage *errorMessage;

    void closeEvent(QCloseEvent *e);
};

#endif // GRAMMAR_H
