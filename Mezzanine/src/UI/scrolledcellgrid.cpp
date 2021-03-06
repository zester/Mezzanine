//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _uiscrolledcellgrid_cpp
#define _uiscrolledcellgrid_cpp

#include "UI/scrolledcellgrid.h"
#include "UI/cell.h"
#include "UI/screen.h"
#include "UI/scrollbar.h"

namespace Mezzanine
{
    namespace UI
    {
        ScrolledCellGrid::ScrolledCellGrid(const String& name, const RenderableRect& Rect, const Real& Thickness, const UI::ScrollbarStyle& Style, Screen* parent)
            : CellGrid(name,Rect,parent),
              HScrollVal(0.0),
              VScrollVal(0.0),
              AutoHide(true),
              ScrollXHiding(true),
              ScrollYHiding(true)
        {
            Paging = CG_Scrolled;
            RenderableRect HoriRect;
            RenderableRect VertRect;
            if(Rect.Relative)
            {
                const Vector2& WinDim = ParentScreen->GetViewportDimensions();
                Real ActThick = Thickness * WinDim.X;

                HoriRect.Position = Vector2(RelPosition.X,(RelPosition.Y + RelSize.Y) - ActThick / WinDim.Y);
                HoriRect.Size = Vector2(RelSize.X - (ActThick / WinDim.X),ActThick / WinDim.Y);
                HoriRect.Relative = Rect.Relative;
                VertRect.Position = Vector2((RelPosition.X + RelSize.X) - Thickness,RelPosition.Y);
                VertRect.Size = Vector2(Thickness,RelSize.Y - (ActThick / WinDim.Y));
                VertRect.Relative = Rect.Relative;
            }else{
                HoriRect.Position = Vector2(Rect.Position.X,(Rect.Position.Y + Rect.Size.Y) - Thickness);
                HoriRect.Size = Vector2(Rect.Size.X - Thickness,Thickness);
                HoriRect.Relative = Rect.Relative;
                VertRect.Position = Vector2((Rect.Position.X + Rect.Size.X) - Thickness,Rect.Position.Y);
                VertRect.Size = Vector2(Thickness,Rect.Size.Y - Thickness);
                VertRect.Relative = Rect.Relative;
            }

            HorizontalScroll = new UI::Scrollbar(Name+"HoriScr",HoriRect,Style,ParentScreen);
            VerticalScroll = new UI::Scrollbar(Name+"VertScr",VertRect,Style,ParentScreen);
            HorizontalScroll->Hide();
            VerticalScroll->Hide();

            CellsAdded = 2;
            AddSubRenderable(1,HorizontalScroll);
            AddSubRenderable(2,VerticalScroll);
        }

        ScrolledCellGrid::~ScrolledCellGrid()
        {
            delete HorizontalScroll;
            delete VerticalScroll;
        }

        Whole ScrolledCellGrid::FindTopRow(const Real& ScrollPos)
        {
            Whole TargetYPos = (WorkAreaLimits.Y - GetActualSize().Y) * ScrollPos;
            Whole CurrentPos = 0;
            for( Whole X = 0 ; X < TheGrid.size() ; X++ )
            {
                CurrentPos+=FixedCellSize.Y;
                if(CurrentPos >= TargetYPos)
                    return X;
                CurrentPos+=CellSpacing.Y;
            }
        }

        Whole ScrolledCellGrid::FindLeftColumn(const Real& ScrollPos)
        {
            Whole TargetXPos = (WorkAreaLimits.X - GetActualSize().X) * ScrollPos;
            Whole CurrentPos = 0;
            Whole MaxColumns = GetMaxColumns();
            for( Whole X = 0 ; X < MaxColumns ; X++ )
            {
                CurrentPos+=FixedCellSize.X;
                if(CurrentPos >= TargetXPos)
                    return X;
                CurrentPos+=CellSpacing.X;
            }
        }

        void ScrolledCellGrid::DrawGrid()
        {
            const Vector2& WinDim = ParentScreen->GetViewportDimensions();
            DrawGrid(WinDim);
        }

