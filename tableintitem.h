/* tableintitem.h */

#include <qwidget.h>
#include <qtable.h>
#include <qvalidator.h>
#include <qlineedit.h>

#ifndef __TABLE_INT_ITEM__
#define __TABLE_INT_ITEM__

class hTableIntItem : public QTableItem {
 public:
 hTableIntItem( QTable *table, EditType et, const QString &text ) :
  QTableItem(table, et, text) {};
  ~hTableIntItem() {};
  QWidget *createEditor() const {
    QWidget* w = new QWidget();
    QLineEdit* ed = new QLineEdit(w);
    QIntValidator* val = new QIntValidator(ed);
    ed->setValidator(val);
    ed->setText(text());
    return ed;
  };
};

class hTableDoubleItem : public QTableItem {
 public:
 hTableDoubleItem( QTable *table, EditType et, const QString &text ) :
  QTableItem(table, et, text) {};
  ~hTableDoubleItem() {};
  QWidget *createEditor() const {
    QWidget* w = new QWidget();
    QLineEdit* ed = new QLineEdit(w);
    QDoubleValidator* val = new QDoubleValidator(ed);
    ed->setValidator(val);
    ed->setText(text());
    return ed;
  };
};

#endif //__TABLE_INT_ITEM__
