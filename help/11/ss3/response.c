static VOID CubeUnitResponse( ss3UNIT_CUBE *Unit, ss3ANIM *Ani )
{
  SS3_RndWs = Ani->W;
  SS3_RndHs = Ani->H;
  SS3_RndWp = SS3_RndHp * Ani->W / Ani->H;
  SS3_RndMatrProjection = MatrProjection(-SS3_RndWp / 2, SS3_RndWp / 2, -SS3_RndHp / 2, SS3_RndHp / 2, SS3_RndProjDist, 1000.0);
}