#version 330 core

layout (location = 0) in vec3 aPos;

out vec4 vertexColor;

uniform float u_time;
uniform float u_properTime;
uniform float u_velocity;
uniform float u_gamma;
uniform mat4 u_motionMat;
uniform mat4 u_refChangeMat;

uniform mat4 u_modelMat;
uniform mat4 u_viewMat;
uniform mat4 u_projectionMat;

float solveTLinear(float k, float x0)
{
    float t = k;
    for (int i = 0; i < 3; i++)
    {
        float func = t - u_velocity * (aPos.y + u_velocity * t) - k;
        float der = 1 - u_velocity * u_velocity;
        t = t - func / der;
    }

    return t;
}

float solveTAngular(float k, float w, float x0, float y0)
{
    float t = k;
    
    for (int i = 0; i < 3; i++)
    {
        float func = t + u_velocity * (x0 * cos(w * t) - y0 * sin(w * t)) - k;
        float der = 1 - u_velocity * w * (x0 * sin(w * t) + y0 * cos(w * t));
        t = t - func / der;
    }

    return t;
}


vec3 applyMat(mat4 m, vec3 v)
{
	vec4 tempVec = vec4(v.xyz, 1.0);
	tempVec = m * tempVec;
	return tempVec.xyz;
}

void main()
{
    float k = u_time / u_gamma;

    //vec3 resultVector = applyMat(u_motionMat, aPos.xyz);
    //float newTime = solveTLinear(k, resultVector.y);
	//float newX = aPos.y + u_velocity * testTime;

    float w = 1;
    float newTime = solveTAngular(k, w, aPos.y, aPos.z);
    float newX = aPos.y * cos(w * newTime) - aPos.z * sin(w * newTime);
    float newY = aPos.y * sin(w * newTime) + aPos.z * cos(w * newTime);

    vec3 finalVector = applyMat(u_motionMat, vec3(newTime, newX, newY));

	//vec3 finalVector = applyMat(u_refChangeMat, vec3(testTime, newX, resultVector.z));

    //gl_Position = u_projectionMat * u_viewMat * u_modelMat * vec4(finalVector.y, finalVector.x, finalVector.z, 1.0);

    gl_Position = vec4(finalVector.y, finalVector.z, 0.0, 1.0);
    vertexColor = vec4(1.0, 0.0, 0.0, 1.0);
}
