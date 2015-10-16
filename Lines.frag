#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

void main()
{
    // Set fragment color from texture
    float alpha = 0.05;
//    gl_FragColor = vec4(1.0, 0.725, 0.784, alpha);
    gl_FragColor = vec4(1.0, 0.0, 0.0, alpha);
}
