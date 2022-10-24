#ifndef MODEL2DRENDERING_H
#define MODEL2DRENDERING_H

#include <QWidget>

class Model2DRendering : public QWidget
{
    Q_OBJECT
public:
    explicit Model2DRendering(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event) override;

signals:

private:
    QColor m_backgroundColor = QColor(0, 0, 0);

};

#endif // MODEL2DRENDERING_H
