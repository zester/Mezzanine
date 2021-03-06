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
#ifndef _vector2_h
#define _vector2_h

#include "crossplatformexport.h"
#include "datatypes.h"
#ifndef SWIG
    #include "XML/xml.h"
#endif

namespace Ogre
{
    class Vector2;
}

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class Vector2
    /// @headerfile vector2.h
    /// @brief This is used to represent a point on a 2 dimentional area, such as a screen.
    /// @details This contains an X and Y value used to represent coordinates.
    /// This also has a number of facilities to make converting to graphics subsystems
    /// as easy as possible.
    ///////////////////////////////////////
    class MEZZ_LIB Vector2
    {
        public:
            /// @brief Coordinate on the X vector.
            Real X;
            /// @brief Coordinate on the Y vector.
            Real Y;
            /// @brief Default Constructor.
            /// @details Basic all zero initialization constructor.
            Vector2();
            /// @brief Real value Constructor.
            /// @details Constructor that sets both vectors.
            /// @param x Coordinate on the X vector.
            /// @param y Coordinate on the Y vector.
            Vector2(const Real& x, const Real& y);
            /// @brief Ogre Value Constructor.
            /// @details Constructor that sets all values to match the Ogre vector.
            /// @param Vec The vector to be copied to make this vector.
            Vector2(const Ogre::Vector2& Vec);

            /// @brief Gets a Ogre vector2.
            /// @details Creates a Ogre vector2 with values equal to this class and returns it.
            Ogre::Vector2 GetOgreVector2() const;
            /// @brief Copies an existing Ogre vector2.
            /// @details This function will copy the values stored in an existing Ogre vector2
            /// and set the values of this class to be the same.
            /// @param Ours The vector2 to be extracted.
            void ExtractOgreVector2(const Ogre::Vector2& Ours);

            /// @brief Sets the values of this vector2 to identity values(0,0).
            void SetIdentity();
            /// @brief Sets the X and Y values of this vector2.
            /// @param x The real that will have this vector's X member set to.
            /// @param y The real that will have this vector's Y member set to.
            void SetValues(const Real& x, const Real& y);

            ///////////////////////////////////////////////////////////////////////////////
            // Equality Comparison operators

            /// @brief Equality Comparison Operator
            /// @details Returns true if X==X and Y==Y. If any of those do not match this returns false.
            /// @param Vec2 This is the other Mezzanine::Vector2.
            bool operator== (const Mezzanine::Vector2 &Vec2);

            /// @brief Equality Comparison Operator
            /// @details Returns true if X!=X or Y!=Y. If any of those do not match this returns false.
            /// @param Vec2 This is the other Mezzanine::Vector2.
            bool operator!= (const Mezzanine::Vector2 &Vec2);

            /// @brief Equality Comparison Operator
            /// @details Returns true if X==X and Y==Y. If any of those do not match this returns false.
            /// @param Vec2 This is the other Ogre::Vector2.
            bool operator== (const Ogre::Vector2 &Vec2);

            /// @brief Equality Comparison Operator
            /// @details Returns true if X!=X or Y!=Y. If any of those do not match this returns false.
            /// @param Vec2 This is the other Ogre::Vector2.
            bool operator!= (const Ogre::Vector2 &Vec2);

            ///////////////////////////////////////////////////////////////////////////////
            // Vector2 Arithmetic with Real

            /// @brief Scaling by multiplication
            /// @details This Multiplies X and Y by scalar
            /// @return This returns a Vector2 that has been scaled
            /// @param scalar This is the amount to scale the Vector2 by
            Vector2 operator* (const Real &scalar) const;

            /// @brief Scaling by Division
            /// @details This Diisionn X and Y by scalar
            /// @return This returns a Vector2 that has been scaled
            /// @param scalar This is the amount to scale the Vector2 by
            Vector2 operator/ (const Real &scalar) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Vector2 Arithmetic and assignment with Real

            /// @brief Scaling by multiplication.
            /// @details This Multiplies X and Y by scalar and stores the changes in this Vector2.
            /// @param scalar This is the amount to scale the Vector2 by.
            void operator*= (const Real &scalar);

            /// @brief Scaling by Division
            /// @details This Division X and Y by scalar and and stores the changes in this Vector2.
            /// @param scalar This is the amount to scale the Vector2 by
            void operator/= (const Real &scalar);

            ///////////////////////////////////////////////////////////////////////////////
            // Arithmetic Operators

            /// @brief Addition Operator
            /// @details Allows for addition from a Mezzanine::Vector2
            /// @param Vec2 This is the other Mezzanine::Vector2
            Vector2 operator+ (const Vector2 &Vec2) const;

            /// @brief Subraction Operator
            /// @details Allows for subtraction from a Mezzanine::Vector2
            /// @param Vec2 This is the other Mezzanine::Vector2
            Vector2 operator- (const Vector2 &Vec2) const;

            /// @brief Multiplaction Operator
            /// @details Allows for multiplaction from a Mezzanine::Vector2
            /// @param Vec2 This is the other Mezzanine::Vector2
            Vector2 operator* (const Vector2 &Vec2) const;

            /// @brief Division Operator
            /// @details Allows for division from a Mezzanine::Vector2
            /// @param Vec2 This is the other Mezzanine::Vector2
            Vector2 operator/ (const Vector2 &Vec2) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Fancy Math

            /// @brief Generates a Vector2 that is perpendicular to this vector.
            /// @return Returns a new Vector2 that is perpendicular to this.
            Vector2 Perpendicular() const;

            /// @brief Normalizes this Vector2.
            /// @return Returns a reference to this.
            Vector2& Normalize();

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization
            // Serializable
            /// @brief Convert this class to an XML::Node ready for serialization
            /// @param CurrentRoot The point in the XML hierarchy that all this vectorw should be appended to.
            void ProtoSerialize(XML::Node& CurrentRoot) const;

            // DeSerializable
            /// @brief Take the data stored in an XML and overwrite this instance of this object with it
            /// @param OneNode and XML::Node containing the data.
            void ProtoDeSerialize(const XML::Node& OneNode);

            /// @brief Get the name of the the XML tag this class will leave behind as its instances are serialized.
            /// @return A string containing "Vector2"
            static String SerializableName();
    };
}

