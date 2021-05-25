#include "stdafx.h"
#include "InstanceTranslator.h"

#include "Ontology.h"
#include "Class.h"
#include "ObjectProperty.h"
#include "DataProperty.h"
#include "Individual.h"


InstanceTranslator::InstanceTranslator(InstMgr*& instList, Ontology*& ontology)
	:m_ontology(ontology),
	m_instList(instList),
	m_simpleGeometry(false)
{
	InitializeSimpleGeomtryMap();
}

InstanceTranslator::~InstanceTranslator()
{
}

void InstanceTranslator::Translate(S2O_Option& opt)
{
	m_simpleGeometry = opt.IsSimpleGeomtry();

	AddIndividualsForInstances();
}


void InstanceTranslator::AddIndividualsForInstances()
{
	// instance count
	int instCnt = m_instList->InstanceCount();

	for (int i = 0; i < instCnt; ++i)
	{
		try
		{
			SDAI_Application_instance_ptr inst = m_instList->GetSTEPentity(i);

			string entName = StrUtil::ToLower(inst->EntityName());
			int entId = inst->StepFileId();

			string indvlName = "i" + to_string(entId);

			Individual* indvl = nullptr;
			m_ontology->AddIndividual(indvlName, indvl);

			// complex instance 
			// e.g. "#3=(NAMED_UNIT(*)PLANE_ANGLE_UNIT()SI_UNIT($,.RADIAN.));"
			if (inst->IsComplex())
			{
				STEPcomplex* comp = (STEPcomplex*)inst;

				while (comp != nullptr)
				{
					SDAI_Application_instance_ptr compInst = (SDAI_Application_instance_ptr)comp;
					entName = StrUtil::ToLower(compInst->EntityName());
					Class* cls = m_ontology->GetClassByName(entName);

					S2OThrowExceptionIf<S2OException>(cls == nullptr, "Invalid class name: " + entName);

					indvl->AddClassType(cls);

					AddPropertyAssertions(indvl, compInst);

					comp = comp->sc;
				}
			}
			else // normal instance
			{
				Class* cls = m_ontology->GetClassByName(entName);

				S2OThrowExceptionIf<S2OException>(cls == nullptr, "Invalid class name: " + entName);

				indvl->AddClassType(cls);

				AddPropertyAssertions(indvl, inst);
			}
		}
		catch (S2OException & e)
		{
			cout << e.GetMessage() << endl;
		}
	}
}

