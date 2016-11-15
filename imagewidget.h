#ifndef CROPGENERALWIDGET_H
#define CROPGENERALWIDGET_H

#include <QWidget>

class Form ;

class ImageWidget : public QWidget
{
    Q_OBJECT
public:
    ImageWidget(Form *parent = 0);
    virtual QSize sizeHint() const;
    ~ImageWidget();
signals:

public slots:
    void setFullImage(QImage &image);
    void setImage(QImage &image);
protected :
    virtual void	paintEvent(QPaintEvent * event);
    QImage background;
    QTransform imageToScreen,screenToImage ;
    Form * form ;
    void resizeEvent(QResizeEvent *event);
private :


};

#endif // CROPGENERALWIDGET_H
