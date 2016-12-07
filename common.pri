CONFIG += c++14

unix {
    # RPATH is set here for convenience to run the programs in the shadow build
    # without installing the shared lib or setting LD_LIBRARY_PATH.
    QMAKE_RPATHDIR += $${top_builddir}/libtrouble/
}

DEFINES += TOPSRCDIR=\\\"\"$${top_srcdir}\"\\\"

