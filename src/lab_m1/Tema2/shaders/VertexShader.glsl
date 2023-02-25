#version 330

// Input
// Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_text_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 CarModel;
uniform vec3 object_color;
uniform float scaleFactor;

// Output
// TODO(student): Output values to fragment shader
out vec3 frag_color;

void main()
{
    // Send output to fragment shader
    if (object_color == vec3(0, 0, 0))
        frag_color = v_color;
    else
        frag_color = object_color;

    // Compute gl_Position
    vec4 world_v_position = Model * vec4(v_position, 1.0);
    vec4 world_car_position = CarModel * vec4(0.0, 0.0, 0.0, 1.0);  // Masina este mereu initial la (0, 0, 0)
    vec3 new_position = world_v_position.xyz;
    new_position.y = world_v_position.y - distance(world_car_position.xyz, world_v_position.xyz) * distance(world_car_position.xyz, world_v_position.xyz) * scaleFactor;

    gl_Position = Projection * View * vec4(new_position, 1.0);

}
