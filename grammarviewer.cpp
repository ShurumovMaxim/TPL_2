#include "grammarviewer.h"
#include "ui_grammarviewer.h"

GrammarViewer::GrammarViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GrammarViewer)
{
    ui->setupUi(this);
}

void GrammarViewer::init(Grammar *g) {
    grammar = g;

    QStringList rs;
    QList<QPair<QString, QString>> rules;

    foreach(QString from, grammar->rules.keys()) {
        rs = grammar->rules.value(from).split("|");
        foreach(QString toChain, rs) {
            rules.append(QPair<QString, QString>(from, toChain));
        }
    }

    ui->rulesList->clear();
    for (int i = 0; i < rules.size(); ++i) {
        ui->rulesList->addItem("(" + QString::number(i) + ")  " + rules.at(i).first + " -> " + rules.at(i).second);
    }
}

GrammarViewer::~GrammarViewer()
{
    delete ui;
}
