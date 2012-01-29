//� Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _uirectangle_h
#define _uirectangle_h

#include "colourvalue.h"
#include "crossplatformexport.h"
#include "uienumerations.h"
#include "uirenderablerect.h"
#include "uibasicrenderable.h"

namespace Gorilla
{
    class Rectangle;
    class Layer;
}

namespace Mezzanine
{
    class UIManager;
    namespace UI
    {
        class Layer;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class Rectangle
        /// @headerfile uirectangle.h
        /// @brief This class is a helper class, specifically for use with background sprites and colours.
        /// @details Unlike a button, this class cannot be interacted with by clicking.
        /// It is important to understand what you want your space to do when selecting the class to use.
        ///////////////////////////////////////
        class MEZZ_LIB Rectangle : public BasicRenderable
        {
            protected:
                Gorilla::Rectangle* GRectangle;
                bool MouseHover;
            public:
                /// @brief Class constructor.
                /// @param Rect The Rect representing the position and size of the rectangle.
                /// @param Layer Pointer to the parent Layer that created this rectangle.
                Rectangle(const RenderableRect& Rect, Layer* PLayer);
                /// @brief Class destructor.
                virtual ~Rectangle();
                /// @brief Sets the visibility of this rectangle.
                /// @param Visible Bool determining whether or not this rectangle should be visible.
                virtual void SetVisible(bool Visible);
                /// @brief Gets the visibility of this rectangle.
                /// @return Returns a bool representing the visibility of this rectangle.
                virtual bool IsVisible() const;
                /// @brief Forces this rectangle to be shown.
                virtual void Show();
                /// @brief Forces this rectangle to hide.
                virtual void Hide();
                /// @brief Determines whether the mouse is over this rectangle.
                /// @return Returns a bool indicating whether the mouse is over this rectangle.
                virtual bool CheckMouseHover();
                /// @brief Gets the stored value of whether or not the mouse is over the rectangle.
                /// @details This function does not perform any checks.  If you want to do a manual check, call CheckMouseHover().
                /// @return Returns the stored value of whether or not the mouse is over the rectangle.
                virtual bool GetMouseHover();
                /// @brief Sets the background colour of the rectangle.
                /// @param Colour A colour value representing the colour to be set.
                virtual void SetBackgroundColour(const ColourValue& Colour);
                /// @brief Sets the background image(if provided in the atlas) of the rectangle.
                /// @param Name The name of the sprite to set as the background.
                virtual void SetBackgroundSprite(const String& Name);
                /// @brief Sets the background image(if provided in the atlas) of the rectangle.
                /// @param Name The name of the sprite to set as the background.
                /// @param Atlas The Atlas to load the sprite from.
                virtual void SetBackgroundSprite(const String& Name, const String& Atlas);
                /// @brief Enables a border and sets it's colour.
                /// @param Colour A colour value representing the colour to be set.
                virtual void SetBorder(const Real& Width, const ColourValue& Colour);
                /// @brief Disables any border set on this rectangle if one is currently set.
                virtual void NoBorder();
                /// @brief Sets the relative top left position of this rectangle.
                /// @param Position A Vector2 representing the location of this rectangle.
                virtual void SetPosition(const Vector2& Position);
                /// @brief Gets the relative top left position of this rectangle.
                /// @return Returns a Vector2 representing the location of this rectangle.
                virtual Vector2 GetPosition() const;
                /// @brief Sets the top left position of this rectangle in pixels.
                /// @param Position A Vector2 representing the location of this rectangle.
                virtual void SetActualPosition(const Vector2& Position);
                /// @brief Gets the top left position of this rectangle in pixels.
                /// @return Returns a Vector2 representing the location of this rectangle.
                virtual Vector2 GetActualPosition() const;
                /// @brief Sets the relative size of this rectangle.
                /// @param Size A vector2 representing the size of this rectangle.
                virtual void SetSize(const Vector2& Size);
                /// @brief Gets the relative size of this rectangle.
                /// @return Returns a vector2 representing the size of this rectangle.
                virtual Vector2 GetSize() const;
                /// @brief Sets the size of this rectangle in pixels.
                /// @param Size A vector2 representing the size of this rectangle.
                virtual void SetActualSize(const Vector2& Size);
                /// @brief Gets the size of this rectangle in pixels.
                /// @return Returns a vector2 representing the size of this rectangle.
                virtual Vector2 GetActualSize() const;
                /// @brief Sets the priority this button should be rendered with.
                /// @details The default value for this is Medium.
                /// @param Priority The priority level to be used when rendering this button.
                virtual void SetRenderPriority(const UI::RenderPriority& Priority);
                /// @brief Gets the priority this button should be rendered with.
                /// @return Returns an enum value representing this button's priority level.
                virtual UI::RenderPriority GetRenderPriority() const;
                /// @brief Sets the Atlas to be assumed when one isn't provided for atlas related tasks.
                /// @param Atlas The name of the atlas to be used.
                virtual void SetPrimaryAtlas(const String& Atlas);
                /// @brief Gets the currently set primary atlas.
                /// @return Returns a string containing the name of the primary atlas that is set, or an empty string if none.
                virtual String GetPrimaryAtlas() const;
                /// @brief Updates the dimensions of this rectangle to match those of the new screen size.
                /// @details This function is called automatically when a viewport changes in size, and shouldn't need to be called manually.
                virtual void UpdateDimensions();
        };//rectangle
    }//UI
}//Mezzanine

#endif