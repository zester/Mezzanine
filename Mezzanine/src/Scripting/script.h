//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _script_h
#define _script_h

/// @file
/// @brief This file has the interfaces for Scripts and tag derived classes.

#include "datatypes.h"
#include "scriptargument.h"
#include "smartptr.h"

namespace Mezzanine
{
    namespace Scripting
    {
        // Forward Declares
        class iScriptCompilable;
        class iScriptMultipleReturn;

        ///////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief The interface for a script
        /// @details All the methods that all scripts for all languages must implement.
        /// These are tiny pieces of data that can be run like miniature programs.
        /// @n @n
        /// These can be executed by passing them to the appropriate Script Manager.
        /// @n @n
        /// This uses multiple inheritance to minimize the amount of features a scripting langauge with need to
        /// implement. It is expected that a simple scripting language may only need to implement the base
        /// @ref iScript interface. Other languages that support more features can have their script classes
        /// multiply inherit from the other classes like
        ///////////////////////////////////////
        class MEZZ_LIB iScript
        {
            public:
                ///////////////////////////////////////////////////////////////////////////////////////////////////
                // Work With Arguments

                /// @brief This adds an argument to be passed to the script.
                /// @details All arguments added with this are passed in FIFO order to the Script during or just
                /// before execution. This should normally run in constant time. Some scripting implementations may
                /// change the order arguments are passed if doing it another way mays more sense.
                /// @param Arg This accepts a CountedPtr to a script argument and The script shares responsibility with caller for deleting the argument.
                virtual void AddArgument(CountedPtr<iScriptArgument> Arg) = 0;

                /// @brief Remove an argument based on a CountedPtr to the script
                /// @detail This searches through the internal list and removes the first entry it finds matching this.
                /// This should be treated as taking linear time, relative to the total count of arguments assigned to this script, to run.
                /// This can be used with AddArgument to re-order the way parameters are passed into a script
                /// @param Arg A CountedPtr matching the one to be removed
                virtual void RemoveArgument(CountedPtr<iScriptArgument> Arg) = 0;

                /// @brief Remove a Script argument based on the order it will be passed into the Script at Execution.
                /// @detail This removes the  specified Argument from the internal list. This should be treated as taking linear
                /// time, relative to the total count of arguments assigned to this script, to run.
                /// @param ArgNumber The number of the Argument to be removed. This behaves similar to an array or vector as it starts counting at 0.
                virtual void RemoveArgument(Whole ArgNumber) = 0;

                /// @brief How many arguments have been attached to this script?
                /// @return A Whole containing the amount of arguments passed in so far.
                virtual Whole GetArgumentCount() const = 0;

                /// @brief Remove all the ARGs!!! http://imgur.com/DJhw7
                /// @details This should run in constant time. It still might be slower than removing and readding just one a few arguments
                /// in simple cases
                virtual void ClearArguments() = 0;

                /// @brief Retrieve a argument previously passed in.
                /// @param ArgNumber The index of the passed parameter to retrun.
                /// @return A reference counted pointer to a ScriptArgument.
                virtual CountedPtr<iScriptArgument> GetArgument(Whole ArgNumber) const = 0;

                ///////////////////////////////////////////////////////////////////////////////////////////////////
                // Source of the Script

                /// @brief Sets the string version of the script.
                /// @param Code A string that defines the source code to be executed or compiled whne running the script.
                /// @detail It is recomended that when this is called that implentors clear any bytecode or any other compiled
                /// version of the script. This will prevent issues with mismatched version of source and bytecode.
                virtual void SetSourceCode(const String& Code) = 0;

                /// @brief If present this returns the code of script
                /// @return This will return either an empty @ref String or the code. In cases where bytcode is set it is advised to clear this in implementations.
                virtual String GetSourceCode() const = 0;

                ///////////////////////////////////////////////////////////////////////////////////////////////////
                // Compilation Detection support

                /// @brief Used to check if there is a bytecode version of the script available.
                /// @return This will return false on all scripts not implementing @ref ScriptCompilable and only false when the bytecode is already compiled.
                virtual bool IsCompiled() const
                    { return false; }

                /// @brief Used to check if this Script supports compilation bytecode.
                /// @return This will return false on all scripts not implementing @ref ScriptCompilable and on those that do implement it.
                virtual bool IsCompilable() const
                    { return false; }

                /// @brief If your only handle to this is a pointer of type @ref iScript this can be called to get a pointer to an @ref iScriptCompilable if it would be valid
                /// @return A null pointer if this conversion is invalid or a valid pointer to this as an @ref iScriptCompilable if it is valid.
                virtual iScriptCompilable* GetAsScriptCompilable()
                    { return 0; }

                /// @copydoc GetAsScriptCompilable
                virtual iScriptCompilable* GetAsScriptCompilable() const
                { return 0; }

                ///////////////////////////////////////////////////////////////////////////////////////////////////
                // Multiple return Detection Support