// We can skip these operators when creating bindings with swig
#ifndef SWIG

/// @brief Used to Serialize an Mezzanine::Vector2 to a human readable stream
/// @details Sends proper XML to the output stream,
/// including versioning information which will be used to maintain backwards compatibility. The current XML format
/// will create one element with no sub-elements. The name of the xml node will be "Vector2". It will have 3 attributes.
/// "Version", will be set to a value of 1, indicating if came from version 1 compatible Vector3. It will also have an "X" and "Y"
/// attributes with values set appropriately. For example '<Vector2 Version="1" X="1" Y="2" />'.
/// @param x The Mezzanine::Vector2 to be converted to a stream of characters.
/// @param stream The place to send the characters, that define the Mezzanine::Vector2.
/// @return Get an std::ostream that was written to, this allow chaining of the << operators.
std::ostream& MEZZ_LIB operator << (std::ostream& stream, const Mezzanine::Vector2& x);

/// @brief Used to de-serialize an Mezzanine::Vector2 from a stream
/// @details This reads in the xml and sets the target vector according to values from the stream.
/// @param Vec The Mezzanine::Vector2 that will accept the values from the xml
/// @param stream The place to get the characters from, that define the Mezzanine::Vector2.
/// @return Get an std::ostream that was read from, this allow chaining of the >> operators.
/// @throw Can throw any exception that any function in the Mezzanine::xml namespace could throw in addition to a Mezzanine::Exception if the serialization version doesn't match.
std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::Vector2& Vec);

/// @brief Converts an XML node into a Mezzanine::Vector2
/// @details This will convert an XML::Node will a valid serialized Mezzanine::Vector2 into a Mezzanine::Vector2
/// @param OneNode An XML Node containing the the text of a Vector2
/// @param Vec the Mezzanine::Vector2 to store the deserialized Vector2
/// @return This returns a reference to the XML::Node for operator chaining or whatever.
/// @throw Can throw any exception that any function in the Mezzanine::xml namespace could throw in addition to a Mezzanine::Exception if the serialization version doesn't match.
void MEZZ_LIB  operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Vector2& Vec);

#endif // \SWIG


#endif
