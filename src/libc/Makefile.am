include ../Make.vars

lib_LTLIBRARIES = libopenflow.la

AM_CPPFLAGS += $(OPENFLOW_CPPFLAGS)
AM_CPPFLAGS += -I$(top_srcdir)/src/nox

libopenflow_la_SOURCES = \
	libc/compiler.h \
	libc/dirs.c \
	libc/dirs.h \
	libc/dynamic-string.c \
	libc/dynamic-string.h \
	libc/fatal-signal.c \
	libc/fatal-signal.h \
	libc/fault.c \
	libc/fault.h \
	libc/hash.c \
	libc/hash.h \
	libc/hmap.c \
	libc/hmap.h \
	libc/ipv6_util.c \
	libc/ipv6_util.h \
	libc/list.c \
	libc/list.h \
	libc/oxm-match.c \
	libc/oxm-match.h \
	libc/ofpbuf.c \
	libc/ofpbuf.h \
	libc/packets.h \
	libc/sat-math.h \
	libc/tag.c \
	libc/tag.h \
	libc/timeval.c \
	libc/timeval.h \
	libc/type-props.h \
	libc/util.c \
	libc/util.h \
	libc/vlog-modules.def \
	libc/vlog.c \
	libc/vlog.h 

AM_CPPFLAGS += -xc++ -DOFL_LOG_VLOG_CC

libopenflow_la_LIBADD = ../oflib/liboflib.la 
