#ifndef DIALOG_H
#define DIALOG_H

#include <QtWidgets>
#include <QString>

class Dialog: public QDialog
{
    Q_OBJECT

  private:
    QGroupBox *gbx1, *gbx2;
    QLineEdit *ed1, *ed2;
    QDialogButtonBox* btns;

  public:
    explicit Dialog(QWidget* parent = nullptr);

  private slots:
    void sendAxes();

  signals:
    void Axes(const QString& axs);

};


#endif // DIALOG_H
