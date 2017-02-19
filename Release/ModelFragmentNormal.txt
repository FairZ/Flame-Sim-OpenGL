#version 430 core
in vec4 vertexWorldPosition_vertex;
in vec3 normal_vertex;
in vec2 texCoord_vertex;
in vec3 tangent_vertex;

uniform vec3 lightColour = {1.0,0.6,0.6};
uniform vec3 emissiveColour = {0,0,0};
uniform vec3 ambientColour  = {0.05f,0.05f,0.1f};
uniform vec3 diffuseColour  = {1.0f,1.0f,1.0f};
uniform float alpha         = 1.0f;
uniform vec4 worldSpaceLightPosition;
uniform mat4 viewMat;

uniform sampler2D colourTexture;
uniform sampler2D normalTexture;

out vec4 fragColour;

vec3 CalculateMappedNormal()
{
	vec3 normal = normalize(normal_vertex);
	vec3 tangent = normalize(tangent_vertex);
	tangent = normalize(tangent - dot(tangent,normal) * normal);
	vec3 bitangent = cross(tangent,normal);
	vec3 mapNormal = texture(normalTexture,texCoord_vertex).xyz;
	mapNormal = 2.0 * mapNormal - vec3(1.0,1.0,1.0);
	mat3 tangentMatrix = mat3(tangent,bitangent,normal);
	vec3 finalNormal = normalize(tangentMatrix * mapNormal);
	return finalNormal;
}

void main()
{
	vec3 toLight = worldSpaceLightPosition.xyz - vertexWorldPosition_vertex.xyz;
	vec3 normal = CalculateMappedNormal();
	float lightDistanceSquared = pow(toLight.x,2) + pow(toLight.y,2) + pow(toLight.z,2);
	float lightPower = 2;

	vec3 texColour = vec3(texture(colourTexture,texCoord_vertex));
	vec3 diffuse = diffuseColour * lightColour * max( dot( normal, toLight ), 0)  / (lightDistanceSquared/lightPower);

	fragColour = vec4( emissiveColour + texColour*(ambientColour + diffuse), alpha);
}

