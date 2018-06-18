#All create stub library for nas-ndi
lib_LTLIBRARIES = libsai-0.9.6.la

COMMON_HARDEN_FLAGS=-fexceptions -fstack-protector-strong -fstack-protector-all -D_FORTIFY_SOURCE=2 -Wall -Wformat -Wformat-security -Werror
C_HARDEN_FLAGS=-Wimplicit-function-declaration
LD_HARDEN_FLAGS=-Wl,-z,defs -Wl,-z,now

libsai_0_9_6_la_SOURCES = src/sai.c
libsai_0_9_6_la_CFLAGS = -I$(top_srcdir)/inc $(COMMON_HARDEN_FLAGS) -fPIC
libsai_0_9_6_la_LDFLAGS = -version-info 1:0:0 -shared $(LD_HARDEN_FLAGS)

#All exported headers

include_HEADERS=inc/sai.h inc/saiacl.h  inc/saibridge.h inc/saibuffer.h  inc/saifcport.h inc/saifcswitch.h \
                inc/saifdb.h  inc/saihash.h  inc/saiipmc.h inc/saiipmcgroup.h \
                inc/sail2mc.h inc/sail2mcgroup.h \
                inc/sailag.h  inc/saimirror.h  inc/saineighbor.h  inc/sainexthop.h  inc/sainexthopgroup.h \
                inc/saiobject.h inc/saipolicer.h  inc/saiport.h  inc/saiqosmap.h  inc/saiqueue.h  \
                inc/sairoute.h inc/sairpfgroup.h inc/saisamplepacket.h  \
                inc/saischeduler.h  inc/saischedulergroup.h  inc/saistatus.h \
                inc/saistp.h inc/saiswitch.h  inc/saitunnel.h inc/saitypes.h inc/saiudf.h inc/saivlan.h inc/saiwred.h \
                inc/saihostif.h inc/saimcastfdb.h inc/saimpls.h inc/sairouterinterface.h \
                inc/saisegmentroute.h inc/saitam.h inc/saiuburst.h inc/saivirtualrouter.h