void InstanceTranslator::AddPropertyAssertions(Individual* indvl, SDAI_Application_instance_ptr inst)
{
	STEPattributeList attrList = inst->attributes;	// attribute list
	int attrCnt = attrList.EntryCount();			// attribute list count

	if (attrCnt == 0)
		return;

	//string entName = StrUtil::ToLower(inst->EntityName());

	map<STEPattribute*, int> redefAttrs;

	for (int i = 0; i < attrCnt; ++i)
	{
		try
		{
			STEPattribute* attr = &attrList.operator[](i);
			S2OThrowExceptionIf<S2OException>(attr == nullptr, "Null STEP attribute.");

			STEPattribute* redefAttr = attr->RedefiningAttr();	// redefined attribute

			// save if the attribute has a redefined attribute
			if (redefAttr)
				redefAttrs.insert({ redefAttr , 1 });
			else
				redefAttrs.insert({ nullptr , 0 });

			if (redefAttrs[attr] == 1)
				continue;

			const AttrDescriptor* attrDes = attr->getADesc();
			string attrName = attrDes->Name();	// attribute name
			string owningEntName = StrUtil::ToLower(attrDes->Owner().Name());

			string objPropName = owningEntName + "_has_" + attrName; // object property name
			ObjectProperty* objProp = m_ontology->GetObjectPropertyByName(objPropName);

			// if an attribute has a redefined attribute, only the redefined attribute contains values
			if (redefAttr)
				attr = redefAttr;

			string attrStr = attr->asStr();

			if (attrStr.empty())
				continue;

			BASE_TYPE attrType = attr->Type();	// attribute type

			if (attrType == SET_TYPE
				|| attrType == BAG_TYPE
				|| attrType == LIST_TYPE
				|| attrType == ARRAY_TYPE)
			{
				STEPaggregate_ptr aggr = attr->Aggregate();
				int aggrCnt = aggr->EntryCount();

				if (aggrCnt == 0)
					continue;

				string aggrName;
				const TypeDescriptor* attrDomTypeDes = attrDes->DomainType();

				string attrIndvlName = indvl->GetName() + "_" + attrName;

				if (attrType == SET_TYPE
					|| attrType == BAG_TYPE)
				{
					AddPropertyAssertionsForSetBag(attrIndvlName, attrStr, attrDomTypeDes, aggrName);
				}
				else if (attrType == LIST_TYPE
					|| attrType == ARRAY_TYPE)
				{			
					// For target classes for simplified geometry
					if (m_simpleGeometry)
					{
						if (IsSimpleGeometryTarget(indvl->GetClassTypeAt(0)->GetName()))
						{
							if (indvl->GetClassTypeAt(0)->GetName() == "coordinates_list")
								AddPropertyAssertionsForCoordinatesList(indvl, attrIndvlName, attrStr);
							else
								AddDataPropertyAssertionsForCartesianPointAndDirection(indvl, attrStr);

							continue;
						}
					}

					AddPropertyAssertionsForArrayList(attrIndvlName, attrStr, attrDomTypeDes, aggrName);
					attrIndvlName = attrIndvlName + "_1"; // for the sake of list and array
				}

				AddObjectPropertyAssertion(indvl, objProp, attrIndvlName);
			}
			else if (attrType == GENERIC_TYPE)
			{
				// if required
				cout << "The attribute \"" << attrName << "\" of the entity \"" << owningEntName << "\" is currently NOT supported." << endl;
				cout << "GENERIC_TYPE was found!" << endl;
			}
			else if (attrType == UNKNOWN_TYPE)
			{
				// if required
				cout << "The attribute \"" << attrName << "\" of the entity \"" << owningEntName << "\" is currently NOT supported." << endl;
				cout << "UNKNOWN_TYPE was found!" << endl;
			}
			else if (attrType == REFERENCE_TYPE)
			{
				AddPropertyAssertion(indvl, objProp, attr);
			}
			else
			{
				AddPropertyAssertion(indvl, objProp, attr);
			}
		}
		catch (S2OException & e)
		{
			cout << e.GetMessage() << endl;
		}
	}

	redefAttrs.clear();
}

