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
#ifndef _objectsettings_h
#define _objectsettings_h

#include "datatypes.h"
#include "vector2.h"
#include "vector3.h"
#include "quaternion.h"
#include "colourvalue.h"
#include "smartptr.h"

#ifdef MEZZXML
#include "xml.h"
#endif

namespace Mezzanine
{
    class ObjectSettingSet;
    class ObjectSettingFile;
    ///////////////////////////////////////////////////////////////////////////////
    /// @class ObjectSettingSetContainer
    /// @headerfile objectsettings.h
    /// @brief A base class for other classes that store ObjectSettingSet's.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB ObjectSettingSetContainer
    {
        public:
            typedef std::vector<ObjectSettingSet*> SubSetVector;
            typedef SubSetVector::iterator SubSetIterator;
            typedef SubSetVector::const_iterator ConstSubSetIterator;
        protected:
            String ContainerName;
            SubSetVector SubSets;
        public:
            /// @brief Class constructor.
            /// @param Name The name of this container.
            ObjectSettingSetContainer(const String& Name);
            /// @brief Class destructor.
            virtual ~ObjectSettingSetContainer();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility
            /// @brief Sets the name of this container.
            /// @param Name The name to be given to this container.
            void SetName(const String& Name);
            /// @brief Gets the name of this container.
            /// @return Returns a const reference to the name of this container.
            ConstString& GetName() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Sub-Set Methods
            /// @brief Creates a new SettingSet as a child of this container.
            /// @param Name The name of the new set to be created.
            /// @return Returns a pointer to the created SettingSet.
            virtual ObjectSettingSet* CreateChildObjectSettingSet(const String& Name);
            /// @brief Adds a SettingSet as a child of this container.
            /// @param ToBeAdded Pointer to the SettingSet to be added.
            virtual void AddChildObjectSettingSet(ObjectSettingSet* ToBeAdded);
            /// @brief Gets a Sub-Set of this container by name.
            /// @param Name The name of the desired Sub-Set.
            /// @return Returns a pointer to the named Sub-Set, or NULL if it doesn't exist.
            ObjectSettingSet* GetChildObjectSettingSet(const String& Name);
            /// @brief Gets a Sub-Set of this container by index.
            /// @param Index The index of the Sub-Set to get.
            /// @return Returns a pointer to the Sub-Set at the specified index.
            ObjectSettingSet* GetChildObjectSettingSet(const Whole& Index);
            /// @brief Destroys a Sub-Set of this container.
            /// @param Name The name of the Sub-Set to be destroyed.
            void DestroyChildObjectSettingSet(const String& Name);
            /// @brief Destroys all child Sub-Sets of this container.
            void DestroyAllObjectSettingSets();

            ///////////////////////////////////////////////////////////////////////////////
            // Iterator Access
            /// @brief Gets an iterator to the first subset of settings in this set.
            SubSetIterator SubSetBegin();
            /// @brief Gets a const iterator to the first subset of settings in this set.
            ConstSubSetIterator SubSetBegin() const;
            /// @brief Gets an iterator to one passed the last subset of settings in this set.
            SubSetIterator SubSetEnd();
            /// @brief Gets a const iterator to one passed the last subset of settings in this set.
            ConstSubSetIterator SubSetEnd() const;
    };//ObjectSettingSetContainer

    ///////////////////////////////////////////////////////////////////////////////
    /// @class ObjectSettingSet
    /// @headerfile objectsettings.h
    /// @brief A class that store's a named set of settings for an object.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB ObjectSettingSet : public ObjectSettingSetContainer
    {
        public:
            typedef std::map<String,String> SettingsMap;
            typedef SettingsMap::iterator SettingsIterator;
            typedef SettingsMap::const_iterator ConstSettingsIterator;
        protected:
            SettingsMap Settings;
            ObjectSettingSet* ParentSet;
        public:
            /// @brief Class constructor.
            /// @param Name The name of this set.
            ObjectSettingSet(const String& Name);
            /// @brief Class destructor.
            virtual ~ObjectSettingSet();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility
            /// @brief Gets the Parent SettingsSet to this set.
            /// @return Returns a pointer to this object's parent, or NULL if it doesn't have one.
            ObjectSettingSet* GetParentSet() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Sub-Set Methods
            /// @copydoc ObjectSettingSetContainer::CreateChildObjectSettingSet(String&)
            ObjectSettingSet* CreateChildObjectSettingSet(const String& Name);
            /// @copydoc ObjectSettingSetContainer::AddChildObjectSettingSet(ObjectSettingSet*)
            void AddChildObjectSettingSet(ObjectSettingSet* ToBeAdded);

            ///////////////////////////////////////////////////////////////////////////////
            // Setting Methods
            /// @brief Sets a setting in this set...of settings.
            /// @param SettingName The name of the new setting.
            /// @param StringValue A String containing the value to be given to this setting.
            void SetSettingValue(const String& SettingName, const String& StringValue);
            /// @brief Gets a String representing the Value currently assigned to the existing setting.
            /// @param SettingName The name of the setting desired.
            /// @return Returns an String containing the requested setting.  If the setting does not exist then the String will be empty.
            String GetSettingValue(const String& SettingName) const;

            ///////////////////////////////////////////////////////////////////////////////
            // Iterator Access
            /// @brief Gets an iterator to the first setting in this set.
            SettingsIterator SettingsBegin();
            /// @brief Gets a const iterator to the first setting in this set.
            ConstSettingsIterator SettingsBegin() const;
            /// @brief Gets an iterator to one passed the last setting in this set.
            SettingsIterator SettingsEnd();
            /// @brief Gets a const iterator to one passed the last setting in this set.
            ConstSettingsIterator SettingsEnd() const;
    };//ObjectSettingSet

