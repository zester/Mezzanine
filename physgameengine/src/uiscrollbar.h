//� Copyright 2010 BlackTopp Studios Inc.
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
#ifndef _uiscrollbar_h
#define _uiscrollbar_h

#include "datatypes.h"
#include "vector2.h"
#include "uiwidget.h"

namespace phys
{
    class UILayer;
    namespace UI
    {
        class Button;
        class Rectangle;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class Scrollbar
        /// @headerfile scrollbar.h
        /// @brief This class is a widget class, to be used alongside any scrollable widget.
        /// @details If you want to have buttons to accompany your scrollbar they'll automatically
        /// have their height match the width of the scrollbar if it's vertical, or their width
        /// match their height of the scrollbar if it's horizontal, based on the dimensions provided. @n
        /// Scrollbars can come in a few styles.  Separate is the typical way to present them with the Up
        /// or Left button being located at the top of left side of the scroller respectively.  Together
        /// is where both scroll buttons are next to each other instead of on opposite sides of the
        /// scroller.
        ///////////////////////////////////////
        class Scrollbar : public Widget
        {
            public:
                enum BarStyle
                {
                    NoButtons,
                    Separate,
                    TogetherUpLeft,
                    TogetherDownRight
                };
            protected:
                UI::Button* Scroller;
                UI::Button* UpLeftButton;
                UI::Button* DownRightButton;
                Real ScrollUpperLimit;
                Real ScrollLowerLimit;
                Real ScrollMinHeight;
                void CreateHorizontalScrollbar(Vector2 Position, Vector2 Size, Scrollbar::BarStyle Style);
                void CreateVerticalScrollbar(Vector2 Position, Vector2 Size, Scrollbar::BarStyle Style);
            public:
                /// @brief Standard initialization constructor.
                /// @param Name The name of this scrollbar.
                /// @param Position A vector2 representing the top left position for this widget to be placed on screen.
                /// @param Size A vector2 representing the size of this widget.
                /// @param Style An enum value representing how you want your scrollbar constructed.  See class details for more info.
                /// @param parent The Layer that created this scrollbar.
                Scrollbar(String& Name, Vector2 Position, Vector2 Size, Scrollbar::BarStyle Style, UILayer* parent);
                /// @brief Standard class destructor.
                virtual ~Scrollbar();
                /// @brief Checks to see if the current mouse position is over this widget.
                /// @return Returns a bool value, true if the mouse is over this widget, false if it's not.
                virtual bool CheckMouseHover();
                /// @brief Sets the relative position of this widget in pixels.
                /// @details The position is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @param Position A vector2 representing the relative position of this widget.
                virtual void SetPosition(Vector2 Position);
                /// @brief Gets the relative position of this widget.
                /// @details The position is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @return Returns a vector2 representing the relative position of this widget.
                virtual Vector2 GetPosition();
                /// @brief Sets the pixel position of this widget.
                /// @param Position A vector2 representing the pixel position of this widget.
                virtual void SetActualPosition(Vector2 Position);
                /// @brief Sets the pixel position of this widget.
                /// @return Returns a vector2 representing the pixel position of this widget.
                virtual Vector2 GetActualPosition();
                /// @brief Sets the relative size of this widget.
                /// @param Size A vector2 representing the relative size of this widget.
                virtual void SetSize(Vector2 Size);
                /// @brief Gets the relative size of this widget.
                /// @details The size is relative to the screen size.  Values range from 0.0 to 1.0.
                /// @return Returns a vector2 representing the relative size of this widget.
                virtual Vector2 GetSize();
                /// @brief Sets the pixel size of this widget.
                /// @param Size A vector2 representing the pixel size of this widget.
                virtual void SetActualSize(Vector2 Size);
                /// @brief Sets the pixel size of this widget.
                /// @return Returns a vector2 representing the pixel size of this widget.
                virtual Vector2 GetActualSize();
        };//scrollbar
    }//UI
}//phys

#endif
