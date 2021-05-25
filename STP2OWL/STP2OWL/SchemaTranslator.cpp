#include "stdafx.h"
#include "SchemaTranslator.h"

#include "Ontology.h"
#include "Class.h"
#include "ObjectProperty.h"
#include "DataProperty.h"
#include "Individual.h"
#include "DataType.h"
#include "AnnotationProperty.h"


SchemaTranslator::SchemaTranslator(const Schema*& schema, Ontology* ontology)
	:m_schema(schema), 
	m_ontology(ontology),
	m_abstractClassCount(0)
{
}

SchemaTranslator::~SchemaTranslator()
{
}

void SchemaTranslator::Translate(S2O_Option& opt)
{
	AddDataTypes();
	AddAnnotationProperties();

	const EntityDescriptorList* eDesList = m_schema->Entities();
	AddClassesForEntities(eDesList);
	AddSuperClassesForEntities(eDesList);
	AddSubClassesFromSupertypeStatement(eDesList);

	const TypeDescriptorList* tDesList = m_schema->Types();
	AddClassesForBaseTypes();
	AddClassesForTypes(tDesList);

	AddDataProperties(opt.Profile());

	if (opt.IsSimpleGeomtry())
		AddDataPropertiesForXYZ();
}


void SchemaTranslator::AddClassesForEntities(const EntityDescriptorList* eDesList)
{
	EntityDescLinkNode* eDesLinkNode = (EntityDescLinkNode*)eDesList->GetHead();

	int entityCnt = eDesList->EntryCount();

	// for explicit attributes
	for (int i = 0; i < entityCnt; ++i)
	{
		try
		{
			EntityDescriptor* eDes = eDesLinkNode->EntityDesc();
			string eName = StrUtil::ToLower(eDes->Name());

			Class* eClass = nullptr;
			m_ontology->AddClass(eName, eClass);

			AddAnnotation(eClass, "ExpConstruct", "entity");

			const AttrDescriptorList* aDesList = &eDes->ExplicitAttr();

			if (!aDesList->GetHead())
			{
				eDesLinkNode = (EntityDescLinkNode*)eDesLinkNode->NextNode();
				continue;
			}

			AddObjPropertiesForExplicitAttributes(eClass, aDesList);

			eDesLinkNode = (EntityDescLinkNode*)eDesLinkNode->NextNode();
		}
		catch (S2OException & e)
		{
			cout << e.GetMessage() << endl;
		}
	}

	// re-init
	eDesLinkNode = (EntityDescLinkNode*)eDesList->GetHead();

	// for inverse attributes
	for (int i = 0; i < entityCnt; ++i)
	{
		try
		{
			EntityDescriptor* eDes = eDesLinkNode->EntityDesc();
			string eName = StrUtil::ToLower(eDes->Name());

			Class* eClass = m_ontology->GetClassByName(eName);
			S2OThrowExceptionIf<S2OException>(eClass == nullptr, "Invalid class name: " + eName);

			const Inverse_attributeList* invAttrList = &eDes->InverseAttr();

			if (!invAttrList->GetHead())
			{
				eDesLinkNode = (EntityDescLinkNode*)eDesLinkNode->NextNode();
				continue;
			}

			AddObjPropertiesForInverseAttributes(eClass, invAttrList);

			eDesLinkNode = (EntityDescLinkNode*)eDesLinkNode->NextNode();
		}
		catch (S2OException & e)
		{
			cout << e.GetMessage() << endl;
		}
	}
}

void SchemaTranslator::AddObjPropertiesForExplicitAttributes(Class* eClass, const AttrDescriptorList* aDesList)
{
	AttrDescLinkNode* aDesLinkNode = (AttrDescLinkNode*)aDesList->GetHead();
	int attrCnt = aDesList->EntryCount();

	for (int i = 0; i < attrCnt; ++i)
	{
		const AttrDescriptor* aDes = aDesLinkNode->AttrDesc();

		AddObjPropertyForAttribute(eClass, aDes);

		aDesLinkNode = (AttrDescLinkNode*)aDesLinkNode->NextNode();
	}
}

void SchemaTranslator::AddObjPropertiesForInverseAttributes(Class* eClass, const Inverse_attributeList* invAttrList)
{
	Inverse_attributeLinkNode* invAttrLinkNode = (Inverse_attributeLinkNode*)invAttrList->GetHead();
	int invAttrCnt = invAttrList->EntryCount();

	for (int i = 0; i < invAttrCnt; ++i)
	{
		Inverse_attribute* invAttr = invAttrLinkNode->Inverse_attr();
		const AttrDescriptor* aDes = invAttr;

		AddObjPropertyForAttribute(eClass, aDes);

		string objPropName = eClass->GetName() + "_has_" + aDes->Name();

		string invEntName = invAttr->inverted_entity_id_();
		string invAttrName = invAttr->inverted_attr_id_();

		Class* invCls = m_ontology->GetClassByName(invEntName);

		if (invCls)
		{
			string invObjPropName = invEntName + "_has_" + invAttrName;

			for (int j = 0; j < invCls->GetObjectPropertySize(); ++j)
			{
				// Add inverse object property. Note that redefined objProps cannot be added to be inverse.
				if (StrUtil::Equal(invObjPropName, invCls->GetObjectPropertyAt(j)->GetName()))
				{
					ObjectProperty* objProp = m_ontology->GetObjectPropertyByName(objPropName);
					ObjectProperty* invObjProp = m_ontology->GetObjectPropertyByName(invObjPropName);

					// Using set and bag causes a problem when adding inverse object properties.
					if (objProp->GetDomainAt(0) == invObjProp->GetRangeAt(0)
						&& objProp->GetRangeAt(0) == invObjProp->GetDomainAt(0))
					{
						objProp->AddInverseProperty(invObjProp);
					}
				}
			}
		}

		invAttrLinkNode = (Inverse_attributeLinkNode*)invAttrLinkNode->NextNode();
	}
}