    ///////////////////////////////////////////////////////////////////////////////
    /// @class ObjectSettingGroup
    /// @headerfile objectsettings.h
    /// @brief A class that store's a collection of SettingSets that can be applied together.
    /// @details
    ///////////////////////////////////////
    class MEZZ_LIB ObjectSettingGroup : public ObjectSettingSetContainer
    {
        protected:
            friend class ObjectSettingFile;
            String OptionalFileName;
            /// @brief Sets the name of the file this group will be saved to.
            /// @param FileName The name of the file this group should be saved to when saving the configuration.
            void SetOptionalFileName(const String& FileName);
        public:
            /// @brief Class constructor.
            /// @param Name The name to be given to this group.
            ObjectSettingGroup(const String& Name);
            /// @brief Class destructor.
            virtual ~ObjectSettingGroup();

            ///////////////////////////////////////////////////////////////////////////////
            // Optional File config
            /// @brief Gets the file name this group will be saved to.
            /// @return Returns a const reference to the currently set file name.  String will be empty if this is not set.
            const String& GetOptionalFileName() const;
    };//ObjectSettingGroup

    ///////////////////////////////////////////////////////////////////////////////
    /// @class ObjectSettingFile
    /// @headerfile objectsettings.h
    /// @brief This class represents a file to be saved containing one or more OhjectSettingGroup's.
    /// @details
    ///////////////////////////////////////
    class ObjectSettingFile
    {
        public:
            typedef std::vector< ObjectSettingGroup* > SaveGroupsContainer;
            typedef SaveGroupsContainer::iterator SaveGroupsIterator;
            typedef SaveGroupsContainer::const_iterator ConstSaveGroupsIterator;
        protected:
            SaveGroupsContainer SaveGroups;
            String File;
        public:
            /// @brief Class constructor.
            /// @param FileName The name of the file the groups in this class will be saved to.
            ObjectSettingFile(const String& FileName);
            /// @brief Class destructor.
            ~ObjectSettingFile();

            ///////////////////////////////////////////////////////////////////////////////
            // File and Group Management
            /// @brief Gets the name of the file.
            /// @return Returns a const reference to the name of this file.
            const String& GetFileName() const;
            /// @brief Adds a group to be saved to this file.
            /// @param Group The group to be added to this file.
            void AddGroup(ObjectSettingGroup* Group);
            /// @brief Gets a group being saved to this file.
            /// @param Name The name of the group to get.
            /// @return Returns a pointer to the named group in this file, or NULL if it does not exist in this file.
            ObjectSettingGroup* GetGroup(const String& Name) const;
            /// @brief Removes a group from this file.
            /// @param Group The group to be removed.
            void RemoveGroup(ObjectSettingGroup* Group);
            /// @brief Gets the container with the groups to be saved to this file.
            /// @return Returns a reference to the container containing all the groups that are to be saved to this file.
            SaveGroupsContainer& GetGroups();

            /// @brief Gets an iterator to the first group in this file.
            SaveGroupsIterator SaveGroupsBegin();
            /// @brief Gets a const iterator to the first group in this file.
            ConstSaveGroupsIterator SaveGroupsBegin() const;
            /// @brief Gets an iterator to one-passed the last group in this file.
            SaveGroupsIterator SaveGroupsEnd();
            /// @brief Gets a const iterator to one-passed the last group in this file.
            ConstSaveGroupsIterator SaveGroupsEnd() const;
    };//ObjectSettingsFile

