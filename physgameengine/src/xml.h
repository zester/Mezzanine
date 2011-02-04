#ifdef PHYSXML 
//© Copyright 2010 Joseph Toppi and John Blackwood 
/* This file is part of The PhysGame Engine. 
 
	The PhysGame Engine is free software: you can redistribute it and/or modify 
	it under the terms of the GNU General Public License as published by 
	the Free Software Foundation, either version 3 of the License, or 
	(at your option) any later version. 
 
	The PhysGame Engine is distributed in the hope that it will be useful, 
	but WITHOUT ANY WARRANTY; without even the implied warranty of 
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
	GNU General Public License for more details. 
 
	You should have received a copy of the GNU General Public License 
	along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.  
*/ 
/* The original authors have included a copy of the license specified above in the 
   'Docs' folder. See 'gpl.txt' 
*/ 
/* We welcome the use of The PhysGame anyone, including companies who wish to  
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
/* 
 * 
 * Software, Files, Libraries and all other items referenced in this clause refers only 
 * to the contents of this file and associated documentation. 
 * 
 * Pugixml parser - version 1.0 
 * -------------------------------------------------------- 
 * Copyright © 2006-2010, by Arseny Kapoulkine (arseny.kapoulkine@gmail.com) 
 * Report bugs and download new versions at http://pugixml.org/ 
 * 
 * This library is distributed under the MIT License. See notice at the end 
 * of this file. 
 * 
 * This work is based on the pugxml parser, which is: 
 * Copyright © 2003, by Kristen Wegner (kristen@tima.net) 
 */ 
 
#ifndef _XMLCONFIG_H 
#define _XMLCONFIG_H 
 
// Uncomment this to enable wchar_t mode 
// #define XML_WCHAR_MODE 
 
// Uncomment this to disable XPath 
// #define XML_NO_XPATH 
 
// Uncomment this to disable STL 
// Note: you can't use XPath with XML_NO_STL 
// #define XML_NO_STL 
 
// Uncomment this to disable exceptions 
// Note: you can't use XPath with XML_NO_EXCEPTIONS 
// #define XML_NO_EXCEPTIONS 
 
// Set this to control attributes for public classes/functions, i.e.: 
// #define PHYS_LIB __declspec(dllexport) // to export all public symbols from DLL 
// #define PHYS_LIB __declspec(dllimport) // to import all classes from DLL 
// #define PHYS_LIB __fastcall // to set calling conventions to all public functions to fastcall 
// In absence of PHYS_LIB/PHYS_LIB definitions PHYS_LIB is used instead 
 
#endif 
 
#ifndef _XML_H 
#define _XML_H 
 
#include "crossplatform.h" 
#include "xmldoc.h" 
 
#ifndef XML_NO_STL 
namespace std 
{ 
	struct bidirectional_iterator_tag; 
 
#ifdef __SUNPRO_CC 
	// Sun C++ compiler has a bug which forces template argument names in forward declarations to be the same as in actual definitions 
	template <class _T> class allocator; 
	template <class _charT> struct char_traits; 
	template <class _charT, class _Traits> class basic_istream; 
	template <class _charT, class _Traits> class basic_ostream; 
	template <class _charT, class _Traits, class _Allocator> class basic_string; 
#else 
	// Borland C++ compiler has a bug which forces template argument names in forward declarations to be the same as in actual definitions 
	template <class _Ty> class allocator; 
	template <class _Ty> struct char_traits; 
	template <class _Elem, class _Traits> class basic_istream; 
	template <class _Elem, class _Traits> class basic_ostream; 
	template <class _Elem, class _Traits, class _Ax> class basic_string; 
#endif 
 
	// Digital Mars compiler has a bug which requires a forward declaration for explicit instantiation (otherwise Type selection is messed up later, producing link errors) 
	// Also note that we have to declare char_traits as a class here, since it's defined that way 
#ifdef __DMC__ 
	template <> class char_traits<char>; 
#endif 
} 
#endif 
 
// Macro for deprecated features 
#ifndef XML_DEPRECATED 
#	if defined(__GNUC__) 
#		define XML_DEPRECATED __attribute__((deprecated)) 
#	elif defined(_MSC_VER) && _MSC_VER >= 1300 
#		define XML_DEPRECATED __declspec(deprecated) 
#	else 
#		define XML_DEPRECATED 
#	endif 
#endif 
 
// Include exception header for XPath 
#if !defined(XML_NO_XPATH) && !defined(XML_NO_EXCEPTIONS) 
#	include <exception> 
#endif 
 
// If no API is defined, assume default 
#ifndef PHYS_LIB 
#   define PHYS_LIB 
#endif 
 
// If no API for classes is defined, assume default 
#ifndef PHYS_LIB 
#   define PHYS_LIB PHYS_LIB 
#endif 
 
// If no API for functions is defined, assume default 
#ifndef PHYS_LIB 
#   define PHYS_LIB PHYS_LIB 
#endif 
 
#include <stddef.h> 
 
// Character interface macros 
#ifdef XML_WCHAR_MODE 
#	define XML_TEXT(t) L ## t 
#	define XML_CHAR wchar_t 
#else 
#	define XML_TEXT(t) t 
#	define XML_CHAR char 
#endif 
 

namespace phys
{ namespace xml
{ 
	// Character Type used for all internal storage and operations; depends on XML_WCHAR_MODE 
	typedef Character char_t; 
 
#ifndef XML_NO_STL 
	// String Type used for operations that work with STL string; depends on XML_WCHAR_MODE 
	typedef std::basic_string<Character, std::char_traits<Character>, std::allocator<Character> > string_t; 
#endif 
} 
} // \phys
 
// The PugiXML namespace 

