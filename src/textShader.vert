#version 120


//uniform mat4 modelViewProjectionMatrix;

attribute vec4 vPosition;
varying  vec2 texcoords;


void main() {

  gl_Position = gl_ModelViewProjectionMatrix*vec4(vPosition.xy, 0.0, 1.0);
  //gl_Position = *vec4(vPosition.xy, 0.0, 1.0);
  texcoords = vPosition.zw;

}
