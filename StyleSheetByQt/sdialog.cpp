#include "sdialog.h"
#include "ui_sdialog.h"
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QPainter>
#include <qmath.h>
#include <QDebug>
#include <QHBoxLayout>

SDialog::SDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SDialog),
    m_bLeftMouseOnPressing(false),
    m_iLeftMousePressRegion(CentralFrame),
    m_bDragResizeable(true)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint | Qt::Window);

    ui->toolButton_windowMin->setIcon(style()->standardIcon(QStyle::SP_TitleBarMinButton));
    ui->toolButton_windowClose->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    ui->toolButton_windowMaxRestore->setIcon(style()->standardIcon(QStyle::SP_TitleBarMaxButton));

    installEventFilter(this);

    setBorderFramesWidth(9);
    setDragResizeable(true);
}

SDialog::~SDialog()
{
    delete ui;
}

void SDialog::setBorderFramesHidden(bool b)
{
    ui->frame_border_left->setHidden(b);
    ui->frame_border_right->setHidden(b);
    ui->frame_border_top->setHidden(b);
    ui->frame_border_bottom->setHidden(b);

    ui->frame_border_bottomLeft->setHidden(b);
    ui->frame_border_bottomRight->setHidden(b);
    ui->frame_border_topLeft->setHidden(b);
    ui->frame_border_topRight->setHidden(b);
}

void SDialog::setBorderFramesWidth(int w)
{
    ui->frame_border_left->setFixedWidth(w);
    ui->frame_border_right->setFixedWidth(w);
    ui->frame_border_top->setFixedHeight(w);
    ui->frame_border_bottom->setFixedHeight(w);

    ui->frame_border_topLeft->setFixedSize(QSize(w, w));
    ui->frame_border_topRight->setFixedSize(QSize(w, w));
    ui->frame_border_bottomLeft->setFixedSize(QSize(w, w));
    ui->frame_border_bottomRight->setFixedSize(QSize(w, w));
}

int SDialog::getBorderFramesWidth() const
{
    return ui->frame_border_bottom->height();
}

void SDialog::setWindowIconHidden(bool b)
{
    ui->toolButton_windowIcon->setHidden(b);
}

void SDialog::setWindowTitleHidden(bool b)
{
    ui->label_windowTitle->setHidden(b);
}

void SDialog::setWindowMinButtonHidden(bool b)
{
    ui->toolButton_windowMin->setHidden(b);
}

void SDialog::setWindowMaxRestoreButtonHidden(bool b)
{
    ui->toolButton_windowMaxRestore->setHidden(b);
}

void SDialog::setWindowCloseButtonHidden(bool b)
{
    ui->toolButton_windowClose->setHidden(b);
}

void SDialog::setToCloseButtonHint()
{
    setWindowMaxRestoreButtonHidden(true);
    setWindowMinButtonHidden(true);
    setWindowIconHidden(true);
    setDragResizeable(false);
}

QFrame *SDialog::centralFrame()
{
    return ui->frame_central;
}

QFrame *SDialog::titleBarFrame()
{
    return ui->frame_titleBar;
}

QHBoxLayout *SDialog::mainToolBarLayout()
{
    return ui->horizontalLayout_mainToolBar;
}

QHBoxLayout *SDialog::titleBarButtonLayout()
{
    return ui->horizontalLayout_titleBarToolButton;
}

QHBoxLayout *SDialog::titleBarToolBarLayout()
{
    return ui->horizontalLayout_titleBarToolBar;
}

QHBoxLayout *SDialog::statusBarLayout()
{
    return ui->horizontalLayout_statuBar;
}

bool SDialog::getDragResizeable() const
{
    return m_bDragResizeable;
}