        void ScrolledCellGrid::DrawGrid(const Vector2& WinDim)
        {
            if(Cells.empty())
                return;
            RegenerateGrid();
            for( Whole X = 0 ; X < VisibleCells.size() ; X++ )
                VisibleCells[X]->Hide();
            VisibleCells.clear();

            Whole CurrentRow = 0;
            Whole CurrentColumn = 0;
            if( GetMaxRows() > GetMaxYCellsPresentable() )
            {
                ScrollYHiding = false;
                if(Visible && AutoHide)
                    VerticalScroll->Show();
                CurrentRow = FindTopRow( VerticalScroll->GetScrollerValue() );
            }else{
                ScrollYHiding = true;
                VerticalScroll->Hide();
            }
            if( GetMaxColumns() > GetMaxXCellsPresentable() )
            {
                ScrollXHiding = false;
                if(Visible && AutoHide)
                    HorizontalScroll->Show();
                CurrentColumn = FindLeftColumn( HorizontalScroll->GetScrollerValue() );
            }else{
                ScrollXHiding = true;
                HorizontalScroll->Hide();
            }

            const Whole StartRow = CurrentRow;
            const Whole StartColumn = CurrentColumn;

            Vector2 ActPos = GetActualPosition();
            Vector2 ActSize = GetActualSize();
            Whole CursorXPos = (Whole)(ActPos.X + EdgeSpacing.X);
            Whole CursorYPos = (Whole)(ActPos.Y + EdgeSpacing.Y);
            const Whole StartXPos = CursorXPos;
            const Whole StartYPos = CursorYPos;

            UI::Cell* CurrentCell = NULL;
            bool StillHasRoom = true;

            switch (Ordering)
            {
                case CG_Horizontal_Vertical_Ascending:
                case CG_Horizontal_Vertical_Decending:
                {
                    if(ActSize.Y < (FixedCellSize.Y + (EdgeSpacing.Y * 2)))
                        return;
                    while(StillHasRoom)
                    {
                        CurrentCell = GetCell(CurrentRow,CurrentColumn);
                        if(!CurrentCell)
                        {
                            CursorYPos+=FixedCellSize.Y + CellSpacing.Y;
                            CurrentRow++;
                            CursorXPos = StartXPos;
                            CurrentColumn = StartColumn;
                            if(CurrentRow >= GetMaxRows())
                                StillHasRoom = false;
                            continue;
                        }
                        Vector2 CellSize = CurrentCell->GetActualSize();
                        if((Real)CursorXPos + CellSize.X > (ActPos.X + ActSize.X) - EdgeSpacing.X)
                        {
                            CursorYPos+=FixedCellSize.Y + CellSpacing.Y;
                            CurrentRow++;
                            CursorXPos = StartXPos;
                            CurrentColumn = StartColumn;
                            if(CursorYPos+FixedCellSize.Y > (ActPos.Y + ActSize.Y) - EdgeSpacing.Y)
                                StillHasRoom = false;
                            continue;
                        }
                        CurrentCell->SetActualPosition(Vector2((Real)CursorXPos,(Real)CursorYPos));
                        if(Visible)
                            CurrentCell->Show();
                        VisibleCells.push_back(CurrentCell);
                        CursorXPos+=(Whole)(CellSize.X + CellSpacing.X);
                        CurrentColumn++;
                    }
                    break;
                }
                case CG_Vertical_Horizontal_Ascending:
                case CG_Vertical_Horizontal_Decending:
                {
                    if(ActSize.X < (FixedCellSize.X + (EdgeSpacing.X * 2)))
                        return;
                    while(StillHasRoom)
                    {
                        CurrentCell = GetCell(CurrentRow,CurrentColumn);
                        if(!CurrentCell)
                        {
                            CursorXPos+=FixedCellSize.X + CellSpacing.X;
                            CurrentColumn++;
                            CursorYPos = StartYPos;
                            CurrentRow = StartRow;
                            if(CurrentColumn >= GetMaxColumns())
                                StillHasRoom = false;
                            continue;
                        }
                        Vector2 CellSize = CurrentCell->GetActualSize();
                        if((Real)CursorYPos + CellSize.Y > (ActPos.Y + ActSize.Y) - EdgeSpacing.Y)
                        {
                            CursorXPos+=FixedCellSize.X + CellSpacing.X;
                            CurrentColumn++;
                            CursorYPos = StartYPos;
                            CurrentRow = StartRow;
                            if(CursorXPos+FixedCellSize.X > (ActPos.X + ActSize.X) - EdgeSpacing.X)
                                StillHasRoom = false;
                            continue;
                        }
                        CurrentCell->SetActualPosition(Vector2((Real)CursorXPos,(Real)CursorYPos));
                        if(Visible)
                            CurrentCell->Show();
                        VisibleCells.push_back(CurrentCell);
                        CursorYPos+=(Whole)(CellSize.Y + CellSpacing.Y);
                        CurrentRow++;
                    }
                    break;
                }
            }
        }

        bool ScrolledCellGrid::GridNeedsRedraw()
        {
            if(Cells.empty())
                return false;
            return (HScrollVal != HorizontalScroll->GetScrollerValue() || VScrollVal != VerticalScroll->GetScrollerValue());
        }

