/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#include"oPRCFile.h"
extern void nev_print_prc_mesh (prc::oPRCFile *, struct PRINT, struct TESS,
			    struct NODES, struct MESH *,
			    struct NODEDATA, struct MESHDATA *);

#ifdef __cplusplus
}
#endif