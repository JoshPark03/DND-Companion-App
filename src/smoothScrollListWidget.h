/*
Name: smoothScrollListWidget.h
Description: Custom list widget class that overrides the default list widget scrolling behavior to allow for smoother scrolling
Authors: Zachary Craig
Other Sources: ...
Date Created: 12/06/2024
Last Modified: 12/6/2024
*/
#include <QApplication>
#include <QListWidget>
#include <QScrollBar>
#include <QWheelEvent>
#include <QMouseEvent>

// Didn't like how the default list widget snapped when scrolling, so this is a custom class that overrides the scroll behavior
class SmoothScrollListWidget : public QListWidget
{
    Q_OBJECT

    public:
        explicit SmoothScrollListWidget(QWidget *parent = nullptr)
            : QListWidget(parent)
        {
            // Set scrolling mode to pixel-based
            setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
            
            // Set step increments for smoother scrolling
            verticalScrollBar()->setSingleStep(100);  // 1 pixel per scroll step
            verticalScrollBar()->setPageStep(20);   // Adjust for smoother scrolling across more items
        }

    protected:
        void wheelEvent(QWheelEvent *event) override
        {
            // Adjust the wheel movement to control the scrolling speed
            if (event->angleDelta().y() != 0)
            {
                // Instead of scrolling one full page at a time, scroll 1 pixel per step
                verticalScrollBar()->setValue(verticalScrollBar()->value() - event->angleDelta().y() / 10); // 10 as the divisor seems to be a good speed
            }
            else
            {
                // If no delta, call base class handler
                QListWidget::wheelEvent(event);
            }
        }
};