void InstanceTranslator::AddPropertyAssertion(Individual* indvl, ObjectProperty* objProp, STEPattribute* attr)
{
	BASE_TYPE attrBaseType = attr->BaseType();	// attribute base type
	string attrName = attr->getADesc()->Name();
	string attrDomTypeName = StrUtil::ToLower(attr->getADesc()->DomainType()->Name());

	if (attrBaseType == sdaiINSTANCE)
	{
		SDAI_Application_instance_ptr inst = attr->Entity();
		int instId = inst->StepFileId();

		if (instId == 0)
			return;

		string attrIndvlName = "i" + to_string(instId);

		AddObjectPropertyAssertion(indvl, objProp, attrIndvlName);
	}
	else if (attrBaseType == sdaiSTRING)
	{
		SDAI_String* val = attr->String();
		string str(val->c_str());

		string attrIndvlName = indvl->GetName() + "_" + attrName;

		AddObjectPropertyAssertion(indvl, objProp, attrIndvlName);
		AddDataPropertyAssertion(m_ontology->GetIndividualByName(attrIndvlName), attrDomTypeName, "to_string", str);
	}
	else if (attrBaseType == sdaiINTEGER)
	{
		SDAI_Integer* val = attr->Integer();
		string str = attr->asStr();

		string attrIndvlName = indvl->GetName() + "_" + attrName;

		AddObjectPropertyAssertion(indvl, objProp, attrIndvlName);
		AddDataPropertyAssertion(m_ontology->GetIndividualByName(attrIndvlName), attrDomTypeName, "to_integer", str);
	}
	else if (attrBaseType == sdaiREAL
		|| attrBaseType == sdaiNUMBER)
	{
		SDAI_Real* val;

		if (attrBaseType == sdaiREAL)
			val = attr->Real();
		else if (attrBaseType == sdaiNUMBER)
			val = attr->Number();

		string str = attr->asStr();

		string attrIndvlName = indvl->GetName() + "_" + attrName;

		AddObjectPropertyAssertion(indvl, objProp, attrIndvlName);
		AddDataPropertyAssertion(m_ontology->GetIndividualByName(attrIndvlName), attrDomTypeName, "to_decimal", str);
	}
	else if (attrBaseType == sdaiBOOLEAN)
	{
		SDAI_BOOLEAN* val = attr->Boolean();
		string str;
		val->asStr(str);

		string attrIndvlName = indvl->GetName() + "_" + attrName;

		AddObjectPropertyAssertion(indvl, objProp, attrIndvlName);
		AddDataPropertyAssertion(m_ontology->GetIndividualByName(attrIndvlName), attrDomTypeName, "to_boolean", str);
	}
	else if (attrBaseType == sdaiBINARY)
	{
		SDAI_Binary* val = attr->Binary();
		string str = val->asStr();

		string attrIndvlName = indvl->GetName() + "_" + attrName;

		AddObjectPropertyAssertion(indvl, objProp, attrIndvlName);
		AddDataPropertyAssertion(m_ontology->GetIndividualByName(attrIndvlName), attrDomTypeName, "to_binary", str);
	}
	else if (attrBaseType == sdaiLOGICAL)
	{
		SDAI_LOGICAL* val = attr->Logical();
		string str;
		val->asStr(str);

		if (StrUtil::Equal(str, "T"))
			str = "TRUE";
		else if (StrUtil::Equal(str, "F"))
			str = "FALSE";
		else
			str = "UNKNOWN";

		string attrIndvlName = str;

		AddObjectPropertyAssertion(indvl, objProp, attrIndvlName);
	}
	else if (attrBaseType == sdaiENUMERATION)
	{
		SDAI_Enum* val = attr->Enum();
		string str;
		val->asStr(str);

		if (str.empty())
			return;

		string attrIndvlName = StrUtil::ToLower(str);

		AddObjectPropertyAssertion(indvl, objProp, attrIndvlName);
	}
	else if (attrBaseType == sdaiAGGR)
	{
		// if required
		string owningEntName = attr->getADesc()->Owner().Name();

		cout << "The attribute \"" << attrName << "\" of the entity \"" << owningEntName << "\" is currently NOT supported." << endl;
		cout << "sdaiAGGR type was found!" << endl;
	}
	else if (attrBaseType == sdaiSELECT)
	{
		string str = attr->asStr();
		string attrIndvlName = indvl->GetName() + "_" + attrName;

		AddPropertyAssertionForSelectFromString(indvl, objProp, attrIndvlName, str);
	}
}

void InstanceTranslator::AddObjectPropertyAssertion(Individual* domainIndvl, ObjectProperty* objProp, string rangeIndvlName)
{
	// remove blank
	if (StrUtil::Exist(rangeIndvlName, " "))
		rangeIndvlName = StrUtil::RemoveCharacter(rangeIndvlName, " ");

	rangeIndvlName = StrUtil::RemoveCharacter(rangeIndvlName, "\n");	// next line

	Individual* attrIndvl = nullptr;
	m_ontology->AddIndividual(rangeIndvlName, attrIndvl);

	domainIndvl->AddObjPropertyAssertion(objProp, attrIndvl);
}

void InstanceTranslator::AddDataPropertyAssertion(Individual* domainIndvl, string domainTypeName, string datPropName, string rangeValue)
{
	if (StrUtil::Equal(datPropName, "to_decimal")
		|| StrUtil::Equal(datPropName, "to_integer")
		|| StrUtil::Equal(datPropName, "has_x")
		|| StrUtil::Equal(datPropName, "has_y")
		|| StrUtil::Equal(datPropName, "has_z"))
	{
		rangeValue = StrUtil::RemoveCharacter(rangeValue, "\n");	// next line
		rangeValue = StrUtil::RemoveCharacter(rangeValue, "\t");	// tab
		rangeValue = StrUtil::RemoveCharacter(rangeValue, " ");		// blank

		if (!StrUtil::Equal(datPropName, "to_integer"))
		{
			if (StrUtil::Exist(rangeValue, ".")
				&& rangeValue.find(".") == rangeValue.length() - 1) // e.g. "1." to "1.0"
				rangeValue = StrUtil::ReplaceCharacter(rangeValue, ".", ".0");

			rangeValue = StrUtil::ReplaceCharacter(rangeValue, ".e", ".0e");
			rangeValue = StrUtil::ReplaceCharacter(rangeValue, ".E", ".0E");
		}
	}
	else if (StrUtil::Equal(datPropName, "to_string"))
	{
		rangeValue = StrUtil::GetStringBetween(rangeValue, "'", "'");
		rangeValue = StrUtil::ReplaceCharacter(rangeValue, "\\", "\\\\");
	}
	else if (StrUtil::Equal(datPropName, "to_boolean"))
	{
		if (StrUtil::Exist(rangeValue, "T"))
			rangeValue = "true";
		else if (StrUtil::Exist(rangeValue, "F"))
			rangeValue = "false";
	}

	Class* domainClass = m_ontology->GetClassByName(domainTypeName);
	DataProperty* datProp = m_ontology->GetDataPropertyByName(datPropName);
	
	S2OThrowExceptionIf<S2OException>(domainClass == nullptr, "Invalid class name: " + domainTypeName);
	S2OThrowExceptionIf<S2OException>(datProp == nullptr, "Invalid data property name: " + datPropName);

	domainIndvl->AddClassType(domainClass);
	domainIndvl->AddDataPropertyAssertion(datProp, rangeValue);
}


