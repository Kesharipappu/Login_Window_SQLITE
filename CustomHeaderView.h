#ifndef CUSTOMHEADERVIEW_H
#define CUSTOMHEADERVIEW_H

#include <QHeaderView>
#include <QPainter>

class CustomHeaderView : public QHeaderView {
    Q_OBJECT

public:
    CustomHeaderView(Qt::Orientation orientation, QWidget *parent = nullptr)
        : QHeaderView(orientation, parent) {}

protected:
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const override {
        QHeaderView::paintSection(painter, rect, logicalIndex);

        if (rect.isValid()) {
            // Draw the sort indicator at the right end of the header section
            QStyleOptionHeader opt;
            initStyleOption(&opt);
            opt.rect = rect;
            opt.section = logicalIndex;

            if (sortIndicatorSection() == logicalIndex) {
                opt.sortIndicator = (sortIndicatorOrder() == Qt::AscendingOrder) ? QStyleOptionHeader::SortUp : QStyleOptionHeader::SortDown;
            } else {
                opt.sortIndicator = QStyleOptionHeader::None;
            }

            if (opt.sortIndicator != QStyleOptionHeader::None) {
                int indicatorWidth = style()->pixelMetric(QStyle::PM_HeaderMarkSize, &opt, this);
                QRect indicatorRect(rect.right() - indicatorWidth - 4, rect.center().y() - indicatorWidth / 2, indicatorWidth, indicatorWidth);
                opt.rect = indicatorRect;
                style()->drawPrimitive(QStyle::PE_IndicatorHeaderArrow, &opt, painter, this);
            }
        }
    }
};

#endif // CUSTOMHEADERVIEW_H
