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
#ifndef _audiosoundscapemanager_h
#define _audiosoundscapemanager_h

#include "worldmanager.h"
#include "managerfactory.h"
#include "Audio/audioenumerations.h"
#include "Threading/workunit.h"

namespace Mezzanine
{
    namespace Audio
    {
        class iListener;
        class SoundProxy;

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the work unit for updating audio buffers as necessary for audio playback.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB iBufferUpdate3DWorkUnit : public Threading::DefaultWorkUnit
        {
        public:
            /// @brief Class destructor.
            virtual ~iBufferUpdate3DWorkUnit() {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief This does any required updating of audio buffers belonging to sound proxies in this manager.
            /// @param CurrentThreadStorage The storage class for all resources owned by this work unit during it's execution.
            virtual void DoWork(Threading::DefaultThreadSpecificStorage::Type& CurrentThreadStorage) = 0;
        };//iBufferUpdate3DWorkUnit

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is the base manager class for audio being played in a 3D environment.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB SoundScapeManager : public WorldManager
        {
        public:
        protected:
        public:
            /// @brief Class constructor.
            SoundScapeManager();
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            SoundScapeManager(XML::Node& XMLNode);
            /// @brief Class destructor.
            virtual ~SoundScapeManager();

            ///////////////////////////////////////////////////////////////////////////////
            // Listener Management

            /// @brief Creates a new @ref iListener.
            /// @return Returns a pointer to the @ref iListener instance that was created.
            virtual iListener* CreateListener() = 0;
            /// @brief Gets an @ref iListener instance by index.
            /// @param Index The index of the @ref iListener to be retrieved.
            /// @return Returns a pointer to the @ref iListener at the specified index.
            virtual iListener* GetListener(const UInt32 Index) const = 0;
            /// @brief Gets the number of @ref iListener instances in this manager.
            /// @return Returns a UInt32 representing the number of @ref iListener instances contained in this manager.
            virtual UInt32 GetNumListeners() const = 0;
            /// @brief Deletes a @ref iListener.
            /// @param ToBeDestroyed A pointer to the @ref iListener you want deleted.
            virtual void DestroyListener(iListener* ToBeDestroyed) = 0;
            /// @brief Deletes all stored @ref iListener instances.
            virtual void DestroyAllListeners() = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Proxy Management

            /// @brief Creates a blank @ref SoundProxy without a stream attached to it.
            /// @param Type A UInt16 that is the ID for the type of sound the created @ref SoundProxy is to be categorized as.
            /// @return Returns a pointer to the @ref SoundProxy instance that was created.
            virtual SoundProxy* CreateSoundProxy(const UInt16 Type) = 0;
            /// @brief Creates a new @ref SoundProxy from a custom stream that is encoded.
            /// @param Type A UInt16 that is the ID for the type of sound the created @ref SoundProxy is to be categorized as.
            /// @param Stream A Datastream containing the data to be streamed for this @ref SoundProxy.
            /// @param Encode The encoding to be expected when decoding audio for this @ref SoundProxy.
            /// @return Returns a pointer to the @ref SoundProxy instance that was created.
            virtual SoundProxy* CreateSoundProxy(const UInt16 Type, Resource::DataStreamPtr Stream, const Audio::Encoding Encode) = 0;
            /// @brief Creates a new @ref SoundProxy from a custom stream that is unencoded.
            /// @param Type A UInt16 that is the ID for the type of sound the created @ref SoundProxy is to be categorized as.
            /// @param Stream A Datastream containing the data to be streamed for this @ref SoundProxy.
            /// @param Frequency The frequency (or sample rate) of the audio data.
            /// @param Config The bit configuration of the audio data.  Helps to determine sample size.
            /// @return Returns a pointer to the @ref SoundProxy instance that was created.
            virtual SoundProxy* CreateSoundProxy(const UInt16 Type, Resource::DataStreamPtr Stream, const UInt32 Frequency, const Audio::BitConfig Config) = 0;
            /// @brief Creates a new @ref SoundProxy from a file.
            /// @param Type A UInt16 that is the ID for the type of sound the created @ref SoundProxy is to be categorized as.
            /// @param FileName The name of the file to read audio data from.
            /// @param Group The resource group in which the file resides.
            /// @return Returns a pointer to the @ref SoundProxy instance that was created.
            virtual SoundProxy* CreateSoundProxy(const UInt16 Type, const String& FileName, const String& Group) = 0;
            /// @brief Creates a new @ref SoundProxy from a memory buffer that is encoded.
            /// @note The buffer is copied and put into a memory stream. Thus safe for disposal afterward.
            /// @param Type A UInt16 that is the ID for the type of sound the created @ref SoundProxy is to be categorized as.
            /// @param StreamName The name to be given to the stream being created.
            /// @param Buffer A pointer to the buffer where the audio data is located.
            /// @param Length The length of the buffer provided to this method(in bytes).
            /// @param Encode The encoding to be expected when decoding audio for this @ref SoundProxy.
            /// @return Returns a pointer to the @ref SoundProxy instance that was created.
            virtual SoundProxy* CreateSoundProxy(const UInt16 Type, const String& StreamName, Char8* Buffer, const UInt32 Length, const Audio::Encoding Encode) = 0;
            /// @brief Creates a new @ref SoundProxy from a memory buffer that is unencoded.
            /// @note The buffer is copied and put into a memory stream. Thus safe for disposal afterward.
            /// @param Type A UInt16 that is the ID for the type of sound the created @ref SoundProxy is to be categorized as.
            /// @param StreamName The name to be given to the stream being created.
            /// @param Buffer A pointer to the buffer where the audio data is located.
            /// @param Length The length of the buffer provided to this method(in bytes).
            /// @param Frequency The frequency (or sample rate) of the audio data.
            /// @param Config The bit configuration of the audio data.  Helps to determine sample size.
            /// @return Returns a pointer to the @ref SoundProxy instance that was created.
            virtual SoundProxy* CreateSoundProxy(const UInt16 Type, const String& StreamName, Char8* Buffer, const UInt32 Length, const UInt32 Frequency, const Audio::BitConfig Config) = 0;
            /// @brief Gets an @ref SoundProxy instance by index.
            /// @param Index The index of the @ref SoundProxy to be retrieved.
            /// @return Returns a pointer to the @ref SoundProxy at the specified index.
            virtual SoundProxy* GetSoundProxy(const UInt32 Index) const = 0;
            /// @brief Gets the number of @ref SoundProxy instances in this manager.
            /// @return Returns a UInt32 representing the number of @ref SoundProxy instances contained in this manager.
            virtual UInt32 GetNumSoundProxies() const = 0;
            /// @brief Deletes a @ref SoundProxy.
            /// @param ToBeDestroyed A pointer to the @ref SoundProxy you want deleted.
            virtual void DestroySoundProxy(SoundProxy* ToBeDestroyed) = 0;
            /// @brief Deletes all stored @ref SoundProxy instances.
            virtual void DestroyAllSoundProxies() = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Convenience Creation Methods

            /// @brief Creates a blank Ambient @ref SoundProxy without a stream attached to it.
            /// @return Returns a pointer to the @ref SoundProxy instance that was created.
            virtual SoundProxy* CreateAmbientSoundProxy();
            /// @brief Creates a blank Dialog @ref SoundProxy without a stream attached to it.
            /// @return Returns a pointer to the @ref SoundProxy instance that was created.
            virtual SoundProxy* CreateDialogSoundProxy();
            /// @brief Creates a blank Effect @ref SoundProxy without a stream attached to it.
            /// @return Returns a pointer to the @ref SoundProxy instance that was created.
            virtual SoundProxy* CreateEffectSoundProxy();
            /// @brief Creates a blank Music @ref SoundProxy without a stream attached to it.
            /// @return Returns a pointer to the @ref SoundProxy instance that was created.
            virtual SoundProxy* CreateMusicSoundProxy();

            /// @brief Creates a new Ambient @ref SoundProxy from a custom stream that is encoded.
            /// @param Stream A Datastream containing the data to be streamed for this @ref SoundProxy.
            /// @param Encode The encoding to be expected when decoding audio for this @ref SoundProxy.
            /// @return Returns a pointer to the @ref SoundProxy instance that was created.
            virtual SoundProxy* CreateAmbientSoundProxy(Resource::DataStreamPtr Stream, const Audio::Encoding Encode);
            /// @brief Creates a new Dialog @ref SoundProxy from a custom stream that is encoded.
            /// @param Stream A Datastream containing the data to be streamed for this @ref SoundProxy.
            /// @param Encode The encoding to be expected when decoding audio for this @ref SoundProxy.
            /// @return Returns a pointer to the @ref SoundProxy instance that was created.
            virtual SoundProxy* CreateDialogSoundProxy(Resource::DataStreamPtr Stream, const Audio::Encoding Encode);
            /// @brief Creates a new Effect @ref SoundProxy from a custom stream that is encoded.
            /// @param Stream A Datastream containing the data to be streamed for this @ref SoundProxy.
            /// @param Encode The encoding to be expected when decoding audio for this @ref SoundProxy.
            /// @return Returns a pointer to the @ref SoundProxy instance that was created.
            virtual SoundProxy* CreateEffectSoundProxy(Resource::DataStreamPtr Stream, const Audio::Encoding Encode);
            /// @brief Creates a new Music @ref SoundProxy from a custom stream that is encoded.
            /// @param Stream A Datastream containing the data to be streamed for this @ref SoundProxy.
            /// @param Encode The encoding to be expected when decoding audio for this @ref SoundProxy.
            /// @return Returns a pointer to the @ref SoundProxy instance that was created.
            virtual SoundProxy* CreateMusicSoundProxy(Resource::DataStreamPtr Stream, const Audio::Encoding Encode);

            /// @brief Creates a new Ambient @ref SoundProxy from a custom stream that is unencoded.
            /// @param Stream A Datastream containing the data to be streamed for this @ref SoundProxy.
            /// @param Frequency The frequency (or sample rate) of the audio data.
            /// @param Config The bit configuration of the audio data.  Helps to determine sample size.
            /// @return Returns a pointer to the @ref SoundProxy instance that was created.
            virtual SoundProxy* CreateAmbientSoundProxy(Resource::DataStreamPtr Stream, const UInt32 Frequency, const Audio::BitConfig Config);
            /// @brief Creates a new Dialog @ref SoundProxy from a custom stream that is unencoded.
            /// @param Stream A Datastream containing the data to be streamed for this @ref SoundProxy.
            /// @param Frequency The frequency (or sample rate) of the audio data.
            /// @param Config The bit configuration of the audio data.  Helps to determine sample size.
            /// @return Returns a pointer to the @ref SoundProxy instance that was created.
            virtual SoundProxy* CreateDialogSoundProxy(Resource::DataStreamPtr Stream, const UInt32 Frequency, const Audio::BitConfig Config);
            /// @brief Creates a new Effect @ref SoundProxy from a custom stream that is unencoded.
            /// @param Stream A Datastream containing the data to be streamed for this @ref SoundProxy.
            /// @param Frequency The frequency (or sample rate) of the audio data.
            /// @param Config The bit configuration of the audio data.  Helps to determine sample size.
            /// @return Returns a pointer to the @ref SoundProxy instance that was created.
            virtual SoundProxy* CreateEffectSoundProxy(Resource::DataStreamPtr Stream, const UInt32 Frequency, const Audio::BitConfig Config);
            /// @brief Creates a new Music @ref SoundProxy from a custom stream that is unencoded.
            /// @param Stream A Datastream containing the data to be streamed for this @ref SoundProxy.
            /// @param Frequency The frequency (or sample rate) of the audio data.
            /// @param Config The bit configuration of the audio data.  Helps to determine sample size.
            /// @return Returns a pointer to the @ref SoundProxy instance that was created.
            virtual SoundProxy* CreateMusicSoundProxy(Resource::DataStreamPtr Stream, const UInt32 Frequency, const Audio::BitConfig Config);

            /// @brief Creates a new Ambient @ref SoundProxy from a file.
            /// @param FileName The name of the file to read audio data from.
            /// @param Group The resource group in which the file resides.
            /// @return Returns a pointer to the @ref SoundProxy instance that was created.
            virtual SoundProxy* CreateAmbientSoundProxy(const String& FileName, const String& Group);
            /// @brief Creates a new Dialog @ref SoundProxy from a file.
            /// @param FileName The name of the file to read audio data from.
            /// @param Group The resource group in which the file resides.
            /// @return Returns a pointer to the @ref SoundProxy instance that was created.
            virtual SoundProxy* CreateDialogSoundProxy(const String& FileName, const String& Group);
            /// @brief Creates a new Effect @ref SoundProxy from a file.
            /// @param FileName The name of the file to read audio data from.
            /// @param Group The resource group in which the file resides.
            /// @return Returns a pointer to the @ref SoundProxy instance that was created.
            virtual SoundProxy* CreateEffectSoundProxy(const String& FileName, const String& Group);
            /// @brief Creates a new Music @ref SoundProxy from a file.
            /// @param FileName The name of the file to read audio data from.
            /// @param Group The resource group in which the file resides.
            /// @return Returns a pointer to the @ref SoundProxy instance that was created.
            virtual SoundProxy* CreateMusicSoundProxy(const String& FileName, const String& Group);

            /// @brief Creates a new Ambient @ref SoundProxy from a memory buffer that is encoded.
            /// @note The buffer is copied and put into a memory stream. Thus safe for disposal afterward.
            /// @param StreamName The name to be given to the stream being created.
            /// @param Buffer A pointer to the buffer where the audio data is located.
            /// @param Length The length of the buffer provided to this method(in bytes).
            /// @param Encode The encoding to be expected when decoding audio for this @ref SoundProxy.
            /// @return Returns a pointer to the @ref SoundProxy instance that was created.
            virtual SoundProxy* CreateAmbientSoundProxy(const String& StreamName, Char8* Buffer, const UInt32 Length, const Audio::Encoding Encode);
            /// @brief Creates a new Dialog @ref SoundProxy from a memory buffer that is encoded.
            /// @note The buffer is copied and put into a memory stream. Thus safe for disposal afterward.
            /// @param SoundName The name of the @ref SoundProxy instance.
            /// @param Buffer A pointer to the buffer where the audio data is located.
            /// @param Length The length of the buffer provided to this method(in bytes).
            /// @param Encode The encoding to be expected when decoding audio for this @ref SoundProxy.
            /// @return Returns a pointer to the @ref SoundProxy instance that was created.
            virtual SoundProxy* CreateDialogSoundProxy(const String& StreamName, Char8* Buffer, const UInt32 Length, const Audio::Encoding Encode);
            /// @brief Creates a new Effect @ref SoundProxy from a memory buffer that is encoded.
            /// @note The buffer is copied and put into a memory stream. Thus safe for disposal afterward.
            /// @param StreamName The name to be given to the stream being created.
            /// @param Buffer A pointer to the buffer where the audio data is located.
            /// @param Length The length of the buffer provided to this method(in bytes).
            /// @param Encode The encoding to be expected when decoding audio for this @ref SoundProxy.
            /// @return Returns a pointer to the @ref SoundProxy instance that was created.
            virtual SoundProxy* CreateEffectSoundProxy(const String& StreamName, Char8* Buffer, const UInt32 Length, const Audio::Encoding Encode);
            /// @brief Creates a new Music @ref SoundProxy from a memory buffer that is encoded.
            /// @note The buffer is copied and put into a memory stream. Thus safe for disposal afterward.
            /// @param StreamName The name to be given to the stream being created.
            /// @param Buffer A pointer to the buffer where the audio data is located.
            /// @param Length The length of the buffer provided to this method(in bytes).
            /// @param Encode The encoding to be expected when decoding audio for this @ref SoundProxy.
            /// @return Returns a pointer to the @ref SoundProxy instance that was created.
            virtual SoundProxy* CreateMusicSoundProxy(const String& StreamName, Char8* Buffer, const UInt32 Length, const Audio::Encoding Encode);

            /// @brief Creates a new Ambient @ref SoundProxy from a memory buffer that is unencoded.
            /// @note The buffer is copied and put into a memory stream. Thus safe for disposal afterward.
            /// @param StreamName The name to be given to the stream being created.
            /// @param Buffer A pointer to the buffer where the audio data is located.
            /// @param Length The length of the buffer provided to this method(in bytes).
            /// @param Frequency The frequency (or sample rate) of the audio data.
            /// @param Config The bit configuration of the audio data.  Helps to determine sample size.
            /// @return Returns a pointer to the @ref SoundProxy instance that was created.
            virtual SoundProxy* CreateAmbientSoundProxy(const String& StreamName, Char8* Buffer, const UInt32 Length, const UInt32 Frequency, const Audio::BitConfig Config);
            /// @brief Creates a new Dialog @ref SoundProxy from a memory buffer that is unencoded.
            /// @note The buffer is copied and put into a memory stream. Thus safe for disposal afterward.
            /// @param StreamName The name to be given to the stream being created.
            /// @param Buffer A pointer to the buffer where the audio data is located.
            /// @param Length The length of the buffer provided to this method(in bytes).
            /// @param Frequency The frequency (or sample rate) of the audio data.
            /// @param Config The bit configuration of the audio data.  Helps to determine sample size.
            /// @return Returns a pointer to the @ref SoundProxy instance that was created.
            virtual SoundProxy* CreateDialogSoundProxy(const String& StreamName, Char8* Buffer, const UInt32 Length, const UInt32 Frequency, const Audio::BitConfig Config);
            /// @brief Creates a new Effect @ref SoundProxy from a memory buffer that is unencoded.
            /// @note The buffer is copied and put into a memory stream. Thus safe for disposal afterward.
            /// @param StreamName The name to be given to the stream being created.
            /// @param Buffer A pointer to the buffer where the audio data is located.
            /// @param Length The length of the buffer provided to this method(in bytes).
            /// @param Frequency The frequency (or sample rate) of the audio data.
            /// @param Config The bit configuration of the audio data.  Helps to determine sample size.
            /// @return Returns a pointer to the @ref SoundProxy instance that was created.
            virtual SoundProxy* CreateEffectSoundProxy(const String& StreamName, Char8* Buffer, const UInt32 Length, const UInt32 Frequency, const Audio::BitConfig Config);
            /// @brief Creates a new Music @ref SoundProxy from a memory buffer that is unencoded.
            /// @note The buffer is copied and put into a memory stream. Thus safe for disposal afterward.
            /// @param StreamName The name to be given to the stream being created.
            /// @param Buffer A pointer to the buffer where the audio data is located.
            /// @param Length The length of the buffer provided to this method(in bytes).
            /// @param Frequency The frequency (or sample rate) of the audio data.
            /// @param Config The bit configuration of the audio data.  Helps to determine sample size.
            /// @return Returns a pointer to the @ref SoundProxy instance that was created.
            virtual SoundProxy* CreateMusicSoundProxy(const String& StreamName, Char8* Buffer, const UInt32 Length, const UInt32 Frequency, const Audio::BitConfig Config);

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc WorldManager::Pause(const UInt32)
            virtual void Pause(const UInt32 PL) = 0;

            /// @copydoc WorldManager::Initialize()
            virtual void Initialize() = 0;
            /// @copydoc ManagerBase::Deinitialize()
            virtual void Deinitialize() = 0;

            /// @brief Gets the work unit responsible for updating the buffers of soundproxies.
            /// @return Returns a pointer to the iBufferUpdate3DWorkUnit used by this manager.
            virtual iBufferUpdate3DWorkUnit* GetBufferUpdate3DWork() = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Type Identifier Methods

            /// @copydoc ManagerBase::GetInterfaceType()
            virtual ManagerType GetInterfaceType() const;
        };//SoundScapeManager
    }//Audio
}//Mezzanine

#endif
