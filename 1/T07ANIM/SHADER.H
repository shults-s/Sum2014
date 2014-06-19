/* FILENAME: SHADER.H
 * PROGRAMMER: EF2
 * PURPOSE: Shader handle declaration module.
 * LAST UPDATE: 13.06.2014
 */

#ifndef __SHADER_H_
#define __SHADER_H_

#include "def.h"

/* Init shader program function */
UINT EF2_ShadProgInit( CHAR *VSFileName, CHAR *FSFileName );

/* Close shader program function */
VOID EF2_ShadProgClose( UINT ProgId );

#endif /* __SHADER_H_ */

/* END OF 'SHADER.H' FILE */
