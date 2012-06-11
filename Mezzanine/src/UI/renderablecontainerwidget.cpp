//© Copyright 2010 - 2012 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/
#ifndef _uirenderablecontainerwidget_cpp
#define _uirenderablecontainerwidget_cpp

#include "UI/renderablecontainerwidget.h"
#include "uimanager.h"
#include "UI/layer.h"
#include "UI/screen.h"
#include "UI/caption.h"
#include "UI/markuptext.h"
#include "UI/linelist.h"
#include "UI/rectangle.h"
#include "UI/button.h"
#include "UI/textbutton.h"
#include "UI/scrollbar.h"
#include "UI/listbox.h"
#include "UI/checkbox.h"
#include "UI/spinner.h"
#include "UI/scrolledcellgrid.h"
#include "UI/pagedcellgrid.h"
#include "UI/dropdownlist.h"
#include "UI/tabset.h"
#include "UI/viewportupdatetool.h"
#include "inputquerytool.h"
#include "metacode.h"
#include "world.h"

namespace Mezzanine
{
    namespace UI
    {
        RenderableContainerWidget::RenderableContainerWidget(ConstString& name, const RenderableRect& Rect, Layer* PLayer)
            : Widget(name,PLayer),
              RenderablesAdded(0)
        {
            Type = Widget::W_GenericWidgetContainer;
            if(Rect.Relative)
            {
                RelPosition = Rect.Position;
                RelSize = Rect.Size;
            }else{
                RelPosition = Rect.Position / ParentLayer->GetParent()->GetViewportDimensions();
                RelSize = Rect.Size / ParentLayer->GetParent()->GetViewportDimensions();
            }

            WidgetBack = new Rectangle(Rect,ParentLayer);
            AddSubRenderable(0,RenderablePair(WidgetBack,NULL));
        }