namespace phys
{ namespace xml
{ 
	// Tree node Types 
	enum NodeType 
	{ 
		NodeNull,		  // Empty (null) node handle 
		NodeDocument,		// A document tree's absolute GetRoot 
		NodeElement,		// Element tag, i.e. '<node/>' 
		NodePcdata,		// Plain character data, i.e. 'text' 
		NodeCdata,			// Character data, i.e. '<![CDATA[text]]>' 
		NodeComment,		// Comment tag, i.e. '<!-- text -->' 
		NodePi,			// Processing instruction, i.e. '<?Name?>' 
		NodeDeclaration,	// Document declaration, i.e. '<?xml version="1.0"?>' 
		NodeDocType		// Document Type declaration, i.e. '<!DOCTYPE doc>' 
	}; 
 
	// Parsing options 
 
	// Minimal parsing mode (equivalent to turning all other flags off). 
	// Only elements and PCDATA sections are added to the DOM tree, no text conversions are performed. 
	const unsigned int ParseMinimal = 0x0000; 
 
	// This flag determines if processing instructions (NodePi) are added to the DOM tree. This flag is off by default. 
	const unsigned int ParsePi = 0x0001; 
 
	// This flag determines if comments (NodeComment) are added to the DOM tree. This flag is off by default. 
	const unsigned int ParseComments = 0x0002; 
 
	// This flag determines if CDATA sections (NodeCdata) are added to the DOM tree. This flag is on by default. 
	const unsigned int ParseCdata = 0x0004; 
 
	// This flag determines if plain character data (NodePcdata) that consist only of whitespace are added to the DOM tree. 
	// This flag is off by default; turning it on usually results in slower parsing and more memory consumption. 
	const unsigned int ParseWsPcdata = 0x0008; 
 
	// This flag determines if character and entity references are expanded during parsing. This flag is on by default. 
	const unsigned int ParseEscapes = 0x0010; 
 
	// This flag determines if EOL characters are normalized (converted to #xA) during parsing. This flag is on by default. 
	const unsigned int ParseEol = 0x0020; 
	 
 	// This flag determines if GetAttribute Values are normalized using CDATA normalization rules during parsing. This flag is on by default. 
 	const unsigned int ParseWconvAttribute = 0x0040; 
 
 	// This flag determines if GetAttribute Values are normalized using NMTOKENS normalization rules during parsing. This flag is off by default. 
 	const unsigned int ParseWnormAttribute = 0x0080; 
	 
	// This flag determines if document declaration (NodeDeclaration) is added to the DOM tree. This flag is off by default. 
	const unsigned int ParseDeclaration = 0x0100; 
 
	// This flag determines if document Type declaration (NodeDocType) is added to the DOM tree. This flag is off by default. 
	const unsigned int ParseDocType = 0x0200; 
 
	// The default parsing mode. 
	// Elements, PCDATA and CDATA sections are added to the DOM tree, character/reference entities are expanded, 
	// End-of-Line characters are normalized, GetAttribute Values are normalized using CDATA normalization rules. 
	const unsigned int ParseDefault = ParseCdata | ParseEscapes | ParseWconvAttribute | ParseEol; 
 
	// The full parsing mode. 
	// Nodes of all Types are added to the DOM tree, character/reference entities are expanded, 
	// End-of-Line characters are normalized, GetAttribute Values are normalized using CDATA normalization rules. 
	const unsigned int ParseFull = ParseDefault | ParsePi | ParseComments | ParseDeclaration | ParseDocType; 
 
	// These flags determine the DocumentEncoding of input data for XML document 
	enum Encoding 
	{ 
		EncodingAuto,	  // Auto-detect input DocumentEncoding using BOM or < / <? detection; use UTF8 if BOM is not found 
		EncodingUTF8,	  // UTF8 DocumentEncoding 
		EncodingUTF16LE,  // Little-endian UTF16 
		EncodingUTF16BE,  // Big-endian UTF16 
		EncodingUTF16,	 // UTF16 with native endianness 
		EncodingUTF32LE,  // Little-endian UTF32 
		EncodingUTF32BE,  // Big-endian UTF32 
		EncodingUTF32,	 // UTF32 with native endianness 
		Encodingwchar_t	  // The same DocumentEncoding wchar_t has (either UTF16 or UTF32) 
	}; 
 
	// Formatting flags 
	 
	// Indent the nodes that are written to output stream with as many indentation strings as deep the node is in DOM tree. This flag is on by default. 
	const unsigned int FormatIndent = 0x01; 
	 
	// Write DocumentEncoding-specific BOM to the output stream. This flag is off by default. 
	const unsigned int FormatWriteBom = 0x02; 
 
	// Use raw output mode (no indentation and no line breaks are written). This flag is off by default. 
	const unsigned int FormatRaw = 0x04; 
	 
	// Omit default XML declaration even if there is no declaration in the document. This flag is off by default. 
	const unsigned int FormatNoDeclaration = 0x08; 
 
	// The default set of formatting flags. 
	// Nodes are indented depending on their depth in DOM tree, a default declaration is output if document has none. 
	const unsigned int FormatDefault = FormatIndent; 
		 
	// Forward declarations 
	struct AttributeStruct; 
	struct NodeStruct; 
 
	class NodeIterator; 
	class AttributeIterator; 
 
	class TreeWalker; 
	 
	class Node; 
 
	#ifndef XML_NO_XPATH 
	class XPathNode; 
	class XPathNodeSet; 
	class XPathQuery; 
	class XPathVariableSet; 
	#endif 
 
	// Writer interface for node printing (see Node::print) 
	class PHYS_LIB Writer 
	{ 
	public: 
		/// @brief Empty Virtual Destructor
		virtual ~Writer() {} 
 
		/// @brief This will be used to output xml to an external source such as a file or stream
		/// @param data This is a pointer to something that will get written to the output
		/// @param size The size in bytes of whatever was passed in.
		virtual void Write(const void* data, size_t size) = 0; 
	}; 
 
	// Writer implementation for FILE* 
	class PHYS_LIB WriterFile: public Writer 
	{ 
	public: 
		// Construct WriterInstance from a FILE* object; void* is used to avoid header dependencies on stdio 
		WriterFile(void* file); 
 
		virtual void Write(const void* data, size_t size); 
 
	private: 
		void* file; 
	}; 
 
	#ifndef XML_NO_STL 
	// Writer implementation for streams 
	class PHYS_LIB WriterStream: public Writer 
	{ 
	public: 
		/// @brief A constructor that accepts a stream of characters
		/// @param stream A stream to send stuff to.
		WriterStream(std::basic_ostream<char, std::char_traits<char> >& stream); 
		
		/// @brief A constructor that accepts a stream of wide characters
		/// @param stream A stream to send stuff to.
		WriterStream(std::basic_ostream<wchar_t, std::char_traits<wchar_t> >& stream); 
 
		virtual void Write(const void* data, size_t size); 
 
	private: 
		std::basic_ostream<char, std::char_traits<char> >* narrow_stream; 
		std::basic_ostream<wchar_t, std::char_traits<wchar_t> >* wide_stream; 
	}; 
	#endif 
 
	// A light-weight handle for manipulating attributes in DOM tree 
	class PHYS_LIB Attribute 
	{ 
		friend class AttributeIterator; 
		friend class Node; 
 
	private: 
		AttributeStruct* _attr; 
	 
		typedef AttributeStruct* Attribute::*unspecified_bool_type; 
 
	public: 
		// Default constructor. Constructs an empty GetAttribute. 
		Attribute(); 
		 
		// Constructs GetAttribute from internal pointer 
		explicit Attribute(AttributeStruct* attr); 
 
		/// @brief Used to convert this to a boolean value in a safe way
		/// @return Returns true if the internal pointer is set and false otherwise, can be used to test validity of this class. 
		operator unspecified_bool_type() const; 
 
		// Borland C++ workaround 
		bool operator!() const; 
 
		// Comparison operators (compares wrapped GetAttribute pointers) 
		bool operator==(const Attribute& r) const; 
		bool operator!=(const Attribute& r) const; 
		bool operator<(const Attribute& r) const; 
		bool operator>(const Attribute& r) const; 
		bool operator<=(const Attribute& r) const; 
		bool operator>=(const Attribute& r) const; 
 
		// Check if GetAttribute is empty 
		bool Empty() const; 
 
		// Get GetAttribute Name/Value, or "" if GetAttribute is empty 
		const char_t* Name() const; 
		const char_t* Value() const; 
 
		// Get GetAttribute Value as a number, or 0 if conversion did not succeed or GetAttribute is empty 
		int AsInt() const; 
		unsigned int AsUint() const; 
		double AsDouble() const; 
		float AsFloat() const;
		Real AsReal() const;
		Whole AsWhole() const;
		Integer AsInteger() const;
		String AsString() const; 
 
		// Get GetAttribute Value as bool (returns true if first character is in '1tTyY' set), or false if GetAttribute is empty 
		bool AsBool() const; 
 
		// Set GetAttribute Name/Value (returns false if GetAttribute is empty or there is not enough memory) 
		bool SetName(const char_t* rhs); 
		/// @brief Set the value of this.
		/// @param rhs The new Value.
		/// @return True if successful, returns false if this is empty or there is not enough memory.
		/// @todo update this to make the error return code redundant and use an exception instead.
		/// @todo Review for possiblity of buffer overflow.
		bool SetValue(const char_t* rhs); 
 
		// Set GetAttribute Value with Type conversion (numbers are converted to strings, boolean is converted to "true"/"false") 
		/// @brief Convert rhs to a character array that contains rhs, then use that as the new value.
		/// @param rhs The new value as an int.
		/// @return True if successful, returns false if Attribute is empty or there is not enough memory.
		/// @todo update this to make the error return code redundant and use an exception instead.
		/// @todo Review for possiblity of buffer overflow.
		bool SetValue(int rhs); 
		/// @brief Convert rhs to a character array that contains rhs, then use that as the new value.
		/// @param rhs The new value as an unsigned int.
		/// @return True if successful, returns false if Attribute is empty or there is not enough memory.
		/// @todo update this to make the error return code redundant and use an exception instead.
		/// @todo Review for possiblity of buffer overflow.
		bool SetValue(unsigned int rhs); 
		/// @brief Convert rhs to a character array that contains rhs, then use that as the new value.
		/// @param rhs The new value as a double.
		/// @return True if successful, returns false if Attribute is empty or there is not enough memory.
		/// @todo update this to make the error return code redundant and use an exception instead.
		/// @todo Review for possiblity of buffer overflow.
		bool SetValue(double rhs); 
		/// @brief Convert rhs to a character array that contains the meaning of rhs, then use that as the new value.
		/// @param rhs This with be interpretted, then converted to "true" or "false"  and used as the new value.
		/// @return True if successful, returns false if Attribute is empty or there is not enough memory.
		/// @todo update this to make the error return code redundant and use an exception instead.
		/// @todo Review for possiblity of buffer overflow.
		bool SetValue(bool rhs);

		/// @brief Convert rhs to a character array that contains the meaning of rhs, then use that as the new value.
		/// @param rhs This with be converted to a character array using the appropriate streaming operator <<, then used as the new value.
		/// @return True if successful, returns false if Attribute is empty or there is not enough memory.
		/// @warning You should not pass classes that stream/serialize to xml into this function, the result will be invalid XML. If you must, find a way to strip out the ">" character, then you can reinsert it later
		/// @todo Strip ">" automatically and provide a method to reconsitute it.
		template <class T> bool SetValue(T rhs)
		{
			return SetValue(ToString(rhs).c_str());
		} 
 
		// Set GetAttribute Value (equivalent to SetValue without error checking) 
		Attribute& operator=(const char_t* rhs); 
		Attribute& operator=(int rhs); 
		Attribute& operator=(unsigned int rhs); 
		Attribute& operator=(double rhs); 
		Attribute& operator=(bool rhs); 
 
		// Get next/previous GetAttribute in the GetAttribute list of the GetParent node 
		Attribute GetNextAttribute() const; 
		Attribute GetPreviousAttribute() const; 
 
		// Get hash Value (unique for handles to the same object) 
		size_t HashValue() const; 
 
		// Get internal pointer 
		AttributeStruct* InternalObject() const; 
	}; 
 
#ifdef __BORLANDC__ 
	// Borland C++ workaround 
	bool PHYS_LIB operator&&(const Attribute& lhs, bool rhs); 
	bool PHYS_LIB operator||(const Attribute& lhs, bool rhs); 
#endif 
 
	// A light-weight handle for manipulating nodes in DOM tree 
	class PHYS_LIB Node 
	{ 
		friend class AttributeIterator; 
		friend class NodeIterator; 
 
	protected: 
		NodeStruct* _GetRoot; 
 
		typedef NodeStruct* Node::*unspecified_bool_type; 
 
	public: 
		// Default constructor. Constructs an empty node. 
		Node(); 
 
		/// @brief Constructs node from internal pointer
		/// @param p An internal node pointer. 
		explicit Node(NodeStruct* p); 
 
		/// @brief Used to convert this to a boolean value in a safe way
		/// @return Returns true if the internal pointer is set and false otherwise, can be used to test validity of this class. 
		operator unspecified_bool_type() const; 
 
		// Borland C++ workaround 
		bool operator!() const; 
	 
		// Comparison operators (compares wrapped node pointers) 
		bool operator==(const Node& r) const; 
		bool operator!=(const Node& r) const; 
		bool operator<(const Node& r) const; 
		bool operator>(const Node& r) const; 
		bool operator<=(const Node& r) const; 
		bool operator>=(const Node& r) const; 
 
		// Check if node is empty. 
		bool Empty() const; 
 
		// Get node Type 
		NodeType Type() const; 
 
		// Get node Name/Value, or "" if node is empty or it has no Name/Value 
		const char_t* Name() const; 
		const char_t* Value() const; 
	 
		// Get GetAttribute list 
		Attribute GetFirstAttribute() const; 
		Attribute GetLastAttribute() const; 
 
		// Get GetChildren list 
		Node GetFirstChild() const; 
		Node GetLastChild() const; 
 
		// Get next/previous sibling in the GetChildren list of the GetParent node 
		Node GetNextSibling() const; 
		/// @brief Attempt to retrieve the previous sibling of this Node.
		/// @details A sibling of a Node is another Node that shares the same parent. If this is and the sibling nodes are valid, this retrieves that Node, otherwise this return an empty Node.
		/// @return A Node that represents a sibling, or an empty Node on failure.
		Node GetPreviousSibling() const; 
		 
		// Get GetParent node 
		Node GetParent() const; 
 
		// Get GetRoot of DOM tree this node belongs to 
		Node GetRoot() const; 
 
		// Get GetChild, GetAttribute or next/previous sibling with the specified Name 
		Node GetChild(const char_t* Name) const; 
		Attribute GetAttribute(const char_t* Name) const; 
		/// @brief Attempt to retrieve the next sibling of this Node with a matching name.
		/// @param Name A c-string that has the name of the node you to find.
		/// @details A sibling of a Node is another Node that shares the same parent. If this is and the sibling nodes are valid, this iterates through Nodes until a sibling with a Matching name is found or all siblings are checked. If a Match is found this retrieves that Node, otherwise this return an empty Node.
		/// @return A Node that represents a sibling with a matching name, or an empty Node on failure.
		Node GetNextSibling(const char_t* Name) const; 
		/// @brief Attempt to retrieve the first previous sibling of this Node with a matching name.
		/// @param Name A c-string that has the name of the node you to find.
		/// @details A sibling of a Node is another Node that shares the same parent. If this is and the sibling nodes are valid, this iterates through Nodes until a sibling with a Matching name is found or all siblings are checked. If a Match is found this retrieves that Node, otherwise this return an empty Node.
		/// @return A Node that represents a sibling with a matching name, or an empty Node on failure.
		Node GetPreviousSibling(const char_t* Name) const; 
 
		// Get GetChild Value of current node; that is, Value of the first GetChild node of Type PCDATA/CDATA 
		const char_t* ChildValue() const; 
 
		// Get GetChild Value of GetChild with specified Name. Equivalent to GetChild(Name).ChildValue(). 
		const char_t* ChildValue(const char_t* Name) const; 
 
		// Set node Name/Value (returns false if node is empty, there is not enough memory, or node can not have Name/Value) 
		bool SetName(const char_t* rhs); 
		/// @brief Set the value of this.
		/// @param rhs The new Value.
		/// @return True if successful, returns false if this is empty or there is not enough memory.
		/// @todo update this to make the error return code redundant and use an exception instead.
		/// @todo Review for possiblity of buffer overflow.
		bool SetValue(const char_t* rhs); 
		 
		// Add GetAttribute with specified Name. Returns added GetAttribute, or empty GetAttribute on errors. 
		
		/// @brief Creates an Attribute and puts it at the end of this Nodes attributes.
		/// @param Name The name of the New attribute to be created
		/// @details This attempts to create an Attribute and stick it at the end of the list of attribute on the current
		/// Node. This will fail and return an Empty Attribute if this Node is neither an Element nor a Declaration. This will
		/// fail and return an empty attribute if this Node is empty.
		/// @return The created Attribute or an empty Attribute on Failure.
		Attribute AppendAttribute(const char_t* Name);  
		Attribute PrependAttribute(const char_t* Name); 
		Attribute InsertAttributeAfter(const char_t* Name, const Attribute& attr); 
		Attribute InsertAttributeBefore(const char_t* Name, const Attribute& attr); 
 
		// Add a copy of the specified GetAttribute. Returns added GetAttribute, or empty GetAttribute on errors. 
		
		/// @brief Copies an Attribute and puts the copy at the end of this Nodes attributes.
		/// @param proto The attribute to be copied.
		/// @details This attempts to create a copy of an attribute Attribute and stick it at the end of the list of attribute on the current
		/// Node. This will fail and return an Empty Attribute if this Node is neither an Element nor a Declaration. This will
		/// fail and return an empty attribute if this Node is empty.
		/// @return The created Attribute or an empty Attribute on Failure.
		Attribute AppendCopy(const Attribute& proto);  
		
		/// @brief Copies an Attribute and puts the copy at the beginning of this Nodes attributes.
		/// @param proto The attribute to be copied.
		/// @details This attempts to create a copy of an attribute Attribute and stick it at the beginning of the list of attribute on the current
		/// Node. This will fail and return an Empty Attribute if this Node is neither an Element nor a Declaration. This will
		/// fail and return an empty attribute if this Node is empty.
		/// @return The created Attribute or an empty Attribute on Failure.
		Attribute PrependCopy(const Attribute& proto);  
		
		/// @brief Copies an Attribute and puts the copy into the list of this Nodes attributes.
		/// @param proto The attribute to be copied.
		/// @param attr An Attribute that represents an Attribute on this Node, and is just before where you want the new copy of proto.
		/// @details This attempts to create a copy of an attribute Attribute and stick it in the middle of the list of attributes, just after a selected attribute, on the current
		/// Node. This will fail and return an Empty Attribute if this Node is neither an Element nor a Declaration. This will
		/// fail and return an empty attribute if this Node is empty.
		/// @return The created Attribute or an empty Attribute on Failure.
		Attribute InsertCopyAfter(const Attribute& proto, const Attribute& attr);  
		Attribute InsertCopyBefore(const Attribute& proto, const Attribute& attr); 
 
		// Add GetChild node with specified Type. Returns added node, or empty node on errors. 
		Node AppendChild(NodeType Type = NodeElement); 
		Node PrependChild(NodeType Type = NodeElement); 
		Node InsertChildAfter(NodeType Type, const Node& node); 
		Node InsertChildBefore(NodeType Type, const Node& node); 
 
		// Add GetChild element with specified Name. Returns added node, or empty node on errors. 
		Node AppendChild(const char_t* Name); 
		Node PrependChild(const char_t* Name); 
		
		/// @brief Creates an element Node as a child of this Node, with the given name at the middle of the children
		/// @param Name The name of the Node to be created.
		/// @param Node The node just before were the Create node is to be placed.
		/// @details Calls @ref Node::InsertChildAfter(NodeType, Node); using NodeElement as the NodeType.
		/// @return The desired Node on success, an empty Node on failure.
		Node InsertChildAfter(const char_t* Name, const Node& node); 
		Node InsertChildBefore(const char_t* Name, const Node& node); 
 
		// Add a copy of the specified node as a GetChild. Returns added node, or empty node on errors. 
		Node AppendCopy(const Node& proto); 
		
		/// @brief Copies a Node and puts the copy at the start of the list of this Nodes Childrem.
		/// @param proto The Node to be copied. If this is emptry, no work is performed.
		/// @return The copied Node on success, an empty Node on failure.
		Node PrependCopy(const Node& proto); 
		Node InsertCopyAfter(const Node& proto, const Node& node); 
		Node InsertCopyBefore(const Node& proto, const Node& node); 
 
		// Remove specified GetAttribute 
		bool RemoveAttribute(const Attribute& a); 
		bool RemoveAttribute(const char_t* Name); 
 
		// Remove specified GetChild 
		bool RemoveChild(const Node& n); 
		
		/// @brief Remove child element as specified by name.
		/// @param Name The name of the Node to remove.
		/// @return True if the removal was successful, false otherwise
		bool RemoveChild(const char_t* Name); 
 
		// Find GetAttribute using predicate. Returns first GetAttribute for which predicate returned true. 
		template <typename Predicate> Attribute FindAttribute(Predicate pred) const 
		{ 
			if (!_GetRoot) return Attribute(); 
			 
			for (Attribute attrib = GetFirstAttribute(); attrib; attrib = attrib.GetNextAttribute()) 
				if (pred(attrib)) 
					return attrib; 
		 
			return Attribute(); 
		} 
 
		// Find GetChild node using predicate. Returns first GetChild for which predicate returned true. 
		template <typename Predicate> Node FindChild(Predicate pred) const 
		{ 
			if (!_GetRoot) return Node(); 
	 
			for (Node node = GetFirstChild(); node; node = node.GetNextSibling()) 
				if (pred(node)) 
					return node; 
		 
			return Node(); 
		} 
 
		// Find node from subtree using predicate. Returns first node from subtree (depth-first), for which predicate returned true. 
		template <typename Predicate> Node FindNode(Predicate pred) const 
		{ 
			if (!_GetRoot) return Node(); 
 
			Node cur = GetFirstChild(); 
			 
			while (cur._GetRoot && cur._GetRoot != _GetRoot) 
			{ 
				if (pred(cur)) return cur; 
 
				if (cur.GetFirstChild()) cur = cur.GetFirstChild(); 
				else if (cur.GetNextSibling()) cur = cur.GetNextSibling(); 
				else 
				{ 
					while (!cur.GetNextSibling() && cur._GetRoot != _GetRoot) cur = cur.GetParent(); 
 
					if (cur._GetRoot != _GetRoot) cur = cur.GetNextSibling(); 
				} 
			} 
 
			return Node(); 
		} 
 
		// Find GetChild node by GetAttribute Name/Value 
		Node FindChildbyAttribute(const char_t* Name, const char_t* attr_Name, const char_t* attr_Value) const; 
		Node FindChildbyAttribute(const char_t* attr_Name, const char_t* attr_Value) const; 
 
	#ifndef XML_NO_STL 
		// Get the absolute node path from GetRoot as a text string. 
		string_t path(char_t delimiter = '/') const; 
	#endif 
 
		// Search for a node by path consisting of node names and . or .. elements. 
		Node FirstElementByPath(const char_t* path, char_t delimiter = '/') const; 
 
		// Recursively Traverse subtree with TreeWalker 
		bool Traverse(TreeWalker& walker); 
	 
	#ifndef XML_NO_XPATH 
		// Select single node by evaluating XPath query. Returns first node from the resulting node set. 
		XPathNode FindSingleNode(const char_t* query, XPathVariableSet* variables = 0) const; 
		XPathNode FindSingleNode(const XPathQuery& query) const; 
 
		// Select node set by evaluating XPath query 
		XPathNodeSet FindNodes(const char_t* query, XPathVariableSet* variables = 0) const; 
		XPathNodeSet FindNodes(const XPathQuery& query) const; 
	#endif 
		 
		// Print subtree using a WriterInstance object 
		void print(Writer& WriterInstance, const char_t* indent = XML_TEXT("\t"), unsigned int flags = FormatDefault, Encoding DocumentEncoding = EncodingAuto, unsigned int depth = 0) const; 
 
	#ifndef XML_NO_STL 
		// Print subtree to stream 
		void print(std::basic_ostream<char, std::char_traits<char> >& os, const char_t* indent = XML_TEXT("\t"), unsigned int flags = FormatDefault, Encoding DocumentEncoding = EncodingAuto, unsigned int depth = 0) const; 
		void print(std::basic_ostream<wchar_t, std::char_traits<wchar_t> >& os, const char_t* indent = XML_TEXT("\t"), unsigned int flags = FormatDefault, unsigned int depth = 0) const; 
	#endif 
 
		// Child nodes iterators 
		typedef NodeIterator iterator; 
 
		iterator begin() const; 
		iterator end() const; 
 
		// Attribute iterators 
		typedef AttributeIterator attribute_iterator; 
 
		attribute_iterator attributes_begin() const; 
		attribute_iterator attributes_end() const; 
 
		// Get node Offset in parsed file/string (in char_t units) for debugging purposes 
		ptrdiff_t OffSetDebug() const; 
 
		// Get hash Value (unique for handles to the same object) 
		size_t HashValue() const; 
 
		// Get internal pointer 
		NodeStruct* InternalObject() const; 
	}; 
 
#ifdef __BORLANDC__ 
	// Borland C++ workaround 
	bool PHYS_LIB operator&&(const Node& lhs, bool rhs); 
	bool PHYS_LIB operator||(const Node& lhs, bool rhs); 
#endif 
 
	// Child node iterator (a bidirectional iterator over a collection of Node) 
	class PHYS_LIB NodeIterator 
	{ 
		friend class Node; 
 
	private: 
		Node _wrap; 
		Node _GetParent; 
 
		NodeIterator(NodeStruct* ref, NodeStruct* GetParent); 
 
	public: 
		// Iterator traits 
		typedef ptrdiff_t difference_type; 
		typedef Node Value_type; 
		typedef Node* pointer; 
		typedef Node& reference; 
 
	#ifndef XML_NO_STL 
		typedef std::bidirectional_iterator_tag iterator_category; 
	#endif 
 
		// Default constructor 
		NodeIterator(); 
 
		// Construct an iterator which points to the specified node 
		NodeIterator(const Node& node); 
 
		// Iterator operators 
		bool operator==(const NodeIterator& rhs) const; 
		bool operator!=(const NodeIterator& rhs) const; 
 
		Node& operator*(); 
		Node* operator->(); 
 
		const NodeIterator& operator++(); 
		NodeIterator operator++(int); 
 
		const NodeIterator& operator--(); 
		NodeIterator operator--(int); 
	}; 
 
	// Attribute iterator (a bidirectional iterator over a collection of Attribute) 
	class PHYS_LIB AttributeIterator 
	{ 
		friend class Node; 
 
	private: 
		Attribute _wrap; 
		Node _GetParent; 
 
		AttributeIterator(AttributeStruct* ref, NodeStruct* GetParent); 
 
	public: 
		// Iterator traits 
		typedef ptrdiff_t difference_type; 
		typedef Attribute Value_type; 
		typedef Attribute* pointer; 
		typedef Attribute& reference; 
 
	#ifndef XML_NO_STL 
		typedef std::bidirectional_iterator_tag iterator_category; 
	#endif 
 
		// Default constructor 
		AttributeIterator(); 
 
		// Construct an iterator which points to the specified GetAttribute 
		AttributeIterator(const Attribute& attr, const Node& GetParent); 
 
		// Iterator operators 
		bool operator==(const AttributeIterator& rhs) const; 
		bool operator!=(const AttributeIterator& rhs) const; 
 
		Attribute& operator*(); 
		Attribute* operator->(); 
 
		const AttributeIterator& operator++(); 
		AttributeIterator operator++(int); 
 
		const AttributeIterator& operator--(); 
		AttributeIterator operator--(int); 
	}; 
 
	// Abstract tree walker class (see Node::Traverse) 
	class PHYS_LIB TreeWalker 
	{ 
		friend class Node; 
 
	private: 
		int _depth; 
	 
	protected: 
		// Get current traversal depth 
		int depth() const; 
	 
	public: 
		TreeWalker(); 
		virtual ~TreeWalker(); 
 
		// Callback that is called when traversal begins 
		virtual bool begin(Node& node); 
 
		// Callback that is called for each node Traversed 
		virtual bool for_each(Node& node) = 0; 
 
		// Callback that is called when traversal ends 
		virtual bool end(Node& node); 
	}; 
 
	// Parsing Status, returned as part of ParseResult object 
	enum ParseStatus 
	{ 
		StatusOk = 0,			  // No error 
 
		StatusFileNotFound,	  // File was not found during load_file() 
		StatusIOError,			// Error reading from file/stream 
		StatusOutOfMemory,	   // Could not allocate memory 
		StatusInternalError,	  // Internal error occurred 
 
		StatusUnrecognizedTag,	// Parser could not determine tag Type 
 
		StatusBadPi,			  // Parsing error occurred while parsing document declaration/processing instruction 
		StatusBadComment,		 // Parsing error occurred while parsing comment 
		StatusBadCdata,		   // Parsing error occurred while parsing CDATA section 
		StatusBadDocType,		 // Parsing error occurred while parsing document Type declaration 
		StatusBadPcdata,		  // Parsing error occurred while parsing PCDATA section 
		StatusBadStartElement,   // Parsing error occurred while parsing start element tag 
		StatusBadAttribute,	   // Parsing error occurred while parsing element GetAttribute 
		StatusBadEndElement,	 // Parsing error occurred while parsing end element tag 
		StatusEndElementMismatch // There was a mismatch of start-end tags (closing tag had incorrect Name, some tag was not closed or there was an excessive closing tag) 
	}; 
 
	// Parsing result 
	struct PHYS_LIB ParseResult 
	{ 
		/// @brief Parsing status ( see @ref ParseStatus ) 
		ParseStatus Status; 
 
		/// @brief Last parsed offset (in char_t units from start of input data) 
		ptrdiff_t Offset; 
 
		/// @brief Source document encoding ( see @ref Encoding ) 
		Encoding DocumentEncoding; 
 
		/// @brief Default constructor, initializes object to failed state. 
		ParseResult(); 
 
		/// @brief Cast to bool operator 
		/// @return This returns true if the ParseResult::Status member is set to ParseStatus::StatusOk, otherwise this returns false. 
		operator bool() const; 
 
		/// @brief Uses the Status member to create a text description. 
		/// @return A const char* with a brief error description based on the ParseResult::Status 
		const char* Description() const; 
	}; 
 
	// Document class (DOM tree GetRoot) 
	class PHYS_LIB Document: public Node 
	{ 
	private: 
		char_t* _buffer; 
 
		char _memory[192]; 
		 
		// Non-copyable semantics 
		Document(const Document&); 
		const Document& operator=(const Document&); 
 
		void create(); 
		void destroy(); 
 
		ParseResult load_buffer_impl(void* contents, size_t size, unsigned int options, Encoding DocumentEncoding, bool is_mutable, bool own); 
 
	public: 
		// Default constructor, makes empty document 
		Document(); 
 
		// Destructor, invalidates all node/GetAttribute handles to this document 
		~Document(); 
 
		// Removes all nodes, leaving the empty document 
		void reset(); 
 
		// Removes all nodes, then copies the entire contents of the specified document 
		void reset(const Document& proto); 
 
	#ifndef XML_NO_STL 
		// Load document from stream. 
		ParseResult load(std::basic_istream<char, std::char_traits<char> >& stream, unsigned int options = ParseDefault, Encoding DocumentEncoding = EncodingAuto); 
		ParseResult load(std::basic_istream<wchar_t, std::char_traits<wchar_t> >& stream, unsigned int options = ParseDefault); 
	#endif 
 
		// Load document from zero-terminated string. No DocumentEncoding conversions are applied. 
		ParseResult load(const char_t* contents, unsigned int options = ParseDefault); 
 
		// Load document from file 
		ParseResult load_file(const char* path, unsigned int options = ParseDefault, Encoding DocumentEncoding = EncodingAuto); 
		ParseResult load_file(const wchar_t* path, unsigned int options = ParseDefault, Encoding DocumentEncoding = EncodingAuto); 
 
		// Load document from buffer. Copies/converts the buffer, so it may be deleted or changed after the function returns. 
		ParseResult load_buffer(const void* contents, size_t size, unsigned int options = ParseDefault, Encoding DocumentEncoding = EncodingAuto); 
 
		// Load document from buffer, using the buffer for in-place parsing (the buffer is modified and used for storage of document data). 
		// You should ensure that buffer data will persist throughout the document's lifetime, and free the buffer memory manually once document is destroyed. 
		ParseResult load_buffer_inplace(void* contents, size_t size, unsigned int options = ParseDefault, Encoding DocumentEncoding = EncodingAuto); 
 
		// Load document from buffer, using the buffer for in-place parsing (the buffer is modified and used for storage of document data). 
		// You should allocate the buffer with pugixml allocation function; document will free the buffer when it is no longer needed (you can't use it anymore). 
		ParseResult load_buffer_inplace_own(void* contents, size_t size, unsigned int options = ParseDefault, Encoding DocumentEncoding = EncodingAuto); 
 
		// Save XML document to WriterInstance (semantics is slightly different from Node::print, see documentation for details). 
		void save(Writer& WriterInstance, const char_t* indent = XML_TEXT("\t"), unsigned int flags = FormatDefault, Encoding DocumentEncoding = EncodingAuto) const; 
 
	#ifndef XML_NO_STL 
		// Save XML document to stream (semantics is slightly different from Node::print, see documentation for details). 
		void save(std::basic_ostream<char, std::char_traits<char> >& stream, const char_t* indent = XML_TEXT("\t"), unsigned int flags = FormatDefault, Encoding DocumentEncoding = EncodingAuto) const; 
		void save(std::basic_ostream<wchar_t, std::char_traits<wchar_t> >& stream, const char_t* indent = XML_TEXT("\t"), unsigned int flags = FormatDefault) const; 
	#endif 
 
		// Save XML to file 
		bool save_file(const char* path, const char_t* indent = XML_TEXT("\t"), unsigned int flags = FormatDefault, Encoding DocumentEncoding = EncodingAuto) const; 
		bool save_file(const wchar_t* path, const char_t* indent = XML_TEXT("\t"), unsigned int flags = FormatDefault, Encoding DocumentEncoding = EncodingAuto) const; 
 
		// Get document element 
		Node document_element() const; 
	}; 
 
#ifndef XML_NO_XPATH 
	// XPath query return Type 
	enum XPathValueType 
	{ 
		XPathTypeNone,	  // Unknown Type (query failed to compile) 
		XPathTypeNodeSet,  // Node set (XPathNodeSet) 
		XPathTypeNumber,	// Number 
		XPathTypeString,	// String 
		XPathTypeBoolean	// Boolean 
	}; 
 
	// XPath parsing result 
	struct PHYS_LIB XPathParseResult 
	{ 
		// Error message (0 if no error) 
		const char* error; 
 
		// Last parsed Offset (in char_t units from string start) 
		ptrdiff_t Offset; 
 
		/// @brief Default constructor, initializes object to failed state. 
		XPathParseResult(); 
 
		/// @brief Cast to bool operator 
		/// @return This returns true if the ParseResult::Status member is set to ParseStatus::StatusOk, otherwise this returns false. 
		operator bool() const; 
 
		/// @brief Uses the Status member to create a text description. 
		/// @return A const char* with a brief error description based on the ParseResult::Status 
		const char* Description() const; 
	}; 
 
	// A single XPath variable 
	class PHYS_LIB XPathVariable 
	{ 
		friend class XPathVariableSet; 
 
	protected: 
		XPathValueType _type; 
		XPathVariable* _next; 
 
		XPathVariable(); 
 
		// Non-copyable semantics 
		XPathVariable(const XPathVariable&); 
		XPathVariable& operator=(const XPathVariable&); 
		 
	public: 
		// Get variable Name 
		const char_t* Name() const; 
 
		// Get variable Type 
		XPathValueType Type() const; 
 
		// Get variable Value; no Type conversion is performed, default Value (false, NaN, empty string, empty node set) is returned on Type mismatch error 
		bool get_boolean() const; 
		double get_number() const; 
		const char_t* get_string() const; 
		const XPathNodeSet& get_NodeSet() const; 
 
		// Set variable Value; no Type conversion is performed, false is returned on Type mismatch error 
		bool set(bool Value); 
		bool set(double Value); 
		bool set(const char_t* Value); 
		bool set(const XPathNodeSet& Value); 
	}; 
 
	// A set of XPath variables 
	class PHYS_LIB XPathVariableSet 
	{ 
	private: 
		XPathVariable* _data[64]; 
 
		// Non-copyable semantics 
		XPathVariableSet(const XPathVariableSet&); 
		XPathVariableSet& operator=(const XPathVariableSet&); 
 
		XPathVariable* find(const char_t* Name) const; 
 
	public: 
		// Default constructor/destructor 
		XPathVariableSet(); 
		~XPathVariableSet(); 
 
		// Add a new variable or get the existing one, if the Types match 
		XPathVariable* add(const char_t* Name, XPathValueType Type); 
 
		// Set Value of an existing variable; no Type conversion is performed, false is returned if there is no such variable or if Types mismatch 
		bool set(const char_t* Name, bool Value); 
		bool set(const char_t* Name, double Value); 
		bool set(const char_t* Name, const char_t* Value); 
		bool set(const char_t* Name, const XPathNodeSet& Value); 
 
		// Get existing variable by Name 
		XPathVariable* get(const char_t* Name); 
		const XPathVariable* get(const char_t* Name) const; 
	}; 
 
	// A compiled XPath query object 
	class PHYS_LIB XPathQuery 
	{ 
	private: 
		void* _impl; 
		XPathParseResult _result; 
 
		typedef void* XPathQuery::*unspecified_bool_type; 
 
		// Non-copyable semantics 
		XPathQuery(const XPathQuery&); 
		XPathQuery& operator=(const XPathQuery&); 
 
	public: 
		// Construct a compiled object from XPath expression. 
		// If XML_NO_EXCEPTIONS is not defined, throws XPathException on compilation errors. 
		explicit XPathQuery(const char_t* query, XPathVariableSet* variables = 0); 
 
		// Destructor 
		~XPathQuery(); 
 
		// Get query expression return Type 
		XPathValueType return_type() const; 
		 
		// Evaluate expression as boolean Value in the specified context; performs Type conversion if necessary. 
		// If XML_NO_EXCEPTIONS is not defined, throws std::bad_alloc on out of memory errors. 
		bool evaluate_boolean(const XPathNode& n) const; 
		 
		// Evaluate expression as double Value in the specified context; performs Type conversion if necessary. 
		// If XML_NO_EXCEPTIONS is not defined, throws std::bad_alloc on out of memory errors. 
		double evaluate_number(const XPathNode& n) const; 
		 
	#ifndef XML_NO_STL 
		// Evaluate expression as string Value in the specified context; performs Type conversion if necessary. 
		// If XML_NO_EXCEPTIONS is not defined, throws std::bad_alloc on out of memory errors. 
		string_t evaluate_string(const XPathNode& n) const; 
	#endif 
		 
		// Evaluate expression as string Value in the specified context; performs Type conversion if necessary. 
		// At most capacity characters are written to the destination buffer, full result size is returned (includes terminating zero). 
		// If XML_NO_EXCEPTIONS is not defined, throws std::bad_alloc on out of memory errors. 
		// If XML_NO_EXCEPTIONS is defined, returns empty  set instead. 
		size_t evaluate_string(char_t* buffer, size_t capacity, const XPathNode& n) const; 
 
		// Evaluate expression as node set in the specified context. 
		// If XML_NO_EXCEPTIONS is not defined, throws XPathException on Type mismatch and std::bad_alloc on out of memory errors. 
		// If XML_NO_EXCEPTIONS is defined, returns empty node set instead. 
		XPathNodeSet evaluate_NodeSet(const XPathNode& n) const; 
 
		// Get parsing result (used to get compilation errors in XML_NO_EXCEPTIONS mode) 
		const XPathParseResult& result() const; 
 
		/// @brief Used to convert this to a boolean value in a safe way
		/// @return Returns true if the internal pointer is set and false otherwise, can be used to test validity of this class. 
		operator unspecified_bool_type() const; 
 
		// Borland C++ workaround 
		bool operator!() const; 
	}; 
	 
	#ifndef XML_NO_EXCEPTIONS 
	// XPath exception class 
	class PHYS_LIB XPathException: public std::exception 
	{ 
	private: 
		XPathParseResult _result; 
 
	public: 
		// Construct exception from parse result 
		explicit XPathException(const XPathParseResult& result); 
 
		// Get error message 
		virtual const char* what() const throw(); 
 
		// Get parse result 
		const XPathParseResult& result() const; 
	}; 
	#endif 
	 
	// XPath node class (either Node or Attribute) 
	class PHYS_LIB XPathNode 
	{ 
	private: 
		Node _node; 
		Attribute _attribute; 
	 
		typedef Node XPathNode::*unspecified_bool_type; 
 
	public: 
		// Default constructor; constructs empty XPath node 
		XPathNode(); 
		 
		// Construct XPath node from XML node/GetAttribute 
		XPathNode(const Node& node); 
		XPathNode(const Attribute& GetAttribute, const Node& GetParent); 
 
		// Get node/GetAttribute, if any 
		Node node() const; 
		Attribute GetAttribute() const; 
		 
		// Get GetParent of contained node/GetAttribute 
		Node GetParent() const; 
 
		/// @brief Used to convert this to a boolean value in a safe way
		/// @return Returns true if the internal pointer is set and false otherwise, can be used to test validity of this class. 
		operator unspecified_bool_type() const; 
		 
		// Borland C++ workaround 
		bool operator!() const; 
 
		// Comparison operators 
		bool operator==(const XPathNode& n) const; 
		bool operator!=(const XPathNode& n) const; 
	}; 
 
#ifdef __BORLANDC__ 
	// Borland C++ workaround 
	bool PHYS_LIB operator&&(const XPathNode& lhs, bool rhs); 
	bool PHYS_LIB operator||(const XPathNode& lhs, bool rhs); 
#endif 
 
	// A fixed-size collection of XPath nodes 
	class PHYS_LIB XPathNodeSet 
	{ 
	public: 
		// Collection Type 
		enum Type_t 
		{ 
			Type_unsorted,			// Not ordered 
			Type_sorted,			// Sorted by document order (ascending) 
			Type_sorted_reverse		// Sorted by document order (descending) 
		}; 
		 
		// Constant iterator Type 
		typedef const XPathNode* const_iterator; 
	 
		// Default constructor. Constructs empty set. 
		XPathNodeSet(); 
 
		// Constructs a set from iterator range; data is not checked for duplicates and is not sorted according to provided Type, so be careful 
		XPathNodeSet(const_iterator begin, const_iterator end, Type_t Type = Type_unsorted); 
 
		// Destructor 
		~XPathNodeSet(); 
		 
		// Copy constructor/assignment operator 
		XPathNodeSet(const XPathNodeSet& ns); 
		XPathNodeSet& operator=(const XPathNodeSet& ns); 
 
		// Get collection Type 
		Type_t Type() const; 
		 
		// Get collection size 
		size_t size() const; 
 
		// Indexing operator 
		const XPathNode& operator[](size_t index) const; 
		 
		// Collection iterators 
		const_iterator begin() const; 
		const_iterator end() const; 
 
		// Sort the collection in ascending/descending order by document order 
		void sort(bool reverse = false); 
		 
		// Get first node in the collection by document order 
		XPathNode first() const; 
		 
		// Check if collection is empty 
		bool Empty() const; 
	 
	private: 
		Type_t _type; 
		 
		XPathNode _storage; 
		 
		XPathNode* _begin; 
		XPathNode* _end; 
 
		void _assign(const_iterator begin, const_iterator end); 
	}; 
#endif 
 
#ifndef XML_NO_STL 
	// Convert wide string to UTF8 
	std::basic_string<char, std::char_traits<char>, std::allocator<char> > PHYS_LIB AsUtf8(const wchar_t* str); 
	std::basic_string<char, std::char_traits<char>, std::allocator<char> > PHYS_LIB AsUtf8(const std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> >& str); 
	 
	// Convert UTF8 to wide string 
	std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > PHYS_LIB AsWide(const char* str); 
	std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > PHYS_LIB AsWide(const std::basic_string<char, std::char_traits<char>, std::allocator<char> >& str); 
#endif 
 
	// Memory allocation function interface; returns pointer to allocated memory or NULL on failure 
	typedef void* (*allocation_function)(size_t size); 
	 
	// Memory deallocation function interface 
	typedef void (*deallocation_function)(void* ptr); 
 
	// Override default memory management functions. All subsequent allocations/deallocations will be performed via supplied functions. 
	void PHYS_LIB SetMemory_management_functions(allocation_function allocate, deallocation_function deallocate); 
	 
	// Get current memory management functions 
	allocation_function PHYS_LIB get_memory_allocation_function(); 
	deallocation_function PHYS_LIB get_memory_deallocation_function(); 
} 
} // \phys
 
