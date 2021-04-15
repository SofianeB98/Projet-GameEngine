#version 120

struct Material {
	vec3 AmbientColor;
	vec3 DiffuseColor;
	vec3 SpecularColor;
	float Shininess;
	float Metalness;
};
uniform Material u_Material;

uniform vec3 u_viewPos;

varying vec2 TexCoord;
varying vec3 Position;
varying vec3 v_normals;

float gamma = 2.2;

// calcul du facteur diffus, suivant la loi du cosinus de Lambert
float Lambert(vec3 N, vec3 L)
{
	return max(0.0, dot(N, L));
}

// cos0 = le resultat du produit scalaire (n.l, v.h, etc)
vec3 FresnelSchlick(vec3 f0, float cos0)
{
	return f0 + (vec3(1.0) - f0) * pow(cos0, 5.0);
}

// calcul du facteur speculaire, methode de Phong
float NormalizePhong(float shininess)
{
	return (shininess + 1.0) / 2.0;
}

float Phong(vec3 N, vec3 L, vec3 V, float shininess)
{
	// reflexion du vecteur incident I (I = -L)
	// suivant la loi de ibn Sahl / Snell / Descartes
	vec3 R = reflect(-L, N);
	return NormalizePhong(shininess) * pow(max(0.0, dot(R, V)), shininess);
}

// calcul du facteur speculaire, methode de Phong modifier par Lafortune
float NormalizePhongModified(float shininess)
{
	return (shininess + 2.0) / 2.0;
}

float PhongModified(vec3 N, vec3 L, vec3 V, float shininess)
{
	float NdotL = max(0.0, dot(N, L));

	// reflexion du vecteur incident I (I = -L)
	// suivant la loi de ibn Sahl / Snell / Descartes
	vec3 R = reflect(-L, N);
	return NormalizePhongModified(shininess) * pow(max(0.0, dot(R, V)), shininess) * NdotL;
}

// calcul du facteur speculaire, methode Blinn-Phong
float BlinnPhong(vec3 N, vec3 L, vec3 V, float shininess)
{
	// reflexion inspire du modele micro-facette (H approxime la normale de la micro-facette)
	vec3 H = normalize(L + V);
	return pow(max(0.0, dot(N, H)), shininess);
}

void main()
{
	// directions des deux lumieres (fixes)
	const vec3 L[1] = vec3[1](normalize(vec3(-1.0, 1.0, 1.0)));
	const vec3 lightColor[1] = vec3[1](vec3(1.0, 1.0, 1.0));
	const float attenuation = 1.0; // on suppose une attenuation faible ici
	// theoriquement, l'attenuation naturelle est proche de 1 / distance²

	vec3 N = normalize(v_normals);
	vec3 V = normalize(u_viewPos - Position);

	vec3 directColor = vec3(0.0);

	vec3 albedo = u_Material.DiffuseColor;

	for (int i = 0; i < 1; i++)
	{
		// les couleurs diffuse et speculaire traduisent l'illumination directe de l'objet
		vec3 diffuseColor = (1.0 - u_Material.Metalness) * albedo * Lambert(N, L[i]);
		vec3 specularColor = mix(vec3(1.0), albedo, u_Material.Metalness) /*u_Material.SpecularColor*/  * PhongModified(N, L[i], V, u_Material.Shininess);

		//reflexivite (reflectance) moyenne des objets non metallique
		vec3 f0 = mix(vec3(0.04), albedo, u_Material.Metalness);
		vec3 R = reflect(-L[i], N);
		vec3 Fspec = FresnelSchlick(f0, dot(R, V));
		vec3 Fdiff = vec3(1.0) - FresnelSchlick(f0, dot(N, L[i]));
		vec3 color = (diffuseColor * Fdiff + specularColor * Fspec);

		directColor += color * lightColor[i] * attenuation;
	}
	// la couleur ambiante traduit une approximation de l'illumination indirecte de l'objet
	vec3 ambientColor = u_Material.AmbientColor;
	vec3 indirectColor = ambientColor;

	vec3 color = directColor + indirectColor;

	gl_FragColor = vec4(color, 1.0);
    //gl_FragColor = vec4(Position.x, Position.y, sin(Position.x * 0.01 - Position.y * 0.01), 1.0);
}