#ifndef CHAINGENERATOR_H
#define CHAINGENERATOR_H

#include <QWidget>
#include "grammar.h"

namespace Ui {
class ChainGenerator;
}

class ChainGenerator : public QWidget
{
    Q_OBJECT

public:
    explicit ChainGenerator(QWidget *parent = nullptr);
    ~ChainGenerator();

    QSet<QString> *chains;

    void setGrammar(Grammar *g);
    void saveChains();


signals:
    void chainsReady();

private slots:
    void on_generate_clicked();

    void on_minLen_valueChanged(int arg1);

    void on_maxLen_valueChanged(int arg1);

    void on_saveChains_clicked();

private:
    Ui::ChainGenerator *ui;
    QErrorMessage *errorMessage;
    Grammar *grammar;

    int count_term(QString);
    int count_nterm(QString);
    bool is_valid(QString);

    int max_cnt, max_len, min_len;

    void closeEvent(QCloseEvent *e);
};

#endif // CHAINGENERATOR_H
