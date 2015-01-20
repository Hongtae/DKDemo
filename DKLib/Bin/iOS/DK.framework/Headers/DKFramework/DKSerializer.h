//
//  File: DKSerializer.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKVariant.h"


////////////////////////////////////////////////////////////////////////////////
//
// DKSerializer
//
// 리소스를 자동으로 XML데이터 형식으로 시리얼라이즈 한다.
// DKVariant 로 저장하며, DKVariant 로 변환할수 있는 데이터는 모두 저장가능함.
//
// External (DKResource) 객체를 포함할때 Include 로 하면, Deserialize 할때 객체를 새로 생성하여 넣는다.
// Reference 로 설정하면, pool 에서 찾아서 넣는다. (기존객체가 있으면 그걸 사용함 - 공유가능한 객체)
//
// Bind 함수를 사용하여 각각의 변수나 객체, 또는 다른 DKSerializer 객체를 바인딩 할수 있다.
//
// 다른 DKSerializer 객체를 바인딩 할때를 제외한 나머지는 모두 getter, setter, 등을 설정해야 한다.
// 다른 DKSerializer 객체를 바인딩 하면 하나의 파일에 모두 포함하게 된다. 상속받는 경우 부모 클래스의 DKSerializer 를 넣어줘야 함.
//
// DKResource 객체를 바인딩 할때는 ExternalResource 상수에 따라 다르게 된다.
//   - ExternalResourceInclude : 무조건 내부에 포함함
//   - ExternalResourceReferenceIfPossible : 가능하다면 외부로 뺀다. (이경우 리소스는 ResourceName 이 존재하지 않으면 내부에 포함함)
//   - ExternalResourceForceReference : 무조건 외부로 뺀다. (ResourceName 이 없으면 오류)
//
// 모든 경우 오류 발생시 omissionHandler 가 있다면 대신 호출됨.
// 
// serialize
//  - getter == NULL 이면 serialize 할때 처리되지 않음.
//  - omission == NULL 이면 external 이 실패했을때 오류.
//  - omission != NULL 이면 external 이 실패했을때 무시됨.
// deserialize
//  - setter == NULL 이면 deserialize 할때 처리되지 않음.
//  - setter != NULL && omission == NULL 이면, 무조건 deserialize 되어야한다. (데이터가 없을경우 오류처리됨)
//  - omission != NULL 이면, 누락된 데이터를 처리할때 사용됨.
//  - omission == NULL 이일때, 데이터가 누락되면 오류. (무조건 동기화 되어야 할 항목으로 간주됨)
//  - checker == NULL 이면 유효성 검사를 하지 않음.
//  - checker != NULL 이면 true 를 리턴해야 유효한 값으로 간주함 (false 면 누락처리)
//
// Note: DKResource 타입이 아니라도 사용은 가능함.
////////////////////////////////////////////////////////////////////////////////


namespace DKFramework
{
	class DKResource;
	class DKResourceLoader;
	class DKSerializer
	{
	public:
		enum SerializeForm : int
		{
			SerializeFormXML				= '_XML',
			SerializeFormBinXML				= 'bXML',
			SerializeFormBinary				= '_BIN',
			SerializeFormCompressedBinary	= 'cBIN',
		};
		enum State
		{
			StateSerializeBegin = 0,
			StateSerializeSucceed,
			StateSerializeFailed,
			StateDeserializeBegin,
			StateDeserializeSucceed,
			StateDeserializeFailed,
		};
		enum ExternalResource
		{
			ExternalResourceInclude = 0,			// 내부에 포함함
			ExternalResourceReferenceIfPossible,	// 가능하면 외부로 뺀다
			ExternalResourceForceReference,			// 무조건 외부로 뺀다
		};

		typedef DKVariant ValueType;
		typedef DKFoundation::DKObject<DKResource> ExternalType;
		typedef DKFoundation::DKArray<ExternalType> ExternalArrayType;
		typedef DKFoundation::DKMap<DKFoundation::DKString, ExternalType> ExternalMapType;