void SDialog::setDragResizeable(bool b)
{
    m_bDragResizeable = b;

    if (!b) {
        ui->frame_border_left->setCursor(QCursor(Qt::ArrowCursor));
        ui->frame_border_right->setCursor(QCursor(Qt::ArrowCursor));
        ui->frame_border_top->setCursor(QCursor(Qt::ArrowCursor));
        ui->frame_border_bottom->setCursor(QCursor(Qt::ArrowCursor));
        ui->frame_border_topLeft->setCursor(QCursor(Qt::ArrowCursor));
        ui->frame_border_topRight->setCursor(QCursor(Qt::ArrowCursor));
        ui->frame_border_bottomLeft->setCursor(QCursor(Qt::ArrowCursor));
        ui->frame_border_bottomRight->setCursor(QCursor(Qt::ArrowCursor));
    } else {
        ui->frame_border_left->setCursor(QCursor(Qt::SizeHorCursor));
        ui->frame_border_right->setCursor(QCursor(Qt::SizeHorCursor));
        ui->frame_border_top->setCursor(QCursor(Qt::SizeVerCursor));
        ui->frame_border_bottom->setCursor(QCursor(Qt::SizeVerCursor));
        ui->frame_border_topLeft->setCursor(QCursor(Qt::SizeFDiagCursor));
        ui->frame_border_topRight->setCursor(QCursor(Qt::SizeBDiagCursor));
        ui->frame_border_bottomLeft->setCursor(QCursor(Qt::SizeBDiagCursor));
        ui->frame_border_bottomRight->setCursor(QCursor(Qt::SizeFDiagCursor));
    }
}

bool SDialog::eventFilter(QObject *o, QEvent *e)
{
    if (e->type() == QEvent::WindowTitleChange)
        ui->label_windowTitle->setText(windowTitle());
    else if (e->type() == QEvent::WindowIconChange)
        ui->toolButton_windowIcon->setIcon(windowIcon());
    else if (e->type() == QEvent::Resize) {
        if (isMaximized()) {
            ui->toolButton_windowMaxRestore->setIcon(style()->standardIcon(QStyle::SP_TitleBarNormalButton));
            setBorderFramesHidden(true);
        } else {
            ui->toolButton_windowMaxRestore->setIcon(style()->standardIcon(QStyle::SP_TitleBarMaxButton));
            setBorderFramesHidden(false);
        }
    }
    return QDialog::eventFilter(o, e);
}

void  SDialog::mousePressEvent(QMouseEvent *e)
{
    if (Qt::LeftButton == e->button()) {
        m_bLeftMouseOnPressing = true;
        m_leftMousePressPos = e->pos();

        QRect titleBarRect = ui->frame_titleBar->geometry();
        QPoint p = ui->frame_background->pos();
        titleBarRect.moveTop(p.y());

        if (ui->frame_border_left->geometry().contains(m_leftMousePressPos)) {  //left
            m_iLeftMousePressRegion = Left;
        } else if (ui->frame_border_right->geometry().contains(m_leftMousePressPos)) {  //right
            m_iLeftMousePressRegion = Right;
        } else if (ui->frame_border_top->geometry().contains(m_leftMousePressPos)) {     //top
            m_iLeftMousePressRegion = Top;
        } else if (ui->frame_border_bottom->geometry().contains(m_leftMousePressPos)) {   //bottom
            m_iLeftMousePressRegion = Bottom;
        } else if (ui->frame_border_topLeft->geometry().contains(m_leftMousePressPos)) {  //topLeft
            m_iLeftMousePressRegion = TopLeft;
        } else if (ui->frame_border_topRight->geometry().contains(m_leftMousePressPos)) {  //topRight
            m_iLeftMousePressRegion = TopRight;
        } else if (ui->frame_border_bottomLeft->geometry().contains(m_leftMousePressPos)) { //bottomLeft
            m_iLeftMousePressRegion = BottomLeft;
        } else if (ui->frame_border_bottomRight->geometry().contains(m_leftMousePressPos)) { //bottomRight
            m_iLeftMousePressRegion = BottomRight;
        } else if (titleBarRect.contains(m_leftMousePressPos)) {
            m_iLeftMousePressRegion = TitleBar;
        } else {
            m_iLeftMousePressRegion = CentralFrame;
        }
    }
}

void  SDialog::mouseReleaseEvent(QMouseEvent *)
{
    m_bLeftMouseOnPressing = false;
}

