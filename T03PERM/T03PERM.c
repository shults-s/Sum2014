/* SS3 03.06.14 */

#include <stdio.h>
#include <windows.h>

#define N 5

INT P[N] = {1,2,3,4,5}, Count = 0, Par = 0;

VOID Write()
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
}

void Swap(int *n, int *m)
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
    Write();
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
  remove("a.log");

  Go(0);

  return 0;
}