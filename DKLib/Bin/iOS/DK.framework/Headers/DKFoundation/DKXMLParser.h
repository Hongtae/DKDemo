//
//  File: DKXMLParser.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKinclude.h"
#include "DKString.h"
#include "DKBuffer.h"
#include "DKStream.h"
#include "DKArray.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKXMLParser
//
// DKXMLDocument 에서 xml 을 읽어서 DOM 을 생성할 때 사용된다.
// SAX 를 사용하기 위해서는 이 객체를 상속받아서 핸들링 해주어야 한다!
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	class DKLIB_API DKXMLParser
	{
	public:
		DKXMLParser(void);
		virtual ~DKXMLParser(void);

		bool BeginHTML(const DKString& url);
		bool BeginHTML(const DKData* buffer);
		bool BeginHTML(DKStream* stream);
		bool BeginXML(const DKString& url);
		bool BeginXML(const DKData* buffer);
		bool BeginXML(DKStream* stream);

		struct Namespace
		{
			DKString	prefix;
			DKString	URI;
		};
		struct Attribute
		{
			DKString	name;
			DKString	prefix;
			DKString	URI;
			DKString	value;
		};
		struct Element
		{
			DKString	name;
			DKString	prefix;
			DKString	URI;
		};
		struct ElementDecl
		{
			enum Type
			{
				TypeUndefined,
				TypeEmpty,
				TypeAny,
				TypeMixed,				// #PCDATA
				TypeElement,
			};
			DKString	name;
			Type		type;
		};
		struct ElementContentDecl
		{
			enum Type
			{
				TypeUndefined,
				TypePCData,
				TypeElement,
				TypeSequence,			// (A,B,..)
				TypeAlternative,		// (A|B|..)
			};
			enum Occurrence
			{
				OccurrenceNoneOrMore,		// *
				OccurrenceOnceOrMore,		// +
				OccurrenceOnceOrNone,		// ?
				OccurrenceOnce,				//
			};
			Type			type;
			DKString		name;
			DKString		prefix;		// namespace
			Occurrence		occurrence;
			DKArray<ElementContentDecl>	contents;
		};
		struct AttributeDecl
		{
			enum Type
			{
				TypeCData,
				TypeID,
				TypeIDRef,
				TypeIDRefs,
				TypeEntity,
				TypeEntities,
				TypeNMToken,
				TypeNMTokens,
				TypeEnumeration,
				TypeNotation,
			};
			enum ValueType
			{
				ValueTypeNone,
				ValueTypeRequired,
				ValueTypeImplied,
				ValueTypeFixed,
			};
			DKString	element;
			DKString	name;
			Type		type;
			DKString	defaultValue;
			ValueType	defaultValueType;
		};
	public:
		static DKString FormatElementContent(const DKXMLParser::ElementContentDecl& content);
		// SAX 로 사용시 override 할것들
		virtual void OnProcessingInstruction(const DKString& target, const DKString& data);
		virtual void OnInternalSubsetDeclaration(const DKString& name, const DKString& externalID, const DKString& systemID);
		virtual void OnExternalSubsetDeclaration(const DKString& name, const DKString& externalID, const DKString& systemID);
		virtual void OnEntityDeclaration(const DKString& name, int type, const DKString& publicID, const DKString& systemID, const DKString& content);
		virtual void OnAttributeDeclaration(const AttributeDecl& attr, const DKArray<DKString>& enumeratedValues);
		virtual void OnElementDeclaration(const ElementDecl& elem, const ElementContentDecl& content);
		virtual void OnStartDocument(void);
		virtual void OnEndDocument(void);
		virtual void OnStartElement(const Element& element, const DKArray<Namespace>& namespaces, const DKArray<Attribute>& attributes);
		virtual void OnEndElement(const Element& element);
		virtual void OnComment(const DKString& comment);
		virtual void OnCharacters(const char* ch, size_t len);				// UTF8 캐릭터 스트링
		virtual void OnCharacterDataBlock(const char* ch, size_t len);		// UTF8 캐릭터 스트링
		virtual void OnWarning(const DKString& mesg);
		virtual void OnError(const DKString& mesg);
		virtual void OnFatalError(const DKString& mesg);

		DKArray<void*> customEntityStorage;
	private:
		void ClearCustomEntityStorage(void);
	};
}

