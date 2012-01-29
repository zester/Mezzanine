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
#ifndef _uibasicrenderable_h
#define _uibasicrenderable_h

#include "uienumerations.h"
#include "vector2.h"

namespace Mezzanine
{
    class UIManager;
    namespace UI
    {
        class Layer;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class BasicRenderable
        /// @headerfile uibasicrenderable.h
        /// @brief
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB BasicRenderable
        {
            protected:
                Layer* Parent;
                UIManager* Manager;
                Vector2 RelPosition;
                Vector2 RelSize;
                String Name;
            public:
                /// @brief Class constructor.
                /// @param name The name to be given to this widget.
                /// @param parent The parent layer that created this widget.
                BasicRenderable(const String& name, Layer* parent);
                /// @brief Class destructor.
                virtual ~BasicRenderable();
                /// @brief Sets the visibility of this renderable.
                /// @param Visible Bool determining whether or not this renderable should be visible.
                virtual void SetVisible(bool Visible) = 0;
                /// @brief Gets the visibility of this renderable.
                /// @return Returns a bool representing the visibility of this renderable.
                virtual bool IsVisible() const = 0;
                /// @brief Forces this renderable to be shown.
                virtual void Show() = 0;
                /// @brief Forces this renderable to hide.
                virtual void Hide() = 0;
                /// @brief Gets the name of this renderable.
                /// @return Returns a string containing the name of this renderable.
                virtual ConstString& GetName() const;
                /// @brief Sets the relative top left position of this renderable.
                /// @param Position A Vector2 representing the location of this renderable.
                virtual void SetPosition(const Vector2& Position) = 0;
                /// @brief Gets the relative top left position of this renderable.
                /// @return Returns a Vector2 representing the location of this renderable.
                virtual Vector2 GetPosition() const = 0;
                /// @brief Sets the top left position of this renderable in pixels.
                /// @param Position A Vector2 representing the location of this renderable.
                virtual void SetActualPosition(const Vector2& Position) = 0;
                /// @brief Gets the top left position of this renderable in pixels.
                /// @return Returns a Vector2 representing the location of this renderable.
                virtual Vector2 GetActualPosition() const = 0;
                /// @brief Sets the relative size of this renderable.
                /// @param Size A vector2 representing the size of this renderable.
                virtual void SetSize(const Vector2& Size) = 0;
                /// @brief Gets the relative size of this renderable.
                /// @return Returns a vector2 representing the size of this renderable.
                virtual Vector2 GetSize() const = 0;
                /// @brief Sets the size of this renderable in pixels.
                /// @param Size A vector2 representing the size of this renderable.
                virtual void SetActualSize(const Vector2& Size) = 0;
                /// @brief Gets the size of this renderable in pixels.
                /// @return Returns a vector2 representing the size of this renderable.
                virtual Vector2 GetActualSize() const = 0;
                /// @brief Sets the priority this button should be rendered with.
                /// @details The default value for this is Medium.
                /// @param Priority The priority level to be used when rendering this button.
                virtual void SetRenderPriority(const UI::RenderPriority& Priority) = 0;
                /// @brief Gets the priority this button should be rendered with.
                /// @return Returns an enum value representing this button's priority level.
                virtual UI::RenderPriority GetRenderPriority() const = 0;
                /// @brief Sets the Atlas to be assumed when one isn't provided for atlas related tasks.
                /// @param Atlas The name of the atlas to be used.
                virtual void SetPrimaryAtlas(const String& Atlas) = 0;
                /// @brief Gets the currently set primary atlas.
                /// @return Returns a string containing the name of the primary atlas that is set, or an empty string if none.
                virtual String GetPrimaryAtlas() const = 0;
                /// @brief Updates the dimensions of this renderable to match those of the new screen size.
                /// @details This function is called automatically when a viewport changes in size, and shouldn't need to be called manually.
                virtual void UpdateDimensions() = 0;
        };//BasicRenderable
    }//UI
}//Mezzanine

#endif