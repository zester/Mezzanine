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
#ifndef _uirectangle_cpp
#define _uirectangle_cpp

#include "UI/rectangle.h"
#include "uimanager.h"
#include "UI/screen.h"
#include "UI/viewportupdatetool.h"
#include "entresol.h"
#include "eventmanager.h"
#include "mathtool.h"
#include "Input/inputmanager.h"
#include "Input/mouse.h"

namespace Mezzanine
{
    namespace UI
    {
        Rectangle::Rectangle(const RenderableRect& Rect, Screen* PScreen)
            : AreaRenderable("Rectangle",PScreen),
              NormalSprite(NULL),
              HoveredSprite(NULL),
              CustomCenter(false),
              BorderWidth(0.0),
              RotAngle(0.0)
        {
            ConstructRectangle(Rect);
        }

        Rectangle::Rectangle(const String& name, const RenderableRect& Rect, Screen* PScreen)
            : AreaRenderable(name,PScreen),
              NormalSprite(NULL),
              HoveredSprite(NULL),
              CustomCenter(false),
              BorderWidth(0.0),
              RotAngle(0.0)
        {
            ConstructRectangle(Rect);
        }

        Rectangle::~Rectangle()
        {
        }

        void Rectangle::ConstructRectangle(const UI::RenderableRect& Rect)
        {
            const Vector2& WinDim = ParentScreen->GetViewportDimensions();
            if(Rect.Relative)
            {
                RelPosition = Rect.Position;
                RelSize = Rect.Size;

                ActPosition = Rect.Position * WinDim;
                ActSize = Rect.Size * WinDim;
            }else{
                RelPosition = Rect.Position / WinDim;
                RelSize = Rect.Size / WinDim;

                ActPosition = Rect.Position;
                ActSize = Rect.Size;
            }
            BackgroundColours[0] = ColourValue::White();
            BackgroundColours[1] = ColourValue::White();
            BackgroundColours[2] = ColourValue::White();
            BackgroundColours[3] = ColourValue::White();
            BorderColours[0] = ColourValue::Black();
            BorderColours[1] = ColourValue::Black();
            BorderColours[2] = ColourValue::Black();
            BorderColours[3] = ColourValue::Black();
            UVs[0] = UVs[1] = UVs[2] = UVs[3] = ParentScreen->GetSolidUV(PriAtlas);
            RotCenter.X = 0.0;
            RotCenter.Y = 0.0;
        }

        void Rectangle::DrawBorder(const Vector2& TopLeft, const Vector2& TopRight, const Vector2& BottomLeft, const Vector2& BottomRight,
                                   const Vector2& OuterTopLeft, const Vector2& OuterTopRight, const Vector2& OuterBottomLeft, const Vector2& OuterBottomRight)
        {
            VertexData Temp;
            Vector2 UV = ParentScreen->GetSolidUV(PriAtlas);

            // North
            PushTriangle(RenderVertices, Temp, TopLeft, OuterTopRight, OuterTopLeft, UV, BorderColours[UI::Border_North],PriAtlas);
            PushTriangle(RenderVertices, Temp, TopLeft, TopRight, OuterTopRight, UV, BorderColours[UI::Border_North],PriAtlas);

            // East
            PushTriangle(RenderVertices, Temp, BottomRight, OuterTopRight, TopRight, UV, BorderColours[UI::Border_East],PriAtlas);
            PushTriangle(RenderVertices, Temp, BottomRight, OuterBottomRight, OuterTopRight, UV, BorderColours[UI::Border_East],PriAtlas);

            // South
            PushTriangle(RenderVertices, Temp, OuterBottomLeft, BottomRight, BottomLeft, UV, BorderColours[UI::Border_South],PriAtlas);
            PushTriangle(RenderVertices, Temp, OuterBottomLeft, OuterBottomRight, BottomRight, UV, BorderColours[UI::Border_South],PriAtlas);

            // West
            PushTriangle(RenderVertices, Temp, OuterBottomLeft, TopLeft, OuterTopLeft, UV, BorderColours[UI::Border_West],PriAtlas);
            PushTriangle(RenderVertices, Temp, OuterBottomLeft, BottomLeft, TopLeft, UV, BorderColours[UI::Border_West],PriAtlas);
        }

