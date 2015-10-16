#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

varying vec2 fTexCoord;
uniform sampler2D texture;

void main()
{
    float x = fTexCoord.x;
    float y = fTexCoord.y;

    gl_FragColor = texture2D(texture, vec2(x, y));
}
