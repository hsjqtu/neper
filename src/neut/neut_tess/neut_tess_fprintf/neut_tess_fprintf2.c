/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_fprintf_.h"

void
neut_tess_fprintf_head (struct TESS Tess, FILE * file)
{
  fprintf (file, "***tess\n");
  fprintf (file, " **format\n");
  fprintf (file, "   2.0\n");
  fprintf (file, " **general\n");
  fprintf (file, "   %d %s\n", Tess.Dim, Tess.Type);

  return;
}

void
neut_tess_fprintf_foot (FILE * file)
{
  fprintf (file, "***end\n");

  return;
}

void
neut_tess_fprintf_cell (struct TESS Tess, FILE * file)
{
  int i, j, qty;
  double *e = ol_e_alloc ();

  fprintf (file, " **cell\n");
  fprintf (file, "  %d\n", Tess.CellQty);

  if (Tess.CellId)
  {
    fprintf (file, "  *id\n");
    qty = 0;
    for (i = 1; i <= Tess.CellQty; i++)
      ut_print_wnc_int_header (file, Tess.CellId[i], &qty, 72, "  ");
    fprintf (file, "\n");
  }

  if (Tess.CellLamId)
  {
    fprintf (file, "  *lamid\n");
    qty = 0;
    for (i = 1; i <= Tess.CellQty; i++)
      ut_print_wnc_int_header (file, Tess.CellLamId[i], &qty, 72, "  ");
    fprintf (file, "\n");
  }

  if (Tess.CellModeId)
  {
    fprintf (file, "  *modeid\n");
    qty = 0;
    for (i = 1; i <= Tess.CellQty; i++)
      ut_print_wnc_int_header (file, Tess.CellModeId[i], &qty, 72, "  ");
    fprintf (file, "\n");
  }

  if (Tess.CellCrySym)
  {
    fprintf (file, "  *crysym\n");
    fprintf (file, "   %s\n", Tess.CellCrySym);
  }

  if (Tess.SeedCoo && Tess.SeedWeight)
  {
    fprintf (file, "  *seed\n");
    // do not change CellQty into SeedQty below, to handle properly the
    // periodic case.
    for (i = 1; i <= Tess.CellQty; i++)
    {
      fprintf (file, " %3d ", i);
      for (j = 0; j < 3; j++)
	fprintf (file, "%.12f ", Tess.SeedCoo[i][j]);
      fprintf (file, "%.12f\n", Tess.SeedWeight[i]);
    }
  }

  if (Tess.CellOri)
  {
    fprintf (file, "  *ori\n");
    fprintf (file, "   e\n");
    qty = 0;
    for (i = 1; i <= Tess.CellQty; i++)
    {
      fprintf (file, "   ");
      ol_q_e (Tess.CellOri[i], e);
      ol_e_fprintf (file, e, "%17.12f");
    }
  }

  ol_e_free (e);

  return;
}

void
neut_tess_fprintf_ver (struct TESS Tess, FILE * file)
{
  int i, j;

  fprintf (file, " **vertex\n");
  fprintf (file, " %d\n", Tess.VerQty);

  for (i = 1; i <= Tess.VerQty; i++)
  {
    fprintf (file, " %3d ", i);
    for (j = 0; j < 3; j++)
      fprintf (file, " %.12f", Tess.VerCoo[i][j]);
    fprintf (file, "     %d\n", Tess.VerState[i]);
  }

  return;
}

void
neut_tess_fprintf_edge (struct TESS Tess, FILE * file)
{
  int i;

  fprintf (file, " **edge\n");
  fprintf (file, " %d\n", Tess.EdgeQty);

  for (i = 1; i <= Tess.EdgeQty; i++)
  {
    fprintf (file, " %3d ", i);
    fprintf (file, " %d %d %d\n", Tess.EdgeVerNb[i][0],
	     Tess.EdgeVerNb[i][1], Tess.EdgeState[i]);
  }

  return;
}

void
neut_tess_fprintf_face (struct TESS Tess, FILE * file)
{
  int i, j;

  fprintf (file, " **face\n");
  fprintf (file, " %d\n", Tess.FaceQty);

  for (i = 1; i <= Tess.FaceQty; i++)
  {
    fprintf (file, " %3d", i);

    fprintf (file, " %d ", Tess.FaceVerQty[i]);
    ut_array_1d_int_fprintf (file, Tess.FaceVerNb[i] + 1, Tess.FaceVerQty[i],
			     "%d");

    fprintf (file, "     %d", Tess.FaceVerQty[i]);
    for (j = 1; j <= Tess.FaceVerQty[i]; j++)
      fprintf (file, " %d", Tess.FaceEdgeOri[i][j] * Tess.FaceEdgeNb[i][j]);
    fprintf (file, "\n");

    fprintf (file, "    ");
    ut_array_1d_fprintf (file, Tess.FaceEq[i], 4, "%.12f");

    fprintf (file, "    ");
    fprintf (file, " %d", Tess.FaceState[i]);
    fprintf (file, " %d ", Tess.FacePt[i]);
    ut_array_1d_fprintf (file, Tess.FacePtCoo[i], 3, "%.12f");
  }

  return;
}

