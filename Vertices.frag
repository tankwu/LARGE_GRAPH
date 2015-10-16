#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

varying vec2 fPosition;
varying vec4 fColor;
varying vec2 fCenter;
varying float fRadius;


void main()
{
    // Set fragment color from texture
    gl_FragColor = fColor;
    float distance = sqrt(((fPosition.x - fCenter.x) *
                           (fPosition.x - fCenter.x)) +
                          ((fPosition.y - fCenter.y) *
                           (fPosition.y - fCenter.y)));
    if (distance >= fRadius) {
        gl_FragColor[0] = 1.0;
        gl_FragColor[1] = 1.0;
        gl_FragColor[2] = 1.0;
        gl_FragColor[3] = 0.0;
    }
}