void SchemaTranslator::AddObjPropertyForAttribute(Class* eClass, const AttrDescriptor* aDes)
{
	string aName = aDes->Name();
	string aObjPropName = eClass->GetName() + "_has_" + aName;

	AttrType_Enum attrType = aDes->AttrType();

	// type of enumeration type occurs an error. "fd_bc_dataset" in AP209 is an example.
	if (!aDes->DomainType())
	{
		if (attrType == AttrType_Explicit)
			cout << "The attribute \"" << aName << "\" of the entity \"" << eClass->GetName() << "\" is currently NOT supported." << endl;

		return;
	}

	int isAggrType = aDes->IsAggrType();

	ObjectProperty* aObjProp = nullptr;
	Class* aObjPropRangeClass = nullptr;

	if (!isAggrType)
	{
		if (attrType == AttrType_Redefining
			|| attrType == AttrType_Deriving)
			return;

		string aTypeName = StrUtil::ToLower(aDes->DomainType()->Name());
		m_ontology->AddClass(aTypeName, aObjPropRangeClass);
	}
	else
	{
		const TypeDescriptor* attrDomTypeDes = aDes->DomainType();
		PrimitiveType attrDomFundType = attrDomTypeDes->FundamentalType();

		string aggrName;

		if (attrDomFundType == SET_TYPE
			|| attrDomFundType == BAG_TYPE)
		{
			AddClassesObjPropertiesForSetBag(attrDomTypeDes, aggrName);
		}
		else if (attrDomFundType == ARRAY_TYPE
			|| attrDomFundType == LIST_TYPE)
		{
			AddClassesObjPropertiesForArrayList(attrDomTypeDes, aggrName);
		}

		if (attrType == AttrType_Redefining
			|| attrType == AttrType_Deriving)
			return;

		aObjPropRangeClass = m_ontology->GetClassByName(aggrName);

		S2OThrowExceptionIf<S2OException>(aObjPropRangeClass == nullptr, "Invalid class name: " + aggrName);
	}

	// this must be located here
	m_ontology->AddObjectProperty(aObjPropName, aObjProp);

	aObjProp->AddDomain(eClass);
	aObjProp->AddRange(aObjPropRangeClass);

	AddAnnotation(aObjProp, "OPType", aObjPropRangeClass->GetName());
	AddAnnotation(aObjProp, "ParameterIndex", to_string(eClass->GetObjectPropertySize()));

	eClass->AddObjectProperty(aObjProp);

	SDAI_LOGICAL isOptionalAttr = aDes->Optional();

	// set cardinality
	if (isAggrType)
	{
		eClass->AddObjectPropertyCardinality(aObjProp, MAXCARD, 1);
	}
	else
	{
		if ((int)isOptionalAttr == 1)
			eClass->AddObjectPropertyCardinality(aObjProp, 0, 1);
		else
			eClass->AddObjectPropertyCardinality(aObjProp, 1, 1);
	}
}


void SchemaTranslator::AddSuperClassesForEntities(const EntityDescriptorList* eDesList)
{
	EntityDescLinkNode* eDesLinkNode = (EntityDescLinkNode*)eDesList->GetHead();

	int entityCnt = eDesList->EntryCount();

	for (int i = 0; i < entityCnt; ++i)
	{
		try
		{
			EntityDescriptor* eDes = eDesLinkNode->EntityDesc();
			const EntityDescriptorList* eSupertypeDesList = &eDes->Supertypes();

			if (!eSupertypeDesList->GetHead())
			{
				eDesLinkNode = (EntityDescLinkNode*)eDesLinkNode->NextNode();
				continue;
			}

			string eName = StrUtil::ToLower(eDes->Name());
			Class* eClass = m_ontology->GetClassByName(eName);
			S2OThrowExceptionIf<S2OException>(eClass == nullptr, "Invalid class name: " + eName);

			EntityDescLinkNode* eSupertypeDesLinkNode = (EntityDescLinkNode*)eSupertypeDesList->GetHead();
			int supertypeCnt = eSupertypeDesList->EntryCount();

			for (int j = 0; j < supertypeCnt; ++j)
			{
				EntityDescriptor* eSupertypeDes = eSupertypeDesLinkNode->EntityDesc();

				string eSupertypeName = StrUtil::ToLower(eSupertypeDes->Name());
				Class* eSupertypeClass = m_ontology->GetClassByName(eSupertypeName);
				S2OThrowExceptionIf<S2OException>(eSupertypeClass == nullptr, "Invalid class name: " + eSupertypeName);

				eClass->AddSuperClass(eSupertypeClass);

				eSupertypeDesLinkNode = (EntityDescLinkNode*)eSupertypeDesLinkNode->NextNode();
			}

			eDesLinkNode = (EntityDescLinkNode*)eDesLinkNode->NextNode();
		}
		catch (S2OException & e)
		{
			cout << e.GetMessage() << endl;
		}
	}
}