    ///////////////////////////////////////////////////////////////////////////////
    /// @class ObjectSettingsHandler
    /// @headerfile objectsettings.h
    /// @brief An abstract class for other classes that manage groups of settings.
    /// @details
    ///////////////////////////////////////
    class ObjectSettingsHandler
    {
        public:
            typedef std::map<String,ObjectSettingGroup*> SettingGroupContainer;
            typedef SettingGroupContainer::iterator SettingGroupIterator;
            typedef SettingGroupContainer::const_iterator ConstSettingGroupIterator;
            typedef std::map<String,ObjectSettingFile*> SettingFilesContainer;
            typedef SettingFilesContainer::iterator SettingFilesIterator;
            typedef SettingFilesContainer::const_iterator ConstSettingFilesIterator;
            /// @typedef SettingGroupVector
            /// @brief Simple typedef for Load method returns.
            typedef std::vector<ObjectSettingGroup*> SettingGroupVector;
        protected:
            SettingGroupContainer SettingGroups;
            SettingFilesContainer SettingFiles;
            bool AutoGenPath;
            bool AutoGenFiles;
#ifdef MEZZXML
            String SettingsFilePath;
            bool SaveCurrent;
            CountedPtr<SettingGroupVector> LoadSettingsFromFile(const String& FileName, const String& Path);
            void SaveSettingsToFile(StringVector& GroupNames, const String& FileName, const String& Path);
            void LoadSettingSetFromXML(XML::Node& XMLNode, ObjectSettingSet* Set);
            void SaveSettingSetToXML(XML::Node& XMLNode, ObjectSettingSet* Set);
            virtual String GetObjectRootNodeName() const = 0;
            virtual XML::Node CreateCurrentSettings() = 0;
#endif
            virtual void ApplySettingGroupImpl(ObjectSettingGroup* Group) = 0;
        public:
            /// @brief Class constructor.
            ObjectSettingsHandler();
            /// @brief Class destructor.
            virtual ~ObjectSettingsHandler();

            ///////////////////////////////////////////////////////////////////////////////
            // Setting Group Handling
            /// @brief Applies all the settings (and their values) defined in a setting group.
            /// @param GroupName The name of the setting group to apply.
            void ApplySettingGroup(const String& GroupName);
            /// @brief Creates a new blank setting group that can have it's settings populated.
            /// @param Name The name to be given to the new settings group.
            /// @return Returns a pointer to the newly created setting group.
            ObjectSettingGroup* CreateSettingGroup(const String& Name);
            /// @brief Gets a setting group by name.
            /// @param Name The name of the setting group to get.
            /// @return Returns a pointer to the named setting group.
            ObjectSettingGroup* GetSettingGroup(const String& Name) const;
            /// @brief Destroys a setting group by name.
            /// @param Name The name of the setting group to destroy.
            void DestroySettingGroup(const String& Name);
            /// @brief Destroys a setting group.
            /// @param ToBeDestroyed A pointer to the group that will be destroyed.
            void DestroySettingGroup(ObjectSettingGroup* ToBeDestroyed);
            /// @brief Destroys all setting groups stored in this handler.
            void DestroyAllSettingGroups();
#ifdef MEZZXML
            ///////////////////////////////////////////////////////////////////////////////
            // Setting File Handling
            /// @brief Creates a new Setting file that will track which groups are a part of it.
            /// @param FileName The name of the file this instance cooresponds to.
            /// @return Returns a pointer to the created ObjectSettingFile.
            ObjectSettingFile* CreateSettingFile(const String& FileName);
            /// @brief Gets a Setting file by name.
            /// @param FileName The name of the setting file to get.
            /// @return Returns a pointer to the named setting file.
            ObjectSettingFile* GetSettingFile(const String& FileName);
            /// @brief Destroys a Setting file by name.
            /// @note This does not destroy any saved files on disk, only the helper class(es) that have been allocated in memory.
            /// @param FileName The name of the setting file to be destroyed.
            void DestroySettingFile(const String& FileName);
            /// @brief Destroys a Setting file.
            /// @note This does not destroy any saved files on disk, only the helper class(es) that have been allocated in memory.
            /// @param ToBeDestroyed The setting file to be destroyed.
            void DestroySettingFile(ObjectSettingFile* ToBeDestroyed);
            /// @brief Destroys all Setting files in this handler.
            /// @note This does not destroy any saved files on disk, only the helper class(es) that have been allocated in memory.
            void DestroyAllSettingFiles();
            /// @brief Assigns a SettingGroup to a file name that will be used when settings are saved.
            /// @param Group The setting group getting it's optional file name updated.
            /// @param FileName The name of the file the provided group will be added to.
            void AddGroupToFile(ObjectSettingGroup* Group, const String& FileName);
            /// @brief Removes a previously assigned group from a save file name.
            /// @param Group The setting group getting removed from it's current optional file name.
            /// @param FileName The name of the file the provided group will be removed from.
            void RemoveGroupFromFile(ObjectSettingGroup* Group, const String& FileName);