void InstanceTranslator::AddPropertyAssertionsForSetBag(string aggrIndvlName, string str, const TypeDescriptor* aggrTypeDes, string& aggrName)
{
	Individual* aggrIndvl = nullptr;
	m_ontology->AddIndividual(aggrIndvlName, aggrIndvl);

	string aggrClassName;
	string targetClassName;

	const TypeDescriptor* attrDomRefType = aggrTypeDes->ReferentType();
	PrimitiveType attrDomRefFundType = attrDomRefType->FundamentalType();

	// split the given string
	vector<string> subStrs;
	StrUtil::SplitStringByCommaInParentheses(str, subStrs);

	int aggrCnt = (int)subStrs.size();

	for (int i = 0; i < aggrCnt; ++i)
	{
		string subStr = subStrs[i];

		S2OThrowExceptionIf<S2OException>(subStr.empty(), "Null string.");

		if (attrDomRefFundType == SET_TYPE
			|| attrDomRefFundType == BAG_TYPE)
		{
			string attrIndvlName = aggrIndvl->GetName() + "_value";
			AddPropertyAssertionsForSetBag(attrIndvlName, subStr, attrDomRefType, targetClassName);
		}
		else if (attrDomRefFundType == LIST_TYPE
			|| attrDomRefFundType == ARRAY_TYPE)
		{
			string attrIndvlName = aggrIndvl->GetName() + "_value";
			AddPropertyAssertionsForArrayList(attrIndvlName, subStr, attrDomRefType, targetClassName);
		}
		else
		{
			targetClassName = StrUtil::ToLower(attrDomRefType->Name());
		}

		PrimitiveType attrDomFundType = aggrTypeDes->FundamentalType();

		if (attrDomFundType == SET_TYPE)
			aggrClassName = "set_of_" + targetClassName;
		else if (attrDomFundType == BAG_TYPE)
			aggrClassName = "bag_of_" + targetClassName;

		aggrName = aggrClassName;

		aggrIndvl->AddClassType(m_ontology->GetClassByName(aggrName));

		string attrIndvlObjPropName = aggrName + "_has_content";
		ObjectProperty* attrIndvlObjProp = m_ontology->GetObjectPropertyByName(attrIndvlObjPropName);

		S2OThrowExceptionIf<S2OException>(attrIndvlObjProp == nullptr, "Invalid object property name: " + attrIndvlObjPropName);

		if (attrDomRefFundType == SET_TYPE
			|| attrDomRefFundType == BAG_TYPE)
		{
			string attrIndvlName = aggrIndvl->GetName() + "_value";
			AddObjectPropertyAssertion(aggrIndvl, attrIndvlObjProp, attrIndvlName);
		}
		else if (attrDomRefFundType == LIST_TYPE
			|| attrDomRefFundType == ARRAY_TYPE)
		{
			string attrIndvlName = aggrIndvl->GetName() + "_value_1";
			AddObjectPropertyAssertion(aggrIndvl, attrIndvlObjProp, attrIndvlName);
		}
		else
		{
			string attrIndvlName = aggrIndvl->GetName() + "_value";
			string attrDomTypeName = StrUtil::ToLower(attrDomRefType->Name());

			if (attrDomRefFundType == sdaiINSTANCE)
			{
				subStr = "i" + StrUtil::RemoveCharacter(subStr, "#");
				AddObjectPropertyAssertion(aggrIndvl, attrIndvlObjProp, subStr);
			}
			else if (attrDomRefFundType == sdaiSTRING)
			{
				AddObjectPropertyAssertion(aggrIndvl, attrIndvlObjProp, attrIndvlName);
				AddDataPropertyAssertion(m_ontology->GetIndividualByName(attrIndvlName), attrDomTypeName, "to_string", subStr);
			}
			else if (attrDomRefFundType == sdaiREAL
				|| attrDomRefFundType == sdaiNUMBER)
			{
				AddObjectPropertyAssertion(aggrIndvl, attrIndvlObjProp, attrIndvlName);
				AddDataPropertyAssertion(m_ontology->GetIndividualByName(attrIndvlName), attrDomTypeName, "to_decimal", subStr);
			}
			else if (attrDomRefFundType == sdaiINTEGER)
			{
				AddObjectPropertyAssertion(aggrIndvl, attrIndvlObjProp, attrIndvlName);
				AddDataPropertyAssertion(m_ontology->GetIndividualByName(attrIndvlName), attrDomTypeName, "to_integer", subStr);
			}
			else if (attrDomRefFundType == sdaiBOOLEAN)
			{
				AddObjectPropertyAssertion(aggrIndvl, attrIndvlObjProp, attrIndvlName);
				AddDataPropertyAssertion(m_ontology->GetIndividualByName(attrIndvlName), attrDomTypeName, "to_boolean", subStr);
			}
			else if (attrDomRefFundType == sdaiBINARY)
			{
				AddObjectPropertyAssertion(aggrIndvl, attrIndvlObjProp, attrIndvlName);
				AddDataPropertyAssertion(m_ontology->GetIndividualByName(attrIndvlName), attrDomTypeName, "to_binary", subStr);
			}
			else if (attrDomRefFundType == sdaiLOGICAL)
			{
				if (StrUtil::Exist(subStr, "T"))
					subStr = "TRUE";
				else if (StrUtil::Exist(subStr, "F"))
					subStr = "FALSE";
				else
					subStr = "UNKNOWN";

				AddObjectPropertyAssertion(aggrIndvl, attrIndvlObjProp, subStr);
			}
			else if (attrDomRefFundType == sdaiENUMERATION)
			{
				if (subStr.empty())
					continue;

				attrIndvlName = StrUtil::ToLower(subStr);
				AddObjectPropertyAssertion(aggrIndvl, attrIndvlObjProp, attrIndvlName);
			}
			else if (attrDomRefFundType == sdaiSELECT)
			{
				AddPropertyAssertionForSelectFromString(aggrIndvl, attrIndvlObjProp, attrIndvlName, subStr);
			}
			else if (attrDomRefFundType == sdaiAGGR)
			{
				// if required
			}
		}
	}

	subStrs.clear();
}

