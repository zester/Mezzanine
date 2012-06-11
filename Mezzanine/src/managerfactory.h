// Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _managerfactory_h
#define _managerfactory_h

#include "datatypes.h"

namespace Mezzanine
{
    class ManagerBase;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class ManagerFactory
    /// @headerfile managerfactory.h
    /// @brief This is a base class for factories that construct the managers the engine uses.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB ManagerFactory
    {
        public:
            /// @brief Class constructor.
            ManagerFactory() {};
            /// @brief Class destructor.
            virtual ~ManagerFactory() {};

            ///////////////////////////////////////////////////////////////////////////////
            // Virtual Methods
            /// @brief Gets the name of the Manager that is created by this factory.
            /// @return Returns the typename of the manager created by this factory.
            virtual String GetManagerTypeName() const = 0;
            /// @brief Creates a manager of the type represented by this factory.
            /// @param Params A NameValuePairList containing the params to be applied during construction.
            /// @return Returns a pointer to the manager created.
            virtual ManagerBase* CreateManager(NameValuePairList& Params) = 0;
#ifdef MEZZXML
            /// @brief Creates a manager from XML.
            /// @param XMLNode The node of the xml document to construct from.
            /// @return Returns a pointer to the manager created.
            virtual ManagerBase* CreateManager(XML::Node& XMLNode) = 0;
#endif
            /// @brief Destroys a Manager created by this factory.
            /// @param ToBeDestroyed A pointer to the manager to be destroyed.
            virtual void DestroyManager(ManagerBase* ToBeDestroyed) = 0;
    };//ManagerFactory
}//Mezzanine

#endif