
#if defined _inc_mikan_ini
    #undef _inc_mikan_ini
#endif

#if defined _inc_mikan_ini_included
    #endinput
#endif

#define _inc_mikan_ini_included

#if !defined _inc_a_samp
    #tryinclude <core>
    #tryinclude <file>
    #tryinclude <string>
    #tryinclude <console>
    
    #if !defined _core_included
        #error "This module needs at least core.inc available"
    #endif

    #if !defined _file_included
        #error "The module needs at least file.inc available"
    #endif

    #if !defined _string_included
        #error "This module needs at least string.inc available"
    #endif

    #if !defined _console_included
        #error "This module needs at least console.inc available"
    #endif
#endif

stock MINI:MINI_Open(const string:file[], filemode:mode)
{
    if (!fexist(file))
        return MINI:0;
    
    new 
        File:tmp = fopen(file, mode);

    if (tmp)
        return MINI:tmp;
    
    return MINI:0;
}

stock MINI_ReadInt(MINI:instance, const string:key[])
{
    if (!instance)
        return cellmin;

    if (MINI_FindKeyValue(instance, key, value))
    {
        if ('0' <= value <= '9')
            return (strval(value));
    }
    printf("Error: cannot find \"%s\" keys");
    return cellmin;
}

static MINI_FindKeyValue(MINI:instance, const string:key[], string:value[])
{
    if (!instance)
        return 0;
        
    new szTmp[256];
    while(fread(File:instance, szTmp, sizeof(szTmp)))
    {
        if (!MINI_FindKeys(key, szTmp))
        {
            break;
        }
        else
        {
            new
                equPos = strfind(szTmp, "=");
            
            strmid(value, equPos, endPos);
            return 1;
        }
    }
    return 0;
}

static MINI_FindKeys(const string:origKey[], const string:buff[])
{
    new 
        tmpPos = 0,
        keyLen = 0;

    // Ignore if found comments
    if (strfind(buff, ";") != -1) return 0;

    while(buff[tmpPos] != '=')
    {
        if (buff[tmpPos] == ' ')
        {
            tmpPos ++;
            continue; // skip if it's space
        }

        if (!('a' <= tolower(buff[tmpPos]) <= 'z' || '0' <= buff[tmpPos] <= '9'))
        {
            return 0; // return 0 if the key it's not alphanumeric
        }

        // if previous word contain space
        if (buff[tmpPos - 1] == ' ')
        {
            return 0; // return 0 if found, because it's invalid key brother.
        }

        tmpPos ++;
        keyLen ++;
    }

    // Extract the keys to be compare
    new szExtractedKeys[64];
    strmid(szExtractedKeys, buff, 0, tmpPos, keyLen + 1);
    //printf("Extracted Keys: %s", szExtractedKeys);

    return (!strcmp(szExtractedKeys, origKey, false, keyLen + 1));
}
