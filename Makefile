include config.mk

SRCDIR = src
CNFDIR = config
INCDIR = include

IMGDIR = ${BUILDROOT}/img
OBJDIR = ${BUILDROOT}/obj
BINDIR = ${IMGDIR}/bin

EXE = ${BINDIR}/${EXENAME}
SRC = ${shell find ${SRCDIR} -type f -name '*.cc'}
OBJ = ${patsubst ${SRCDIR}/%, ${OBJDIR}/%.o, ${SRC}}

_CXXFLAGS = \
		   -std=c++20 \
		   -DEXENAME=\"${EXENAME}\" -DVERSION=\"${VERSION}\" \
		   ${CXXFLAGS} -I${INCDIR} -I${CNFDIR}

_LDFLAGS = \
		   ${LDFLAGS}

.PHONY: \
	all     \
	clean   \
	install \
	lines   \
	options \
	rebuild

all: ${EXE}

options:
	@${CXX} --version >&2
	@echo "CXXFLAGS = $$(printf "%s " ${_CXXFLAGS})" >&2
	@echo "LDFLAGS  = $$(printf "%s " ${_LDFLAGS})"  >&2

rebuild: clean all

${CNFDIR}/%: ${CNFDIR}/%.def
	@echo "[def] $<"
	@cp $< $@

${EXE}: ${OBJ}
	@\
	if [ ! -d $$(dirname $@) ]; then     \
	    echo "[dir] $$(dirname $@)" >&2; \
	    mkdir -p $$(dirname $@);         \
	fi
	@echo "[lnk] $@" >&2
	@${CXX} ${_LDFLAGS} -o $@ ${OBJ}

${SRCDIR}/suschad.cc: ${CNFDIR}/events

${OBJDIR}/%.cc.o: ${SRCDIR}/%.cc
	@scripts/compile ${CXX} ${SRCDIR} ${OBJDIR} $< ${_CXXFLAGS}

lines:
	@scripts/lines ${SRCDIR} ${INCDIR}

clean:
	@ \
	if [ -d "${BUILDROOT}" ]; then              \
	    echo "[del] ${BUILDROOT}" >&2;          \
	    rm -rf ${BUILDROOT};                    \
	    for f in $$(find . -name '*.plist'); do \
	        echo "[del] $$f";                   \
	        rm -f $$f;                          \
	    done;                                   \
	fi

install:
	@scripts/install ${IMGDIR} ${INSTALLPREFIX}
