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
#ifndef _CountedPtrtests_h
#define _CountedPtrtests_h

#include "main.h"

#include <iostream>
#include <typeinfo>

class SmartPtrTests : public UnitTestGroup
{
    public:
        virtual void RunTests(bool RunAutomaticTests, bool RunInteractiveTests)
        {
            if (RunAutomaticTests)
            {
                AddTestResult("CountedPtr::CreateClass", Unknown);
                AddTestResult("CountedPtr::CreatePOD", Unknown);
                AddTestResult("CountedPtr::CreateInterReferenceCount", Unknown);
                AddTestResult("CountedPtr::ScopedChanged", Unknown);
                AddTestResult("CountedPtr::operator=", Unknown);
                AddTestResult("CountedPtr::CopyConstructor", Unknown);
                AddTestResult("CountedPtr::CopyConstructorIntrusiveCount", Unknown);
                AddTestResult("CountedPtr::operator->", Unknown);
                AddTestResult("CountedPtr::operator*", Unknown);
                AddTestResult("CountedPtr::ReferenceCountTraits::BasicType", Unknown);
                AddTestResult("CountedPtr::ReferenceCountTraits::IntrusiveType", Unknown);
                AddTestResult("CountedPtr::ReferenceCountTraits::BasicPointer", Unknown);
                AddTestResult("CountedPtr::ReferenceCountTraits::IntrusivePointer", Unknown);
                AddTestResult("CountedPtr::ReferenceCountBasicPointer", Unknown);
                AddTestResult("CountedPtr::ReferenceCountIntrusivePointer", Unknown);

                std::cout << std::endl << "Creating A Vector3 in a counted pointer: VecPtr" << std::endl;
                Mezzanine::CountedPtr<Mezzanine::Vector3> VecPtr (new Mezzanine::Vector3(3.0,4.0,5.0));
                std::cout << "Count of references: " << VecPtr.use_count() << std::endl;

                if( !AddSuccessFromBool("CountedPtr::CreateClass",1 == VecPtr.use_count()) )
                    { return; } // If we cannot create a pointer then nothing else matters

                std::cout << "Creating creating a scoped copy of counted pointer." << std::endl;
                bool notfailed=true;
                {
                    Mezzanine::CountedPtr<Mezzanine::Vector3> StabbyScoped (VecPtr);
                    std::cout << "In scope count of references: " << VecPtr.use_count() << std::endl;
                    if (2 != VecPtr.use_count())
                        { notfailed=false; }
                }
                std::cout << "Post scope count of references: " << VecPtr.use_count() << std::endl;
                AddSuccessFromBool("CountedPtr::ScopedChanged", 1 == VecPtr.use_count() && notfailed);

                std::cout << std::endl << "Creating A fresh Vector3 in a counted pointer: OtherPtr." << std::endl;
                Mezzanine::CountedPtr<Mezzanine::Vector3> OtherPtr (new Mezzanine::Vector3(0.0,0.0,0.0));
                std::cout << "Count of OtherPtr references: " << OtherPtr.use_count() << std::endl;
                std::cout << "Creating OtherPtr2 with copy constructor, using OtherPtr as Original."<< std::endl;
                Mezzanine::CountedPtr<Mezzanine::Vector3> OtherPtr2 (OtherPtr);
                std::cout << "Count of OtherPtr references: " << OtherPtr.use_count() << std::endl;
                AddSuccessFromBool("CountedPtr::CopyConstructor",2 == OtherPtr.use_count());

                std::cout << std::endl << "Assigning OtherPtr the values of VecPtr." << std::endl;
                OtherPtr = VecPtr;
                AddSuccessFromBool("CountedPtr::operator=",2 == VecPtr.use_count() && 2 == OtherPtr.use_count() && 1 == OtherPtr2.use_count());
                AddSuccessFromBool("CountedPtr::operator->",3.0 == OtherPtr->X && 4.0 == OtherPtr->Y && 5.0 == OtherPtr->Z);
                AddSuccessFromBool("CountedPtr::operator*",3.0 == (*OtherPtr).X && 4.0 == (*OtherPtr).Y && 5.0 == (*OtherPtr).Z);


                AddSuccessFromBool("CountedPtr::ReferenceCountTraits::BasicType",(typeid(ReferenceCount<int>) == typeid(ReferenceCountTraits<int>::ManagedType)));
                AddSuccessFromBool("CountedPtr::ReferenceCountTraits::IntrusiveType",typeid(IntrusiveRefCount) == typeid(ReferenceCountTraits<IntrusiveRefCount>::ManagedType));
                AddSuccessFromBool("CountedPtr::ReferenceCountTraits::BasicPointer",typeid(ReferenceCount<int>*) == typeid(ReferenceCountTraits<int>::PtrType));
                AddSuccessFromBool("CountedPtr::ReferenceCountTraits::IntrusivePointer",typeid(IntrusiveRefCount*) == typeid(ReferenceCountTraits<IntrusiveRefCount>::PtrType));

                std::cout << std::endl << "Creating an int in a counted pointer: IntPtr" << std::endl;
                int* IntPointer = new int(10);
                Mezzanine::CountedPtr<int> intPtr (IntPointer);
                if( !AddSuccessFromBool("CountedPtr::CreatePOD",1 == intPtr.use_count()) )
                    { return; }
                AddSuccessFromBool("CountedPtr::ReferenceCountBasicPointer",typeid(ReferenceCount<int>*) == typeid(ReferenceCountTraits<int>::ConstructionPointer(IntPointer)));

                std::cout << std::endl << "Creating an IntrusiveRefCount in a counted pointer: RefCounterPointer" << std::endl;
                IntrusiveRefCount* RefCounterPointer = new IntrusiveRefCount();
                Mezzanine::CountedPtr<IntrusiveRefCount> RefCounterPtr (RefCounterPointer);
                if( !AddSuccessFromBool("CountedPtr::CreateInterReferenceCount",1 == RefCounterPtr.use_count()) )
                    { return; }
                AddSuccessFromBool("CountedPtr::ReferenceCountIntrusivePointer",typeid(IntrusiveRefCount*) == typeid(ReferenceCountTraits<IntrusiveRefCount>::ConstructionPointer(RefCounterPointer)));

                std::cout << "Creating creating a scoped copy of internally counted pointer." << std::endl;
                notfailed=true;
                {
                    Mezzanine::CountedPtr<IntrusiveRefCount> StabbyScoped (RefCounterPtr);
                    std::cout << "In scope count of references: " << RefCounterPtr.use_count() << std::endl;
                    if (2 != RefCounterPtr.use_count())
                        { notfailed=false; }
                }
                std::cout << "Post scope count of references: " << RefCounterPtr.use_count() << std::endl;
                AddSuccessFromBool("CountedPtr::CopyConstructorIntrusiveCount",1 == RefCounterPtr.use_count() && notfailed);

            }else{
                AddTestResult("CountedPtr::CreateClass", Skipped);
                AddTestResult("CountedPtr::CreatePOD", Skipped);
                AddTestResult("CountedPtr::CreateInterReferenceCount", Skipped);
                AddTestResult("CountedPtr::ScopedChanged", Skipped);
                AddTestResult("CountedPtr::operator=", Skipped);
                AddTestResult("CountedPtr::CopyConstructor", Skipped);
                AddTestResult("CountedPtr::CopyConstructorIntrusiveCount", Skipped);
                AddTestResult("CountedPtr::operator->", Skipped);
                AddTestResult("CountedPtr::operator*", Skipped);
                AddTestResult("CountedPtr::ReferenceCountTraits::BasicType", Skipped);
                AddTestResult("CountedPtr::ReferenceCountTraits::IntrusiveType", Skipped);
                AddTestResult("CountedPtr::ReferenceCountTraits::BasicPointer", Skipped);
                AddTestResult("CountedPtr::ReferenceCountTraits::IntrusivePointer", Skipped);
                AddTestResult("CountedPtr::ReferenceCountBasicPointer", Skipped);
                AddTestResult("CountedPtr::ReferenceCountIntrusivePointer", Skipped);
            }

        }
};
#endif