void  SDialog::mouseMoveEvent(QMouseEvent *e)
{
    if (m_bLeftMouseOnPressing) {

        if (isMaximized())
            return;

        //move dialog
        if (TitleBar == m_iLeftMousePressRegion) {
            if (!isMaximized()) {
                QDesktopWidget w;
                QPoint posMoveTo = e->globalPos() - m_leftMousePressPos;
                if (posMoveTo.y() >= w.availableGeometry().bottom() - 40)
                    posMoveTo.setY(w.availableGeometry().bottom() - 40);
                move(posMoveTo);
            }
        }

        //resize dialog by dragging border
        int w = getBorderFramesWidth();
        if (!getDragResizeable())
            return;
        if (Left == m_iLeftMousePressRegion) {             //left edge
            QRect r = geometry();
            r.setLeft(e->globalPos().x() - w);
            if (r.width() >= minimumWidth())
                setGeometry(r);
        } else if (Right == m_iLeftMousePressRegion) {  //right edge
            QRect r = geometry();
            r.setRight(e->globalPos().x() + w);
            if (r.width() >= minimumWidth())
                setGeometry(r);
        } else if (Top == m_iLeftMousePressRegion) {       //top edge
            QRect r = geometry();
            r.setTop(e->globalPos().y() - w);
            if (r.height() >= minimumHeight())
                setGeometry(r);
        } else if (Bottom == m_iLeftMousePressRegion) {   //bottom edge
            QRect r = geometry();
            r.setBottom(e->globalPos().y() + w);
            if (r.height() >= minimumHeight())
                setGeometry(r);
        } else if (TopLeft == m_iLeftMousePressRegion) {  //topLeft corner
            QRect r = geometry();
            r.setTopLeft(e->globalPos() - QPoint(w, w));

            if (r.height() < minimumHeight())
                r.setTop(r.bottom() - minimumHeight());
            if (r.width() < minimumWidth())
                r.setLeft(r.right() - minimumWidth());
            setGeometry(r);
        } else if (TopRight == m_iLeftMousePressRegion) {  //topRight corner
            QRect r = geometry();
            r.setTopRight(e->globalPos() - QPoint(-w, w));

            if (r.height() < minimumHeight())
                r.setTop(r.bottom() - minimumHeight());
            if (r.width() < minimumWidth())
                r.setRight(r.left() + minimumWidth());
            setGeometry(r);
        } else if (BottomLeft == m_iLeftMousePressRegion) { //bottomLeft corner
            QRect r = geometry();
            r.setBottomLeft(e->globalPos() - QPoint(w, -w));

            if (r.height() < minimumHeight())
                r.setBottom(r.top() + minimumHeight());
            if (r.width() < minimumWidth())
                r.setLeft(r.right() - minimumWidth());
            setGeometry(r);
        } else if (BottomRight == m_iLeftMousePressRegion) { //bottomRight corner
            QRect r = geometry();
            r.setBottomRight(e->globalPos() - QPoint(-w, -w));

            if (r.height() < minimumHeight())
                r.setBottom(r.top() + minimumHeight());
            if (r.width() < minimumWidth())
                r.setRight(r.left() + minimumWidth());
            setGeometry(r);
        }
    }
}

void  SDialog::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (ui->frame_titleBar->geometry().contains(e->pos())
            && ui->toolButton_windowMaxRestore->isVisible())
        on_toolButton_windowMaxRestore_clicked();
}

void SDialog::paintEvent(QPaintEvent *e)
{
    if (isMaximized())
        QDialog::paintEvent(e);
    else {
        QPainter p(this);
        int w = getBorderFramesWidth();
        QImage img(":/resources/images/others/border_shadow_black.png");
        QRect r_top(w, 0, width()-2*w, w),
                r_bottom(w, height()-w, width()-2*w, w),
                r_left(0, w, w, height()-2*w),
                r_right(width()-w, w, w, height()-2*w),
                r_topLeft(0, 0, w, w),
                r_topRight(width()-w, 0, w, w),
                r_bottomLeft(0, height()-w, w, w),
                r_bottomRight(width()-w, height()-w, w, w);

        p.drawImage(r_top, img.copy(w, 0, img.width()-2*w, w));
        p.drawImage(r_bottom, img.copy(w, img.height()-w, img.width()-2*w, w));
        p.drawImage(r_left, img.copy(0, w, w, img.height()-2*w));
        p.drawImage(r_right, img.copy(img.width()-w, w, w, img.height()-2*w));

        p.drawImage(r_topLeft, img.copy(0, 0, w, w));
        p.drawImage(r_topRight, img.copy(img.width()-w, 0, w, w));
        p.drawImage(r_bottomLeft, img.copy(0, img.height()-w, w, w));
        p.drawImage(r_bottomRight, img.copy(img.width()-w, img.height()-w, w, w));
    }
}

void SDialog::keyPressEvent(QKeyEvent *e)
{
    if (Qt::Key_Escape != e->key())
        QDialog::keyPressEvent(e);
}

void SDialog::on_toolButton_windowMin_clicked()
{
    showMinimized();
}

void SDialog::on_toolButton_windowMaxRestore_clicked()
{
    if (isMaximized())
        showNormal();
    else
        showMaximized();
}

void SDialog::on_toolButton_windowClose_clicked()
{
    close();
}
