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
#ifndef _resourcedatastream_cpp
#define _resourcedatastream_cpp

#include "Resource/datastream.h"
#include "stringtool.h"
#include "exception.h"

#include <cstring>
#include <cstdio>
#include <algorithm>

#define TEMP_STREAM_SIZE 128

namespace Mezzanine
{
    namespace Resource
    {
#ifdef USENEWDATASTREAM
        void IOStream::Advance(const StreamOff Count)
        {
            this->SetStreamPosition(Count,SO_Current);
        }

        void IOStream::SetStreamPosition(StreamPos Position)
        {
            this->SetReadPosition(Position);
            this->SetWritePosition(Position);
        }

        void IOStream::SetStreamPosition(StreamOff Offset, SeekOrigin Origin)
        {
            this->SetReadPosition(Offset,Origin);
            this->SetWritePosition(Offset,Origin);
        }

        StreamPos IOStream::GetStreamPosition(bool Read = true)
        {
            if(Read) return this->GetReadPosition();
            else return this->GetWritePosition();
        }
#else //USENEWDATASTREAM
        ///////////////////////////////////////////////////////////////////////////////
        // DataStream Methods
        ///////////////////////////////////////
        DataStream::DataStream(const UInt16 Flags)
            : SFlags(Flags),
              Size(0)
        {
        }

        DataStream::~DataStream()
        {
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        StreamSize DataStream::GetSize() const
        {
            return Size;
        }

        bool DataStream::IsReadable() const
        {
            return (SFlags & DataStream::SF_Read);
        }

        bool DataStream::IsWriteable() const
        {
            return (SFlags & DataStream::SF_Write);
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Stream Access and Manipulation

        ///////////////////////////////////////////////////////////////////////////////
        // Formatting Methods

        String DataStream::GetAsString()
        {
            size_t BufferSize = (this->Size > 0 ? this->Size : 4096);
            char* Buffer = new char[BufferSize];

            this->SetStreamPosition(0);
            String Ret;
            while (!EoF())
            {
                size_t BytesRead = Read(Buffer,BufferSize);
                Ret.append(Buffer,BytesRead);
            }
            delete[] Buffer;
            return Ret;
        }

        size_t DataStream::ReadLine(Char8* Buffer, size_t MaxCount, const String& Delim)
        {
            bool TrimCR = false;
            if(Delim.find_first_of('\n') != String::npos)
            {
                TrimCR = true;
            }

            char Temp[TEMP_STREAM_SIZE];
            size_t ChunkSize = std::min(MaxCount,(size_t)TEMP_STREAM_SIZE - 1);
            size_t TotalCount = 0;
            size_t ReadCount = 0;

            while(ChunkSize && (ReadCount = Read(Temp,ChunkSize)) != 0)
            {
                Temp[ReadCount] = '\0';
                size_t Pos = std::strcspn(Temp,Delim.c_str());

                if(Pos < ReadCount)
                {
                    this->Advance((long)(Pos + 1 - ReadCount));
                }

                if(Buffer)
                {
                    std::memcpy(Buffer + TotalCount,Temp,Pos);
                }
                TotalCount += Pos;

                if(Pos < ReadCount)
                {
                    if(TrimCR && TotalCount && Buffer[TotalCount - 1] == '\r')
                    {
                        --TotalCount;
                    }
                    break;
                }

                ChunkSize = std::min(MaxCount - TotalCount,(size_t)TEMP_STREAM_SIZE - 1);
            }
            Buffer[TotalCount] = '\0';
            return TotalCount;
        }

        String DataStream::GetLine(bool Trim)
        {
            char Temp[TEMP_STREAM_SIZE];
            String Ret;
            size_t ReadCount;

            while( (ReadCount = Read(Temp,TEMP_STREAM_SIZE - 1)) != 0 )
            {
                Temp[ReadCount] = '\0';

                char* Pos = std::strchr(Temp,'\n');
                if(Pos != 0)
                {
                    this->Advance((long)(Pos + 1 - Temp - ReadCount));
                    *Pos = '\0';
                }

                Ret += Temp;

                if(Pos != 0)
                {
                    if(Ret.length() && Ret[Ret.length() - 1] == '\r')
                    {
                        Ret.erase(Ret.length() - 1, 1);
                    }
                    break;
                }
            }

            if(Trim)
            {
                StringTools::Trim(Ret);
            }

            return Ret;
        }

        size_t DataStream::SkipLine(const String& Delim)
        {
            char Temp[TEMP_STREAM_SIZE];
            size_t TotalBytes = 0;
            size_t ReadCount = 0;

            while( (ReadCount = Read(Temp,TEMP_STREAM_SIZE - 1)) != 0 )
            {
                Temp[ReadCount] = '\0';
                size_t Position = std::strcspn(Temp,Delim.c_str());

                if(Position < ReadCount)
                {
                    this->Advance((long)(Position + 1 - ReadCount));
                    TotalBytes += Position + 1;
                    break;
                }

                TotalBytes += ReadCount;
            }

            return TotalBytes;
        }
#endif
    }//Resource
}//Mezzanine

#endif
