#include <gui/widget.h>
#include <common/types.h>
#include <common/graphicsContext.h>


using namespace myos::gui;
using namespace myos::common;


Widget::Widget(
    Widget* parent,
    int32_t x,
    int32_t y,
    int32_t w,
    int32_t h,
    uint8_t r,
    uint8_t g,
    uint8_t b)
  : KeyboardEventHandler()
{
    this->parent = parent;
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->r = r;
    this->g = g;
    this->b = b;
    this->Focusable = true;
}

Widget::~Widget()
{

}

void Widget::GetFocus(Widget* widget)
{
    if (parent != 0)
    {
        parent->GetFocus(widget);
    }
}

void Widget::ModelToScreen(int32_t &x, int32_t &y)
{
    if(parent != 0)
        parent->ModelToScreen(x,y);
    x += this->x;
    y += this->y;
}

void Widget::Draw(GraphicsContext* gc)
{
    int X = 0;
    int Y = 0;
    ModelToScreen(X, Y);
    gc->FillRectangle(X, Y, w, h, r, g, b);
}

void Widget::OnMouseDown(int32_t x, int32_t y, uint8_t button)
{
    if (Focusable)
    {
        GetFocus(this);
    }
}

void Widget::OnMouseUp(int32_t x, int32_t y, uint8_t button)
{

}

void Widget::OnMouseMove(int32_t oldX, int32_t oldY, int32_t newX, int32_t newY)
{

}

bool Widget::ContainsCoordinate(int32_t x, int32_t y)
{
    return this->x <= x && x < this->x + this->w
            && this->y <= y && y < this->y + this->h;
}








CompositeWidget::CompositeWidget(
    Widget* parent, int32_t x, int32_t y, int32_t w, 
    int32_t h, uint8_t r, uint8_t g, uint8_t b)
  : Widget(parent, x, y, w, h, r, g, b)
{
    this->focusedChild = 0;
    this->numChildren = 0;
}

CompositeWidget::~CompositeWidget()
{

}


void CompositeWidget::GetFocus(Widget* widget)
{
    this->focusedChild = widget;
    if (parent != 0)
    {
        parent->GetFocus(this);
    }
}

bool CompositeWidget::AddChild(Widget* child)
{
    if (numChildren >= 100)
    {
        return false;
    }
    children[numChildren++] = child;
    return true;
}

void CompositeWidget::Draw(GraphicsContext* gc)
{
    Widget::Draw(gc);
    for (int i = numChildren - 1; i >= 0; --i)
    {
        children[i]->Draw(gc);
    }
}

void CompositeWidget::OnMouseDown(int32_t x, int32_t y, uint8_t button)
{
    for (int i = 0; i < numChildren; ++i)
    {
        if (children[i]->ContainsCoordinate(x - this->x, y - this->y))
        {
            children[i]->OnMouseDown(x - this->x, y - this->y, button); 
            break;
        }
    }
}   

void CompositeWidget::OnMouseUp(int32_t x, int32_t y, uint8_t button)
{
    for (int i = 0; i < numChildren; ++i)
    {
        if (children[i]->ContainsCoordinate(x - this->x, y - this->y))
        {
            children[i]->OnMouseUp(x - this->x, y - this->y, button); 
            break;
        }
    }

}   

void CompositeWidget::OnMouseMove(int32_t oldX, int32_t oldY, int32_t newX, int32_t newY)
{
    int firstChild = -1;
    for (int i = 0; i < numChildren; ++i)
    {
        if (children[i]->ContainsCoordinate(oldX - this->x, oldY - this->y))
        {
            children[i]->OnMouseMove(oldX - this->x, oldY - this->y, newX - this->x, newY - this->y);
            firstChild = i;
            break;
        }
    }

     for (int i = 0; i < numChildren; ++i)
    {
        if (children[i]->ContainsCoordinate(newX - this->x, newY - this->y))
        {
            if (firstChild != i)
            {
                children[i]->OnMouseMove(oldX - this->x, oldY - this->y, newX - this->x, newY - this->y);
            }
            break;
        }
    }

}   

void CompositeWidget::OnKeyDown(char ch)
{
    if (focusedChild != 0)
    {
        this->focusedChild->OnKeyDown(ch);
    }

}

void CompositeWidget::OnKeyUp(char ch)
{
    if (focusedChild != 0)
    {
        this->focusedChild->OnKeyUp(ch);
    }
}

