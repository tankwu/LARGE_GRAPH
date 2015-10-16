#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

attribute vec4 pPosition;

void main()
{
    // Calculate vertex position in screen space
    gl_Position = pPosition;
}
