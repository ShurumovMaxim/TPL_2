#ifndef GRAMMARVIEWER_H
#define GRAMMARVIEWER_H

#include <QWidget>
#include "grammar.h"


namespace Ui {
class GrammarViewer;
}

class GrammarViewer : public QWidget
{
    Q_OBJECT

public:
    explicit GrammarViewer(QWidget *parent = nullptr);
    ~GrammarViewer();

    void init(Grammar *g);

private:
    Ui::GrammarViewer *ui;
    Grammar *grammar;
};

#endif // GRAMMARVIEWER_H