        void Rectangle::DrawFill(const Vector2& TopLeft, const Vector2& TopRight, const Vector2& BottomLeft, const Vector2& BottomRight)
        {
            if(BackgroundColours[0].A != 0.0)
            {
                VertexData Temp;
                // Triangle A
                PushVertex(RenderVertices, Temp, BottomLeft.X, BottomLeft.Y, UVs[3], BackgroundColours[3],PriAtlas);    // Left/Bottom  3
                PushVertex(RenderVertices, Temp, TopRight.X, TopRight.Y, UVs[1], BackgroundColours[1],PriAtlas);    // Right/Top    1
                PushVertex(RenderVertices, Temp, TopLeft.X, TopLeft.Y, UVs[0], BackgroundColours[0],PriAtlas);    // Left/Top     0

                // Triangle B
                PushVertex(RenderVertices, Temp, BottomLeft.X, BottomLeft.Y, UVs[3], BackgroundColours[3],PriAtlas);    // Left/Bottom   3
                PushVertex(RenderVertices, Temp, BottomRight.X, BottomRight.Y, UVs[2], BackgroundColours[2],PriAtlas);    // Right/Bottom  2
                PushVertex(RenderVertices, Temp, TopRight.X, TopRight.Y, UVs[1], BackgroundColours[1],PriAtlas);    // Right/Top     1
            }
        }

        void Rectangle::RotationTransform(Vector2& TopLeft, Vector2& TopRight, Vector2& BottomLeft, Vector2& BottomRight)
        {
            if(0 != RotAngle)
            {
                Real RotCos = MathTools::Cos(RotAngle);
                Real RotSin = MathTools::Sin(RotAngle);

                if (!CustomCenter)
                {
                    RotCenter.X = TopLeft.X + (BottomRight.X - TopLeft.X) / 2;
                    RotCenter.Y = TopLeft.Y + (BottomRight.Y - TopLeft.Y) / 2;
                }

                TopLeft.X -= RotCenter.X;
                TopLeft.Y -= RotCenter.Y;

                TopRight.X -= RotCenter.X;
                TopRight.Y -= RotCenter.Y;

                BottomLeft.X -= RotCenter.X;
                BottomLeft.Y -= RotCenter.Y;

                BottomRight.X -= RotCenter.X;
                BottomRight.Y -= RotCenter.Y;

                Vector2 A2 = TopLeft, B2 = TopRight, C2 = BottomLeft, D2 = BottomRight;
                TopLeft.X = A2.X * RotCos - A2.Y * RotSin;
                TopLeft.Y = A2.X * RotSin + A2.Y * RotCos;

                TopRight.X = B2.X * RotCos - B2.Y * RotSin;
                TopRight.Y = B2.X * RotSin + B2.Y * RotCos;

                BottomLeft.X = C2.X * RotCos - C2.Y * RotSin;
                BottomLeft.Y = C2.X * RotSin + C2.Y * RotCos;

                BottomRight.X = D2.X * RotCos - D2.Y * RotSin;
                BottomRight.Y = D2.X * RotSin + D2.Y * RotCos;

                TopLeft.X += RotCenter.X;
                TopLeft.Y += RotCenter.Y;

                TopRight.X += RotCenter.X;
                TopRight.Y += RotCenter.Y;

                BottomLeft.X += RotCenter.X;
                BottomLeft.Y += RotCenter.Y;

                BottomRight.X += RotCenter.X;
                BottomRight.Y += RotCenter.Y;
            }
        }

