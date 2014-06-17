/* FILE NAME   : a.frag
 * PURPOSE     : Simple fragment shader.
 * PROGRAMMER  : SS3.
 * LAST UPDATE : 17.06.2014
 */

#version 400

/* Выходное значение цвета рисования */
out vec4 OutColor;

// Материал
uniform float Trans;

// Текстура
uniform sampler2D DrawTexture;

// Входные данные
in vec2 DrawTexCoord;
in vec4 CameraPos;


void main( void )
{
  vec4 texc = texture2D(DrawTexture, DrawTexCoord.xy);


  if (CameraPos.z < -1)
    ;//discard;

  OutColor = vec4(texc.xyz, Trans);
}