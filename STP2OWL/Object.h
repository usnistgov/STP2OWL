#pragma once

class AnnotationProperty;

typedef pair<AnnotationProperty*, string> Annotation;

class Object
{
public:
	Object();
	Object(string name);
	~Object();

	void SetName(string name) { m_name = name; }
	string GetName() { return m_name; }

	void AddAnnotation(Annotation anno) { m_annotations.push_back(anno); }
	int GetAnnotationSize() { return (int)m_annotations.size(); }
	Annotation GetAnnotationAt(int index) { return m_annotations[index]; }
	
	string GetAnnotationStringByAnnotationProperty(AnnotationProperty* annoProp);

	string GetPrefix() { return m_prefix; }
	void SetPrefix(string prefix) { m_prefix = prefix; }

protected:
	virtual void Clear();

private:
	string m_name;
	string m_prefix;

	vector<Annotation> m_annotations;
};