		typedef DKFoundation::DKFunctionSignature<void (ValueType&)> ValueGetter;
		typedef DKFoundation::DKFunctionSignature<void (ValueType&)> ValueSetter;
		typedef DKFoundation::DKFunctionSignature<bool (const ValueType&)> ValueChecker;

		typedef DKFoundation::DKFunctionSignature<void (ExternalType&)> ExternalGetter;
		typedef DKFoundation::DKFunctionSignature<void (ExternalType&)> ExternalSetter;
		typedef DKFoundation::DKFunctionSignature<bool (const ExternalType&)> ExternalChecker;

		typedef DKFoundation::DKFunctionSignature<void (ExternalArrayType&)> ExternalArrayGetter;
		typedef DKFoundation::DKFunctionSignature<void (ExternalArrayType&)> ExternalArraySetter;
		typedef DKFoundation::DKFunctionSignature<bool (const ExternalArrayType&)> ExternalArrayChecker;

		typedef DKFoundation::DKFunctionSignature<void (ExternalMapType&)> ExternalMapGetter;
		typedef DKFoundation::DKFunctionSignature<void (ExternalMapType&)> ExternalMapSetter;
		typedef DKFoundation::DKFunctionSignature<bool (const ExternalMapType&)> ExternalMapChecker;

		typedef DKFoundation::DKOperation OmissionHandler;
		typedef DKFoundation::DKFunctionSignature<void (State)> Callback;

		DKSerializer(void);
		virtual ~DKSerializer(void);

		void SetCallback(Callback* c);
		void SetResourceClass(const DKFoundation::DKString& rc);
		DKFoundation::DKString ResourceClass(void) const;

		bool Bind(const DKFoundation::DKString& key, DKSerializer* s, OmissionHandler* omission);
		bool Bind(const DKFoundation::DKString& key, ValueGetter* getter, ValueSetter* setter, ValueChecker* checker, OmissionHandler* omission);
		bool Bind(const DKFoundation::DKString& key, ExternalGetter* getter, ExternalSetter* setter, ExternalChecker* checker, ExternalResource ext, OmissionHandler* omission);
		bool Bind(const DKFoundation::DKString& key, ExternalArrayGetter* getter, ExternalArraySetter* setter, ExternalArrayChecker* checker, ExternalResource ext, OmissionHandler* omission);
		bool Bind(const DKFoundation::DKString& key, ExternalMapGetter* getter, ExternalMapSetter* setter, ExternalMapChecker* checker, ExternalResource ext, OmissionHandler* omission);
		void Unbind(const DKFoundation::DKString& key);

		DKFoundation::DKObject<DKFoundation::DKData> Serialize(SerializeForm sf) const;
		size_t Serialize(SerializeForm sf, DKFoundation::DKStream* output) const;
		DKFoundation::DKObject<DKFoundation::DKXMLElement> SerializeXML(SerializeForm sf) const;
		bool Deserialize(const DKFoundation::DKXMLElement* e, DKResourceLoader* p) const;
		bool Deserialize(DKFoundation::DKStream* s, DKResourceLoader* p) const;
		bool Deserialize(const DKFoundation::DKData* d, DKResourceLoader* p) const;

		typedef DKFoundation::DKFunctionSignature<DKFoundation::DKObject<DKSerializer> (const DKFoundation::DKString&)> Selector;
		static bool RestoreObject(const DKFoundation::DKXMLElement* e, DKResourceLoader* p, Selector* sel);
		static bool RestoreObject(DKFoundation::DKStream* s, DKResourceLoader* p, Selector* sel);
		static bool RestoreObject(const DKFoundation::DKData* d, DKResourceLoader* p, Selector* sel);

