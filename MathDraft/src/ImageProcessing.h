#pragma once
#include <opencv2/opencv.hpp>

#include <random>
#include <optional>
#include <vector>


#include "glm/glm.hpp"
using namespace cv;

const int WIDTH = 1280;
const int HEIGHT = 720;

#define POINT2(v) Point(v.x,v.y)
#define COLOR(c) c.x, c.y, c.z

glm::vec2 middlePoint(WIDTH / 2, HEIGHT / 2);
#define MIDDLE_POINT (WIDTH / 2), (HEIGHT / 2)

glm::vec3 cameraPosition(MIDDLE_POINT, 300.0f);



struct Sphere
{
	Sphere(glm::vec3 position, glm::vec3 color, float radius) :position(position), color(color), radius(radius) {}
	glm::vec3 position;
	glm::vec3 color;
	float radius;
};

struct Plane
{
	Plane(glm::vec3 position, glm::vec3 normal, float d) :position(position), normal(normal), d(d) {}
	glm::vec3 position;
	glm::vec3 normal;
	float d;
};

struct Light
{
	Light(glm::vec3 position, glm::vec3 color) :position(position), color(color) {}
	glm::vec3 position;
	glm::vec3 color;
};

struct Ray
{
	Ray(glm::vec3 position, glm::vec3 direction, glm::vec3 preNormal) :position(position), direction(direction), preNormal(preNormal) {}
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 preNormal;
};

float sqr(float value)
{
	return value * value;
}

float sqr(glm::vec3 value)
{
	return value.x * value.x + value.y * value.y + value.z * value.z;
}

void GenerateRandomPixels(Mat& mat)
{
	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			mat.at<Vec3b>(Point(i, j)) = Vec3b(rand() % 255, rand() % 255, rand() % 255);
		}
	}
}


//Return First as Nearest and Second as farthest
std::optional<std::pair<glm::vec3, glm::vec3>> RaySphereIntersects(Sphere& sphere, glm::vec3& v, glm::vec3& cameraPosition)
{
	float a, b, c;
	a = sqr(v);
	b = 2 * glm::dot(v, cameraPosition - sphere.position);
	c = sqr(cameraPosition - sphere.position) - sqr(sphere.radius);

	float determant = sqr(b) - 4 * a * c;

	if (determant < 0)
		return std::nullopt;

	float t1 = (-b + glm::sqrt(determant)) / (2 * a);
	float t2 = (-b - glm::sqrt(determant)) / (2 * a);

	glm::vec3 point1 = cameraPosition + glm::min(t1, t2) * v;
	glm::vec3 point2 = cameraPosition + glm::max(t1, t2) * v;

	return std::make_pair(point1, point2);
}

std::optional<glm::vec3> RayIntersectPlane(Plane& plane, glm::vec3& startPosition, glm::vec3& v)
{

	float t = (-(glm::dot(startPosition, plane.normal)) + plane.d) / glm::dot(v, plane.normal);

	if (t <= 0)
		return std::nullopt;
	else
		return (startPosition + t * v);
}

glm::vec3 CastRay(Ray& ray)
{
	ofr
}


void Render(Mat& mat)
{
	Sphere sphere1(glm::vec3(MIDDLE_POINT, -350), glm::vec3(0, 0, 255), 350.0f);
	Light light1(glm::vec3(WIDTH / 2 - 300, HEIGHT / 2 + 300, 300.0f), glm::vec3(255, 255, 255));

	Plane plane1(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);

	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < HEIGHT; j++)
		{
			const int flippedJ = HEIGHT - j - 1;
			glm::vec3 ray(glm::vec3(i, j, 0) - cameraPosition);
			ray = glm::normalize(ray);
			auto spherePoints = RaySphereIntersects(sphere1, ray, cameraPosition);
			if (spherePoints != std::nullopt)
			{
				glm::vec3& color = sphere1.color;

				glm::vec3 normal = glm::normalize(spherePoints->first - sphere1.position);
				glm::vec3 fragLightRay = glm::normalize(light1.position - spherePoints->first);

				float diffuse = glm::dot(normal, fragLightRay);

				int specularFactor = 32;
				float specular = glm::pow(glm::max(glm::dot(glm::normalize(glm::reflect(-fragLightRay, normal)), glm::normalize(cameraPosition - spherePoints->first)), 0.0f), specularFactor);

				mat.at<Vec3b>(Point(i, flippedJ)) = Vec3b(COLOR(color)) * (diffuse + specular);
			}
			else
			{
				auto planePoint = RayIntersectPlane(plane1, cameraPosition, ray);
				if (planePoint != std::nullopt)
				{
					if (glm::cos(glm::distance(*planePoint, glm::vec3(0, 0, 0)) / 500.0f) >= 0)
						mat.at<Vec3b>(Point(i, flippedJ)) = Vec3b(246, 93, 117);
					else
						mat.at<Vec3b>(Point(i, flippedJ)) = Vec3b(0, 0, 0);
				}
			}

		}
}