#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 CarModel;
uniform vec3 object_color;
uniform float scaleFactor;

// Output
out vec3 frag_normal;
out vec3 frag_color;
out vec2 tex_coord;


void main()
{
    frag_normal = v_normal;
    frag_color = v_color;
    tex_coord = v_texture_coord;

    vec4 world_v_position = Model * vec4(v_position, 1.0);
    vec4 world_car_position = CarModel * vec4(0.0, 0.0, 0.0, 1.0);  // Masina este mereu initial la (0, 0, 0)
    vec3 new_position = world_v_position.xyz;
    new_position.y = world_v_position.y - distance(world_car_position.xyz, world_v_position.xyz) * distance(world_car_position.xyz, world_v_position.xyz) * scaleFactor;

    gl_Position = Projection * View * vec4(new_position, 1.0);
}
