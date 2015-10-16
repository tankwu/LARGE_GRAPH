#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

/* Update Subdivision Point Position */

//varying vec2 fTexCoord;
//uniform sampler2D forceTexture;

uniform sampler2D   posTexture;

uniform float       S;
uniform float       P;
uniform float       EPS;
uniform float       ATTRACTION;
uniform float       ELECTRO;

varying vec2        fPosition;
varying float       scale_x;
varying float       scale_y;
varying float       pixelWidth;
varying float       pixelHeight;

//varying     float test;

void calcDistance(in vec3 p1, in vec3 p2, out float result)
{
    float x = p1.x - p2.x;
    float y = p1.y - p2.y;
    float z = p1.z - p2.z;
    result = sqrt(x * x + y * y + z * z);
}

void calAttraction(in vec3 A, in vec3 B, out vec3 Force, in float kp)
{
    // Hooke's Law: F = -kx
    float force = kp;
    vec3 v_force;
    v_force.x = B.x - A.x;
    v_force.y = B.y - A.y;
    v_force.z = B.z - A.z;
    //norVector(v_force);

    if (abs(B.x - A.x) < EPS || abs(B.y - A.y) < EPS) {
        Force.x = 0.0;
        Force.y = 0.0;
        Force.z = 0.0;
    } else {
        Force.x = force * v_force.x;
        Force.y = force * v_force.y;
        Force.z = force * v_force.z;
    }
    //qDebug() << "attrction force: " << force;
}

void calElectrostatic(in vec3 A, in vec3 B, out vec3 Force,
                      in float particleLength) {
    // Coulomb's Law: F = k(Qq/r^2)
    float force = (ELECTRO / pow(particleLength, 1));
    vec3 v_force;
    v_force.x = B.x - A.x;
    v_force.y = B.y - A.y;
    v_force.z = B.z - A.z;
    //norVector(v_force);

    if (abs(B.x - A.x) < EPS || abs(B.y - A.y) < EPS) {
        Force.x = 0.0;
        Force.y = 0.0;
        Force.z = 0.0;
    } else {
        Force.x = force * v_force.x;
        Force.y = force * v_force.y;
        Force.z = force * v_force.z;
    }
    //qDebug() << "electrostatic force: " << force;
}

void addForce(in vec3 p1, in vec3 p2, out vec3 Force) {
    float xForce = p1.x + p2.x;
    float yForce = p1.y + p2.y;
    float zForce = p1.z + p2.z;
    Force.x = xForce;
    Force.y = yForce;
    Force.z = zForce;
}

void updateForcePosition(in float x, in float y, in float kp, out vec3 Force)
{
    float thres = 1.0e-5;

    vec3 attr_left_force;
    vec3 attr_right_force;
    vec3 attr_force;

    vec3 electro_force_acc;
    vec3 electro_force;
    vec3 total;

    float current_x = x;
    float current_y = y;
    float previous_x = x;
    float previous_y = y;
    float next_x = x;
    float next_y = y;

    if (x > 0.0) {
        previous_x = x - scale_x;
    }
    if (y > 0.0) {
        previous_y = y - scale_y;
    }
    if (x < 1.0) {
        next_x = x + scale_x;
    }
    if (y < 1.0) {
        next_y = y + scale_y;
    }

    vec3 current_position = texture2D(posTexture, vec2(x, y)).rgb;
    vec3 previous_position_H = texture2D(posTexture, vec2(previous_x, y)).rgb;
    vec3 next_position_H = texture2D(posTexture, vec2(next_x, y)).rgb;
    vec3 electro_position_V;

    calAttraction(current_position, previous_position_H, attr_left_force, kp);

    calAttraction(current_position, next_position_H, attr_right_force, kp);

    addForce(attr_left_force, attr_right_force, attr_force);

    /*
    for (float k = 0.0; k <= 1.0; k += scale_y) {
        if (y != k) {
            float result;
            electro_position_V = texture2D(posTexture, vec2(x, k)).rgb;
            calcDistance(current_position, electro_position_V, result);
            calElectrostatic(current_position, electro_position_V,
                             electro_force_acc, result);

            electro_force.x += electro_force_acc.x;
            electro_force.y += electro_force_acc.y;
            electro_force.z += electro_force_acc.z;
        }
    }
    */

    for (int k = 1; k < pixelHeight; k++) {
        next_y = y + k * scale_y;
        if (next_y <= 1.0) {
            float result;
            electro_position_V = texture2D(posTexture, vec2(x, next_y)).rgb;
            calcDistance(current_position, electro_position_V, result);
            calElectrostatic(current_position, electro_position_V,
                             electro_force_acc, result);

            electro_force.x += electro_force_acc.x;
            electro_force.y += electro_force_acc.y;
            electro_force.z += electro_force_acc.z;
        } else {
            break;
        }
    }

    for (int k = 1; k < pixelHeight; k++) {
        previous_y = y - k * scale_y;
        if (previous_y >= 0.0) {
            float result;
            electro_position_V = texture2D(posTexture, vec2(x, previous_y)).rgb;
            calcDistance(current_position, electro_position_V, result);
            calElectrostatic(current_position, electro_position_V,
                             electro_force_acc, result);

            electro_force.x += electro_force_acc.x;
            electro_force.y += electro_force_acc.y;
            electro_force.z += electro_force_acc.z;
        } else {
            break;
        }
    }

    addForce(attr_force, electro_force, total);

    Force.x = total.x;
    Force.y = total.y;
    Force.z = total.z;
}

void updateConstants(in float x, in float y, in int P, out float kp)
{
    float length;
    float first = 0.0;
    float last = (P + 1) * scale_x;

    vec3 first_rgb = texture2D(posTexture, vec2(first, y)).rgb;
    vec3 last_rgb = texture2D(posTexture, vec2(last, y)).rgb;

    calcDistance(first_rgb, last_rgb, length);

    kp = ATTRACTION / (length * (P + 1));
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
    float constant;

    int count;

    if (x >= size) {
        gl_FragColor.r = 0.0;
        gl_FragColor.g = 0.0;
        gl_FragColor.b = 0.0;
        gl_FragColor.a = 1.0;
    } else if (x == 0.0 || x == last) {
        vec3 current_position_rgb = texture2D(posTexture,
                                              vec2(current_x, current_y)).rgb;
        gl_FragColor.r = current_position_rgb.r;
        gl_FragColor.g = current_position_rgb.g;
        gl_FragColor.b = current_position_rgb.b;
        gl_FragColor.a = 1.0;
    } else {
        updateConstants(x, y, P, kp);

        updateForcePosition(x, y, kp, Force);

        float position_x = texture2D(posTexture, vec2(x, y)).r;
        float position_y = texture2D(posTexture, vec2(x, y)).g;
        float position_z = texture2D(posTexture, vec2(x, y)).b;
        float position_a = texture2D(posTexture, vec2(x, y)).a;

        gl_FragColor.r = position_x + S * Force.x;
        gl_FragColor.g = position_y + S * Force.y;
        gl_FragColor.b = position_z + S * Force.z;
        gl_FragColor.a = position_a;

    }

//    vec3 current_rgb = texture2D(posTexture, vec2(current_x, current_y)).rgb;
//    gl_FragColor = vec4(current_rgb.r, current_rgb.g, current_rgb.b, 1.0);
//    gl_FragColor = vec4(size, last, P, 1.0);
//    gl_FragColor = vec4(Force.x, Force.y, Force.z, 1.0);
}
