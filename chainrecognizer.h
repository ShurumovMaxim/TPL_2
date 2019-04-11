#ifndef CHAINRECOGNIZER_H
#define CHAINRECOGNIZER_H

#include <QWidget>
#include "chaingenerator.h"
#include <QPair>

namespace Ui {
class ChainRecognizer;
}

class ChainRecognizer : public QWidget
{
    Q_OBJECT

public:
    explicit ChainRecognizer(QWidget *parent = nullptr);
    ~ChainRecognizer();

    void init(Grammar *g, QSet<QString> *c);
    void saveChainRecognize();

private slots:
    void on_recognize_clicked();

    void on_chainsList_activated(const QString &arg1);

private:
    Ui::ChainRecognizer *ui;
    QErrorMessage *errorMessage;

    Grammar *grammar;
    QSet<QString> *chains;
    QString current_chain;
    QList<QPair<QString, QString>> rules;
    QVector<int> chain_output;

    QString createItem(QString sym, QList< QPair<QString, QString> > rules, QStringList &left);
    void R(int i, int j, QString nterm);

};

#endif // CHAINRECOGNIZER_H
