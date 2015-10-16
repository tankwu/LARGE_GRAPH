#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

/* Update Subdivision Point Position */
attribute vec2  vPosition;

uniform float   fWidth;
uniform float   fHeight;

varying vec2    fPosition;
varying float   scale_x;
varying float   scale_y;
varying float   pixelWidth;
varying float   pixelHeight;

void main()
{
    scale_x   = 1.0 / (fWidth - 1.0);
    scale_y   = 1.0 / (fHeight - 1.0);

    float factor_x  = fWidth / (fWidth - 1.0);
    float factor_y  = fHeight / (fHeight - 1.0);

    pixelWidth  = fWidth;
    pixelHeight = fHeight;

    gl_Position =  vec4(vPosition.x , vPosition.y , 0.0, 1.0);

    // scale according to screen coord
    fPosition.x = vPosition.x * factor_x;
    fPosition.y = vPosition.y * factor_y;

    // map to texture coord
    fPosition.x = (fPosition.x + 1.0) * 0.5;
    fPosition.y = (fPosition.y + 1.0) * 0.5;
}
