/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* configuration manager to use */
#define CONFIG_MGR_MODULE_NAME "gsettingsmgr"

/* always defined to indicate that i18n is enabled */
#define ENABLE_NLS 1

/* The domain to use with gettext */
#define GETTEXT_PACKAGE "nemiver"

/* disables glib deprecation warnings */
/* #undef GLIB_DISABLE_DEPRECATION_WARNINGS */

/* gcc visibility support */
/* #undef HAS_GCC_VISIBILITY_SUPPORT */

/* libvtr 2.90 support */
/* #undef HAS_VTE_2_90 */

/* libvte 2.91 support */
#define HAS_VTE_2_91 1

/* Define to 1 if you have the `bind_textdomain_codeset' function. */
#define HAVE_BIND_TEXTDOMAIN_CODESET 1

/* define if the Boost library is available */
#define HAVE_BOOST /**/

/* define if the Boost::Test_Exec_Monitor library is available */
#define HAVE_BOOST_TEST_EXEC_MONITOR /**/

/* Define to 1 if you have the <boost/tr1/memory.hpp> header file. */
#define HAVE_BOOST_TR1_MEMORY_HPP 1

/* Define to 1 if you have the <boost/tr1/tuple.hpp> header file. */
#define HAVE_BOOST_TR1_TUPLE_HPP 1

/* Define to 1 if you have the <boost/tr1/unordered_map.hpp> header file. */
#define HAVE_BOOST_TR1_UNORDERED_MAP_HPP 1

/* define if the Boost::Unit_Test_Framework library is available */
#define HAVE_BOOST_UNIT_TEST_FRAMEWORK /**/

/* Define to 1 if you have the <boost/variant.hpp> header file. */
#define HAVE_BOOST_VARIANT_HPP 1

/* Define to 1 if you have the `dcgettext' function. */
#define HAVE_DCGETTEXT 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define if the GNU gettext() function is already present or preinstalled. */
#define HAVE_GETTEXT 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define if your <locale.h> file defines LC_MESSAGES. */
#define HAVE_LC_MESSAGES 1

/* Define to 1 if you have the <libutil.h> header file. */
/* #undef HAVE_LIBUTIL_H */

/* Define to 1 if you have the <locale.h> header file. */
#define HAVE_LOCALE_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <pty.h> header file. */
#define HAVE_PTY_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <tr1/memory> header file. */
#define HAVE_TR1_MEMORY 1

/* Define to 1 if you have the <tr1/tuple> header file. */
#define HAVE_TR1_TUPLE 1

/* Define to 1 if you have the <tr1/unordered_map> header file. */
#define HAVE_TR1_UNORDERED_MAP 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the <util.h> header file. */
/* #undef HAVE_UTIL_H */

/* Define to the sub-directory where libtool stores uninstalled libraries. */
#define LT_OBJDIR ".libs/"

/* enable debug messages */
#define NEMIVER_DEBUG 1

/* Name of package */
#define PACKAGE "nemiver"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "http://bugzilla.gnome.org/enter_bug.cgi?product=nemiver"

/* Define to the full name of this package. */
#define PACKAGE_NAME "nemiver"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "nemiver 0.9.6"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "nemiver"

/* Define to the home page for this package. */
#define PACKAGE_URL "https://wiki.gnome.org/Apps/Nemiver"

/* Define to the version of this package. */
#define PACKAGE_VERSION "0.9.6"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Enable extensions on AIX 3, Interix.  */
#ifndef _ALL_SOURCE
# define _ALL_SOURCE 1
#endif
/* Enable GNU extensions on systems that have them.  */
#ifndef _GNU_SOURCE
# define _GNU_SOURCE 1
#endif
/* Enable threading extensions on Solaris.  */
#ifndef _POSIX_PTHREAD_SEMANTICS
# define _POSIX_PTHREAD_SEMANTICS 1
#endif
/* Enable extensions on HP NonStop.  */
#ifndef _TANDEM_SOURCE
# define _TANDEM_SOURCE 1
#endif
/* Enable general extensions on Solaris.  */
#ifndef __EXTENSIONS__
# define __EXTENSIONS__ 1
#endif


/* Version number of package */
#define VERSION "0.9.6"

/* compile the dynamic layout */
#define WITH_DYNAMICLAYOUT 1

/* build with gsettings instead of gconf */
#define WITH_GSETTINGS 1

/* compile the the memory view widget */
#define WITH_MEMORYVIEW 1

/* compile the workbench */
#define WITH_WORKBENCH 1

/* Define to 1 if on MINIX. */
/* #undef _MINIX */

/* Define to 2 if the system does not provide POSIX.1 features except with
   this defined. */
/* #undef _POSIX_1_SOURCE */

/* Define to 1 if you need to in order for `stat' and other things to work. */
/* #undef _POSIX_SOURCE */
