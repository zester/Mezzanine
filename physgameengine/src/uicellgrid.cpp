//� Copyright 2010 - 2011 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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
#ifndef _uicellgrid_cpp
#define _uicellgrid_cpp

#include "uicellgrid.h"
#include "uicell.h"
#include "uirectangle.h"
#include "uilayer.h"
#include "uiscreen.h"
#include "uiscrollbar.h"

#include "metacode.h"
#include "inputquerytool.h"

namespace phys
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @internal
        /// @struct CellPrioritySorter
        /// @headerfile uicellgrid.h
        /// @brief This is a widget used to determine spacing and ordering of other widgets(Cells) on a grid like surface.
        /// @details
        ///////////////////////////////////////
        struct CellPrioritySorter
        {
            bool operator()(Cell* First, Cell* Second)
            {
                return First->GetPriority() < Second->GetPriority();
            }
        };

        CellGrid::CellGrid(const String& name, const Vector2& Position, const Vector2& Size, Layer* parent)
            : Widget(name,parent),
              EdgeSpacing(Vector2(0,0)),
              CellSpacing(Vector2(0.01,0.01)),
              FixedCellSize(Vector2(0.1,0.1)),
              Ordering(CellGrid::CG_Horizontal_Vertical_Ascending),
              GridDirty(false),
              Selected(NULL)
        {
            Type = Widget::CellGrid;
            RelPosition = Position;
            RelSize = Size;
            WorkAreaLimits = Size * Parent->GetParent()->GetViewportDimensions();

            GridBack = new Rectangle(Position,Size,Parent);
        }

        CellGrid::~CellGrid()
        {
            DestroyAllCells();
            for( Whole X = 0 ; X < TheGrid.size() ; X++ )
                delete TheGrid[X];
            TheGrid.clear();
            delete GridBack;
        }

        Whole CellGrid::GetMaxRows()
        {
            return TheGrid.size();
        }

        Whole CellGrid::GetMaxColumes()
        {
            Whole CurrentBest = 0;
            for( std::vector<CellVector*>::iterator it = TheGrid.begin() ; it != TheGrid.end() ; it++ )
            {
                if(CurrentBest < (*it)->size())
                    CurrentBest = (*it)->size();
            }
            return CurrentBest;
        }

        Whole CellGrid::FindTallestInRow(const Whole& RowIndex)
        {
            if(TheGrid.size() < RowIndex)
                return 0;
            if(TheGrid.at(RowIndex)->empty())
                return 0;
            Whole CurrentBest = 0;
            for( CellVector::iterator Cit = TheGrid.at(RowIndex)->begin() ; Cit != TheGrid.at(RowIndex)->end() ; Cit++ )
            {
                Whole CellHeight = (*Cit)->GetActualSize().Y;
                if(CellHeight > CurrentBest)
                    CurrentBest = CellHeight;
            }
            return CurrentBest;
        }

        Whole CellGrid::FindWidestInColumn(const Whole& ColumnIndex)
        {
            Whole CurrentBest = 0;
            for( std::vector<CellVector*>::iterator it = TheGrid.begin() ; it != TheGrid.end() ; it++ )
            {
                if((*it)->size() >= ColumnIndex)
                {
                    Cell* CurrCell = (*it)->at(ColumnIndex);
                    Whole CellWidth = CurrCell->GetActualSize().X;
                    if(CellWidth > CurrentBest)
                        CurrentBest = CellWidth;
                }
            }
            return CurrentBest;
        }

        void CellGrid::CreateOrDestroyRow(CellVector* ToDestroy)
        {
            if(!ToDestroy)
            {
                TheGrid.push_back(new CellVector());
                return;
            }
            for( std::vector<CellVector*>::iterator it = TheGrid.begin() ; it != TheGrid.end() ; it++ )
            {
                if(ToDestroy == (*it))
                {
                    for( CellVector::iterator Cit = (*it)->begin() ; Cit != (*it)->end() ; Cit++ )
                        delete (*Cit);
                    (*it)->clear();
                    delete ToDestroy;
                    TheGrid.erase(it);
                    return;
                }
            }
        }

        void CellGrid::ClearGrid()
        {
            for( std::vector<CellVector*>::iterator it = TheGrid.begin() ; it != TheGrid.end() ; it++ )
                (*it)->clear();
        }

        void CellGrid::RegenerateGrid()
        {
            if(!GridDirty)
                return;
            ClearGrid();
            Vector2 CGPosition = GridBack->GetActualPosition();
            Vector2 WindowSize = Parent->GetParent()->GetViewportDimensions();

            Whole CurrentColumn = 0;
            Whole CurrentRow = 0;
            Whole CurrentXposition = (Whole)CGPosition.X + (Whole)(EdgeSpacing.X * WindowSize.X);
            Whole CurrentYposition = (Whole)CGPosition.Y + (Whole)(EdgeSpacing.Y * WindowSize.Y);
            Whole StartXposition = CurrentXposition;
            Whole StartYposition = CurrentYposition;
            Whole RemainingRow = (Whole)(WorkAreaLimits.X - ((EdgeSpacing.X * WindowSize.X) * 2));
            Whole RemainingColumn = (Whole)(WorkAreaLimits.Y - ((EdgeSpacing.Y * WindowSize.Y) * 2));

            Cell* CurrCell = NULL;

            if( CellGrid::CG_Horizontal_Vertical_Ascending == Ordering || CellGrid::CG_Horizontal_Vertical_Decending == Ordering )
            {
                for( CellList::iterator it = Cells.begin() ; it != Cells.end() ; it++ )
                {
                    CurrCell = (*it);
                    Vector2 CellSize = CurrCell->GetActualSize();
                    if(RemainingRow < CellSize.X + EdgeSpacing.X)
                    {
                        if(RemainingColumn < CellSize.Y + EdgeSpacing.Y)
                            break;
                        CurrentXposition = StartXposition;
                        CurrentColumn = 0;
                        CurrentYposition+=FixedCellSize.Y;
                        CurrentRow++;
                        if(CurrentRow > TheGrid.size())
                            CreateOrDestroyRow(0);
                    }
                    TheGrid.at(CurrentRow)->push_back(CurrCell);
                    CurrentXposition+=CellSize.X + CellSpacing.X;
                    CurrentColumn++;
                }
            }
            else if( CellGrid::CG_Vertical_Horizontal_Ascending == Ordering || CellGrid::CG_Vertical_Horizontal_Decending == Ordering )
            {
                for( CellList::iterator it = Cells.begin() ; it != Cells.end() ; it++ )
                {
                    CurrCell = (*it);
                    Vector2 CellSize = CurrCell->GetActualSize();
                    if(RemainingColumn < CellSize.Y + EdgeSpacing.Y)
                    {
                        if(RemainingRow < CellSize.X + EdgeSpacing.X)
                            break;
                        CurrentYposition = StartYposition;
                        CurrentRow = 0;
                        CurrentXposition+=FixedCellSize.X;
                        CurrentColumn++;
                    }
                    TheGrid.at(CurrentRow)->push_back(CurrCell);
                    CurrentYposition+=CellSize.Y + CellSpacing.Y;
                    CurrentRow++;
                    if(CurrentRow > TheGrid.size())
                        CreateOrDestroyRow(0);
                }
            }
        }

        void CellGrid::Update(bool Force)
        {
            if(!Force)
                SubWidgetUpdate();
            MetaCode::ButtonState State = InputQueryTool::GetMouseButtonState(1);
            if(HoveredSubWidget)
            {
                if(MetaCode::BUTTON_PRESSING == State)
                {
                    SubWidgetFocus = HoveredSubWidget;
                }
                else if(MetaCode::BUTTON_LIFTING == State)
                {
                    Selected = static_cast<Cell*>(HoveredSubWidget);
                    Selected->DoSelectedItems();
                }
            }
            if(SubWidgetFocus && (SubWidgetFocus != HoveredSubWidget))
            {
                SubWidgetFocusUpdate(true);
            }
            else if(MetaCode::BUTTON_DOWN == State && Force)
            {
                SubWidgetFocusUpdate(Force);
            }
            if(MetaCode::BUTTON_LIFTING == State)
            {
                SubWidgetFocus = NULL;
            }
            if(GridNeedsRedraw())
                DrawGrid();
        }

        void CellGrid::SetVisible(bool visible)
        {
            GridBack->SetVisible(visible);
            for( CellVector::iterator it = VisibleCells.begin() ; it != VisibleCells.end() ; it++ )
            {
                (*it)->SetVisible(visible);
            }
            Visible = visible;
        }

        bool CellGrid::IsVisible()
        {
            return Visible && Parent->IsVisible() && Parent->GetParent()->IsVisible();
        }

        void CellGrid::Show()
        {
            GridBack->Show();
            for( CellVector::iterator it = VisibleCells.begin() ; it != VisibleCells.end() ; it++ )
            {
                (*it)->Show();
            }
            Visible = true;
        }

        void CellGrid::Hide()
        {
            GridBack->Hide();
            for( CellVector::iterator it = VisibleCells.begin() ; it != VisibleCells.end() ; it++ )
            {
                (*it)->Hide();
            }
            Visible = false;
        }

        void CellGrid::SetFixedCellSize(const Vector2& FixedSize)
        {
            FixedCellSize = FixedSize;
        }

        Vector2 CellGrid::GetFixedCellSize()
        {
            return FixedCellSize;
        }

        void CellGrid::AddCell(Cell* ToBeAdded)
        {
            ToBeAdded->SetSize(FixedCellSize);
            Cells.push_back(ToBeAdded);
            GridDirty = true;
        }

        Cell* CellGrid::GetCell(const String& Name)
        {
            if(Cells.empty())
                return 0;
            Cell* CurrCell = NULL;
            for( CellList::iterator it = Cells.begin() ; it != Cells.end() ; it++ )
            {
                CurrCell = (*it);
                if(Name == CurrCell->GetName())
                    return CurrCell;
            }
            return 0;
        }

        Cell* CellGrid::GetCell(const Whole& Row, const Whole& Column)
        {
            if(Row > TheGrid.size())
                return 0;

            CellVector* CellRow = TheGrid.at(Row);

            if(Column > CellRow->size())
                return 0;

            return CellRow->at(Column);
        }

        void CellGrid::RemoveCell(Cell* ToBeRemoved)
        {
            GridDirty = true;
        }

        void CellGrid::DestroyAllCells()
        {
            for( std::vector<CellVector*>::iterator it = TheGrid.begin() ; it != TheGrid.end() ; it++ )
            {
                for( CellVector::iterator Cit = (*it)->begin() ; Cit != (*it)->end() ; Cit++ )
                    delete (*Cit);
                (*it)->clear();
            }
            GridDirty = true;
        }

        void CellGrid::GenerateGrid()
        {
            if(!GridDirty)
                return;
            RegenerateGrid();
        }

        void CellGrid::SetWorkAreaLimits(const Vector2& AreaLimit)
        {
            WorkAreaLimits = AreaLimit;
        }

        Vector2 CellGrid::GetWorkAreaLimits()
        {
            return WorkAreaLimits;
        }

        void CellGrid::SetEdgeSpacing(const Vector2& Spacing)
        {
            EdgeSpacing = Spacing;
        }

        Vector2 CellGrid::GetEdgeSpacing()
        {
            return EdgeSpacing;
        }

        void CellGrid::SetCellSpacing(const Vector2& Spacing)
        {
            CellSpacing = Spacing;
        }

        Vector2 CellGrid::GetCellSpacing()
        {
            return CellSpacing;
        }

        bool CellGrid::CellsOffGrid()
        {
            Whole GridCount = 0;
            for( std::vector<CellVector*>::iterator it = TheGrid.begin() ; it != TheGrid.end() ; it++ )
                GridCount+=(*it)->size();
            return GridCount < Cells.size();
        }

        void CellGrid::SortGrid()
        {
            Cells.sort(CellPrioritySorter());
            if( CellGrid::CG_Horizontal_Vertical_Decending == Ordering || CellGrid::CG_Vertical_Horizontal_Decending == Ordering )
                Cells.reverse();
        }

        bool CellGrid::CheckMouseHover()
        {
            if(!Visible)
                return false;
            for( CellVector::iterator it = VisibleCells.begin() ; it != VisibleCells.end() ; it++ )
            {
                if((*it)->IsVisible())
                {
                    if((*it)->CheckMouseHover())
                    {
                        HoveredSubWidget = (*it);
                        HoveredButton = HoveredSubWidget->GetHoveredButton();
                        return true;
                    }
                }
            }
            if(GridBack->CheckMouseHover())
            {
                HoveredSubWidget = NULL;
                HoveredButton = NULL;
                return true;
            }
            HoveredSubWidget = NULL;
            HoveredButton = NULL;
            return false;
        }

        void CellGrid::SetPosition(const Vector2& Position)
        {
            RelPosition = Position;
            GridBack->SetPosition(Position);
            DrawGrid();
        }

        Vector2 CellGrid::GetPosition()
        {
            return RelPosition;
        }

        void CellGrid::SetActualPosition(const Vector2& Position)
        {
            RelPosition = Position / Parent->GetParent()->GetViewportDimensions();
            GridBack->SetActualPosition(Position);
            DrawGrid();
        }

        Vector2 CellGrid::GetActualPosition()
        {
            return RelPosition * Parent->GetParent()->GetViewportDimensions();
        }

        void CellGrid::SetSize(const Vector2& Size)
        {
            RelSize = Size;
            GridBack->SetSize(Size);
            DrawGrid();
        }

        Vector2 CellGrid::GetSize()
        {
            return RelSize;
        }

        void CellGrid::SetActualSize(const Vector2& Size)
        {
            RelSize = Size / Parent->GetParent()->GetViewportDimensions();
            GridBack->SetActualSize(Size);
            DrawGrid();
        }

        Vector2 CellGrid::GetActualSize()
        {
            return RelSize * Parent->GetParent()->GetViewportDimensions();
        }

        Rectangle* CellGrid::GetGridBack()
        {
            return GridBack;
        }

        Cell* CellGrid::GetSelected()
        {
            return Selected;
        }
    }//ui
}//phys

#endif