                /// @brief Does this script support multiple return values.
                /// @detail Some scripting language support return tuples of values(Python), return an array of values (javascript), returning tables made of records
                /// which are groups or values(sql), and some allow return an arbitrary number of items that could be tables, or values and allow for tables to contain
                /// more tables and values(Lua). This allows for checking for an interface to retrieve some of these.
                virtual bool CanReturnMultples()
                    { return false; }

                /// @brief If your only handle to this is a pointer of type @ref iScript this can be called to get a pointer to an @ref iScriptMultipleReturn if it would be valid
                /// @return A null pointer if this conversion is invalid or a valid pointer to this as an @ref iScriptMultipleReturn if it is valid.
                virtual iScriptMultipleReturn* GetAsiScriptMultipleReturn()
                    { return 0; }

        };

        ///////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief A way to store and pass compile binary scripts.
        /// @details Intended for use with @ref ScriptCompilable as a basic way to store and pass bytecode around.
        /// This does not delete the binary on destruction and is not intended to. Some script interpretter's
        /// will manage their own memory and in other situations is seems likely the Game developor will want to
        /// manage bytecode lifecycle. Otherwise creating a derived class may be useful.
        /// @n @n
        /// This is designed to be fairly minimalistic and pointer-like so that it can be passed by value to reduce
        /// double pointer indirection and caching costs.
        struct ByteCode
        {
            public:

                /// @brief How many bytes is @ref Binary in size. This is set to 0 if @ref Binary is invalid and should be a null pointer.
                Whole Size;

                /// @brief A pointer to the actual compiled bytecode.
                UInt8* Binary;

                /// @brief Default constructor, set everything to zero.
                ByteCode() :
                    Size(0),
                    Binary(NULL)
                    {}

                /// @brief Verbose constructor, set everything custom on creation.
                /// @param PredeterminedSize The size to set on creation.
                /// @param PreCompiledBinary A pointer to the compiled byte in memory, to be set on creation.
                ByteCode(Integer PredeterminedSize, UInt8* PreCompiledBinary) :
                    Size(PredeterminedSize),
                    Binary(PreCompiledBinary)
                    {}
        };

        ///////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief The interface for a script that can be compiled to bytecode
        /// @details All the members that all script for all languages must implement that
        /// support dynamic compilation in the Mezzanine.
        /// @n @n
        /// These are tiny pieces of data that can be run like miniature programs. In some cases
        /// they will be compiled to a bytecode that can swiftly be executed by the appropriate
        /// bytecode interpretter. This is generally faster than interpretting text, but slower than
        /// running machine code.
        /// @n @n
        /// This class is designed for use with multiple inheritance. If a script returns true from
        /// @ref IsCompilable then its pointer can safely be cast to a ScriptCompilable pointer.
        /// @todo Add sample code of safe cast in ScriptCompilable, becuase that is kinda wierd.
        ///////////////////////////////////////
        class MEZZ_LIB iScriptCompilable : public virtual iScript
        {
            public:

                /// @brief Set the bytecode used when this script is executed.
                /// @param Code The Binary version of the script
                /// @details This is what will be executed. No reverse compiling support is provided, so it is advisable
                /// that implementations of this either clear the source code or set it to the source that matches the
                /// compiled binary.
                virtual void SetByteCode(ByteCode Code) = 0;

                /// @brief Get the compiled version of the code if it is available.
                /// @return If there is valid byte code this will retrieve that, otherwise this will return an empty bytecode.
                virtual ByteCode GetByteCode() const = 0;

                /// @brief Has this script already been compiled into a bytecode.
                /// @return True if there is bytecode available false otherwise.
                virtual bool IsCompiled() const = 0;

                /// @brief Any script implementing this class is compilable.
                /// @brief returns true.
                virtual bool IsCompilable() const
                    { return true; }

                virtual iScriptCompilable* GetAsScriptCompilable()
                    { return this; }
        };


        /// @brief A group of arguments that can be returned from some scripts
        typedef std::vector< CountedPtr<iScriptArgument> > ArgumentSet;

        /// @brief This script can return simple group of values.
        /// @details This loosely correlates to a tuple like the simple returns
        /// in Lua or any return in Python. This cannot handle returns that include
        /// tuples that contain tuples in a graceful way.
        class MEZZ_LIB iScriptMultipleReturn : public virtual iScript
        {
                /// @brief Does this script support multiple return values.
                /// @return Any implementation of this returns true.
                virtual bool CanReturnMultples() const
                    { return true; }

                virtual iScriptMultipleReturn* GetAsiScriptMultipleReturn()
                    { return this; }

                /// @brief How many values are being returned
                /// @return A Whole with the amount of items available to be returned now.
                virtual Whole GetReturnCount() const = 0;

                /// @brief Get the returns from the last exection of the script
                /// @return An ArgumentSet that can be iterated over to get all the values returned.
                virtual ArgumentSet GetAllReturns() const = 0;
        };


    }


}//Mezzanine



#endif // \_script_h