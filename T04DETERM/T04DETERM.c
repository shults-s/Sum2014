/* SS3 03.06.14 */

#include <stdio.h>
#include <windows.h>

#define MAX 10
double A[MAX][MAX], GlobalDeterminant = 0;
int P[MAX], Par, N;

//INT P[N] = {1,2,3,4,5}, Count = 0, Par = 0;

/*VOID Write()
{
  FILE *f;
  int i;

  if ((f = fopen("E:\\spr03\\a.log", "a")) != NULL)
  {
    Count++;
    fprintf(f, "%3d: ", Count);
    for(i = 0; i < N; i++)
      fprintf(f, "%i", P[i]);
    fprintf(f, " - %i", Par);
    fprintf(f, "\n");
    fclose(f);
  }
}*/

VOID Load( char *FileName )
{
  FILE *f;
  INT i, j;

  N = 0;

  f = fopen(FileName, "r");
  if (f == NULL)
    return;

  fscanf(f, "%i", &N);

  if (N < 0)
    N = 0;
  else
    if (N > MAX)
      N = MAX;

  for(i = 0; i < N; i++)
    for(j = 0; j < N; j++)
      fscanf(f, "%lf", &A[i][j]);
}

VOID Swap(int *n, int *m)
{
  int tmp;

  tmp = *n;
  *n = *m;
  *m = tmp;
}

VOID Go ( INT Pos )
{
  int i = 0;

  if (Pos == N)
  {
    double prod = 1;
    for(i = 0; i < N; i++)
      prod *= A[i][P[i]];

    if (!Par)
      GlobalDeterminant += prod;
    else
      GlobalDeterminant -= prod;

    return ;
  }
  else
  {
    Go(Pos + 1);
    for(i = Pos + 1; i < N; i++)
    {
      Swap(&P[Pos], &P[i]);
      Par = !Par;
      Go(Pos + 1);
      Swap(&P[Pos], &P[i]);
      Par = !Par;
    }
  }

}

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  FILE *f;
  int i;

  for(i = 0; i < MAX; i++)
    P[i] = i;

  Load("load.txt");

  Go(0);

  if ((f = fopen("E:\\spr03\\a.log", "w")) != NULL)
  {
    fprintf(f, "GlobalDeterminant = %lf", GlobalDeterminant);
    fclose(f);
  }

  return 0;
}