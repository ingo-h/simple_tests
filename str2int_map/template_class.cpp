// Copyright (C) 2024+ GPL 3 and higher by Ingo Höft, <Ingo@Hoeft-online.de>
// Redistribution only with this Copyright remark. Last modified: 2024-10-19

#include "template_class.hpp"

template <typename T>
size_t CStrIntMap<T>::get_str_int(const char* name) {
    if (name == nullptr || name[0] == '\0')
        return -1;
    for (size_t i{0}; i < m_table.size(); i++) {
        if (strcmp(name, m_table[i].name) == 0)
            return i;
    }
    return -1;
}
