#ifndef _itemshopcell_cpp
#define _itemshopcell_cpp

#include "itemshopcell.h"

ItemShopCell::ItemShopCell(const String& name, const UI::RenderableRect& Rect, const String& ItemName, UI::Layer* parent)
    : UI::Cell(name,parent)
{
    Real LineHeight = Rect.Relative ? Rect.Size.Y * 0.25 : (Rect.Size.Y / Parent->GetParent()->GetViewportDimensions().Y) * 0.25;
    ItemCaption = new UI::Caption(name,Rect,LineHeight,ItemName,parent);
}

ItemShopCell::~ItemShopCell()
{
    delete ItemCaption;
}

void ItemShopCell::UpdateImpl(bool Force)
{

}

void ItemShopCell::SetVisibleImpl(bool visible)
{
    ItemCaption->SetVisible(visible);
}

bool ItemShopCell::CheckMouseHoverImpl()
{
    if(ItemCaption->CheckMouseHover()) return true;
    else return false;
}

void ItemShopCell::SetPosition(const Vector2& Position)
{
    RelSize = Position;
    ItemCaption->SetPosition(Position);
}

void ItemShopCell::SetActualPosition(const Vector2& Position)
{
    RelSize = Position / Parent->GetParent()->GetViewportDimensions();
    ItemCaption->SetActualPosition(Position);
}

void ItemShopCell::SetSize(const Vector2& Size)
{
    RelSize = Size;
    ItemCaption->SetSize(Size);
}

void ItemShopCell::SetActualSize(const Vector2& Size)
{
    RelSize = Size / Parent->GetParent()->GetViewportDimensions();
    ItemCaption->SetActualSize(Size);
}

void ItemShopCell::DoSelectedItems()
{

}

void ItemShopCell::UpdateDimensions()
{
    UI::WidgetResult Result = UI::ViewportUpdateTool::UpdateWidget(this);
    RelPosition = Result.first / UI::ViewportUpdateTool::GetNewSize();
    RelSize = Result.second / UI::ViewportUpdateTool::GetNewSize();
    ItemCaption->UpdateDimensions();
    SetPosition(RelPosition);
}

UI::Caption* ItemShopCell::GetItemCaption()
{
    return ItemCaption;
}

ItemShopCB::ItemShopCB()
{

}

ItemShopCB::~ItemShopCB()
{

}

void ItemShopCB::SetCaller(UI::Cell* Caller)
{

}

void ItemShopCB::DoSelectedItems()
{

}

void ItemShopCB::DoUnselectedItems()
{

}

#endif