            ///////////////////////////////////////////////////////////////////////////////
            // Setting Saving/Path Handling
            /// @brief Sets the path to be assumed when saving/loading settings to files.
            /// @param Path The path to be assumed.
            void SetSettingsFilePath(const String& Path);
            /// @brief Gets the currently set settings file path.
            /// @return Returns a string containing the path to use when a save/load path isn't specified.
            const String& GetSettingsFilePath() const;
            /// @brief Sets whether or not to save the current config as it's own group when saving settings.
            /// @details Any group labeled "Current" (case sensative) will be used and immediately applied upon loading. @n
            /// If this is enabled and no group specifiers are used, then the Current settings will automatically be saved any time the groups are saved.
            /// When using functions with group specifiers, you can include a string "Current" to save the current settings. @n
            /// It's also important to note that the "Current" settings group, although it'll be present in saved files, is not a group traversible in code.
            /// It is solely intended to save the existing state at shutdown, and resume with that exact config.
            /// @param Save Whether or not to have the current settings when other groups are saved.
            void SetSaveCurrent(bool Save);
            /// @brief Gets if autosaving the current config is enabled.
            /// @return Returns true if the current config is auto-saved when saving happens, false otherwise.
            bool GetSaveCurrent() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Loading Utilities
            /// @brief Loads settings from a settings file.
            /// @param FileName The name of the file to load and parse.
            /// @param Group The asset group where the file can be found.
            /// @return Returns a vector containing all the setting groups that were created while loading settings.
            CountedPtr<SettingGroupVector> LoadSettingsFromGroup(const String& FileName, const String& Group);
            /// @brief Loads settings from a settings file.
            /// @param FileName The name of the file to load and parse.
            /// @param Path The filesystem path where the file can be found.
            /// @return Returns a vector containing all the setting groups that were created while loading settings.
            CountedPtr<SettingGroupVector> LoadSettings(const String& FileName, const String& Path);
            /// @brief Loads settings from a settings file using the specified settings path.
            /// @note Using this function means it will use the preset settings file path stored in this handler.
            /// The path must be set before calling this.
            /// @param FileName The name of the file to load and parse.
            /// @return Returns a vector containing all the setting groups that were created while loading settings.
            CountedPtr<SettingGroupVector> LoadSettings(const String& FileName);
            /// @brief Loads settings from an xml node.
            /// @param RootSettings The node that reprsents the root of the settings group or groups.
            /// @return Returns a vector containing all the setting groups that were created while loading settings.
            CountedPtr<SettingGroupVector> LoadSettingsFromXML(XML::Node& RootSettings);

            ///////////////////////////////////////////////////////////////////////////////
            // Saving Utilities
            /// @brief Saves all settings with their optional file names specified to their specified files.
            void SaveAllSettings();
            /// @brief Saves all settings that have their optional filename set to the specified file.
            /// @param Filename The optional filename that will be used to determine which groups will be saved, and to what file.
            /// @param Path The path to place the file being saved.
            void SaveSettingsByFile(const String& FileName, const String& Path);
            /// @brief Saves all settings that have their optional filename set to the specified file.
            /// @note Using this function means it will use the preset settings file path stored in this handler.
            /// The path must be set before calling this.
            /// @param Filename The optional filename that will be used to determine which groups will be saved, and to what file.
            void SaveSettingsByFile(const String& FileName);
            /// @brief Saves all settings to a settings file.
            /// @param GroupNames A string vector containing the names for all the settings groups to save.
            /// @param Filename The name of the file to save the current settings to.
            /// @param Path The path to place the file being saved.
            void SaveSettingGroups(StringVector& GroupNames, const String& FileName, const String& Path);
            /// @brief Saves all settings to a settings file using the specified settings path.
            /// @note Using this function means it will use the preset settings file path stored in this handler.
            /// The path must be set before calling this.
            /// @param GroupNames A string vector containing the names for all the settings groups to save.
            /// @param Filename The name of the file to save the current settings to.
            void SaveSettingGroups(StringVector& GroupNames, const String& FileName);
            /// @brief Saves all the current setting groups as children of the provided XML node.
            /// @param RootSettings The node to populate with all currently loaded settings groups.
            void SaveSettingsToXML(XML::Node& RootSettings);
            /// @brief Saves the named settings groups as children of the provided XML node.
            /// @param GroupNames A string vector containing the names for all the settings groups to save.
            /// @param RootSettings The node to populate with all currently loaded settings groups.
            void SaveSettingsToXML(StringVector& GroupNames, XML::Node& RootSettings);
#endif
    };//ObjectSettingsHandler
}//Mezzanine

#endif
