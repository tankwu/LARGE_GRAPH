#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

/* HBlurVertex&HorizontalShader.glsl */
attribute vec4 vPosition;
attribute vec2 vTexCoord;
 
varying vec2 v_texCoord;
varying vec2 v_blurTexCoords[14];
 
void main()
{
    float frequnce = 10;
    gl_Position = vPosition;
    vTexCoord = vTexCoord;
    v_blurTexCoords[ 0] = vTexCoord + vec2(-0.00028 * frequnce, 0.0)
                                    + vec2(0.0, -0.00028 * frequnce);
    v_blurTexCoords[ 1] = vTexCoord + vec2(-0.00024 * frequnce, 0.0)
                                    + vec2(0.0, -0.00024 * frequnce);
    v_blurTexCoords[ 2] = vTexCoord + vec2(-0.00020 * frequnce, 0.0)
                                    + vec2(0.0, -0.00020 * frequnce);
    v_blurTexCoords[ 3] = vTexCoord + vec2(-0.00016 * frequnce, 0.0)
                                    + vec2(0.0, -0.00016 * frequnce);
    v_blurTexCoords[ 4] = vTexCoord + vec2(-0.00012 * frequnce, 0.0)
                                    + vec2(0.0, -0.00012 * frequnce);
    v_blurTexCoords[ 5] = vTexCoord + vec2(-0.00008 * frequnce, 0.0)
                                    + vec2(0.0, -0.00008 * frequnce);
    v_blurTexCoords[ 6] = vTexCoord + vec2(-0.00004 * frequnce, 0.0)
                                    + vec2(0.0, -0.00004 * frequnce);
    v_blurTexCoords[ 7] = vTexCoord + vec2( 0.00004 * frequnce, 0.0)
                                    + vec2(0.0,  0.00004 * frequnce);
    v_blurTexCoords[ 8] = vTexCoord + vec2( 0.00008 * frequnce, 0.0)
                                    + vec2(0.0,  0.00008 * frequnce);
    v_blurTexCoords[ 9] = vTexCoord + vec2( 0.00012 * frequnce, 0.0)
                                    + vec2(0.0,  0.00012 * frequnce);
    v_blurTexCoords[10] = vTexCoord + vec2( 0.00016 * frequnce, 0.0)
                                    + vec2(0.0,  0.00016 * frequnce);
    v_blurTexCoords[11] = vTexCoord + vec2( 0.00020 * frequnce, 0.0)
                                    + vec2(0.0,  0.00020 * frequnce);
    v_blurTexCoords[12] = vTexCoord + vec2( 0.00024 * frequnce, 0.0)
                                    + vec2(0.0,  0.00024 * frequnce);
    v_blurTexCoords[13] = vTexCoord + vec2( 0.00028 * frequnce, 0.0)
                                    + vec2(0.0,  0.00028 * frequnce);
}