void SchemaTranslator::AddSubClassesFromSupertypeStatement(const EntityDescriptorList* eDesList)
{
	EntityDescLinkNode* eDesLinkNode = (EntityDescLinkNode*)eDesList->GetHead();

	int entityCnt = eDesList->EntryCount();

	for (int i = 0; i < entityCnt; ++i)
	{
		try
		{
			EntityDescriptor* eDes = eDesLinkNode->EntityDesc();
			const EntityDescriptorList* eSubtypeDesList = &eDes->Subtypes();

			if (!eSubtypeDesList->GetHead())
			{
				eDesLinkNode = (EntityDescLinkNode*)eDesLinkNode->NextNode();
				continue;
			}

			string eName = StrUtil::ToLower(eDes->Name());
			Class* eClass = m_ontology->GetClassByName(eName);

			string superTypeStmt = eDes->Supertype_Stmt();

			if (!StrUtil::Exist(superTypeStmt, "SUPERTYPE OF"))
			{
				eDesLinkNode = (EntityDescLinkNode*)eDesLinkNode->NextNode();
				continue;
			}

			superTypeStmt = StrUtil::RemoveCharacter(superTypeStmt, "\n");
			superTypeStmt = StrUtil::GetStringBetween(superTypeStmt, "SUPERTYPE OF ( ", ")");

			if (StrUtil::Exist(superTypeStmt, "ONEOF")
				|| StrUtil::Exist(superTypeStmt, "AND ")
				|| StrUtil::Exist(superTypeStmt, "ANDOR "))
			{
				AddSubClassesFromONEOForANDorANDOR(superTypeStmt, eClass);
			}
			else
			{
				// only when one subclass exists.
				// if required
			}

			eDesLinkNode = (EntityDescLinkNode*)eDesLinkNode->NextNode();
		}
		catch (S2OException & e)
		{
			cout << e.GetMessage() << endl;
		}
	}
}

void SchemaTranslator::AddSubClassesFromONEOForANDorANDOR(string str, Class*& superclass)
{
	string tmp = str;
	string oneOf = "ONEOF";
	string leftParen = "(", rightParen = ")";
	string blank = " ";
	string comma = ",";

	while (StrUtil::Exist(tmp, oneOf))
	{
		int oneOfLoc = tmp.find(oneOf);
		int leftParenLoc = tmp.find(leftParen, oneOfLoc);

		string oneOfStmt = StrUtil::GetStringBetweenParenthesis(tmp, leftParenLoc);

		Class* absCls = nullptr;
		m_ontology->AddClass(GetAbstractClassName(), absCls);

		// recursive call
		AddSubClassesFromONEOForANDorANDOR(oneOfStmt, absCls);

		oneOfStmt = oneOf + blank + leftParen + oneOfStmt + rightParen;
		tmp = StrUtil::ReplaceCharacter(tmp, oneOfStmt, absCls->GetName());
	}

	vector<string> subClassNames;
	StrUtil::SplitStringBySeparator(tmp, comma, subClassNames);

	int subClassNameSize = (int)subClassNames.size();

	for (int i = 0; i < subClassNameSize; ++i)
	{
		string subClassName = subClassNames[i];

		if (StrUtil::Exist(subClassName, "AND ")
			|| StrUtil::Exist(subClassName, "ANDOR "))
		{
			Class* absCls = nullptr;
			m_ontology->AddClass(GetAbstractClassName(), absCls);

			AddSubClassesFromANDorANDOR(subClassName, absCls);

			if (subClassNameSize == 1)
				absCls->AddEquivalentClass(superclass);
			else
			{
				superclass->AddDisjointUnionOf(absCls);
				superclass->AddDisjointClass(absCls);

				//superclass->AddObjectUnionOf(absCls);
			}
		}
		else
		{
			subClassName = StrUtil::RemoveCharacter(subClassName, blank);

			Class* cls = m_ontology->GetClassByName(subClassName);

			S2OThrowExceptionIf<S2OException>(cls == nullptr, "Invalid class name: " + subClassName);

			if (subClassNameSize == 1)
				cls->AddEquivalentClass(superclass);
			else
			{
				superclass->AddDisjointUnionOf(cls);
				superclass->AddDisjointClass(cls);

				//superclass->AddObjectUnionOf(cls);
			}
		}
	}

	subClassNames.clear();
}

