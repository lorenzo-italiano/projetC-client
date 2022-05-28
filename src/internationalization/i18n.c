#include "conf.h"

/**
 * Available lang :
 * 0 : fr
 * 1 : en
 */

#if LANG == 0
#include "i18n_fr.c"
#elif LANG == 1
#include "i18n_en.c"
#else
#include "i18n_fr.c"
#endif
