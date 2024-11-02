#pragma once

// Singleton macro
#define	DECLARE_SINGLE(Type)	\
private:\
	static Type*	m_inst;\
public:\
	static Type* GetInst()\
	{\
		if (!m_inst)\
			m_inst = new Type;\
		return m_inst;\
	}\
	static void DestroyInst()\
	{\
		if(m_inst)\
		{\
			delete m_inst;\
			m_inst = nullptr;\
		}\
	}\
private:\
	Type();\
	~Type();

#define	DEFINITION_SINGLE(Type)	Type* Type::m_inst = nullptr;