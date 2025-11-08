#include <cache/share_string.h>

ShareString::ShareString() :
    m_string_reference{StringPool::acquire()}
{
    m_string_reference->data = "";
    m_string_reference->count = 1;

    m_start_index = 0;
    m_length = m_string_reference->data.length();
}

ShareString::ShareString(const std::string& data) :
    m_string_reference{StringPool::acquire()}
{
    m_string_reference->data = data;
    m_string_reference->count = 1;

    m_start_index = 0;
    m_length = m_string_reference->data.length();
}

ShareString::ShareString(std::string&& data) :
    m_string_reference{StringPool::acquire()}
{
    m_string_reference->data = std::move(data);
    m_string_reference->count = 1;

    m_start_index = 0;
    m_length = m_string_reference->data.length();
}

ShareString::ShareString(const ShareString& copy) :
    m_string_reference{copy.m_string_reference},
    m_start_index{copy.m_start_index},
    m_length{copy.m_length}
{
    m_string_reference->retain();
}

ShareString::ShareString(ShareString&& copy):
    m_string_reference{copy.m_string_reference},
    m_start_index{copy.m_start_index},
    m_length{copy.m_length}
{
    m_string_reference->retain();
}

ShareString& ShareString::operator=(const ShareString& copy)
{
    // Release current data if it exists
    check_release_current_data();

    m_string_reference = copy.m_string_reference;
    m_string_reference->retain();
    m_start_index = copy.m_start_index;
    m_length = copy.m_length;

    return *this;
}

ShareString& ShareString::operator=(ShareString&& copy)
{
    // Release current data if it exists
    check_release_current_data();

    m_string_reference = copy.m_string_reference;
    m_string_reference->retain();
    m_start_index = copy.m_start_index;
    m_length = copy.m_length;

    return *this;
}

ShareString ShareString::from_substr(size_t start_index, size_t length) const
{
    if (start_index + length <= m_length)
    {
        ShareString new_string = *this; // Copy current ShareString
        new_string.m_start_index = m_start_index + start_index;
        new_string.m_length = length;
        return new_string;
    }
    else
    {
        throw std::out_of_range("ShareString - Substr out of range, start_index: " + std::to_string(start_index) + ", length: " + std::to_string(length) + ", m_length: " + std::to_string(m_length));
    }
}

ShareString ShareString::from_substr_with_end(size_t start_index, size_t end_index) const
{
    if (start_index < m_length && end_index <= m_length && start_index < end_index)
    {
        ShareString new_string = *this; // Copy current ShareString
        new_string.m_start_index = m_start_index + start_index;
        new_string.m_length = end_index - start_index + 1; // Include the end index
        return new_string;
    }
    else
    {
        throw std::out_of_range("ShareString - Substr with end out of range, start_index: " + std::to_string(start_index) + ", end_index: " + std::to_string(end_index) + ", m_length: " + std::to_string(m_length));
    }
}

ShareString::~ShareString()
{
    check_release_current_data();
}

void ShareString::check_release_current_data()
{
    if (m_string_reference != nullptr)
    {
        if (m_string_reference->release() == 1)
        {
            StringPool::release(m_string_reference);
        }

        m_string_reference = nullptr;
    }
}