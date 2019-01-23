#pragma once

#include <string>

namespace bling { namespace utils { namespace patterns {
    typedef std::string EventType;

    struct Event
    {
        virtual ~Event(){;}
        EventType m_name;
    };
}}}