#include "stylesheetdialog.h"
#include "ui_testframe.h"

StyleSheetDialog::StyleSheetDialog(QWidget *parent)
    : SDialog(parent),
      ui(new Ui::TestFrame),
      isProgressBarRunning(false)
{
    ui->setupUi(centralFrame());
    setWindowIconHidden(true);
    setWindowTitle("Qt Style Sheet Test");

    connect(ui->pushButton_playProgressBarAnimation, SIGNAL(clicked()),
            this, SLOT(on_pushButton_playProgressBarAnimation_clicked()));

    resize(800, 600);
}

StyleSheetDialog::~StyleSheetDialog()
{
    delete ui;
}

void StyleSheetDialog::on_pushButton_playProgressBarAnimation_clicked()
{
    if (!isProgressBarRunning) {
        isProgressBarRunning = true;

        ui->progressBar->setMaximum(100);
        ui->progressBar->setValue(0);

        UpdateProgressBarThread *thread = new UpdateProgressBarThread(this);
        connect(thread, SIGNAL(progress(int,int)), this, SLOT(updateProgressBar(int,int)));
        connect(thread, SIGNAL(finished()), this, SLOT(onUpdateProgressBarThreadFinished()));
        thread->start();
    }
}

void StyleSheetDialog::updateProgressBar(int max, int value)
{
    ui->progressBar->setMaximum(max);
    ui->progressBar->setValue(value);
}

void StyleSheetDialog::onUpdateProgressBarThreadFinished()
{
    isProgressBarRunning = false;
    UpdateProgressBarThread *p = dynamic_cast<UpdateProgressBarThread*>(sender());
    delete p;
}
