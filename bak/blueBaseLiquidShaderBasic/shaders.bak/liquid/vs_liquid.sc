$input a_position, a_texcoord0
$output v_texcoord0, v_pos

#include <bgfx_shader.sh>

uniform vec4 u_time;  // Add this uniform

void main()
{
    v_texcoord0 = a_texcoord0;
    
    vec4 worldPos = mul(u_model[0], vec4(a_position, 1.0));
    v_pos = worldPos.xyz;
    
    // Apply wave effect in world space
    worldPos.y += sin(worldPos.x * 2.0 + u_time.x) * 0.1 + 
                  cos(worldPos.z * 2.0 + u_time.x * 0.7) * 0.1;
    
    gl_Position = mul(u_viewProj, worldPos);
}