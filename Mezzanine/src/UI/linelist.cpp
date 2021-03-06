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
#ifndef _uilinelist_cpp
#define _uilinelist_cpp

#include "UI/linelist.h"
#include "uimanager.h"
#include "UI/screen.h"
#include "UI/vertex.h"
#include "uienumerations.h"
#include "UI/viewportupdatetool.h"
#include "entresol.h"

namespace Mezzanine
{
    namespace UI
    {
        LineList::LineList(const String& name, Screen* PScreen)
            : BasicRenderable(name,PScreen)
        {
        }

        LineList::~LineList()
        {
        }

        void LineList::Begin(const Whole& LineThickness, const ColourValue& LineColour)
        {
            Positions.clear();
            Thickness = LineThickness;
            Colour = LineColour;
            _MarkDirty();
        }

        void LineList::AddPoint(const Real& X, const Real& Y)
        {
            AddPoint(Vector2(X,Y));
        }

        void LineList::AddPoint(const Vector2& Position)
        {
            Positions.push_back(Position * ParentScreen->GetViewportDimensions());
        }

        void LineList::AddActualPoint(const Real& X, const Real& Y)
        {
            AddPoint(Vector2(X,Y));
        }

        void LineList::AddActualPoint(const Vector2& Position)
        {
            Positions.push_back(Position);
        }

        void LineList::End(bool Closed)
        {
            IsClosed = Closed;
            _MarkDirty();
        }

        void LineList::UpdateDimensions()
        {
            Vector2 OldMid = ViewportUpdateTool::GetOldSize() * 0.5;
            Vector2 NewMid = ViewportUpdateTool::GetNewSize() * 0.5;

            for( Whole Index = 0 ; Index < Positions.size() ; Index++ )
            {
                Positions[Index] = (Positions[Index] - OldMid) + NewMid;
            }
        }

        void LineList::_Redraw()
        {
            if(Dirty == false)
                return;
            RenderVertices.clear();
            if(!Visible)
            {
                Dirty = false;
                return;
            }
            if(Positions.size() < 2)
                return;

            VertexData Temp;
            Real HalfThickness = Thickness * 0.5;
            Vector2 PerpNorm, LastLeft, LastRight, ThisLeft, ThisRight, UV = ParentScreen->GetSolidUV(PriAtlas);
            size_t Index = 1;
            for(  ; Index < Positions.size() ; Index++ )
            {
                PerpNorm  = (Positions[Index] - Positions[Index - 1]).Perpendicular().Normalize();
                LastLeft  = Positions[Index -1 ] - PerpNorm * HalfThickness;
                LastRight = Positions[Index -1 ] + PerpNorm * HalfThickness;
                ThisLeft  = Positions[Index] - PerpNorm * HalfThickness;
                ThisRight = Positions[Index] + PerpNorm * HalfThickness;

                // Triangle A
                PushVertex(RenderVertices,Temp,LastRight.X,LastRight.Y,UV,Colour,PriAtlas);      // Left/Bottom
                PushVertex(RenderVertices,Temp,ThisLeft.X,ThisLeft.Y,UV,Colour,PriAtlas);        // Right/Top
                PushVertex(RenderVertices,Temp,LastLeft.X,LastLeft.Y,UV,Colour,PriAtlas);        // Left/Top
                // Triangle B
                PushVertex(RenderVertices,Temp,LastRight.X,LastRight.Y,UV,Colour,PriAtlas);      // Left/Bottom
                PushVertex(RenderVertices,Temp,ThisRight.X,ThisRight.Y,UV,Colour,PriAtlas);      // Right/Bottom
                PushVertex(RenderVertices,Temp,ThisLeft.X,ThisLeft.Y,UV,Colour,PriAtlas);        // Right/Top
            }

            if(IsClosed)
            {
                Index = Positions.size() - 1;
                PerpNorm  = (Positions[0] - Positions[Index]).Perpendicular().Normalize();
                LastLeft  = Positions[Index] - PerpNorm * HalfThickness;
                LastRight = Positions[Index] + PerpNorm * HalfThickness;
                ThisLeft  = Positions[0] - PerpNorm * HalfThickness;
                ThisRight = Positions[0] + PerpNorm * HalfThickness;

                // Triangle A
                PushVertex(RenderVertices,Temp,LastRight.X,LastRight.Y,UV,Colour,PriAtlas);       // Left/Bottom
                PushVertex(RenderVertices,Temp,ThisLeft.X,ThisLeft.Y,UV,Colour,PriAtlas);         // Right/Top
                PushVertex(RenderVertices,Temp,LastLeft.X,LastLeft.Y,UV,Colour,PriAtlas);         // Left/Top
                // Triangle B
                PushVertex(RenderVertices,Temp,LastRight.X,LastRight.Y,UV,Colour,PriAtlas);       // Left/Bottom
                PushVertex(RenderVertices,Temp,ThisRight.X,ThisRight.Y,UV,Colour,PriAtlas);       // Right/Bottom
                PushVertex(RenderVertices,Temp,ThisLeft.X,ThisLeft.Y,UV,Colour,PriAtlas);         // Right/Top
            }
        }

        void LineList::_AppendVertices(ScreenVertexData& Vertices)
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