void SchemaTranslator::AddSubClassesFromANDorANDOR(string str, Class*& superclass)
{
	string tmp = str;
	string AND = "AND ", ANDOR = "ANDOR ";
	string leftParen = "(", rightParen = ")";
	string blank = " ";

	// get string between parenthesis
	tmp = StrUtil::GetStringBetween(tmp, leftParen, rightParen);

	string sep; // separator

	// no AP has a statement with both AND and ANDOR.
	if (StrUtil::Exist(str, AND))
		sep = AND;
	else if (StrUtil::Exist(str, ANDOR))
		sep = ANDOR;

	while (StrUtil::Exist(tmp, leftParen)
		&& StrUtil::Exist(tmp, rightParen))
	{
		int sepLoc = tmp.find(sep);
		int leftParenLoc = -1;

		int flag = 0;

		// get the location of the left parenthesis closest to the separator on the left side
		for (int i = sepLoc; i >= 0; --i)
		{
			string ch(1, tmp.at(i));

			if (StrUtil::Equal(ch, leftParen))
				flag++;
			else if (StrUtil::Equal(ch, rightParen))
				flag--;

			if (flag == 1)
			{
				leftParenLoc = i;
				break;
			}
		}

		string sepStmt = StrUtil::GetStringBetweenParenthesis(tmp, leftParenLoc);

		Class* absCls = nullptr;
		m_ontology->AddClass(GetAbstractClassName(), absCls);

		// recursive call
		AddSubClassesFromANDorANDOR(sepStmt, absCls);

		sepStmt = leftParen + sepStmt + rightParen;
		tmp = StrUtil::ReplaceCharacter(tmp, sepStmt, absCls->GetName());
	}

	vector<string> subClassNames;
	StrUtil::SplitStringBySeparator(tmp, sep, subClassNames);

	int subClassNameSize = (int)subClassNames.size();

	for (int i = 0; i < subClassNameSize; ++i)
	{
		string subClassName = subClassNames[i];
		subClassName = StrUtil::RemoveCharacter(subClassName, blank);

		Class* cls = m_ontology->GetClassByName(subClassName);

		S2OThrowExceptionIf<S2OException>(cls == nullptr, "Invalid class name: " + subClassName);

		if (StrUtil::Equal(sep, AND))
			superclass->AddObjectIntersectionOf(cls);
		else if (StrUtil::Equal(sep, ANDOR))
			superclass->AddObjectUnionOf(cls);
	}

	subClassNames.clear();
}


void SchemaTranslator::AddClassesForBaseTypes()
{
	Class* attrCls = nullptr;
	m_ontology->AddClass("attribute", attrCls);
	AddAnnotation(attrCls, "ExpConstruct", "attribute");

	Class* binCls = nullptr;
	m_ontology->AddClass("binary", binCls);
	AddAnnotation(binCls, "ExpConstruct", "datatype");
	binCls->AddSuperClass(attrCls);

	Class* boolCls = nullptr;
	m_ontology->AddClass("boolean", boolCls);
	AddAnnotation(boolCls, "ExpConstruct", "datatype");
	boolCls->AddSuperClass(attrCls);

	Class* intCls = nullptr;
	m_ontology->AddClass("integer", intCls);
	AddAnnotation(intCls, "ExpConstruct", "datatype");
	intCls->AddSuperClass(attrCls);

	Class* numCls = nullptr;
	m_ontology->AddClass("number", numCls);
	AddAnnotation(numCls, "ExpConstruct", "datatype");
	numCls->AddSuperClass(attrCls);

	Class* realCls = nullptr;
	m_ontology->AddClass("real", realCls);
	AddAnnotation(realCls, "ExpConstruct", "datatype");
	realCls->AddSuperClass(attrCls);

	Class* strCls = nullptr;
	m_ontology->AddClass("string", strCls);
	AddAnnotation(strCls, "ExpConstruct", "datatype");
	strCls->AddSuperClass(attrCls);

	Class* logiCls = nullptr;
	m_ontology->AddClass("logical", logiCls);
	AddAnnotation(logiCls, "ExpConstruct", "datatype");
	logiCls->AddSuperClass(attrCls);

	// logical individuals
	Individual* logical_true = nullptr;
	m_ontology->AddIndividual("TRUE", logical_true);
	logical_true->AddClassType(logiCls);

	Individual* logical_false = nullptr;
	m_ontology->AddIndividual("FALSE", logical_false);
	logical_false->AddClassType(logiCls);

	Individual* logical_unknown = nullptr;
	m_ontology->AddIndividual("UNKNOWN", logical_unknown);
	logical_unknown->AddClassType(logiCls);

	logiCls->AddObjectOneOf(logical_true);
	logiCls->AddObjectOneOf(logical_false);
	logiCls->AddObjectOneOf(logical_unknown);
}

