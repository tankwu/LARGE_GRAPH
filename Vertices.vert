#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

attribute vec4 vPosition;
attribute vec4 vColor;
attribute vec4 vCenter;
attribute float vRadius;

varying vec2 fPosition;
varying vec4 fColor;
varying vec2 fCenter;
varying float fRadius;

void main()
{
    // Calculate vertex position in screen space
    gl_Position = vPosition;

    fPosition = vec2(vPosition.x, vPosition.y);
    fColor = vColor;
    fCenter = vCenter;
    fRadius = vRadius;
}