#if !defined(XML_NO_STL) && (defined(_MSC_VER) || defined(__ICC)) 
namespace std 
{ 
	// Workarounds for (non-standard) iterator category detection for older versions (MSVC7/IC8 and earlier) 
	std::bidirectional_iterator_tag PHYS_LIB _Iter_cat(const pugi::NodeIterator&); 
	std::bidirectional_iterator_tag PHYS_LIB _Iter_cat(const pugi::AttributeIterator&); 
} 
#endif 
 
#if !defined(XML_NO_STL) && defined(__SUNPRO_CC) 
namespace std 
{ 
	// Workarounds for (non-standard) iterator category detection 
	std::bidirectional_iterator_tag PHYS_LIB __iterator_category(const pugi::NodeIterator&); 
	std::bidirectional_iterator_tag PHYS_LIB __iterator_category(const pugi::AttributeIterator&); 
} 
#endif 
 
#endif 
 
/* 
 * 
 * Software, Files, Libraries and all other items referenced in this clause refers only 
 * to the contents of this file and associated documentation. 
 * 
 * Copyright © 2006-2010 Arseny Kapoulkine 
 * 
 * Permission is hereby granted, free of charge, to any person 
 * obtaining a copy of this software and associated documentation 
 * files (the "Software"), to deal in the Software without 
 * restriction, including without limitation the rights to use, 
 * copy, modify, merge, publish, distribute, sublicense, and/or sell 
 * copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following 
 * conditions: 
 * 
 * The above copyright notice and this permission notice shall be 
 * included in all copies or substantial portions of the Software. 
 *  
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
 * OTHER DEALINGS IN THE SOFTWARE. 
 */ 
#endif // \PHYSXML 