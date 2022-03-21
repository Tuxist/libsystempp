/*******************************************************************************
 * Copyright (c) 2022, Jan Koester jan.koester@gmx.net
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 * Neither the name of the <organization> nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************/

#pragma once

#define LC_GLOBAL_LOCALE	((locale_t) -1L)
#define LC_CTYPE	0
#define LC_NUMERIC	1
#define LC_TELEPHONE	10
#define LC_MEASUREMENT	11
#define LC_IDENTIFICATION	12
#define LC_TIME	2
#define LC_COLLATE	3
#define LC_MONETARY	4
#define LC_MESSAGES	5
#define LC_ALL	6
#define LC_PAPER	7
#define LC_NAME	8
#define LC_ADDRESS	9

typedef void* __locale_t; 
typedef __locale_t locale_t; 

#ifdef __cplusplus
extern "C" {
#endif
    struct lconv {
        char    *currency_symbol;
        char    *decimal_point;
        char     frac_digits;
        char    *grouping;
        char    *int_curr_symbol;
        char     int_frac_digits;
        char     int_n_cs_precedes;
        char     int_n_sep_by_space;
        char     int_n_sign_posn;
        char     int_p_cs_precedes;
        char     int_p_sep_by_space;
        char     int_p_sign_posn;
        char    *mon_decimal_point;
        char    *mon_grouping;
        char    *mon_thousands_sep;
        char    *negative_sign;
        char     n_cs_precedes;
        char     n_sep_by_space;
        char     n_sign_posn;
        char    *positive_sign;
        char     p_cs_precedes;
        char     p_sep_by_space;
        char     p_sign_posn;
        char    *thousands_sep;
    };
    
    locale_t      duplocale(locale_t);
    void          freelocale(locale_t);
    struct lconv *localeconv(void);
    locale_t      newlocale(int, const char *, locale_t);
    char         *setlocale(int, const char *);
    locale_t      uselocale (locale_t);
    
#ifdef __cplusplus
}
#endif
