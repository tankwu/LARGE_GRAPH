#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

/* Update Subdivision Point Position */

//varying vec2 fTexCoord;
//uniform sampler2D forceTexture;

uniform sampler2D   posTexture;

uniform float       P;

varying vec2        fPosition;
varying float       scale_x;
varying float       scale_y;
varying float       pixelWidth;
varying float       pixelHeight;

void updateEdgeDivisions(in float x, in float y, out vec3 result)
{
    float current_x = x;
    float current_y = y;
    float previous_x = x;
    float previous_y = y;
    float next_x = x;
    float next_y = y;

    float old_size = ((P - 1) / 2) + 2 * scale_x;
    float new_size = P + 2 * scale_x;

    int point_id = x / scale_x;

    if (x == 0.0) {
        // source
        vec3 current_position = texture2D(posTexture, vec2(current_x,
                                                           current_y)).rgb;
        result.x = current_position.x;
        result.y = current_position.y;
        result.z = current_position.z;
    } else if (x == new_size - scale_x) {
        // target
        float old_x = old_size - scale_x;
        vec3 current_position = texture2D(posTexture, vec2(old_x,
                                                           current_y)).rgb;
        result.x = current_position.x;
        result.y = current_position.y;
        result.z = current_position.z;
    } else {
        if (point_id % 2 == 0) {
            int old_id = point_id / 2;
            float old_target_x = (point_id - old_id) * scale_x;
            vec3 current_position = texture2D(posTexture, vec2(old_target_x,
                                                               current_y)).rgb;
            result.x = current_position.x;
            result.y = current_position.y;
            result.z = current_position.z;
        } else {
            int old_id = point_id / 2;
            float old_next_x = (point_id - old_id) * scale_x;
            float old_previous_x = ((point_id - old_id) - 1) * scale_x;
            vec3 previous_position = texture2D(posTexture, vec2(old_previous_x,
                                                                current_y)).rgb;
            vec3 next_position = texture2D(posTexture, vec2(old_next_x,
                                                            current_y)).rgb;
            result.x = (previous_position.r + next_position.r) / 2.0;
            result.y = (previous_position.g + next_position.g) / 2.0;
            result.z = (previous_position.b + next_position.b) / 2.0;
        }
    }
}

void main()
{
    vec3 Force;

    float thres = 1.0e-4;

    float x = fPosition.x;
    float y = fPosition.y;

    float size = (P + 2) * scale_x;
    float last = (P + 1) * scale_x;

    for (int i = 0; i < pixelWidth; i++) {
        float input = 0.0 + i * scale_x;
        if (abs(x - input) < thres) {
            x = input;
            break;
        }
    }

    for (int i = 0; i < pixelHeight; i++) {
        float input = 0.0 + i * scale_y;
        if (abs(y - input) < thres) {
            y = input;
            break;
        }
    }

    float current_x = x;
    float current_y = y;
    float previous_x = x;
    float previous_y = y;
    float next_x = x;
    float next_y = y;

    float kp;
    float length;

    vec3 result;

    if (x >= size) {
        gl_FragColor.r = 0.0;
        gl_FragColor.g = 0.0;
        gl_FragColor.b = 0.0;
        gl_FragColor.a = 1.0;
    } else {
        updateEdgeDivisions(x, y, result);
        gl_FragColor.r = result.x;
        gl_FragColor.g = result.y;
        gl_FragColor.b = result.z;
        gl_FragColor.a = 1.0;
    }

//    gl_FragColor = vec4(x, y, 0.33, 1.0);
}
