#ifndef CLICKEDBTN_H
#define CLICKEDBTN_H
#include <QPushButton>

class ClickedBtn:public QPushButton
{
    Q_OBJECT
public:
    ClickedBtn(QWidget *parent = nullptr);
    ~ClickedBtn();
    void SetState(QString nomal, QString hover, QString press);
protected:
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
           // Qt5+ 使用 QEnterEvent
    virtual void enterEvent(QEnterEvent *event) override;
#else
           // Qt4 使用 QEvent
    virtual void enterEvent(QEvent *event) override;
#endif
    virtual void leaveEvent(QEvent *event) override; // 鼠标离开
    virtual void mousePressEvent(QMouseEvent *event) override; // 鼠标按下
    virtual void mouseReleaseEvent(QMouseEvent *event) override; // 鼠标释放
private:
    QString _normal;
    QString _hover;
    QString _press;
};

#endif // CLICKEDBTN_H
