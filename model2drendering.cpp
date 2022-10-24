#include "model2drendering.h"

#include <QBrush>
#include <QPainter>
#include <QPen>

Model2DRendering::Model2DRendering(QWidget *parent) : QWidget(parent)
{

}

void Model2DRendering::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QBrush brush(m_backgroundColor);
    QPen pen(m_backgroundColor);
    painter.setBrush(brush);
    painter.setPen(pen);
    painter.drawRect(rect());
}