void InstanceTranslator::AddPropertyAssertionsForArrayList(string aggrIndvlName, string str, const TypeDescriptor* aggrTypeDes, string& aggrName)
{
	string aggrClassName;
	string targetClassName;

	const TypeDescriptor* attrDomRefType = aggrTypeDes->ReferentType();
	PrimitiveType attrDomRefFundType = attrDomRefType->FundamentalType();

	// split the given string
	vector<string> subStrs;
	StrUtil::SplitStringByCommaInParentheses(str, subStrs);

	int aggrCnt = (int)subStrs.size();

	for (int i = 0; i < aggrCnt; ++i)
	{
		Individual* aggrIndvl = nullptr;
		m_ontology->AddIndividual(aggrIndvlName + "_" + to_string(i + 1), aggrIndvl);

		string subStr = subStrs[i];

		S2OThrowExceptionIf<S2OException>(subStr.empty(), "Null string.");

		if (attrDomRefFundType == SET_TYPE
			|| attrDomRefFundType == BAG_TYPE)
		{
			string attrIndvlName = aggrIndvl->GetName() + "_value";
			AddPropertyAssertionsForSetBag(attrIndvlName, subStr, attrDomRefType, targetClassName);
		}
		else if (attrDomRefFundType == LIST_TYPE
			|| attrDomRefFundType == ARRAY_TYPE)
		{
			string attrIndvlName = aggrIndvl->GetName() + "_value";
			AddPropertyAssertionsForArrayList(attrIndvlName, subStr, attrDomRefType, targetClassName);
		}
		else
		{
			targetClassName = StrUtil::ToLower(attrDomRefType->Name());
		}

		PrimitiveType attrDomFundType = aggrTypeDes->FundamentalType();

		if (attrDomFundType == LIST_TYPE)
			aggrClassName = "list_of_" + targetClassName;
		else if (attrDomFundType == ARRAY_TYPE)
			aggrClassName = "array_of_" + targetClassName;

		aggrName = aggrClassName;

		aggrIndvl->AddClassType(m_ontology->GetClassByName(aggrName));

		string attrIndvlObjPropName = aggrName + "_has_content";
		ObjectProperty* attrIndvlObjProp = m_ontology->GetObjectPropertyByName(attrIndvlObjPropName);

		S2OThrowExceptionIf<S2OException>(attrIndvlObjProp == nullptr, "Invalid object property name: " + attrIndvlObjPropName);

		if (attrDomRefFundType == SET_TYPE
			|| attrDomRefFundType == BAG_TYPE)
		{
			string attrIndvlName = aggrIndvl->GetName() + "_value";
			AddObjectPropertyAssertion(aggrIndvl, attrIndvlObjProp, attrIndvlName);
		}
		else if (attrDomRefFundType == LIST_TYPE
			|| attrDomRefFundType == ARRAY_TYPE)
		{
			string attrIndvlName = aggrIndvl->GetName() + "_value_1";
			AddObjectPropertyAssertion(aggrIndvl, attrIndvlObjProp, attrIndvlName);
		}
		else
		{
			string attrIndvlName = aggrIndvl->GetName() + "_value";
			string attrDomTypeName = StrUtil::ToLower(attrDomRefType->Name());

			if (attrDomRefFundType == sdaiINSTANCE)
			{
				subStr = "i" + StrUtil::RemoveCharacter(subStr, "#");
				AddObjectPropertyAssertion(aggrIndvl, attrIndvlObjProp, subStr);
			}
			else if (attrDomRefFundType == sdaiSTRING)
			{
				AddObjectPropertyAssertion(aggrIndvl, attrIndvlObjProp, attrIndvlName);
				AddDataPropertyAssertion(m_ontology->GetIndividualByName(attrIndvlName), attrDomTypeName, "to_string", subStr);
			}
			else if (attrDomRefFundType == sdaiREAL
				|| attrDomRefFundType == sdaiNUMBER)
			{
				AddObjectPropertyAssertion(aggrIndvl, attrIndvlObjProp, attrIndvlName);
				AddDataPropertyAssertion(m_ontology->GetIndividualByName(attrIndvlName), attrDomTypeName, "to_decimal", subStr);
			}
			else if (attrDomRefFundType == sdaiINTEGER)
			{
				AddObjectPropertyAssertion(aggrIndvl, attrIndvlObjProp, attrIndvlName);
				AddDataPropertyAssertion(m_ontology->GetIndividualByName(attrIndvlName), attrDomTypeName, "to_integer", subStr);
			}
			else if (attrDomRefFundType == sdaiBOOLEAN)
			{
				AddObjectPropertyAssertion(aggrIndvl, attrIndvlObjProp, attrIndvlName);
				AddDataPropertyAssertion(m_ontology->GetIndividualByName(attrIndvlName), attrDomTypeName, "to_boolean", subStr);
			}
			else if (attrDomRefFundType == sdaiBINARY)
			{
				AddObjectPropertyAssertion(aggrIndvl, attrIndvlObjProp, attrIndvlName);
				AddDataPropertyAssertion(m_ontology->GetIndividualByName(attrIndvlName), attrDomTypeName, "to_binary", subStr);
			}
			else if (attrDomRefFundType == sdaiLOGICAL)
			{
				if (StrUtil::Exist(subStr, "T"))
					subStr = "TRUE";
				else if (StrUtil::Exist(subStr, "F"))
					subStr = "FALSE";
				else
					subStr = "UNKNOWN";

				AddObjectPropertyAssertion(aggrIndvl, attrIndvlObjProp, subStr);
			}
			else if (attrDomRefFundType == sdaiENUMERATION)
			{
				if (subStr.empty())
					continue;

				attrIndvlName = StrUtil::ToLower(subStr);
				AddObjectPropertyAssertion(aggrIndvl, attrIndvlObjProp, attrIndvlName);
			}
			else if (attrDomRefFundType == sdaiSELECT)
			{
				AddPropertyAssertionForSelectFromString(aggrIndvl, attrIndvlObjProp, attrIndvlName, subStr);
			}
			else if (attrDomRefFundType == sdaiAGGR)
			{
				// if required
			}
		}

		attrIndvlObjPropName = aggrName + "_has_next";
		attrIndvlObjProp = m_ontology->GetObjectPropertyByName(attrIndvlObjPropName);

		S2OThrowExceptionIf<S2OException>(attrIndvlObjProp == nullptr, "Invalid object property name: " + attrIndvlObjPropName);

		if (i < aggrCnt - 1)
		{
			AddObjectPropertyAssertion(aggrIndvl, attrIndvlObjProp, aggrIndvlName + "_" + to_string(i + 2));
		}
		else // last
		{
			Individual* emptyIndvl = nullptr;
			m_ontology->AddIndividual(aggrIndvlName + "_last", emptyIndvl);

			AddObjectPropertyAssertion(aggrIndvl, attrIndvlObjProp, emptyIndvl->GetName());

			string emptyClsName = "empty" + aggrName;
			Class* emptyCls = m_ontology->GetClassByName(emptyClsName);
			
			S2OThrowExceptionIf<S2OException>(emptyCls == nullptr, "Invalid class name: " + emptyClsName);

			emptyIndvl->AddClassType(emptyCls);
		}
	}

	subStrs.clear();
}

