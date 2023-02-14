#include "itoa.h"

char *itoa(int value, char *string, int radix)
{
    char *pcVar1;

    pcVar1 = ltoa(value, string, radix);
    return pcVar1;
}

char *ltoa(long value, char *string, int radix)
{
    char cVar1;
    char *pcVar2;
    int iVar3;
    char tmp[33];
    long i;
    int sign;
    char *sp;
    unsigned long v;
    char *tp;

    tp = tmp;
    if (string == (char *)0x0)
    {
        string = (char *)0x0;
    }
    else if ((radix < 0x25) && (1 < radix))
    {
        if ((radix == 10) && (value < 0))
        {
            sign = 1;
        }
        else
        {
            sign = 0;
        }
        v = value;
        if (sign != 0)
        {
            v = -value;
        }
        while ((v != 0 || (tp == tmp)))
        {
            iVar3 = v - radix * (v / radix);
            v = v / radix;
            cVar1 = iVar3;
            if (iVar3 < 10)
            {
                pcVar2 = tp + 1;
                *tp = cVar1 + '0';
                tp = pcVar2;
            }
            else
            {
                pcVar2 = tp + 1;
                *tp = cVar1 + 'W';
                tp = pcVar2;
            }
        }
        sp = string;
        if (sign != 0)
        {
            sp = string + 1;
            *string = '-';
        }
        while (tmp < tp)
        {
            tp = tp + -1;
            *sp = *tp;
            sp = sp + 1;
        }
        *sp = '\0';
    }
    else
    {
        string = (char *)0x0;
    }
    return string;
}

char *utoa(unsigned int value, char *string, int radix)
{
    char *pcVar1;

    pcVar1 = ultoa(value, string, radix);
    return pcVar1;
}

char *ultoa(unsigned long value, char *string, int radix)
{
    char cVar1;
    char *pcVar2;
    int iVar3;
    char tmp[33];
    long i;
    char *sp;
    unsigned long v;
    char *tp;

    tp = tmp;
    if (string == (char *)0x0)
    {
        string = (char *)0x0;
    }
    else if ((radix < 0x25) && (v = value, 1 < radix))
    {
        while ((v != 0 || (sp = string, tp == tmp)))
        {
            iVar3 = v - radix * (v / radix);
            v = v / radix;
            cVar1 = iVar3;
            if (iVar3 < 10)
            {
                pcVar2 = tp + 1;
                *tp = cVar1 + '0';
                tp = pcVar2;
            }
            else
            {
                pcVar2 = tp + 1;
                *tp = cVar1 + 'W';
                tp = pcVar2;
            }
        }
        while (tmp < tp)
        {
            tp = tp + -1;
            *sp = *tp;
            sp = sp + 1;
        }
        *sp = '\0';
    }
    else
    {
        string = (char *)0x0;
    }
    return string;
}
