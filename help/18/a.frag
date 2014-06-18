/* FILE NAME   : a.frag
 * PURPOSE     : Simple fragment shader.
 * PROGRAMMER  : VG4.
 * LAST UPDATE : 13.06.2014
 */

#version 430

/* Выходное значение цвета рисования */
out vec4 OutColor;

/* Глобальные данные */
uniform float Time;
uniform vec4 LightPosition;
uniform vec4 LightColor;
uniform vec3 ViewDir;

/* Материал */
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Phong;
uniform float Trans;

uniform float IsTextureUsed;

uniform vec4 UseColor;
uniform mat4 MatrWVP;
uniform mat4 MatrWorldInverseTranspose;
uniform mat4 MatrWorld;
uniform mat4 MatrView;

uniform vec4 CameraLoc;
uniform vec4 ViewPos;

uniform float PartNo;
uniform float NumOfParts;

/* Текстура */
uniform sampler2D DrawTexture;

/* Входные данные */
in vec4 DrawColor;
in vec3 DrawPos;
in vec2 DrawTexCoord;
in vec3 DrawNormal;
in vec4 CameraPos;

vec3 Illum( vec3 N )
{
  vec3 color, L, V;


  V = normalize(CameraLoc.xyz - DrawPos.xyz);
  L = normalize(vec3(1 + 5 * sin(Time + 3), 3, 1 + 4 * sin(Time * 1.1)));
  N = faceforward(N, V, N);

  /* Ambient */
  color = Ka + vec3(0.1, 0.1, 0.1) * 0;

  /* Diffuse */
  float nl = dot(N, L);
  if (nl > 0)
  {
    if (IsTextureUsed == 0)
    {
      vec4 texc = texture2D(DrawTexture, (DrawTexCoord.st));
      //texc = vec4(1, 1, 1, 1) * 0.5;
      color += texc.xyz * nl;
    }
    else
      color += Kd * nl;
  }

  /* Specular */
  vec3 R = reflect(V, N);
  float rl = dot(R, L);
  if (rl > 0)
    color += Ks * pow(dot(R, L), Phong);
  return color;
}


/* Main function */
void main( void )
{
  float r = PartNo  / (NumOfParts - 1);
  if (r > abs(sin(Time)))
    discard;
  OutColor = vec4(Illum(DrawNormal.xyz), Trans);
//  OutColor = vec4(normalize(CameraPos.xyz - DrawPos.xyz), 1);
} /* End of 'main' function */

/* End of 'a.frag' file */