void SchemaTranslator::AddClassesForTypes(const TypeDescriptorList* tDesList)
{
	TypeDescLinkNode* tDesLinkNode = (TypeDescLinkNode*)tDesList->GetHead();
	int typeCnt = tDesList->EntryCount();

	// for all types except reference type
	for (int i = 0; i < typeCnt; ++i)
	{
		try
		{
			const TypeDescriptor* tDes = tDesLinkNode->TypeDesc();
			PrimitiveType tDesFundType = tDes->FundamentalType();

			string tName = StrUtil::ToLower(tDes->Name());

			Class* tClass = nullptr;
			m_ontology->AddClass(tName, tClass);

			if (tDesFundType == sdaiBINARY
				|| tDesFundType == sdaiBOOLEAN
				|| tDesFundType == sdaiINTEGER
				|| tDesFundType == sdaiLOGICAL
				|| tDesFundType == sdaiNUMBER
				|| tDesFundType == sdaiREAL
				|| tDesFundType == sdaiSTRING)
			{
				string tSuperClassName = StrUtil::ToLower(tDes->Description());

				// In IFC schemas, there are "string (22) fixed" and "string (255)" that should be changed to "string".
				if (StrUtil::Exist(tSuperClassName, "string ("))
					tSuperClassName = "string";

				Class* tSuperClass = nullptr;
				m_ontology->AddClass(tSuperClassName, tSuperClass);

				tClass->AddSuperClass(tSuperClass);

				AddAnnotation(tClass, "ExpConstruct", "datatype");
			}
			else if (tDesFundType == sdaiENUMERATION)
			{
				EnumTypeDescriptor* enumTypeDes = (EnumTypeDescriptor*)tDes;
				SDAI_Enum* enumTypes = enumTypeDes->CreateEnum();

				int enumCnt = enumTypes->asInt();

				for (int j = 0; j < enumCnt; ++j)
				{
					string enumName = StrUtil::ToLower(enumTypes->get_value_at(j));

					if (StrUtil::Equal(enumName, "unset")) // exp2cxx makes this enum item by default
						continue;

					Individual* indvl = nullptr;
					m_ontology->AddIndividual(enumName, indvl);

					tClass->AddObjectOneOf(indvl);
					indvl->AddClassType(tClass);

					AddAnnotation(indvl, "ExpConstruct", "enumeration");
				}

				AddAnnotation(tClass, "ExpConstruct", "enumerationtype");
			}
			else if (tDesFundType == sdaiSELECT)
			{
				SelectTypeDescriptor* selTypeDes = (SelectTypeDescriptor*)tDes;
				TypeDescriptorList* selTypeDesList = &selTypeDes->Elements();
				int selCnt = selTypeDesList->EntryCount();

				TypeDescLinkNode* sTypeDesLinkNode = (TypeDescLinkNode*)selTypeDesList->GetHead();

				for (int j = 0; j < selCnt; ++j)
				{
					const TypeDescriptor* sTypeDes = sTypeDesLinkNode->TypeDesc();

					string selName = StrUtil::ToLower(sTypeDes->Name());

					Class* selClass = nullptr;
					m_ontology->AddClass(selName, selClass);
					S2OThrowExceptionIf<S2OException>(selClass == nullptr, "Invalid class name: " + selName);

					selClass->AddSuperClass(tClass);
					//tClass->AddDisjointUnionOf(selClass);

					sTypeDesLinkNode = (TypeDescLinkNode*)sTypeDesLinkNode->NextNode();
				}

				AddAnnotation(tClass, "ExpConstruct", "selecttype");
			}
			else if (tDesFundType == SET_TYPE
				|| tDesFundType == BAG_TYPE)
			{
				string aggrName;

				AddClassesObjPropertiesForSetBag(tDes, aggrName);

				Class* aggrCls = m_ontology->GetClassByName(aggrName);
				S2OThrowExceptionIf<S2OException>(aggrCls == nullptr, "Invalid class name: " + aggrName);

				if (tDesFundType == SET_TYPE)
					AddAnnotation(tClass, "ExpConstruct", "settype");
				else if (tDesFundType == BAG_TYPE)
					AddAnnotation(tClass, "ExpConstruct", "bagtype");
			}
			else if (tDesFundType == ARRAY_TYPE
				|| tDesFundType == LIST_TYPE)
			{
				string aggrName;
				AddClassesObjPropertiesForArrayList(tDes, aggrName);

				Class* aggrCls = m_ontology->GetClassByName(aggrName);
				S2OThrowExceptionIf<S2OException>(aggrCls == nullptr, "Invalid class name: " + aggrName);

				if (tDesFundType == ARRAY_TYPE)
					AddAnnotation(tClass, "ExpConstruct", "arraytype");
				else if (tDesFundType == LIST_TYPE)
					AddAnnotation(tClass, "ExpConstruct", "listtype");
			}
			else
			{
				// if required
			}

			tDesLinkNode = (TypeDescLinkNode*)tDesLinkNode->NextNode();
		}
		catch (S2OException & e)
		{
			cout << e.GetMessage() << endl;
		}
	}

	// re-init
	tDesLinkNode = (TypeDescLinkNode*)tDesList->GetHead();

	// for reference type
	for (int i = 0; i < typeCnt; ++i)
	{
		try
		{
			const TypeDescriptor* tDes = tDesLinkNode->TypeDesc();
			PrimitiveType tDesFundType = tDes->FundamentalType();

			if (tDesFundType == REFERENCE_TYPE)
			{
				string tName = StrUtil::ToLower(tDes->Name());
				string tSuperClassName = StrUtil::ToLower(tDes->Description());

				Class* tClass = nullptr, * tSuperClass = nullptr;
				m_ontology->AddClass(tName, tClass);
				m_ontology->AddClass(tSuperClassName, tSuperClass);

				S2OThrowExceptionIf<S2OException>(tClass == nullptr, "Invalid class name: " + tName);
				S2OThrowExceptionIf<S2OException>(tSuperClass == nullptr, "Invalid class name: " + tSuperClassName);

				tClass->AddSuperClass(tSuperClass);

				const TypeDescriptor* refTypeDes = tDes->ReferentType();
				string expConsType;

				if (refTypeDes) // datatypes (e.g. string, integer, real etc.)
				{
					string fundType;

					while (refTypeDes)
					{
						fundType = StrUtil::ToLower(refTypeDes->Description());
						refTypeDes = refTypeDes->ReferentType();
					}

					Class* fundCls = m_ontology->GetClassByName(fundType);
					S2OThrowExceptionIf<S2OException>(fundCls == nullptr, "Invalid class name: " + fundType);

					AnnotationProperty* annoProp = m_ontology->GetAnnotationPropertyByName("ExpConstruct");
					expConsType = fundCls->GetAnnotationStringByAnnotationProperty(annoProp);
				}
				else // select, enum etc.
				{
					AnnotationProperty* annoProp = m_ontology->GetAnnotationPropertyByName("ExpConstruct");
					expConsType = tSuperClass->GetAnnotationStringByAnnotationProperty(annoProp);
				}

				AddAnnotation(tClass, "ExpConstruct", expConsType);
			}

			tDesLinkNode = (TypeDescLinkNode*)tDesLinkNode->NextNode();
		}
		catch (S2OException & e)
		{
			cout << e.GetMessage() << endl;
		}
	}
}


