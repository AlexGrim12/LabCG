#pragma once
// Clases de iluminacion: nombres de uniform = los de shaders/shader_light.frag
#include <string>
#include <glew.h>
#include <glm.hpp>

class Light
{
public:
	Light() : color(1.0f), ambientIntensity(1.0f), diffuseIntensity(0.0f) {}
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity)
		: color(red, green, blue), ambientIntensity(aIntensity), diffuseIntensity(dIntensity) {}
protected:
	glm::vec3 color;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;
	void SendBase(GLuint prog, const std::string& name) const
	{
		glUniform3f(glGetUniformLocation(prog, (name + ".color").c_str()), color.x, color.y, color.z);
		glUniform1f(glGetUniformLocation(prog, (name + ".ambientIntensity").c_str()), ambientIntensity);
		glUniform1f(glGetUniformLocation(prog, (name + ".diffuseIntensity").c_str()), diffuseIntensity);
	}
};

// direction = hacia donde VIAJA la luz (el shader usa normales hacia adentro, como el curso)
class DirectionalLight : public Light
{
public:
	DirectionalLight() : Light(), direction(0.0f, -1.0f, 0.0f) {}
	DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xDir, GLfloat yDir, GLfloat zDir)
		: Light(red, green, blue, aIntensity, dIntensity), direction(xDir, yDir, zDir) {}
	void UseLight(GLuint prog) const
	{
		SendBase(prog, "directionalLight.base");
		glUniform3f(glGetUniformLocation(prog, "directionalLight.direction"), direction.x, direction.y, direction.z);
	}
private:
	glm::vec3 direction;
};

class PointLight : public Light
{
public:
	PointLight() : Light(), position(0.0f), constant(1.0f), linear(0.0f), exponent(0.0f) {}
	PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfloat con, GLfloat lin, GLfloat exp)
		: Light(red, green, blue, aIntensity, dIntensity), position(xPos, yPos, zPos),
		constant(con), linear(lin), exponent(exp) {}
	void UseLight(GLuint prog, int index) const
	{
		std::string n = "pointLights[" + std::to_string(index) + "]";
		SendBase(prog, n + ".base");
		glUniform3f(glGetUniformLocation(prog, (n + ".position").c_str()), position.x, position.y, position.z);
		glUniform1f(glGetUniformLocation(prog, (n + ".constant").c_str()), constant);
		glUniform1f(glGetUniformLocation(prog, (n + ".linear").c_str()), linear);
		glUniform1f(glGetUniformLocation(prog, (n + ".exponent").c_str()), exponent);
	}
	void SetPosition(GLfloat x, GLfloat y, GLfloat z) { position = glm::vec3(x, y, z); }
private:
	glm::vec3 position;
	GLfloat constant, linear, exponent;
};

class Material
{
public:
	Material() : specularIntensity(0.0f), shininess(0.0f) {}
	Material(GLfloat sIntensity, GLfloat shine) : specularIntensity(sIntensity), shininess(shine) {}
	void UseMaterial(GLuint prog) const
	{
		glUniform1f(glGetUniformLocation(prog, "material.specularIntensity"), specularIntensity);
		glUniform1f(glGetUniformLocation(prog, "material.shininess"), shininess);
	}
private:
	GLfloat specularIntensity, shininess;
};