void InstanceTranslator::AddPropertyAssertionForSelectFromString(Individual* indvl, ObjectProperty* objProp, string attrIndvlName, string str)
{
	// remove null at the end
	if (StrUtil::EndWith(str, string(1, '\0')))
		str = StrUtil::RemoveCharacter(str, string(1, '\0'));

	if (StrUtil::StartWith(str, "#")
		&& !StrUtil::Exist(str, "(")
		&& !StrUtil::Exist(str, ")")) // instance
	{
		str = "i" + StrUtil::RemoveCharacter(str, "#");
		AddObjectPropertyAssertion(indvl, objProp, str);
	}
	else if (StrUtil::Exist(str, "(")
		&& StrUtil::Exist(str, ")"))
	{
		int leftParenLoc = str.find("(");

		string typeName = StrUtil::ToLower(str.substr(0, leftParenLoc));
		string typeValue = StrUtil::GetStringBetweenParenthesis(str, leftParenLoc);

		Class* cls = m_ontology->GetClassByName(typeName);

		S2OThrowExceptionIf<S2OException>(cls == nullptr, "Invalid class name: " + typeName);

		string annoStr = cls->GetAnnotationAt(0).second;

		if (StrUtil::Equal(annoStr, "enumerationtype"))
		{
			typeValue = StrUtil::GetStringBetween(typeValue, ".", ".");
			typeValue = StrUtil::ToLower(typeValue);

			AddObjectPropertyAssertion(indvl, objProp, typeValue);
		}
		else if (StrUtil::Equal(annoStr, "datatype"))
		{
			// find a superclass that is of datatype
			for (int i = 0; i < cls->GetSuperClassSize(); ++i)
			{
				Class* supCls = cls->GetSuperClassAt(i);
				string superAnnoStr = supCls->GetAnnotationAt(0).second;

				if (StrUtil::Equal(supCls->GetName(), "attribute"))
					break;

				if (!StrUtil::Equal(annoStr, superAnnoStr))
					continue;

				cls = supCls;
				i = -1;
			}

			string clsName = cls->GetName();

			// check datatype
			if (StrUtil::Equal(clsName, "real")
				|| StrUtil::Equal(clsName, "number"))
			{
				AddObjectPropertyAssertion(indvl, objProp, attrIndvlName);
				AddDataPropertyAssertion(m_ontology->GetIndividualByName(attrIndvlName), typeName, "to_decimal", typeValue);
			}
			else if (StrUtil::Equal(clsName, "integer"))
			{
				AddObjectPropertyAssertion(indvl, objProp, attrIndvlName);
				AddDataPropertyAssertion(m_ontology->GetIndividualByName(attrIndvlName), typeName, "to_integer", typeValue);
			}
			else if (StrUtil::Equal(clsName, "string"))
			{
				AddObjectPropertyAssertion(indvl, objProp, attrIndvlName);
				AddDataPropertyAssertion(m_ontology->GetIndividualByName(attrIndvlName), typeName, "to_string", typeValue);
			}
			else if (StrUtil::Equal(clsName, "binary"))
			{
				AddObjectPropertyAssertion(indvl, objProp, attrIndvlName);
				AddDataPropertyAssertion(m_ontology->GetIndividualByName(attrIndvlName), typeName, "to_binary", typeValue);
			}
			else if (StrUtil::Equal(clsName, "boolean"))
			{
				AddObjectPropertyAssertion(indvl, objProp, attrIndvlName);
				AddDataPropertyAssertion(m_ontology->GetIndividualByName(attrIndvlName), typeName, "to_boolean", typeValue);
			}
			else if (StrUtil::Equal(clsName, "logical"))
			{
				if (StrUtil::Exist(typeValue, "T"))
					typeValue = "TRUE";
				else if (StrUtil::Exist(typeValue, "F"))
					typeValue = "FALSE";
				else
					typeValue = "UNKNOWN";

				AddObjectPropertyAssertion(indvl, objProp, typeValue);
			}
			else
			{
				// if required
			}
		}
	}
	else
	{
		// if required
	}
}