void SchemaTranslator::AddClassesObjPropertiesForSetBag(const TypeDescriptor* aggrTypeDes, string& aggrName)
{
	string aggrClassName;

	PrimitiveType aggrType = aggrTypeDes->FundamentalType();

	string targetClassName;

	if (!aggrTypeDes->ReferentType()) // types for set, bag
	{
		string temp = aggrTypeDes->Description();
		targetClassName = temp.substr(temp.find_last_of("OF") + 2, temp.length());
	}
	else
	{
		const TypeDescriptor* aggrRefTypeDes = aggrTypeDes->ReferentType();
		PrimitiveType aggrRefFundType = aggrRefTypeDes->FundamentalType();

		if (aggrRefFundType == SET_TYPE
			|| aggrRefFundType == BAG_TYPE)
		{
			AddClassesObjPropertiesForSetBag(aggrRefTypeDes, aggrName);
			targetClassName = aggrName;
		}
		else if (aggrRefFundType == ARRAY_TYPE
			|| aggrRefFundType == LIST_TYPE)
		{
			AddClassesObjPropertiesForArrayList(aggrRefTypeDes, aggrName);
			targetClassName = aggrName;
		}
		else
		{
			targetClassName = StrUtil::ToLower(aggrRefTypeDes->Name());
		}
	}

	if (aggrType == SET_TYPE)
		aggrClassName = "set_of_" + targetClassName;
	else if (aggrType == BAG_TYPE)
		aggrClassName = "bag_of_" + targetClassName;

	aggrName = aggrClassName;

	string aggrObjPropName = aggrClassName + "_has_content";

	if (m_ontology->GetClassByName(aggrClassName)
		&& m_ontology->GetObjectPropertyByName(aggrObjPropName))
		return;

	Class* targetClass = nullptr;
	m_ontology->AddClass(targetClassName, targetClass);

	Class* aggrClass = nullptr;
	m_ontology->AddClass(aggrClassName, aggrClass);

	ObjectProperty* aggrObjProp = nullptr;
	m_ontology->AddObjectProperty(aggrObjPropName, aggrObjProp);

	aggrObjProp->AddDomain(aggrClass);
	aggrObjProp->AddRange(targetClass);

	aggrClass->AddObjectProperty(aggrObjProp);

	// if there's no cardinality restriction
	if (StrUtil::Exist(string(aggrTypeDes->Description()), "SET OF")
		|| StrUtil::Exist(string(aggrTypeDes->Description()), "BAG OF"))
	{
		aggrClass->AddObjectPropertyCardinality(aggrObjProp, MAXCARD, MAXCARD);
	}
	else
	{
		AggrTypeDescriptor* aggrDes = (AggrTypeDescriptor*)aggrTypeDes;
		SDAI_Integer b1 = aggrDes->Bound1();
		SDAI_Integer b2 = aggrDes->Bound2();

		aggrClass->AddObjectPropertyCardinality(aggrObjProp, b1, b2);
	}

	if (aggrType == SET_TYPE)
		AddAnnotation(aggrClass, "ExpConstruct", "set");
	else if (aggrType == BAG_TYPE)
		AddAnnotation(aggrClass, "ExpConstruct", "bag");
}

