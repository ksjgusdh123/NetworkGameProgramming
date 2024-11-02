#include "Ref.h"

void CRef::AddRef()
{
    ++m_refCount;
}

int CRef::Release()
{
    --m_refCount;

    if (m_refCount <= 0)
    {
        delete this;
        return 0;
    }

    return m_refCount;
}

void CRef::Destroy()
{
    m_active = false;
}
