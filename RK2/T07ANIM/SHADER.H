/* FILENAME: SHADER.H
 * PROGRAMMER: RK2
 * PURPOSE: Shader handle declaration module.
 * LAST UPDATE: 13.06.2014
 */

#ifndef __SHADER_H_
#define __SHADER_H_

#include "def.h"

/* Shader program initialization function. */
UINT RK2_ShadProgInit( CHAR *VSFileName, CHAR *FSFileName );

/* Removing shader program function. */
VOID RK2_ShadProgClose( UINT ProgId );

#endif /* __SHADER_H_ */

/* END OF 'SHADER.H' FILE */
