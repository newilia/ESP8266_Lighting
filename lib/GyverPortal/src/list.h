#pragma once
// список функций автообновления

enum GPtype {
    T_CSTR,
    T_STRING,
    T_TIME,
    T_DATE,
    T_CHECK,
    T_BYTE,
    T_INT,
    T_FLOAT,
    T_COLOR,
    T_COLOR_INT,
    T_UNDEFINED
};

struct GPlist 
{
    ~GPlist() 
    {
        clear();
    }

    struct Entry
    {
        void* val = nullptr;
        char* name = nullptr;
        char* formName = nullptr;
        GPtype type = T_UNDEFINED;
        bool skip = false;
    };

    void init(int n) 
    {
        clear();
        entries = new Entry[n];
        am = n;
    }
    
    void add(void* data, const char* name, GPtype type) 
    {
        if (idx < am)
        {
            entries[idx].val = data;
            entries[idx].name = (char*)name;
            entries[idx].type = type;
            entries[idx].skip = true;
            idx++;
        }
    }
    
    void add(void* data, const char* form, const char* name, GPtype type) 
    {
        if (idx < am) 
        {
            entries[idx].val = data;
            entries[idx].formName = (char*)form;
            entries[idx].name = (char*)name;
            entries[idx].type = type;
            entries[idx].skip = false;
            idx++;
        }
    }

    void clear() 
    {
        delete entries;
        idx = am = 0;
    }
    
    int idx = 0;
    int am = 0;
    Entry * entries = nullptr;
};