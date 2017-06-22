#ifndef SDIALOG_H
#define SDIALOG_H

#include <QDialog>

namespace Ui {
class SDialog;
}

class QFrame;
class QHBoxLayout;

class SDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SDialog(QWidget *parent = 0);
    ~SDialog();

public slots:
    void setBorderFramesHidden(bool b);
    void setBorderFramesWidth(int w);
    int getBorderFramesWidth() const;

    void setWindowIconHidden(bool b);
    void setWindowTitleHidden(bool b);
    void setWindowMinButtonHidden(bool b);
    void setWindowMaxRestoreButtonHidden(bool b);
    void setWindowCloseButtonHidden(bool b);
    void setToCloseButtonHint();

    QFrame *centralFrame();
    QFrame *titleBarFrame();
    QHBoxLayout *mainToolBarLayout();
    QHBoxLayout *titleBarButtonLayout();
    QHBoxLayout *titleBarToolBarLayout();
    QHBoxLayout *statusBarLayout();

    bool getDragResizeable() const;
    void setDragResizeable(bool b);

protected:
    bool eventFilter(QObject *o, QEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);
    void keyPressEvent(QKeyEvent *e);

    enum LeftMousePressRegion {
        Left, Right, Top, Bottom, TopLeft, TopRight, BottomLeft, BottomRight, TitleBar, CentralFrame
    };

private slots:
    void on_toolButton_windowMin_clicked();
    void on_toolButton_windowMaxRestore_clicked();
    void on_toolButton_windowClose_clicked();

private:
    Ui::SDialog *ui;
    bool m_bLeftMouseOnPressing;
    QPoint m_leftMousePressPos;
    int m_iLeftMousePressRegion;

    bool m_bDragResizeable;
};

#endif // SDIALOG_H
