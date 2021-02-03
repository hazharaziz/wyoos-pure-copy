#include <common/types.h>
#include <gui/window.h>
#include <drivers/mouse.h>


using namespace myos::common;
using namespace myos::gui;
using namespace myos::drivers;



Window::Window(Widget* parent, myos::common::int32_t x,
    myos::common::int32_t y, myos::common::int32_t w,
    myos::common::int32_t h, myos::common::uint8_t r,
    myos::common::uint8_t g, myos::common::uint8_t b)
  : CompositeWidget(parent, x, y, w, h, r, g, b)
{
    Dragging = false;
}

Window::~Window()
{

}

void Window::OnMouseDown(myos::common::int32_t x, myos::common::int32_t y, myos::common::uint8_t button)
{
    Dragging = button == 1;
    CompositeWidget::OnMouseDown(x, y, button);
}

void Window::OnMouseUp(myos::common::int32_t x, myos::common::int32_t y, myos::common::uint8_t button)
{
    Dragging = false;
    CompositeWidget::OnMouseUp(x, y, button);
}

void Window::OnMouseMove(myos::common::int32_t oldX, myos::common::int32_t oldY, myos::common::int32_t newX, myos::common::int32_t newY)
{
    if (Dragging)
    {
        this->x += newX - oldX;
        this->y += newY - oldY;
    }
    CompositeWidget::OnMouseMove(oldX, oldY, newX, newY);
}