        void ScrolledCellGrid::SetVisibleImpl(bool visible)
        {
            if( !AutoHide )
            {
                HorizontalScroll->SetVisible(visible);
                VerticalScroll->SetVisible(visible);
            }else{
                if( !ScrollXHiding )
                {
                    HorizontalScroll->SetVisible(visible);
                }
                if( !ScrollYHiding )
                {
                    VerticalScroll->SetVisible(visible);
                }
            }
            CellGrid::SetVisibleImpl(visible);
        }

        bool ScrolledCellGrid::CheckMouseHoverImpl()
        {
            if(CellGrid::CheckMouseHoverImpl())
                return true;
            if(HorizontalScroll->CheckMouseHover())
            {
                HoveredSubWidget = HorizontalScroll;
                return true;
            }
            if(VerticalScroll->CheckMouseHover())
            {
                HoveredSubWidget = VerticalScroll;
                return true;
            }
            return false;
        }

        void ScrolledCellGrid::SetAutoHide(bool Auto)
        {
            AutoHide = Auto;
        }

        bool ScrolledCellGrid::GetAutoHide()
        {
            return AutoHide;
        }

        void ScrolledCellGrid::DestroyAllCells()
        {
            CellGrid::DestroyAllCells();
            AddSubRenderable(1,HorizontalScroll);
            AddSubRenderable(2,VerticalScroll);
        }

        void ScrolledCellGrid::SetPosition(const Vector2& Position)
        {
            Vector2 HScrollOffset = VerticalScroll->GetPosition() - Position;
            HorizontalScroll->SetPosition(Position + HScrollOffset);

            Vector2 VScrollOffset = VerticalScroll->GetPosition() - Position;
            VerticalScroll->SetPosition(Position + VScrollOffset);

            CellGrid::SetPosition(Position);
        }

        void ScrolledCellGrid::SetActualPosition(const Vector2& Position)
        {
            Vector2 HScrollOffset = VerticalScroll->GetActualPosition() - Position;
            HorizontalScroll->SetActualPosition(Position + HScrollOffset);

            Vector2 VScrollOffset = VerticalScroll->GetActualPosition() - Position;
            VerticalScroll->SetActualPosition(Position + VScrollOffset);

            CellGrid::SetActualPosition(Position);
        }

        void ScrolledCellGrid::SetSize(const Vector2& Size)
        {
            Vector2 HScrollP((RelPosition.X + Size.X) - HorizontalScroll->GetSize().X,RelPosition.Y);
            Vector2 HScrollS(HorizontalScroll->GetSize().X,Size.Y);
            HorizontalScroll->SetPosition(HScrollP);
            HorizontalScroll->SetSize(HScrollS);

            Vector2 VScrollP((RelPosition.X + Size.X) - VerticalScroll->GetSize().X,RelPosition.Y);
            Vector2 VScrollS(VerticalScroll->GetSize().X,Size.Y);
            VerticalScroll->SetPosition(VScrollP);
            VerticalScroll->SetSize(VScrollS);

            CellGrid::SetSize(Size);
        }

        void ScrolledCellGrid::SetActualSize(const Vector2& Size)
        {
            Vector2 HScrollP((GetActualPosition().X + Size.X) - HorizontalScroll->GetActualSize().X,GetActualPosition().Y);
            Vector2 HScrollS(HorizontalScroll->GetActualSize().X,Size.Y);
            HorizontalScroll->SetActualPosition(HScrollP);
            HorizontalScroll->SetActualSize(HScrollS);

            Vector2 VScrollP((GetActualPosition().X + Size.X) - VerticalScroll->GetActualSize().X,GetActualPosition().Y);
            Vector2 VScrollS(VerticalScroll->GetActualSize().X,Size.Y);
            VerticalScroll->SetActualPosition(VScrollP);
            VerticalScroll->SetActualSize(VScrollS);

            CellGrid::SetActualSize(Size);
        }

        void ScrolledCellGrid::UpdateDimensions()
        {
            const Vector2& WinDim = ParentScreen->GetViewportDimensions();
            CellGrid::UpdateDimensions();
            HorizontalScroll->UpdateDimensions();
            VerticalScroll->UpdateDimensions();
            GridDirty = true;
            RegenerateGrid(WinDim);
            DrawGrid(WinDim);
        }

        UI::Scrollbar* ScrolledCellGrid::GetHorizontalScroll()
        {
            return HorizontalScroll;
        }

        UI::Scrollbar* ScrolledCellGrid::GetVerticalScroll()
        {
            return VerticalScroll;
        }
    }//ui
}//Mezzanine

#endif