        RenderableContainerWidget::~RenderableContainerWidget()
        {
            delete WidgetBack;
            for( std::vector<OffsetButtonInfo>::iterator it = Buttons.begin() ; it != Buttons.end() ; it++ )
            {
                UI::Button* button = (*it).Object;
                delete button;
            }
            Buttons.clear();

            for( std::vector<OffsetRectangleInfo>::iterator it = Rectangles.begin() ; it != Rectangles.end() ; it++ )
            {
                UI::Rectangle* rectangle = (*it).Object;
                delete rectangle;
            }
            Rectangles.clear();

            for( std::vector<OffsetCaptionInfo>::iterator it = Captions.begin() ; it != Captions.end() ; it++ )
            {
                UI::Caption* caption = (*it).Object;
                delete caption;
            }
            Captions.clear();

            for( std::vector<OffsetMarkupTextInfo>::iterator it = MarkupTexts.begin() ; it != MarkupTexts.end() ; it++ )
            {
                UI::MarkupText* markup = (*it).Object;
                delete markup;
            }
            MarkupTexts.clear();

            for( std::vector<OffsetWidgetInfo>::iterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
            {
                UI::Widget* widget = (*it).Object;
                delete widget;
            }
            Widgets.clear();
        }

        void RenderableContainerWidget::UpdateImpl(bool Force)
        {
        }

        void RenderableContainerWidget::SetVisibleImpl(bool visible)
        {
            WidgetBack->SetVisible(visible);
            for( std::vector<OffsetButtonInfo>::iterator it = Buttons.begin() ; it != Buttons.end() ; it++ )
            {
                (*it).Object->SetVisible(visible);
            }
            for( std::vector<OffsetRectangleInfo>::iterator it = Rectangles.begin() ; it != Rectangles.end() ; it++ )
            {
                (*it).Object->SetVisible(visible);
            }
            for( std::vector<OffsetCaptionInfo>::iterator it = Captions.begin() ; it != Captions.end() ; it++ )
            {
                (*it).Object->SetVisible(visible);
            }
            for( std::vector<OffsetMarkupTextInfo>::iterator it = MarkupTexts.begin() ; it != MarkupTexts.end() ; it++ )
            {
                (*it).Object->SetVisible(visible);
            }
            for( std::vector<OffsetWidgetInfo>::iterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
            {
                (*it).Object->SetVisible(visible);
            }
        }

        bool RenderableContainerWidget::CheckMouseHoverImpl()
        {
            if(HoveredButton)
            {
                if(HoveredButton->CheckMouseHover())
                    return true;
            }
            if(HoveredSubWidget)
            {
                if(HoveredSubWidget->CheckMouseHover())
                {
                    HoveredButton = HoveredSubWidget->GetHoveredButton();
                    return true;
                }
            }
            for( std::vector<OffsetButtonInfo>::iterator it = Buttons.begin() ; it != Buttons.end() ; it++ )
            {
                if((*it).Object->CheckMouseHover())
                {
                    HoveredSubWidget = NULL;
                    HoveredButton = (*it).Object;
                    return true;
                }
            }
            for( std::vector<OffsetWidgetInfo>::iterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
            {
                if((*it).Object->CheckMouseHover())
                {
                    HoveredSubWidget = (*it).Object;
                    HoveredButton = HoveredSubWidget->GetHoveredButton();
                    return true;
                }
            }
            if(WidgetBack->CheckMouseHover())
            {
                HoveredSubWidget = NULL;
                HoveredButton = NULL;
                return true;
            }
            return false;
        }

        void RenderableContainerWidget::SetLocation(const Vector2& Position)
        {
            WidgetBack->SetActualPosition(Position);
            for( std::vector<OffsetButtonInfo>::iterator it = Buttons.begin() ; it != Buttons.end() ; it++ )
            {
                (*it).Object->SetActualPosition(Position+(*it).Offset);
            }
            for( std::vector<OffsetRectangleInfo>::iterator it = Rectangles.begin() ; it != Rectangles.end() ; it++ )
            {
                (*it).Object->SetActualPosition(Position+(*it).Offset);
            }
            for( std::vector<OffsetCaptionInfo>::iterator it = Captions.begin() ; it != Captions.end() ; it++ )
            {
                (*it).Object->SetActualPosition(Position+(*it).Offset);
            }
            for( std::vector<OffsetMarkupTextInfo>::iterator it = MarkupTexts.begin() ; it != MarkupTexts.end() ; it++ )
            {
                (*it).Object->SetActualPosition(Position+(*it).Offset);
            }
            for( std::vector<OffsetWidgetInfo>::iterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
            {
                (*it).Object->SetActualPosition(Position+(*it).Offset);
            }
        }

        void RenderableContainerWidget::SetArea(const Vector2& Size)
        {
            Vector2 OldSize = WidgetBack->GetActualSize();
            Vector2 Position = WidgetBack->GetActualPosition();
            WidgetBack->SetActualSize(Size);
            for( std::vector<OffsetButtonInfo>::iterator it = Buttons.begin() ; it != Buttons.end() ; it++ )
            {
                (*it).Offset = CalculateOffset(Size,OldSize,(*it).Offset,(*it).Anchor);
                if((*it).Tether != RT_TetherNone)
                    (*it).Object->SetActualSize(CalculateSize(Size,OldSize,(*it).Object->GetActualSize(),(*it).Tether));
                (*it).Object->SetActualPosition(Position+(*it).Offset);
            }
            for( std::vector<OffsetRectangleInfo>::iterator it = Rectangles.begin() ; it != Rectangles.end() ; it++ )
            {
                (*it).Offset = CalculateOffset(Size,OldSize,(*it).Offset,(*it).Anchor);
                if((*it).Tether != RT_TetherNone)
                    (*it).Object->SetActualSize(CalculateSize(Size,OldSize,(*it).Object->GetActualSize(),(*it).Tether));
                (*it).Object->SetActualPosition(Position+(*it).Offset);
            }
            for( std::vector<OffsetCaptionInfo>::iterator it = Captions.begin() ; it != Captions.end() ; it++ )
            {
                (*it).Offset = CalculateOffset(Size,OldSize,(*it).Offset,(*it).Anchor);
                if((*it).Tether != RT_TetherNone)
                    (*it).Object->SetActualSize(CalculateSize(Size,OldSize,(*it).Object->GetActualSize(),(*it).Tether));
                (*it).Object->SetActualPosition(Position+(*it).Offset);
            }
            for( std::vector<OffsetMarkupTextInfo>::iterator it = MarkupTexts.begin() ; it != MarkupTexts.end() ; it++ )
            {
                (*it).Offset = CalculateOffset(Size,OldSize,(*it).Offset,(*it).Anchor);
                //if((*it).Tether != RT_TetherNone)
                //    (*it).Object->SetActualSize(CalculateSize(Size,OldSize,(*it).Object->GetActualSize(),(*it).Tether));
                (*it).Object->SetActualPosition(Position+(*it).Offset);
            }
            for( std::vector<OffsetWidgetInfo>::iterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
            {
                (*it).Offset = CalculateOffset(Size,OldSize,(*it).Offset,(*it).Anchor);
                if((*it).Tether != RT_TetherNone)
                    (*it).Object->SetActualSize(CalculateSize(Size,OldSize,(*it).Object->GetActualSize(),(*it).Tether));
                (*it).Object->SetActualPosition(Position+(*it).Offset);
            }
        }

        Vector2 RenderableContainerWidget::CalculateOffset(const Vector2& NewSize, const Vector2& OldSize, const Vector2& EleOffset, const UI::ResizeableAnchor& Anchor)
        {
            Vector2 NewOffset;
            switch (Anchor)
            {
                case RA_AnchorTopLeft:
                {
                    NewOffset = EleOffset;
                    break;
                }
                case RA_AnchorTop:
                {
                    NewOffset.X = EleOffset.X + ((NewSize.X - OldSize.X) * 0.5);
                    NewOffset.Y = EleOffset.Y;
                    break;
                }
                case RA_AnchorTopRight:
                {
                    NewOffset.X = EleOffset.X + (NewSize.X - OldSize.X);
                    NewOffset.Y = EleOffset.Y;
                    break;
                }
                case RA_AnchorLeft:
                {
                    NewOffset.X = EleOffset.X;
                    NewOffset.Y = EleOffset.Y + ((NewSize.Y - OldSize.Y) * 0.5);
                    break;
                }
                case RA_AnchorMiddle:
                {
                    NewOffset.X = EleOffset.X + ((NewSize.X - OldSize.X) * 0.5);
                    NewOffset.Y = EleOffset.Y + ((NewSize.Y - OldSize.Y) * 0.5);
                    break;
                }
                case RA_AnchorRight:
                {
                    NewOffset.X = EleOffset.X + (NewSize.X - OldSize.X);
                    NewOffset.Y = EleOffset.Y + ((NewSize.Y - OldSize.Y) * 0.5);
                    break;
                }
                case RA_AnchorBottomLeft:
                {
                    NewOffset.X = EleOffset.X;
                    NewOffset.Y = EleOffset.Y + (NewSize.Y - OldSize.Y);
                    break;
                }
                case RA_AnchorBottom:
                {
                    NewOffset.X = EleOffset.X + ((NewSize.X - OldSize.X) * 0.5);
                    NewOffset.Y = EleOffset.Y + (NewSize.Y - OldSize.Y);
                    break;
                }
                case RA_AnchorBottomRight:
                {
                    NewOffset = EleOffset + (NewSize - OldSize);
                    break;
                }
                default:
                    return NewOffset;
            }
            return NewOffset;
        }

        Vector2 RenderableContainerWidget::CalculateSize(const Vector2& NewSize, const Vector2& OldSize, const Vector2& EleSize, const UI::ResizeableTether& Tether)
        {
            Vector2 NewEleSize;
            switch (Tether)
            {
                case RT_TetherBoth:
                {
                    NewEleSize = EleSize + (NewSize - OldSize);
                    break;
                }
                case RT_TetherNone:
                {
                    NewEleSize = EleSize;
                    break;
                }
                case RT_TetherHorizontal:
                {
                    NewEleSize.X = EleSize.X + (NewSize.X - OldSize.X);
                    NewEleSize.Y = EleSize.Y;
                    break;
                }
                case RT_TetherVertical:
                {
                    NewEleSize.X = EleSize.X;
                    NewEleSize.Y = EleSize.Y + (NewSize.Y - OldSize.Y);
                    break;
                }
                default:
                    return NewEleSize;
            }
            return NewEleSize;
        }

        void RenderableContainerWidget::SetPosition(const Vector2& Position)
        {
            RelPosition = Position;
            Vector2 ActPos = Position * ParentLayer->GetParent()->GetViewportDimensions();
            SetLocation(ActPos);
        }

        void RenderableContainerWidget::SetActualPosition(const Vector2& Position)
        {
            RelPosition = Position / ParentLayer->GetParent()->GetViewportDimensions();
            SetLocation(Position);
        }

        void RenderableContainerWidget::SetSize(const Vector2& Size)
        {
            RelSize = Size;
            Vector2 ActSize = Size * ParentLayer->GetParent()->GetViewportDimensions();
            SetArea(ActSize);
        }

        void RenderableContainerWidget::SetActualSize(const Vector2& Size)
        {
            RelSize = Size / ParentLayer->GetParent()->GetViewportDimensions();
            SetArea(Size);
        }

        void RenderableContainerWidget::UpdateDimensions()
        {
            const Vector2& WinDim = ParentLayer->GetParent()->GetViewportDimensions();
            //const Vector2& OldViewportSize = ViewportUpdateTool::GetOldSize();
            Real Scale = ViewportUpdateTool::GetNewSize().Y / ViewportUpdateTool::GetOldSize().Y;
            WidgetResult Result = ViewportUpdateTool::UpdateWidget(this);
            RelPosition = Result.first / ViewportUpdateTool::GetNewSize();
            RelSize = Result.second / ViewportUpdateTool::GetNewSize();
            WidgetBack->UpdateDimensions();
            for( std::vector<OffsetButtonInfo>::iterator it = Buttons.begin() ; it != Buttons.end() ; it++ )
            {
                (*it).Offset = ((*it).Offset * Scale);
                (*it).Object->UpdateDimensions();
            }
            for( std::vector<OffsetRectangleInfo>::iterator it = Rectangles.begin() ; it != Rectangles.end() ; it++ )
            {
                (*it).Offset = ((*it).Offset * Scale);
                (*it).Object->UpdateDimensions();
            }
            for( std::vector<OffsetCaptionInfo>::iterator it = Captions.begin() ; it != Captions.end() ; it++ )
            {
                (*it).Offset = ((*it).Offset * Scale);
                (*it).Object->UpdateDimensions();
            }
            for( std::vector<OffsetMarkupTextInfo>::iterator it = MarkupTexts.begin() ; it != MarkupTexts.end() ; it++ )
            {
                (*it).Offset = ((*it).Offset * Scale);
                (*it).Object->UpdateDimensions();
            }
            for( std::vector<OffsetWidgetInfo>::iterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
            {
                (*it).Offset = ((*it).Offset * Scale);
                (*it).Object->UpdateDimensions();
            }
            SetPosition(RelPosition);
        }

        Rectangle* RenderableContainerWidget::GetWidgetBack()
        {
            return WidgetBack;
        }

        Button* RenderableContainerWidget::CreateButton(ConstString& Name, const RenderableRect& Rect)
        {
            RenderablesAdded++;
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * ParentLayer->GetParent()->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            OffsetButtonInfo buttoninfo(new Button(Name,Rect,ParentLayer),UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Buttons.push_back(buttoninfo);
            AddSubRenderable(RenderablesAdded,RenderablePair(buttoninfo.Object,NULL));
            if(Manager->ButtonAutoRegisterEnabled())
            {
                std::vector<MetaCode::InputCode>* Codes = Manager->GetAutoRegisteredCodes();
                for( Whole X = 0 ; X < Codes->size() ; X++ )
                    buttoninfo.Object->BindActivationKeyOrButton(Codes->at(X));
            }
            buttoninfo.Object->SetVisible(Visible);
            return buttoninfo.Object;
        }

        TextButton* RenderableContainerWidget::CreateTextButton(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, ConstString& Text)
        {
            RenderablesAdded++;
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * ParentLayer->GetParent()->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            TextButton* tbutton = new TextButton(Name,Rect,Glyph,Text,ParentLayer);
            OffsetButtonInfo tbuttoninfo(tbutton,UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Buttons.push_back(tbuttoninfo);
            AddSubRenderable(RenderablesAdded,RenderablePair(tbutton,NULL));
            if(Manager->ButtonAutoRegisterEnabled())
            {
                std::vector<MetaCode::InputCode>* Codes = Manager->GetAutoRegisteredCodes();
                for( Whole X = 0 ; X < Codes->size() ; X++ )
                    tbutton->BindActivationKeyOrButton(Codes->at(X));
            }
            tbutton->SetVisible(Visible);
            return tbutton;
        }

        TextButton* RenderableContainerWidget::CreateTextButton(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, ConstString& Text)
        {
            RenderablesAdded++;
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * ParentLayer->GetParent()->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            TextButton* tbutton = new TextButton(Name,Rect,LineHeight,Text,ParentLayer);
            OffsetButtonInfo tbuttoninfo(tbutton,UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Buttons.push_back(tbuttoninfo);
            AddSubRenderable(RenderablesAdded,RenderablePair(tbutton,NULL));
            if(Manager->ButtonAutoRegisterEnabled())
            {
                std::vector<MetaCode::InputCode>* Codes = Manager->GetAutoRegisteredCodes();
                for( Whole X = 0 ; X < Codes->size() ; X++ )
                    tbutton->BindActivationKeyOrButton(Codes->at(X));
            }
            tbutton->SetVisible(Visible);
            return tbutton;
        }

        Button* RenderableContainerWidget::GetButton(ConstString& Name)
        {
            for ( std::vector<OffsetButtonInfo>::iterator it = Buttons.begin() ; it != Buttons.end() ; it++ )
            {
                if ( Name == (*it).Object->GetName() )
                {
                    Button* button = (*it).Object;
                    return button;
                }
            }
            return 0;
        }

        Button* RenderableContainerWidget::GetButton(const Whole& Index)
        {
            return Buttons[Index].Object;
        }

        OffsetButtonInfo* RenderableContainerWidget::GetOffsetButtonInfo(ConstString& Name)
        {
            for ( std::vector<OffsetButtonInfo>::iterator it = Buttons.begin() ; it != Buttons.end() ; it++ )
            {
                if ( Name == (*it).Object->GetName() )
                {
                    OffsetButtonInfo* Offset = &(*it);
                    return Offset;
                }
            }
            return 0;
        }

        OffsetButtonInfo* RenderableContainerWidget::GetOffsetButtonInfo(const Whole& Index)
        {
            OffsetButtonInfo* Offset = &Buttons[Index];
            return Offset;
        }

        Whole RenderableContainerWidget::GetNumButtons()
        {
            return Buttons.size();
        }

        void RenderableContainerWidget::DestroyButton(UI::Button* ToBeDestroyed)
        {
            for ( std::vector<OffsetButtonInfo>::iterator it = Buttons.begin() ; it != Buttons.end() ; it++ )
            {
                if ( ToBeDestroyed == (*it).Object )
                {
                    delete ToBeDestroyed;
                    Buttons.erase(it);
                    return;
                }
            }
        }

        Rectangle* RenderableContainerWidget::CreateRectangle(const RenderableRect& Rect)
        {
            RenderablesAdded++;
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * ParentLayer->GetParent()->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            OffsetRectangleInfo rectangle(new Rectangle(Rect,ParentLayer),UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Rectangles.push_back(rectangle);
            AddSubRenderable(RenderablesAdded,RenderablePair(rectangle.Object,NULL));
            rectangle.Object->SetVisible(Visible);
            return rectangle.Object;
        }

        Rectangle* RenderableContainerWidget::GetRectangle(const Whole& Index)
        {
            return Rectangles[Index].Object;
        }

        OffsetRectangleInfo* RenderableContainerWidget::GetOffsetRectangleInfo(const Whole& Index)
        {
            OffsetRectangleInfo* Offset = &Rectangles[Index];
            return Offset;
        }

        Whole RenderableContainerWidget::GetNumRectangles()
        {
            return Rectangles.size();
        }

        void RenderableContainerWidget::DestroyRectangle(UI::Rectangle* ToBeDestroyed)
        {
            for ( std::vector<OffsetRectangleInfo>::iterator it = Rectangles.begin() ; it != Rectangles.end() ; it++ )
            {
                if ( ToBeDestroyed == (*it).Object )
                {
                    delete ToBeDestroyed;
                    Rectangles.erase(it);
                    return;
                }
            }
        }

        Caption* RenderableContainerWidget::CreateCaption(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String& Text)
        {
            RenderablesAdded++;
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * ParentLayer->GetParent()->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            OffsetCaptionInfo caption(new Caption(Name,Rect,Glyph,Text,ParentLayer),UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Captions.push_back(caption);
            AddSubRenderable(RenderablesAdded,RenderablePair(caption.Object,NULL));
            caption.Object->SetVisible(Visible);
            return caption.Object;
        }

        Caption* RenderableContainerWidget::CreateCaption(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& Text)
        {
            RenderablesAdded++;
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * ParentLayer->GetParent()->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            OffsetCaptionInfo caption(new Caption(Name,Rect,LineHeight,Text,ParentLayer),UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Captions.push_back(caption);
            AddSubRenderable(RenderablesAdded,RenderablePair(caption.Object,NULL));
            caption.Object->SetVisible(Visible);
            return caption.Object;
        }

        Caption* RenderableContainerWidget::GetCaption(ConstString& Name)
        {
            for ( std::vector<OffsetCaptionInfo>::iterator it = Captions.begin() ; it != Captions.end() ; it++ )
            {
                if ( Name == (*it).Object->GetName() )
                {
                    Caption* caption = (*it).Object;
                    return caption;
                }
            }
            return 0;
        }

        Caption* RenderableContainerWidget::GetCaption(const Whole& Index)
        {
            return Captions[Index].Object;
        }

        OffsetCaptionInfo* RenderableContainerWidget::GetOffsetCaptionInfo(ConstString& Name)
        {
            for ( std::vector<OffsetCaptionInfo>::iterator it = Captions.begin() ; it != Captions.end() ; it++ )
            {
                if ( Name == (*it).Object->GetName() )
                {
                    OffsetCaptionInfo* Offset = &(*it);
                    return Offset;
                }
            }
            return 0;
        }

        OffsetCaptionInfo* RenderableContainerWidget::GetOffsetCaptionInfo(const Whole& Index)
        {
            OffsetCaptionInfo* Offset = &Captions[Index];
            return Offset;
        }

        Whole RenderableContainerWidget::GetNumCaptions()
        {
            return Captions.size();
        }

        void RenderableContainerWidget::DestroyCaption(UI::Caption* ToBeDestroyed)
        {
            for ( std::vector<OffsetCaptionInfo>::iterator it = Captions.begin() ; it != Captions.end() ; it++ )
            {
                if ( ToBeDestroyed == (*it).Object )
                {
                    delete ToBeDestroyed;
                    Captions.erase(it);
                    return;
                }
            }
        }

        MarkupText* RenderableContainerWidget::CreateMarkupText(ConstString& Name, const RenderableRect& Rect, const Whole& Glyph, const String& Text)
        {
            RenderablesAdded++;
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * ParentLayer->GetParent()->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            MarkupText* markup = new MarkupText(Name,Rect,Glyph,Text,ParentLayer);
            OffsetMarkupTextInfo markupinfo(markup,UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            MarkupTexts.push_back(markupinfo);
            AddSubRenderable(RenderablesAdded,RenderablePair(markupinfo.Object,NULL));
            markupinfo.Object->SetVisible(Visible);
            return markupinfo.Object;
        }

        MarkupText* RenderableContainerWidget::CreateMarkupText(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String& Text)
        {
            RenderablesAdded++;
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * ParentLayer->GetParent()->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            MarkupText* markup = new MarkupText(Name,Rect,LineHeight,Text,ParentLayer);
            OffsetMarkupTextInfo markupinfo(markup,UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            MarkupTexts.push_back(markupinfo);
            AddSubRenderable(RenderablesAdded,RenderablePair(markupinfo.Object,NULL));
            markupinfo.Object->SetVisible(Visible);
            return markupinfo.Object;
        }

        MarkupText* RenderableContainerWidget::GetMarkupText(ConstString& Name)
        {
            for ( std::vector<OffsetMarkupTextInfo>::iterator it = MarkupTexts.begin() ; it != MarkupTexts.end() ; it++ )
            {
                if ( Name == (*it).Object->GetName() )
                {
                    MarkupText* markup = (*it).Object;
                    return markup;
                }
            }
            return 0;
        }

        MarkupText* RenderableContainerWidget::GetMarkupText(const Whole& Index)
        {
            return MarkupTexts[Index].Object;
        }

        OffsetMarkupTextInfo* RenderableContainerWidget::GetOffsetMarkupTextInfo(ConstString& Name)
        {
            for ( std::vector<OffsetMarkupTextInfo>::iterator it = MarkupTexts.begin() ; it != MarkupTexts.end() ; it++ )
            {
                if ( Name == (*it).Object->GetName() )
                {
                    OffsetMarkupTextInfo* Offset = &(*it);
                    return Offset;
                }
            }
            return 0;
        }

        OffsetMarkupTextInfo* RenderableContainerWidget::GetOffsetMarkupTextInfo(const Whole& Index)
        {
            OffsetMarkupTextInfo* Offset = &MarkupTexts[Index];
            return Offset;
        }

        Whole RenderableContainerWidget::GetNumMarkupTexts()
        {
            return MarkupTexts.size();
        }

        void RenderableContainerWidget::DestroyMarkupText(UI::MarkupText* ToBeDestroyed)
        {
            for ( std::vector<OffsetMarkupTextInfo>::iterator it = MarkupTexts.begin() ; it != MarkupTexts.end() ; it++ )
            {
                if ( ToBeDestroyed == (*it).Object )
                {
                    delete ToBeDestroyed;
                    MarkupTexts.erase(it);
                    return;
                }
            }
        }

        Widget* RenderableContainerWidget::GetWidget(ConstString& Name)
        {
            for ( std::vector<OffsetWidgetInfo>::iterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
            {
                if ( Name == (*it).Object->GetName() )
                {
                    Widget* widget = (*it).Object;
                    return widget;
                }
            }
            return 0;
        }

        Widget* RenderableContainerWidget::GetWidget(const Whole& Index)
        {
            return Widgets[Index].Object;
        }

        OffsetWidgetInfo* RenderableContainerWidget::GetOffsetWidgetInfo(ConstString& Name)
        {
            for ( std::vector<OffsetWidgetInfo>::iterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
            {
                if ( Name == (*it).Object->GetName() )
                {
                    OffsetWidgetInfo* Offset = &(*it);
                    return Offset;
                }
            }
            return 0;
        }

        OffsetWidgetInfo* RenderableContainerWidget::GetOffsetWidgetInfo(const Whole& Index)
        {
            OffsetWidgetInfo* Offset = &Widgets[Index];
            return Offset;
        }

        Whole RenderableContainerWidget::GetNumWidgets()
        {
            return Widgets.size();
        }

        void RenderableContainerWidget::DestroyWidget(UI::Widget* ToBeDestroyed)
        {
            for ( std::vector<OffsetWidgetInfo>::iterator it = Widgets.begin() ; it != Widgets.end() ; it++ )
            {
                if ( ToBeDestroyed == (*it).Object )
                {
                    delete (*it).Object;
                    Widgets.erase(it);
                    return;
                }
            }
        }

        Scrollbar* RenderableContainerWidget::CreateScrollbar(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& Style)
        {
            RenderablesAdded++;
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * ParentLayer->GetParent()->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            Scrollbar* Scroll = new Scrollbar(Name,Rect,Style,ParentLayer);
            OffsetWidgetInfo ScrollInfo(Scroll,UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Widgets.push_back(ScrollInfo);
            AddSubRenderable(RenderablesAdded,RenderablePair(NULL,Scroll));
            Scroll->SetVisible(Visible);
            return Scroll;
        }

        CheckBox* RenderableContainerWidget::CreateCheckBox(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const String &LabelText)
        {
            RenderablesAdded++;
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * ParentLayer->GetParent()->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            CheckBox* Check = new CheckBox(Name,Rect,LineHeight,LabelText,ParentLayer);
            OffsetWidgetInfo CheckInfo(Check,UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Widgets.push_back(CheckInfo);
            AddSubRenderable(RenderablesAdded,RenderablePair(NULL,Check));
            Check->SetVisible(Visible);
            return Check;
        }

        ListBox* RenderableContainerWidget::CreateListBox(ConstString& Name, const RenderableRect& Rect, const UI::ScrollbarStyle& ScrollbarStyle)
        {
            RenderablesAdded++;
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * ParentLayer->GetParent()->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            ListBox* LB = new ListBox(Name,Rect,ScrollbarStyle,ParentLayer);
            OffsetWidgetInfo LBInfo(LB,UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Widgets.push_back(LBInfo);
            AddSubRenderable(RenderablesAdded,RenderablePair(NULL,LB));
            LB->SetVisible(Visible);
            return LB;
        }

        Spinner* RenderableContainerWidget::CreateSpinner(ConstString& Name, const RenderableRect& Rect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight)
        {
            RenderablesAdded++;
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * ParentLayer->GetParent()->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            Spinner* Spn = new Spinner(Name,Rect,SStyle,GlyphHeight,ParentLayer);
            OffsetWidgetInfo SpnInfo(Spn,UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Widgets.push_back(SpnInfo);
            AddSubRenderable(RenderablesAdded,RenderablePair(NULL,Spn));
            Spn->SetVisible(Visible);
            return Spn;
        }

        ScrolledCellGrid* RenderableContainerWidget::CreateScrolledCellGrid(ConstString& Name, const RenderableRect& Rect, const Real& Thickness, const UI::ScrollbarStyle& Style)
        {
            RenderablesAdded++;
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * ParentLayer->GetParent()->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            ScrolledCellGrid* SCG = new ScrolledCellGrid(Name,Rect,Thickness,Style,ParentLayer);
            OffsetWidgetInfo SCGInfo(SCG,UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Widgets.push_back(SCGInfo);
            AddSubRenderable(RenderablesAdded,RenderablePair(NULL,SCG));
            SCG->SetVisible(Visible);
            return SCG;
        }

        PagedCellGrid* RenderableContainerWidget::CreatePagedCellGrid(ConstString& Name, const RenderableRect& Rect, const RenderableRect& SpnRect, const UI::SpinnerStyle& SStyle, const Real& GlyphHeight)
        {
            RenderablesAdded++;
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * ParentLayer->GetParent()->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            PagedCellGrid* PCG = new PagedCellGrid(Name,Rect,SpnRect,SStyle,GlyphHeight,ParentLayer);
            OffsetWidgetInfo PCGInfo(PCG,UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Widgets.push_back(PCGInfo);
            AddSubRenderable(RenderablesAdded,RenderablePair(NULL,PCG));
            PCG->SetVisible(Visible);
            return PCG;
        }

        DropDownList* RenderableContainerWidget::CreateDropDownList(ConstString& Name, const RenderableRect& Rect, const Real& LineHeight, const UI::ScrollbarStyle& ScrollStyle)
        {
            RenderablesAdded++;
            Vector2 Offset;
            if(Rect.Relative) Offset = (Rect.Position - RelPosition) * ParentLayer->GetParent()->GetViewportDimensions();
            else Offset = Rect.Position - GetActualPosition();
            DropDownList* DDL = new DropDownList(Name,Rect,LineHeight,ScrollStyle,ParentLayer);
            OffsetWidgetInfo DDLInfo(DDL,UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Widgets.push_back(DDLInfo);
            AddSubRenderable(RenderablesAdded,RenderablePair(NULL,DDL));
            DDL->SetVisible(Visible);
            return DDL;
        }

        TabSet* RenderableContainerWidget::CreateTabSet(ConstString& Name, const RenderableRect& SetRect)
        {
            RenderablesAdded++;
            Vector2 Offset;
            if(SetRect.Relative) Offset = (SetRect.Position - RelPosition) * ParentLayer->GetParent()->GetViewportDimensions();
            else Offset = SetRect.Position - GetActualPosition();
            TabSet* TS = new TabSet(Name,SetRect,ParentLayer);
            OffsetWidgetInfo TSInfo(TS,UI::RA_AnchorMiddle,UI::RT_TetherBoth,Offset);
            Widgets.push_back(TSInfo);
            AddSubRenderable(RenderablesAdded,RenderablePair(NULL,TS));
            TS->SetVisible(Visible);
            return TS;
        }
    }//ui
}//Mezzanine

#endif