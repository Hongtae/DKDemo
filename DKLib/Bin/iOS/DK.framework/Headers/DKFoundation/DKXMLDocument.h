//
//  File: DKXMLDocument.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKinclude.h"
#include "DKObject.h"
#include "DKString.h"
#include "DKData.h"
#include "DKXMLParser.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKXMLDocument
// XML 과 HTML 을 파싱할 수 있다.
// DOM 모델을 제공하며 내부적으로 DKXMLParser 를 사용한다.
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	class DKLIB_API DKXMLDocument
	{
	public:
		enum Type
		{
			TypeXML,
			TypeHTML,
		};
		struct Namespace
		{
			DKString	prefix;
			DKString	URI;
		};
		struct Attribute
		{
			DKObject<Namespace>	ns;
			DKString			name;
			DKString			value;
		};
		class Node
		{
		public:
			virtual ~Node(void) {}
			enum NodeType
			{
				NodeTypeUnknown = 0,
				NodeTypeInstruction,
				NodeTypeDocTypeDecl,
				NodeTypeElementDecl,
				NodeTypeAttributeDecl,
				NodeTypeComment,
				NodeTypeElement,
				NodeTypeCData,			// 파서를 통하지 않는 문자열들
				NodeTypePCData,		// 파서를 통하는 문자열들 (일부 기호가 치환된다)
			};
			NodeType			Type(void) const;
			virtual DKString	Export(void) const = 0;
		protected:
			Node(NodeType t) : type(t) {}
		private:
			const NodeType	type;
		};
		struct CData : public Node
		{
			CData(void) : Node(NodeTypeCData) {}
			DKStringU8 value;		// UTF-8 string
			DKString Export(void) const;
		};
		struct PCData : public Node
		{
			PCData(void) : Node(NodeTypePCData) {}
			DKString value;
			DKString Export(void) const;
		};
		struct Comment : public Node
		{
			Comment(void) : Node(NodeTypeComment) {}
			DKString value;
			DKString Export(void) const;
		};
		struct Element : public Node
		{
			Element(void) : Node(NodeTypeElement) {}
			DKObject<Namespace>				ns;
			DKString						name;
			DKArray<Attribute>				attributes;
			DKArray<Namespace> 				namespaces;
			DKArray<DKObject<Node>>		nodes;
			DKString Export(void) const;
			DKString Export(DKArray<Namespace>& writtenNS) const;
		};
		struct Instruction : public Node
		{
			Instruction(void) : Node(NodeTypeInstruction) {}
			DKString						target;
			DKString						data;
			DKString Export(void) const;
		};
		struct ElementDecl : public Node		// DTD Element
		{
			ElementDecl(void) : Node(NodeTypeElementDecl) {}
			DKXMLParser::ElementDecl		decl;
			DKXMLParser::ElementContentDecl	contents;
			DKString Export(void) const;
		};
		struct AttributeDecl : public Node	// DTD
		{
			AttributeDecl(void) : Node(NodeTypeAttributeDecl) {}
			DKXMLParser::AttributeDecl		decl;
			DKArray<DKString>				enumeratedValues;
			DKString Export(void) const;
		};
		struct DocTypeDecl : public Node
		{
			DocTypeDecl(void) : Node(NodeTypeDocTypeDecl) {}
			DKString						name;
			DKString						externalID;
			DKString						systemID;
			DKArray<DKObject<Node>>		nodes;
			DKString Export(void) const;
		};
		DKXMLDocument(void);
		DKXMLDocument(DocTypeDecl* dtd, Element* root);
		DKXMLDocument(Element* root);
		~DKXMLDocument(void);

		// url 또는 file 로 읽을때
		static DKObject<DKXMLDocument> Open(Type t, const DKString& fileOrURL, DKString* desc = NULL);
		// buffer 로 읽을때는 XML 은 인코딩이 표기되어 있지만, html 은 그렇지 못하다.
		// html 을 버퍼로 읽을때는 반드시 UTF8 이어야 한다.
		static DKObject<DKXMLDocument> Open(Type t, const DKData* buffer, DKString* desc = NULL);
		static DKObject<DKXMLDocument> Open(Type t, DKStream* stream, DKString* desc = NULL);

		DKObject<DKData> Export(DKStringEncoding e) const;
		size_t Export(DKStringEncoding e, DKStream* output) const;

		////////////////////////////////////////////////////////////////////////////////
		Element*			RootElement(void);
		const Element*		RootElement(void) const;
		void				SetRootElement(Element* e);				
		// DTD subset
		void				SetDTD(DocTypeDecl* d);				
		DocTypeDecl*		DTD(void);
		const DocTypeDecl*	DTD(void) const;

	private:
		DKArray<DKObject<Node>>		nodes;			// 모든 노드들
		class DocumentBuilder;
	};

	typedef DKXMLDocument::Namespace		DKXMLNamespace;
	typedef DKXMLDocument::Attribute		DKXMLAttribute;
	typedef DKXMLDocument::Node				DKXMLNode;
	typedef DKXMLDocument::CData			DKXMLCData;
	typedef DKXMLDocument::PCData			DKXMLPCData;
	typedef DKXMLDocument::Comment			DKXMLComment;
	typedef DKXMLDocument::Element			DKXMLElement;
	typedef DKXMLDocument::Instruction		DKXMLInstruction;
	typedef DKXMLDocument::ElementDecl		DKXMLElementDecl;
	typedef DKXMLDocument::AttributeDecl	DKXMLAttributeDecl;
	typedef DKXMLDocument::DocTypeDecl		DKXMLDocTypeDecl;
}
