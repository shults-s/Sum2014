/* output matrix function
 * *Messege - Mesege box Title
 * M - matrix for output 
 */
void PrintMatrix (CHAR *Messege, MATRIX M)
{
  /* output Messege bilt buffer */
  char Buf[300];
  /* output Messege bilt */
  sprintf( Buf, " / %6.3lf %6.3lf %6.3lf %6.3lf \\\n |%6.3lf %6.3lf %6.3lf %6.3lf  |\n|  %6.3lf %6.3lf %6.3lf %6.3lf  |\n\\ %6.3lf %6.3lf %6.3lf %6.3lf /\n", M.A[0][0], M.A[0][1], M.A[0][2], M.A[0][3], M.A[1][0], M.A[1][1], M.A[1][2], M.A[1][3], M.A[2][0], M.A[2][1], M.A[2][2], M.A[2][3], M.A[3][0], M.A[3][1], M.A[3][2], M.A[3][3]);
  /* Messege output */
  MessageBox( NULL, Buf, Messege, MB_OK | MB_ICONINFORMATION); 
}