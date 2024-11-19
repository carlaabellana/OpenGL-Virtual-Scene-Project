#version 330

out vec4 fragColor;

in vec3 v_vertex;
in vec3 v_color;
in vec2 v_uv;
in vec3 v_normal;

uniform sampler2D u_texture;
uniform vec3 u_light_dir;

uniform sampler2D u_texture_normal;
uniform sampler2D u_texture_spec;
uniform sampler2D u_texture_trans;

uniform vec3 u_light_color;
uniform vec3 u_cam_pos;
uniform vec3 u_ambient;
uniform vec3 u_diffuse;
uniform vec3 u_specular;
uniform float u_shininess;

uniform bool earth;
uniform bool rings;
uniform bool sun;
uniform bool clouds;

uniform float u_alpha;

out vec4 FragColor;
out vec4 BloomColor;


mat3 cotangent_frame(vec3 N, vec3 p, vec2 uv){ 
// get edge vectors of the pixel triangle
	vec3 dp1 = dFdx(p);
	vec3 dp2 = dFdy(p);
	vec2 duv1 = dFdx(uv);
	vec2 duv2 = dFdy(uv);

	// solve the linear system
	vec3 dp2perp = cross(dp2, N);
	vec3 dp1perp = cross(N, dp1);
	vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
	vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;
	
	// construct a scale-invariant frame
	float invmax = inversesqrt(max(dot(T, T), dot(B, B)));
	return mat3(T * invmax, B * invmax, N);
}

// assume N, the interpolated vertex normal and
// V, the view vector (vertex to eye)
vec3 perturbNormal(vec3 N, vec3 V, vec2 texcoord, vec3 normal_pixel) {

	normal_pixel = normal_pixel * 2.0 - 1.0;
	mat3 TBN = cotangent_frame(N, V, texcoord);
	return normalize(TBN * normal_pixel);
}

void main(void)
{	
	vec3 texture_color = texture(u_texture, v_uv).xyz;

	if (earth){
		vec3 texture_normal = texture(u_texture_normal, v_uv).xyz;
		vec3 texture_spec = texture(u_texture_spec, v_uv).xyz;

		vec3 N = normalize(v_normal);
		vec3 N_orig = normalize(v_normal);
		N = perturbNormal(N, v_vertex, v_uv, texture_normal);
		N = mix(N_orig, N, 2.0f);
		vec3 L = normalize(u_light_dir - v_vertex);
		float NdotL = max(dot(N,L), 0.0);
		vec3 diffuse_color = texture_color * NdotL;
	
		vec3 ambient_color = texture_color * u_ambient;

		vec3 R = normalize(-reflect(L, N) );
		vec3 E = normalize(u_cam_pos - v_vertex);
		float RdotE = max( dot(R,E) , 0.0 );
		vec3 spec_color = texture_color * pow (RdotE , u_shininess);

		vec3 H = normalize(L + E);
		float NdotH = max(dot(N,H) , 0.0 );
		vec3 bling_color = texture_color * pow(NdotH, u_shininess);

		vec3 final_color = ambient_color + diffuse_color + bling_color * texture_spec;

		fragColor =  vec4(final_color, 1.0);

	} else if (rings){
		if(texture_color.rgb == vec3(1.0, 1.0, 1.0)){
			discard;
		}

		vec3 N = normalize(v_normal);
		vec3 L = normalize(u_light_dir - v_vertex);
		float NdotL = max(dot(N,L), 0.0);
		vec3 diffuse_color = texture_color * NdotL;

		vec3 ambient_color = texture_color * u_ambient;

		vec3 R = normalize(-reflect(L, N) );
		vec3 E = normalize(u_cam_pos - v_vertex);
		float RdotE = max( dot(R,E) , 0.0 );
		vec3 spec_color = texture_color * pow (RdotE , u_shininess);
		
		vec3 H = normalize(L + E);
		float NdotH = max( dot(N,H) , 0.0 );
		vec3 bling_color = texture_color * pow(NdotH, u_shininess);

		vec3 final_color = ambient_color + diffuse_color + bling_color;

		fragColor = vec4(final_color, u_alpha);
	
	} else if (sun){ 

		if(FragColor.r > 0.05f)
			FragColor.r *= 5.0f;
		float brightness = dot(FragColor.rgb, vec3(0.2126f, 0.7152f, 0.0722f));
			if(brightness > 0.15f)
				BloomColor = vec4(FragColor.rgb, 1.0f);
			else
				BloomColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);

		fragColor = vec4(texture_color.xyz, 1.0);
		
	} else if (clouds){
		vec3 texture_trans = texture(u_texture_trans, v_uv).xyz;
		if (texture_trans.r < 0.1f && texture_trans.g < 0.1f && texture_trans.b < 0.1){
			discard;
		}
		texture_color = texture_color * texture_trans;
		vec3 N = normalize(v_normal);
		vec3 L = normalize(u_light_dir - v_vertex);
		float NdotL = max(dot(N,L), 0.0);
		vec3 diffuse_color = texture_color * NdotL;
	
		vec3 ambient_color = texture_color * u_ambient;

		vec3 R = normalize(-reflect(L, N) );
		vec3 E = normalize(u_cam_pos - v_vertex);
		float RdotE = max( dot(R,E) , 0.0 );
		vec3 spec_color = texture_color * pow (RdotE , u_shininess);

		vec3 H = normalize(L + E);
		float NdotH = max( dot(N,H) , 0.0 );
		vec3 bling_color = texture_color * pow(NdotH, u_shininess);

		vec3 final_color = ambient_color + diffuse_color + bling_color;

		fragColor =  vec4(final_color, u_alpha);
	}else {
		vec3 N = normalize(v_normal);
		vec3 L = normalize(u_light_dir - v_vertex);
		float NdotL = max(dot(N,L), 0.0);
		vec3 diffuse_color = texture_color * NdotL;
	
		vec3 ambient_color = texture_color * u_ambient;

		vec3 R = normalize(-reflect(L, N) );
		vec3 E = normalize(u_cam_pos - v_vertex);
		float RdotE = max( dot(R,E) , 0.0 );
		vec3 spec_color = texture_color * pow (RdotE , u_shininess);

		vec3 H = normalize(L + E);
		float NdotH = max( dot(N,H) , 0.0 );
		vec3 bling_color = texture_color * pow(NdotH, u_shininess);

		vec3 final_color = ambient_color + diffuse_color + bling_color;

		fragColor =  vec4(final_color, u_alpha);
	}

}
