#ifndef STYLESHEETDIALOG_H
#define STYLESHEETDIALOG_H

#include "sdialog.h"
#include <QThread>

namespace Ui {
class TestFrame;
}

///
/// \brief The StyleSheetDialog class
///
class StyleSheetDialog : public SDialog
{
    Q_OBJECT

public:
    StyleSheetDialog(QWidget *parent = 0);
    ~StyleSheetDialog();

private slots:
    void on_pushButton_playProgressBarAnimation_clicked();
    void updateProgressBar(int max, int value);
    void onUpdateProgressBarThreadFinished();

private:
    Ui::TestFrame *ui;

    bool isProgressBarRunning;
};

class UpdateProgressBarThread : public QThread
{
    Q_OBJECT
public:
    UpdateProgressBarThread(QObject *parent = 0) : QThread(parent)
    {}

protected:
    void run()
    {
        int max = 100;
        int value = 0;

        while (value <= max) {
            progress(max, value);
            value ++;
            msleep(100);
        }
    }

signals:
    void progress(int max, int value);
};

#endif // STYLESHEETDIALOG_H
