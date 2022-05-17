/**
 * Available lang :
 * 0 : fr
 * 1 : en
 */

#define LANG 1

#if LANG == 0
#include "i18n_fr.c"
#elif LANG == 1
#include "i18n_en.c"
#else
#include "i18n_fr.c"
#endif