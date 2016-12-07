#version 330

struct DirLight 
{
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight 
{
    vec3 position;
    
    // Used for attenuation
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight 
{
    vec3 position;
    vec3 direction;
    float cutOffAngle;
  
    // Used for attenuation
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

#define NR_DIRECTIONAL_LIGHTS 1
#define NR_POINT_LIGHTS 1
#define NR_SPOT_LIGHTS 2


in vec3 f_position;
in vec3 f_normal;
in vec2 texCoord;

out vec4 frag_color;

uniform DirLight dirLights[NR_DIRECTIONAL_LIGHTS];
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLights[NR_SPOT_LIGHTS];
uniform float shininess;

uniform sampler2D gSampler;

// Function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 pos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 pos, vec3 viewDir);

void main()
{    
    // Normalize the input lighting vectors
    vec3 norm = normalize(f_normal);
    vec3 viewDir = normalize(-f_position);

    vec3 color = vec3(0.0);

    // Directional lights
    for(int d_light = 0; d_light < NR_DIRECTIONAL_LIGHTS; d_light++)
        color += CalcDirLight(dirLights[d_light], norm, viewDir);
    // Point lights
    for(int p_light = 0; p_light < NR_POINT_LIGHTS; p_light++)
        color += CalcPointLight(pointLights[p_light], norm, f_position, viewDir);
    // Spot lights
    for(int s_light = 0; s_light < NR_SPOT_LIGHTS; s_light++)
        color += CalcSpotLight(spotLights[s_light], norm, f_position, viewDir);

    
    frag_color = vec4(color, 1.0) * texture2D(gSampler, texCoord.st);
    frag_color.a = 1.0;
}

// Calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    // Direction from point to light
    vec3 lightDir = normalize(-light.direction);
    // Halfway vector
    vec3 halfway = normalize( lightDir + viewDir);

    // Compute terms in the illumination equation
    // Diffuse
    float Kd = max( dot(lightDir, normal), 0.0 );
    vec3  diffuse = Kd * light.diffuse;
    // Ambient
    vec3 ambient = light.ambient;
    // Specular
    float Ks = pow( max(dot(normal, halfway), 0.0), shininess );
    vec3 specular = Ks * light.specular;
    // discard the specular highlight if the light's behind the vertex
    if( dot(lightDir, normal) < 0.0 )  specular = vec3(0.0, 0.0, 0.0);

    return (ambient + diffuse + specular);
}

// Calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 pos, vec3 viewDir)
{
    // Direction from point to light
    vec3 lightDir = normalize(light.position - pos);
    // Halfway vector
    vec3 halfway = normalize( lightDir + viewDir);

    // Compute terms in the illumination equation
    // Diffuse
    float Kd = max( dot(lightDir, normal), 0.0 );
    vec3  diffuse = Kd * light.diffuse;
    // Ambient
    vec3 ambient = light.ambient;
    // Specular
    float Ks = pow( max(dot(normal, halfway), 0.0), shininess );
    vec3 specular = Ks * light.specular;
    // discard the specular highlight if the light's behind the vertex
    if( dot(lightDir, normal) < 0.0 )  specular = vec3(0.0, 0.0, 0.0);

    // Attenuation
    float distance = length(light.position - pos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Combine results
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

// Calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 pos, vec3 viewDir)
{
    // Direction from point to light
    vec3 lightDir = normalize(light.position - pos);
    // Halfway vector
    vec3 halfway = normalize( lightDir + viewDir);

    vec3 S = normalize(-light.direction);

    if (dot(S, lightDir) > cos(radians(light.cutOffAngle)))
    {
        // if 90 degrees: 0 intensity
        // intensity increases as angle decreases to 0
        float intensity = max(dot(normal, lightDir), 0.0);

        if(intensity > 0.0)
        {
            // Compute terms in the illumination equation
            // Diffuse
            float Kd = max( dot(lightDir, normal), 0.0 );
            vec3  diffuse = Kd * light.diffuse;
            // Ambient
            vec3 ambient = light.ambient;
            // Specular
            float Ks = pow( max(dot(normal, halfway), 0.0), shininess );
            vec3 specular = Ks * light.specular;
            // discard the specular highlight if the light's behind the vertex
            if( dot(lightDir, normal) < 0.0 )  specular = vec3(0.0, 0.0, 0.0);

            // Attenuation
            float distance = length(light.position - pos);
            float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

            // Combine results
            ambient *= attenuation * intensity;
            diffuse *= attenuation * intensity;
            specular *= attenuation * intensity;

            return(ambient + diffuse + specular);
        }
    }

    return vec3(0.0, 0.0, 0.0);    
}