void SchemaTranslator::AddClassesObjPropertiesForArrayList(const TypeDescriptor* aggrTypeDes, string& aggrName)
{
	string aggrClassName;
	string emptyAggrClassName;

	PrimitiveType aggrType = aggrTypeDes->FundamentalType();

	string targetClassName;

	if (!aggrTypeDes->ReferentType()) // types for array, list
	{
		string temp = aggrTypeDes->Description();
		targetClassName = temp.substr(temp.find_last_of("OF") + 2, temp.length());
	}
	else
	{
		const TypeDescriptor* aggrRefTypeDes = aggrTypeDes->ReferentType();
		PrimitiveType aggrRefFundType = aggrRefTypeDes->FundamentalType();

		if (aggrRefFundType == SET_TYPE
			|| aggrRefFundType == BAG_TYPE)
		{
			AddClassesObjPropertiesForSetBag(aggrRefTypeDes, aggrName);
			targetClassName = aggrName;
		}
		else if (aggrRefFundType == ARRAY_TYPE
			|| aggrRefFundType == LIST_TYPE)
		{
			AddClassesObjPropertiesForArrayList(aggrRefTypeDes, aggrName);
			targetClassName = aggrName;
		}
		else
		{
			targetClassName = StrUtil::ToLower(aggrRefTypeDes->Name());
		}
	}

	if (aggrType == ARRAY_TYPE)
	{
		aggrClassName = "array_of_" + targetClassName;
		emptyAggrClassName = "emptyarray_of_" + targetClassName;
	}
	else if (aggrType == LIST_TYPE)
	{
		aggrClassName = "list_of_" + targetClassName;
		emptyAggrClassName = "emptylist_of_" + targetClassName;
	}

	aggrName = aggrClassName;

	if (m_ontology->GetClassByName(aggrClassName)
		&& m_ontology->GetClassByName(emptyAggrClassName))
		return;

	string aggrObjPropName_1 = "has" + aggrClassName;
	string aggrObjPropName_2 = aggrClassName + "_has_content";
	string aggrObjPropName_3 = aggrClassName + "_is_followed_by";
	string aggrObjPropName_4 = aggrClassName + "_has_next";

	Class* targetClass = nullptr;
	m_ontology->AddClass(targetClassName, targetClass);

	Class* aggrClass = nullptr;
	m_ontology->AddClass(aggrClassName, aggrClass);

	Class* emptyAggrClass = nullptr;
	m_ontology->AddClass(emptyAggrClassName, emptyAggrClass);

	ObjectProperty* aggrObjProp_1 = nullptr;
	m_ontology->AddObjectProperty(aggrObjPropName_1, aggrObjProp_1);

	ObjectProperty* aggrObjProp_2 = nullptr;
	m_ontology->AddObjectProperty(aggrObjPropName_2, aggrObjProp_2);

	ObjectProperty* aggrObjProp_3 = nullptr;
	m_ontology->AddObjectProperty(aggrObjPropName_3, aggrObjProp_3);

	ObjectProperty* aggrObjProp_4 = nullptr;
	m_ontology->AddObjectProperty(aggrObjPropName_4, aggrObjProp_4);

	aggrObjProp_4->AddSuperProperty(aggrObjProp_3);
	aggrObjProp_3->AddSuperProperty(aggrObjProp_1);
	aggrObjProp_2->AddSuperProperty(aggrObjProp_1);

	aggrObjProp_1->AddDomain(aggrClass);
	aggrObjProp_2->AddRange(targetClass);
	aggrObjProp_3->AddRange(aggrClass);

	aggrObjProp_2->SetFunctional(true);
	aggrObjProp_3->SetTransitive(true);

	aggrClass->AddObjectProperty(aggrObjProp_2);
	aggrClass->AddObjectPropertyCardinality(aggrObjProp_2, MAXCARD, MAXCARD);

	aggrClass->AddObjectProperty(aggrObjProp_3);
	aggrClass->AddObjectPropertyCardinality(aggrObjProp_3, MAXCARD, MAXCARD);
	
	emptyAggrClass->AddSuperClass(aggrClass);

	emptyAggrClass->AddObjectProperty(aggrObjProp_2);
	emptyAggrClass->AddObjectPropertyCardinality(aggrObjProp_2, 0, 0);

	if (aggrType == ARRAY_TYPE)
		AddAnnotation(aggrClass, "ExpConstruct", "array");
	else if (aggrType == LIST_TYPE)
		AddAnnotation(aggrClass, "ExpConstruct", "list");
}

void SchemaTranslator::AddAnnotation(Object* obj, string annoPropName, string annoStr)
{
	Annotation anno;
	anno.first = m_ontology->GetAnnotationPropertyByName(annoPropName);
	anno.second = annoStr;

	obj->AddAnnotation(anno);
}


void SchemaTranslator::AddDataTypes()
{
	DataType* xsd_binary = nullptr;
	m_ontology->AddDataType("xsd:hexBinary", xsd_binary);

	DataType* xsd_boolean = nullptr;
	m_ontology->AddDataType("xsd:boolean", xsd_boolean);

	DataType* xsd_decimal = nullptr;
	m_ontology->AddDataType("xsd:decimal", xsd_decimal);

	DataType* xsd_integer = nullptr;
	m_ontology->AddDataType("xsd:integer", xsd_integer);

	DataType* xsd_string = nullptr;
	m_ontology->AddDataType("xsd:string", xsd_string);
}

