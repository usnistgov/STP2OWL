#pragma once

class AnnotationProperty;

typedef pair<AnnotationProperty*, string> Annotation;

class Object
{
public:
	Object();
	Object(const string& name);
	virtual ~Object();

	void SetName(const string& name) { m_name = name; }
	const string& GetName() const { return m_name; }

	void AddAnnotation(Annotation& anno) { m_annotations.emplace_back(anno); }
	const int GetAnnotationSize() const { return (int)m_annotations.size(); }
	Annotation GetAnnotationAt(const int& index) const { return m_annotations[index]; }
	
	const string GetAnnotationStringByAnnotationProperty(AnnotationProperty* annoProp) const;

	const string& GetPrefix() const { return m_prefix; }
	void SetPrefix(const string& prefix) { m_prefix = prefix; }

protected:
	virtual void Clear();

private:
	string m_name;
	string m_prefix;

	vector<Annotation> m_annotations;
};