// For simplified geometry representation
void InstanceTranslator::AddDataPropertyAssertionsForCartesianPointAndDirection(Individual* indvl, string str)
{
	vector<string> subStrs;
	StrUtil::SplitStringByCommaInParentheses(str, subStrs);

	string classTypeName = indvl->GetClassTypeAt(0)->GetName();

	string x_coord = subStrs[0];
	AddDataPropertyAssertion(indvl, classTypeName, "has_x", x_coord);

	string y_coord = subStrs[1];
	AddDataPropertyAssertion(indvl, classTypeName, "has_y", y_coord);

	string z_coord = subStrs[2];
	AddDataPropertyAssertion(indvl, classTypeName, "has_z", z_coord);
}

void InstanceTranslator::AddPropertyAssertionsForCoordinatesList(Individual* indvl, string aggrIndvlName, string str)
{
	ObjectProperty* indvlObjProp = m_ontology->GetObjectPropertyByName("coordinates_list_has_position_coords");
	AddObjectPropertyAssertion(indvl, indvlObjProp, aggrIndvlName + "_" + to_string((int)1));

	string targetClassName = "cartesian_point";
	string aggrClassName = "list_of_" + targetClassName;

	vector<string> subStrs;
	StrUtil::SplitStringByCommaInParentheses(str, subStrs);

	int aggrCnt = (int)subStrs.size();

	for (int i = 0; i < aggrCnt; ++i)
	{
		Individual* aggrIndvl = nullptr;
		m_ontology->AddIndividual(aggrIndvlName + "_" + to_string(i + 1), aggrIndvl);
		aggrIndvl->AddClassType(m_ontology->GetClassByName(aggrClassName));

		string subStr = subStrs[i];
		if (StrUtil::Exist(subStr, "\n"))
			int a = 0;

		string attrIndvlName = aggrIndvl->GetName() + "_value";

		Individual* attrIndvl = nullptr;
		m_ontology->AddIndividual(attrIndvlName, attrIndvl);

		attrIndvl->AddClassType(m_ontology->GetClassByName(targetClassName));

		AddDataPropertyAssertionsForCartesianPointAndDirection(attrIndvl, subStr);

		string attrIndvlObjPropName = aggrClassName + "_has_content";
		ObjectProperty* attrIndvlObjProp = m_ontology->GetObjectPropertyByName(attrIndvlObjPropName);

		AddObjectPropertyAssertion(aggrIndvl, attrIndvlObjProp, attrIndvlName);

		attrIndvlObjPropName = aggrClassName + "_has_next";
		attrIndvlObjProp = m_ontology->GetObjectPropertyByName(attrIndvlObjPropName);

		if (i < aggrCnt - 1)
		{
			AddObjectPropertyAssertion(aggrIndvl, attrIndvlObjProp, aggrIndvlName + "_" + to_string(i + 2));
		}
		else // last
		{
			Individual* emptyIndvl = nullptr;
			m_ontology->AddIndividual(aggrIndvlName + "_last", emptyIndvl);

			AddObjectPropertyAssertion(aggrIndvl, attrIndvlObjProp, emptyIndvl->GetName());

			Class* emptyCls = m_ontology->GetClassByName("empty" + aggrClassName);
			emptyIndvl->AddClassType(emptyCls);
		}
	}
}

void InstanceTranslator::InitializeSimpleGeomtryMap(void)
{
	m_simpleGeometryMap.insert({ "cartesian_point" , 1 });
	m_simpleGeometryMap.insert({ "direction" , 1 });
	m_simpleGeometryMap.insert({ "coordinates_list" , 1 });
}
