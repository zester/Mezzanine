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
#ifndef _uicell_h
#define _uicell_h

#include "uiwidget.h"

namespace phys
{
    namespace UI
    {
        class CellCallback;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class Cell
        /// @headerfile uicell.h
        /// @brief This class is a base class to be used with the CellGrid class.
        /// @details This class is intended to be inherited from, like the Widget class, but simply adds
        /// some functionality to ease it's use and placement on a CellGrid.
        ///////////////////////////////////////
        class PHYS_LIB Cell : public Widget
        {
            protected:
                Whole SortPriority;
                bool Selected;
                CellCallback* Callback;
                /// @brief For use with widget update/automation.
                virtual void Update(bool Force = false) = 0;
            public:
                /// @brief Class constructor.
                Cell(const String& name, Layer* parent);
                /// @brief Class destructor.
                ~Cell();
                /// @brief Sets the priority of this cell for use in sorting on the cell grid.
                /// @param Priority The priority to be set.
                virtual void SetPriority(const Whole& Priority);
                /// @brief Gets the currently set priority of this Cell.
                /// @return Returns a Whole representing the current priority of this Cell.
                virtual Whole GetPriority();
                /// @brief Sets whether or not this Cell is currently selected.
                /// @param Select Whether or not this Cell should be activated.
                virtual void SetSelected(bool Select);
                /// @brief Gets whether or not this Cell is currently selected.
                /// @return Returns a bool indicating whether or not this Cell is currently selected.
                virtual bool IsSelected();
                /// @brief Sets a callback for this Cell.
                /// @details You can pass in a null pointer to disable the callback of a Cell.
                /// @param CB The callback to be attached to this Cell.
                virtual void SetCellCallback(CellCallback* CB);
                /// @brief Sets the visibility of this widget.
                /// @param visible Bool determining whether or not this widget should be visible.
                virtual void SetVisible(bool visible) = 0;
                /// @brief Gets the visibility of this widget.
                /// @return Returns a bool representing the visibility of this widget.
                virtual bool IsVisible() = 0;
                /// @brief Forces this widget to be shown.
                virtual void Show() = 0;
                /// @brief Forces this widget to hide.
                virtual void Hide() = 0;
                /// @brief Checks to see if the current mouse position is over this widget.
                /// @return Returns a bool value, true if the mouse is over this widget, false if it's not.
                virtual bool CheckMouseHover() = 0;
                /// @brief Sets the relative position of this widget.
                /// @details The position is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @param Position A vector2 representing the relative position of this widget.
                virtual void SetPosition(const Vector2& Position) = 0;
                /// @brief Gets the relative position of this widget.
                /// @details The position is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @return Returns a vector2 representing the relative position of this widget.
                virtual Vector2 GetPosition() = 0;
                /// @brief Sets the pixel position of this widget.
                /// @param Position A vector2 representing the pixel position of this widget.
                virtual void SetActualPosition(const Vector2& Position) = 0;
                /// @brief Sets the pixel position of this widget.
                /// @return Returns a vector2 representing the pixel position of this widget.
                virtual Vector2 GetActualPosition() = 0;
                /// @brief Sets the relative size of this widget.
                /// @details The size is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @param Size A vector2 representing the relative size of this widget.
                virtual void SetSize(const Vector2& Size) = 0;
                /// @brief Gets the relative size of this widget.
                /// @details The size is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @return Returns a vector2 representing the relative size of this widget.
                virtual Vector2 GetSize() = 0;
                /// @brief Sets the pixel size of this widget.
                /// @param Size A vector2 representing the pixel size of this widget.
                virtual void SetActualSize(const Vector2& Size) = 0;
                /// @brief Sets the pixel size of this widget.
                /// @return Returns a vector2 representing the pixel size of this widget.
                virtual Vector2 GetActualSize() = 0;
                /// @brief Overloaded Less-Than operator used for sorting on the grid.
                /// @param Other The other Cell to be compared to this one.
                virtual bool operator<(Cell* Other);
                /// @brief Overloaded Greater-Than operator used for sorting on the grid.
                /// @param Other The other Cell to be compared to this one.
                virtual bool operator>(Cell* Other);
        };//Cell

        ///////////////////////////////////////////////////////////////////////////////
        /// @class ButtonCallback
        /// @headerfile uibutton.h
        /// @brief This class provides customizable functionality to the button class.
        /// @details This is a pure virtual class that must be inherited from for use with specialized
        /// behaviors when working with buttons.
        ///////////////////////////////////////
        class PHYS_LIB CellCallback
        {
            protected:
                Cell* Caller;
            public:
                /// @brief Class constructor.
                /// @param CallerButton The Cell to which this callback belongs.
                CellCallback(Cell* CallerCell);
                /// @brief Class Destructor.
                ~CellCallback();
                /// @brief The hover function for this callback.  This will be called every time the
                /// Cell is hovered over by the mouse.
                virtual void DoSelectedItems() = 0;
                /// @brief The activation function for this callback.  This will be called every time the
                /// Cell is activated by the mouse or keyboard.
                virtual void DoUnselectedItems() = 0;
        };//cellcallback
    }//ui
}//phys

#endif