	private:
		struct VariantEntity;
		struct SerializerEntity;
		struct ExternalEntity;
		struct ExternalEntityArray;
		struct ExternalEntityMap;
		struct Entity
		{
			DKFoundation::DKObject<OmissionHandler>	omission;		// 누락된 일반 데이터 핸들러 (NULL 일경우 무조건 동기화 되어야 됨)
			virtual ~Entity(void)											{}
			virtual const VariantEntity*		Variant(void) const			{return NULL;}
			virtual const SerializerEntity*		Serializer(void) const		{return NULL;}
			virtual const ExternalEntity*		External(void) const		{return NULL;}
			virtual const ExternalEntityArray*	ExternalArray(void) const	{return NULL;}
			virtual const ExternalEntityMap*	ExternalMap(void) const		{return NULL;}
		};
		struct VariantEntity : public Entity
		{
			DKFoundation::DKObject<ValueGetter>		getter;			// 일반 데이터 getter
			DKFoundation::DKObject<ValueSetter>		setter;			// 일반 데이터 setter
			DKFoundation::DKObject<ValueChecker>	checker;		// 타입체크 (deserialize 할때만 유효한지 검사함)
			const VariantEntity*	Variant(void) const				{return this;}
		};
		struct SerializerEntity : public Entity
		{
			DKFoundation::DKObject<DKSerializer> serializer;
			const SerializerEntity*		Serializer(void) const		{return this;}
		};
		struct ExternalEntity : public Entity
		{
			DKFoundation::DKObject<ExternalGetter>	getter;			// 외부 데이터 getter
			DKFoundation::DKObject<ExternalSetter>	setter;			// 외부 데이터 setter
			DKFoundation::DKObject<ExternalChecker>	checker;		// 외부 데이터 checker (유효한 형식의 데이터인지 확인할때 사용함)
			ExternalResource			external;
			const ExternalEntity*		External(void) const		{return this;}
		};
		struct ExternalEntityArray : public Entity
		{
			DKFoundation::DKObject<ExternalArrayGetter>		getter;		// 외부 데이터 getter
			DKFoundation::DKObject<ExternalArraySetter>		setter;		// 외부 데이터 setter
			DKFoundation::DKObject<ExternalArrayChecker>	checker;	// 외부 데이터 checker (유효한 형식의 데이터인지 확인할때 사용함)
			ExternalResource			external;
			const ExternalEntityArray*	ExternalArray(void) const		{return this;}
		};
		struct ExternalEntityMap : public Entity
		{
			DKFoundation::DKObject<ExternalMapGetter>	getter;			// 외부 데이터 getter
			DKFoundation::DKObject<ExternalMapSetter>	setter;			// 외부 데이터 setter
			DKFoundation::DKObject<ExternalMapChecker>	checker;		// 외부 데이터 checker (유효한 형식의 데이터인지 확인할때 사용함)
			ExternalResource			external;
			const ExternalEntityMap*	ExternalMap(void) const			{return this;}
		};

		typedef DKFoundation::DKMap<DKFoundation::DKString, Entity*> EntityMap;
		EntityMap entityMap;
		DKFoundation::DKString resourceClass;
		DKFoundation::DKObject<Callback> callback;
		DKFoundation::DKSpinLock	lock;

		struct DeserializerEntity		// 복구용 데이터
		{
			ValueType rootValue;
			DKFoundation::DKMap<DKFoundation::DKString, ExternalType> externals;				// 외부 데이터들
			DKFoundation::DKMap<DKFoundation::DKString, ExternalArrayType> externalArrays;
			DKFoundation::DKMap<DKFoundation::DKString, ExternalMapType> externalMaps;
			DKFoundation::DKArray<DKFoundation::DKObject<DKFoundation::DKOperation>> operations;
			DKFoundation::DKObject<Callback> callback;
		};

		bool DeserializeXMLOperations(const DKFoundation::DKXMLElement* e, DKFoundation::DKArray<DKFoundation::DKObject<DeserializerEntity>>& entities, DKResourceLoader* pool) const;
		bool DeserializeBinaryOperations(DKFoundation::DKStream* s, DKFoundation::DKArray<DKFoundation::DKObject<DeserializerEntity>>& entities, DKResourceLoader* pool) const;
		size_t SerializeBinary(SerializeForm sf, DKFoundation::DKStream* output) const;
		bool DeserializeBinary(DKFoundation::DKStream* s, DKResourceLoader* p) const;
		static bool DeserializeBinary(DKFoundation::DKStream* s, DKResourceLoader* p, Selector* sel);
		
		// 복사 막음
		DKSerializer(const DKSerializer&);
		DKSerializer& operator = (const DKSerializer&);

		class EntityRestore;
	};
}
