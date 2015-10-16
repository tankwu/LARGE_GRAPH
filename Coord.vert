#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

attribute vec3 vPosition;
attribute  vec2 vTexCoord;
varying vec2 fTexCoord;

void main()
{
    fTexCoord = vTexCoord;
    gl_Position =  vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);
}

