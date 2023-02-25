#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform vec3 light_direction2;
uniform vec3 light_position2;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// TODO(student): Declare any other uniforms
uniform int spotL;

uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    vec3 L = normalize(light_position - world_position);
    vec3 V = normalize( eye_position - world_position );
    vec3 H = normalize( L + V );

    vec3 L2 = normalize(light_position2 - world_position);
    vec3 H2 = normalize( L2 + V );

    // TODO(student): Define ambient, diffuse and specular light components
    float ambient_light = 0.25 * material_kd;
    
    float diffuse_light = material_kd * 1 * max (dot (world_normal, L), 0);

    float diffuse_light2 = material_kd * 1 * max (dot (world_normal, L2), 0);

    vec3 R = reflect (-L, world_normal);
    float specular_light = material_ks * 1 * ((dot (world_normal, L) > 0) ? 1 : 0) * pow(max(dot(V, R), 0), material_shininess);

    vec3 R2 = reflect (-L2, world_normal);
    float specular_light2 = material_ks * 1 * ((dot (world_normal, L2) > 0) ? 1 : 0) * pow(max(dot(V, R2), 0), material_shininess);

    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Phong (1975) shading method. Don't mix them up!
    if (diffuse_light > 0)
    {
    }

    // TODO(student): If (and only if) the light is a spotlight, we need to do
    // some additional things.
    float dist = distance(world_position,light_position);
    float factorDeAtenuare = 1 / pow(dist, 2);

    float dist2 = distance(world_position,light_position2);
    float factorDeAtenuare2 = 1 / pow(dist2, 2);

    vec3 color;
    if (spotL == 1) {
        float cut_off = radians(30.0);
        float spot_light = dot(-L, light_direction);
        float spot_light_limit = cos(cut_off);

        if (spot_light > cos(cut_off)) {
            float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
            float light_att_factor = pow(linear_att, 2);
            color = object_color + ambient_light + factorDeAtenuare * light_att_factor * (diffuse_light + specular_light);
        }
    }
    else {
        color = object_color + ambient_light + factorDeAtenuare * (diffuse_light + specular_light);
    }

    vec3 color2;
    if (spotL == 1) {
        float cut_off = radians(30.0);
        float spot_light = dot(-L2, light_direction2);
        float spot_light_limit = cos(cut_off);

        if (spot_light > cos(cut_off)) {
            float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
            float light_att_factor = pow(linear_att, 2);
            color2 = object_color + ambient_light + factorDeAtenuare2 * light_att_factor * (diffuse_light2 + specular_light2);
        }
    }
    else {
        color2 = object_color + ambient_light + factorDeAtenuare2 * (diffuse_light2 + specular_light2);
    }


    // TODO(student): Compute the total light. You can just add the components
    // together, but if you're feeling extra fancy, you can add individual
    // colors to the light components. To do that, pick some vec3 colors that
    // you like, and multiply them with the respective light components.

    // TODO(student): Write pixel out color
    out_color = vec4(color + color2, 1);

}