void SchemaTranslator::AddDataProperties(OWL2Profile owl2Profile)
{
	DataProperty* to_binary = nullptr;
	m_ontology->AddDataProperty("to_binary", to_binary);
	Class* binaryCls = m_ontology->GetClassByName("binary");
	to_binary->AddDomain(binaryCls);
	to_binary->AddRange(m_ontology->GetDataTypeByName("xsd:hexBinary"));
	binaryCls->AddDataProperty(to_binary);
	binaryCls->AddDataPropertyCardinality(to_binary, 0, 1);

	DataProperty* to_boolean = nullptr;
	m_ontology->AddDataProperty("to_boolean", to_boolean);
	Class* booleanCls = m_ontology->GetClassByName("boolean");
	to_boolean->AddDomain(booleanCls);

	// Use xsd:string instead of xsd:boolean because DL and RL do not allow xsd:boolean
	if (owl2Profile == OWL2Profile::OWL2_QL
		|| owl2Profile == OWL2Profile::OWL2_EL)
		to_boolean->AddRange(m_ontology->GetDataTypeByName("xsd:string"));
	else
		to_boolean->AddRange(m_ontology->GetDataTypeByName("xsd:boolean"));

	booleanCls->AddDataProperty(to_boolean);
	booleanCls->AddDataPropertyCardinality(to_boolean, 0, 1);

	DataProperty* to_decimal = nullptr;
	m_ontology->AddDataProperty("to_decimal", to_decimal);
	Class* realCls = m_ontology->GetClassByName("real");
	Class* numberCls = m_ontology->GetClassByName("number");
	to_decimal->AddDomain(realCls);
	to_decimal->AddDomain(numberCls);
	to_decimal->AddRange(m_ontology->GetDataTypeByName("xsd:decimal"));
	realCls->AddDataProperty(to_decimal);
	realCls->AddDataPropertyCardinality(to_decimal, 0, 1);
	numberCls->AddDataProperty(to_decimal);
	numberCls->AddDataPropertyCardinality(to_decimal, 0, 1);

	DataProperty* to_integer = nullptr;
	m_ontology->AddDataProperty("to_integer", to_integer);
	Class* integerCls = m_ontology->GetClassByName("integer");
	to_integer->AddDomain(integerCls);
	to_integer->AddRange(m_ontology->GetDataTypeByName("xsd:integer"));
	integerCls->AddDataProperty(to_integer);
	integerCls->AddDataPropertyCardinality(to_decimal, 0, 1);

	DataProperty* to_string = nullptr;
	m_ontology->AddDataProperty("to_string", to_string);
	Class* stringCls = m_ontology->GetClassByName("string");
	to_string->AddDomain(stringCls);
	to_string->AddRange(m_ontology->GetDataTypeByName("xsd:string"));
	stringCls->AddDataProperty(to_string);
	stringCls->AddDataPropertyCardinality(to_decimal, 0, 1);
}

void SchemaTranslator::AddAnnotationProperties()
{
	AnnotationProperty* expConstruct = nullptr;
	m_ontology->AddAnnotationProperty("ExpConstruct", expConstruct);
	expConstruct->AddRange(m_ontology->GetDataTypeByName("xsd:string"));

	AnnotationProperty* opType = nullptr;
	m_ontology->AddAnnotationProperty("OPType", opType);
	opType->AddRange(m_ontology->GetDataTypeByName("xsd:string"));

	AnnotationProperty* parameterIndex = nullptr;
	m_ontology->AddAnnotationProperty("ParameterIndex", parameterIndex);
	parameterIndex->AddRange(m_ontology->GetDataTypeByName("xsd:integer"));

	AnnotationProperty* supertypeIndex = nullptr;
	m_ontology->AddAnnotationProperty("SupertypeIndex", supertypeIndex);
	supertypeIndex->AddRange(m_ontology->GetDataTypeByName("xsd:integer"));
}


// For simplified geometry representation
void SchemaTranslator::AddDataPropertiesForXYZ()
{
	DataProperty* has_x = nullptr;
	m_ontology->AddDataProperty("has_x", has_x);
	DataProperty* has_y = nullptr;
	m_ontology->AddDataProperty("has_y", has_y);
	DataProperty* has_z = nullptr;
	m_ontology->AddDataProperty("has_z", has_z);

	Class* cartesianPointCls = m_ontology->GetClassByName("cartesian_point");
	Class* directionCls = m_ontology->GetClassByName("direction");
	has_x->AddDomain(cartesianPointCls);
	has_x->AddDomain(directionCls);
	has_x->AddRange(m_ontology->GetDataTypeByName("xsd:decimal"));
	has_y->AddDomain(cartesianPointCls);
	has_y->AddDomain(directionCls);
	has_y->AddRange(m_ontology->GetDataTypeByName("xsd:decimal"));
	has_z->AddDomain(cartesianPointCls);
	has_z->AddDomain(directionCls);
	has_z->AddRange(m_ontology->GetDataTypeByName("xsd:decimal"));

	cartesianPointCls->AddDataProperty(has_x);
	cartesianPointCls->AddDataPropertyCardinality(has_x, 0, 1);
	cartesianPointCls->AddDataProperty(has_y);
	cartesianPointCls->AddDataPropertyCardinality(has_y, 0, 1);
	cartesianPointCls->AddDataProperty(has_z);
	cartesianPointCls->AddDataPropertyCardinality(has_z, 0, 1);

	directionCls->AddDataProperty(has_x);
	directionCls->AddDataPropertyCardinality(has_x, 0, 1);
	directionCls->AddDataProperty(has_y);
	directionCls->AddDataPropertyCardinality(has_y, 0, 1);
	directionCls->AddDataProperty(has_z);
	directionCls->AddDataPropertyCardinality(has_z, 0, 1);

	Class* coordsList = m_ontology->GetClassByName("coordinates_list");

	if (coordsList)
	{
		ObjectProperty* objProp = m_ontology->GetObjectPropertyByName("coordinates_list_has_position_coords");
		objProp->AddRange(m_ontology->GetClassByName("list_of_cartesian_point"));

		coordsList->AddObjectPropertyCardinality(objProp, MAXCARD, 1);
	}
}