void
neut_tess_fprintf_poly (struct TESS Tess, FILE * file)
{
  int i, j;

  fprintf (file, " **polyhedron\n");
  fprintf (file, " %d\n", Tess.PolyQty);

  for (i = 1; i <= Tess.PolyQty; i++)
  {
    fprintf (file, " %3d %d", i, Tess.PolyFaceQty[i]);
    for (j = 1; j <= Tess.PolyFaceQty[i]; j++)
      fprintf (file, " %d", Tess.PolyFaceOri[i][j] * Tess.PolyFaceNb[i][j]);
    fprintf (file, "\n");
  }

  return;
}

void
neut_tess_fprintf_dom (struct TESS Tess, FILE * file)
{
  int i, j;

  if (Tess.Level == 0)
    return;

  fprintf (file, " **domain\n");
  fprintf (file, "  *general\n");
  fprintf (file, "   %s\n", Tess.DomType);

  fprintf (file, "  *vertex\n");
  fprintf (file, "   %d\n", Tess.DomVerQty);

  for (i = 1; i <= Tess.DomVerQty; i++)
  {
    fprintf (file, " %3d ", i);
    for (j = 0; j < 3; j++)
      fprintf (file, " %.12f", Tess.DomVerCoo[i][j]);
    fprintf (file, " %s\n", Tess.DomVerLabel[i]);
    fprintf (file, "    %3d %d\n", 1, Tess.DomTessVerNb[i]);
  }

  if (Tess.Dim > 1)
  {
    fprintf (file, "  *edge\n");
    fprintf (file, "   %d\n", Tess.DomEdgeQty);

    for (i = 1; i <= Tess.DomEdgeQty; i++)
    {
      fprintf (file, " %3d %s\n", i, Tess.DomEdgeLabel[i]);
      fprintf (file, "   %3d ", Tess.DomTessEdgeQty[i]);
      ut_array_1d_int_fprintf (file, Tess.DomTessEdgeNb[i] + 1,
			       Tess.DomTessEdgeQty[i], "%d");
      fprintf (file, "     ");
      ut_array_1d_int_fprintf (file, Tess.DomEdgeVerNb[i], 2, "%d");
    }
  }

  if (Tess.Dim > 2)
  {
    fprintf (file, "  *face\n");
    fprintf (file, "   %d\n", Tess.DomFaceQty);

    for (i = 1; i <= Tess.DomFaceQty; i++)
    {
      fprintf (file, " %3d ", i);

      fprintf (file, "%d ", Tess.DomFaceVerQty[i]);
      ut_array_1d_int_fprintf (file, Tess.DomFaceVerNb[i] + 1,
			       Tess.DomFaceVerQty[i], "%d");
      fprintf (file, "     ");
      fprintf (file, "%d ", Tess.DomFaceVerQty[i]);
      ut_array_1d_int_fprintf (file, Tess.DomFaceEdgeNb[i] + 1,
			       Tess.DomFaceVerQty[i], "%d");

      fprintf (file, "     ");
      ut_array_1d_fprintf (file, Tess.DomFaceEq[i], 4, "%.12f");

      fprintf (file, "     %s\n", Tess.DomFaceLabel[i]);

      fprintf (file, "   %3d ", Tess.DomTessFaceQty[i]);
      ut_array_1d_int_fprintf (file, Tess.DomTessFaceNb[i] + 1,
			       Tess.DomTessFaceQty[i], "%d");
    }
  }

  return;
}

void
neut_tess_fprintf_per (struct TESS Tess, FILE * file)
{
  neut_tess_fprintf_per_gen (Tess, file);
  neut_tess_fprintf_per_seed (Tess, file);
  neut_tess_fprintf_per_ver (Tess, file);
  neut_tess_fprintf_per_edge (Tess, file);
  neut_tess_fprintf_per_face (Tess, file);

  return;
}

void
neut_tess_fprintf_scale (struct TESS Tess, FILE * file)
{
  neut_tess_fprintf_scale_gen (Tess, file);
  neut_tess_fprintf_scale_cellid (Tess, file);

  return;
}
