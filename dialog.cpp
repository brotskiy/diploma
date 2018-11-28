#include "dialog.h"

Dialog::Dialog(QWidget* parent): QDialog(parent)
{
  setFixedSize(300, 200);

  setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);
  setWindowTitle("Choose projection");

  setAttribute(Qt::WA_DeleteOnClose);

  btns = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

  gbx1 = new QGroupBox("Axis X", this);
  gbx2 = new QGroupBox("Axis Y", this);

  ed1 = new QLineEdit(this);
  ed2 = new QLineEdit(this);

  QVBoxLayout* vbx = new QVBoxLayout(this);
  vbx->addWidget(gbx1);
  vbx->addWidget(gbx2);
  vbx->addWidget(btns);
  this->setLayout(vbx);

  QHBoxLayout* hbx1 = new QHBoxLayout(gbx1);
  hbx1->addWidget(ed1);
  gbx1->setLayout(hbx1);

  QHBoxLayout* hbx2 = new QHBoxLayout(gbx2);
  hbx2->addWidget(ed2);
  gbx2->setLayout(hbx2);

  connect(btns, &QDialogButtonBox::rejected, this, &Dialog::reject);
  connect(btns, &QDialogButtonBox::accepted, this, &Dialog::sendAxes);
}

void Dialog::sendAxes()
{
  emit Axes(ed1->text() + " " + ed2->text());
  accept();
}