        void Rectangle::SetSprite(Sprite* PSprite)
        {
            if(PSprite == NULL)
            {
                PriAtlas = ParentScreen->GetPrimaryAtlas();
                UVs[0] = UVs[1] = UVs[2] = UVs[3] = ParentScreen->GetSolidUV(PriAtlas);
                _MarkDirty();
            }else{
                PriAtlas = PSprite->Atlas;
                Real TexelOffsetX = ParentScreen->GetTexelOffsetX();
                Real TexelOffsetY = ParentScreen->GetTexelOffsetY();
                if(TexelOffsetX) TexelOffsetX /= ParentScreen->GetTextureSize(PSprite->Atlas).X;
                if(TexelOffsetY) TexelOffsetY /= ParentScreen->GetTextureSize(PSprite->Atlas).Y;
                UVs[0].X = UVs[3].X = PSprite->UVLeft - TexelOffsetX;
                UVs[0].Y = UVs[1].Y = PSprite->UVTop - TexelOffsetY;
                UVs[1].X = UVs[2].X = PSprite->UVRight + TexelOffsetX;
                UVs[2].Y = UVs[3].Y = PSprite->UVBottom + TexelOffsetY;
                _MarkDirty();
            }
        }

        void Rectangle::SetHovered(bool Hovered)
        {
            if(Hovered == MouseHover)
                return;
            if(HoveredSprite)
            {
                if(Hovered && !MouseHover)
                    SetSprite(HoveredSprite);
                else if(!Hovered && MouseHover)
                    SetSprite(NormalSprite);
            }
            MouseHover = Hovered;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Hover Methods

        bool Rectangle::CheckMouseHover()
        {
            if(!IsVisible())
            {
                SetHovered(false);
                return MouseHover;
            }
            Vector2 MouseLoc = Input::InputManager::GetSingletonPtr()->GetSystemMouse()->GetViewportPosition();
            SetHovered((MouseLoc.X >= ActPosition.X && MouseLoc.X <= ActPosition.X + ActSize.X) && (MouseLoc.Y >= ActPosition.Y && MouseLoc.Y <= ActPosition.Y + ActSize.Y));
            return MouseHover;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Visibility Methods

        bool Rectangle::IsVisible() const
        {
            return BasicRenderable::IsVisible() &&
                ( BackgroundColours[0] != 0 &&
                  BackgroundColours[1] != 0 &&
                  BackgroundColours[2] != 0 &&
                  BackgroundColours[3] != 0 );
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Background Methods

        void Rectangle::SetBackgroundColour(const ColourValue& Colour)
        {
            BackgroundColours[0] = Colour;
            BackgroundColours[1] = Colour;
            BackgroundColours[2] = Colour;
            BackgroundColours[3] = Colour;
            _MarkDirty();
        }

        void Rectangle::SetBackgroundColour(const UI::QuadCorner& Corner, const ColourValue& Colour)
        {
            BackgroundColours[Corner] = Colour;
            _MarkDirty();
        }

        void Rectangle::SetBackgroundSprite(Sprite* PSprite)
        {
            NormalSprite = PSprite;
            if( MouseHover && HoveredSprite )
                return;
            SetSprite(PSprite);
        }

        void Rectangle::SetBackgroundSprite(const String& SpriteName)
        {
            Sprite* PSprite = ParentScreen->GetSprite(SpriteName,PriAtlas);
            SetBackgroundSprite(PSprite);
        }

        void Rectangle::SetBackgroundSprite(const String& SpriteName, const String& Atlas)
        {
            Sprite* PSprite = ParentScreen->GetSprite(SpriteName,Atlas);
            SetBackgroundSprite(PSprite);
        }

        void Rectangle::SetHoveredSprite(Sprite* PSprite)
        {
            HoveredSprite = PSprite;
            if(MouseHover)
            {
                SetSprite(PSprite);
            }
        }

        void Rectangle::SetHoveredSprite(const String& SpriteName)
        {
            Sprite* PSprite = ParentScreen->GetSprite(SpriteName,PriAtlas);
            SetHoveredSprite(PSprite);
        }

        void Rectangle::SetHoveredSprite(const String& SpriteName, const String& Atlas)
        {
            Sprite* PSprite = ParentScreen->GetSprite(SpriteName,Atlas);
            SetHoveredSprite(PSprite);
        }

        void Rectangle::SetBackgroundGradient(const UI::Gradient& Grad, const ColourValue& ColourA, const ColourValue& ColourB)
        {
            if (UI::Gradient_NorthSouth == Grad)
            {
                BackgroundColours[0] = ColourA;
                BackgroundColours[1] = ColourA;
                BackgroundColours[2] = ColourB;
                BackgroundColours[3] = ColourB;
            }
            else if (UI::Gradient_WestEast == Grad)
            {
                BackgroundColours[0] = ColourA;
                BackgroundColours[3] = ColourA;
                BackgroundColours[1] = ColourB;
                BackgroundColours[2] = ColourB;
            }
            else if (UI::Gradient_Diagonal_1 == Grad)
            {
                ColourValue Average;
                Average.R = (ColourA.R + ColourB.R) * 0.5f;
                Average.G = (ColourA.G + ColourB.G) * 0.5f;
                Average.B = (ColourA.B + ColourB.B) * 0.5f;
                Average.A = (ColourA.A + ColourB.A) * 0.5f;
                BackgroundColours[0] = ColourA;
                BackgroundColours[1] = Average;
                BackgroundColours[2] = ColourB;
                BackgroundColours[3] = Average;
            }
            else if (UI::Gradient_Diagonal_2 == Grad)
            {
                ColourValue Average;
                Average.R = (ColourA.R + ColourB.R) * 0.5f;
                Average.G = (ColourA.G + ColourB.G) * 0.5f;
                Average.B = (ColourA.B + ColourB.B) * 0.5f;
                Average.A = (ColourA.A + ColourB.A) * 0.5f;
                BackgroundColours[0] = Average;
                BackgroundColours[1] = ColourA;
                BackgroundColours[2] = Average;
                BackgroundColours[3] = ColourB;
            }
            _MarkDirty();
        }

        ColourValue Rectangle::GetBackgroundColour(const UI::QuadCorner& Corner) const
        {
            return BackgroundColours[Corner];
        }

        void Rectangle::SetBorderWidth(const Real& Width)
        {
            BorderWidth = Width;
            _MarkDirty();
        }

        void Rectangle::SetBorderColour(const ColourValue& Colour)
        {
            BorderColours[0] = Colour;
            BorderColours[1] = Colour;
            BorderColours[2] = Colour;
            BorderColours[3] = Colour;
            _MarkDirty();
        }

        void Rectangle::SetBorderColour(const UI::Border& Side, const ColourValue& Colour)
        {
            BorderColours[Side] = Colour;
            _MarkDirty();
        }

        void Rectangle::SetBorder(const Real& Width, const ColourValue& Colour)
        {
            SetBorderWidth(Width);
            SetBorderColour(Colour);
        }

        void Rectangle::SetBorder(const Real& Width, const ColourValue& North, const ColourValue& South, const ColourValue& East, const ColourValue& West)
        {
            SetBorderWidth(Width);
            BorderColours[UI::Border_North] = North;
            BorderColours[UI::Border_South] = South;
            BorderColours[UI::Border_East] = East;
            BorderColours[UI::Border_West] = West;
        }

        void Rectangle::NoBorder()
        {
            SetBorderWidth(0.0);
            SetBorderColour(ColourValue::Black());
        }

        Real Rectangle::GetBorderWidth() const
        {
            return BorderWidth;
        }

        ColourValue Rectangle::GetBorderColour(const UI::Border& Side) const
        {
            return BorderColours[Side];
        }

        void Rectangle::SetRotationDegrees(const Real& Degrees)
        {
            RotAngle = MathTools::DegreesToRadians(Degrees);
            _MarkDirty();
        }

        void Rectangle::SetRotationRadians(const Real& Radians)
        {
            RotAngle = Radians;
            _MarkDirty();
        }

        Real Rectangle::GetRotationDegrees() const
        {
            return MathTools::RadiansToDegrees(RotAngle);
        }

        Real Rectangle::GetRotationRadians() const
        {
            return RotAngle;
        }

        void Rectangle::SetRotationCenter(bool Custom, const Vector2& Center)
        {
            CustomCenter = Custom;
            RotCenter = Center;
            _MarkDirty();
        }

        Vector2 Rectangle::GetRotationCenter() const
        {
            return RotCenter;
        }

        void Rectangle::SetPosition(const Vector2& Position)
        {
            const Vector2& WinDim = ParentScreen->GetViewportDimensions();
            RelPosition = Position;
            ActPosition = Position * WinDim;
            _MarkDirty();
        }

        void Rectangle::SetActualPosition(const Vector2& Position)
        {
            const Vector2& WinDim = ParentScreen->GetViewportDimensions();
            RelPosition = Position / WinDim;
            ActPosition = Position;
            _MarkDirty();
        }

        void Rectangle::SetSize(const Vector2& Size)
        {
            const Vector2& WinDim = ParentScreen->GetViewportDimensions();
            RelSize = Size;
            ActSize = Size * WinDim;
            _MarkDirty();
        }

        void Rectangle::SetActualSize(const Vector2& Size)
        {
            const Vector2& WinDim = ParentScreen->GetViewportDimensions();
            RelSize = Size / WinDim;
            ActSize = Size;
            _MarkDirty();
        }

        void Rectangle::UpdateDimensions()
        {
            //this->SetActualPosition(RelPosition * ParentScreen->GetViewportDimensions());
            //this->SetActualSize(RelSize * ParentScreen->GetViewportDimensions());
            ViewportUpdateTool::UpdateRectangleRenderable(this);
        }

        void Rectangle::_Redraw()
        {
            if(Dirty == false)
                return;
            RenderVertices.clear();
            if(!Visible)
            {
                Dirty = false;
                return;
            }

            Real TexelOffsetX = ParentScreen->GetTexelOffsetX();
            Real TexelOffsetY = ParentScreen->GetTexelOffsetY();
            Vector2 TopLeft, TopRight, BottomLeft, BottomRight;
            TopLeft.X = ActPosition.X + TexelOffsetX;                   TopLeft.Y = ActPosition.Y + TexelOffsetY;
            TopRight.X = (ActPosition.X + ActSize.X) + TexelOffsetX;    TopRight.Y = ActPosition.Y + TexelOffsetY;
            BottomLeft.X = ActPosition.X + TexelOffsetX;                BottomLeft.Y = (ActPosition.Y + ActSize.Y) + TexelOffsetY;
            BottomRight.X = (ActPosition.X + ActSize.X) + TexelOffsetX; BottomRight.Y = (ActPosition.Y + ActSize.Y) + TexelOffsetY;
            Vector2 OuterTopLeft = TopLeft, OuterTopRight = TopRight, OuterBottomLeft = BottomLeft, OuterBottomRight = BottomRight;

            // Rotation
            RotationTransform(TopLeft,TopRight,BottomLeft,BottomRight);

            // Border
            if(0.0 != BorderWidth)
            {
                OuterTopLeft.X -= BorderWidth;     OuterTopLeft.Y -= BorderWidth;
                OuterTopRight.X += BorderWidth;    OuterTopRight.Y -= BorderWidth;
                OuterBottomLeft.X -= BorderWidth;  OuterBottomLeft.Y += BorderWidth;
                OuterBottomRight.X += BorderWidth; OuterBottomRight.Y += BorderWidth;

                RotationTransform(OuterTopLeft,OuterTopRight,OuterBottomLeft,OuterBottomRight);
                DrawBorder(TopLeft,TopRight,BottomLeft,BottomRight,
                           OuterTopLeft,OuterTopRight,OuterBottomLeft,OuterBottomRight);
            }
            // Fill
            DrawFill(TopLeft,TopRight,BottomLeft,BottomRight);

            Dirty = false;
        }

        void Rectangle::_AppendVertices(ScreenVertexData& Vertices)
        {
            switch(Priority)
            {
                case UI::RP_Low:
                {
                    for( Whole X = 0 ; X < RenderVertices.size() ; ++X )
                    {
                        Vertices.LowVertices.push_back(RenderVertices[X]);
                    }
                    break;
                }
                case UI::RP_Medium:
                {
                    for( Whole X = 0 ; X < RenderVertices.size() ; ++X )
                    {
                        Vertices.MediumVertices.push_back(RenderVertices[X]);
                    }
                    break;
                }
                case UI::RP_High:
                {
                    for( Whole X = 0 ; X < RenderVertices.size() ; ++X )
                    {
                        Vertices.HighVertices.push_back(RenderVertices[X]);
                    }
                    break;
                }
            }
        }
    }//UI
}//Mezzanine

#endif
