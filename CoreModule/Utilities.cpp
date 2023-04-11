#include "Utilities.h"
//---------Color--------------

using namespace KritiaEngine;

KritiaEngine::Color::Color()
{
	this->r = 1;
	this->g = 1;
	this->b = 1;
	this->a = 1;
}

KritiaEngine::Color::Color(float r, float g, float b, float a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

KritiaEngine::Color::Color(const Vector4& vec4)
{
	this->r = vec4.x;
	this->g = vec4.y;
	this->b = vec4.z;
	this->a = vec4.w;
}

Vector3 KritiaEngine::Color::GetRGB()
{
	return Vector3(r, g, b);
}

Color KritiaEngine::Color::operator+(const Color& color)
{
	return Color(this->r + color.r, this->g + color.g, this->b + color.b, this->a + color.a);
}

Color KritiaEngine::Color::operator-(const Color& color)
{
	return Color(this->r - color.r, this->g - color.g, this->b - color.b, this->a - color.a);
}

Color KritiaEngine::Color::operator*(float a)
{
	return Color(this->r * a, this->g * a, this->b * a, this->a * a);
}

Color KritiaEngine::Color::operator/(float a)
{
	return Color(this->r / a, this->g / a, this->b / a, this->a / a);
}

void KritiaEngine::Color::operator+=(const Color& color)
{
	this->r += color.r;
	this->g += color.g;
	this->b += color.b;
	this->a += color.a;
}

void KritiaEngine::Color::operator-=(const Color& color)
{
	this->r -= color.r;
	this->g -= color.g;
	this->b -= color.b;
	this->a -= color.a;
}

void KritiaEngine::Color::operator*=(float a)
{
	this->r *= a;
	this->g *= a;
	this->b *= a;
	this->a *= a;
}

void KritiaEngine::Color::operator/=(float a)
{
	this->r /= a;
	this->g /= a;
	this->b /= a;
	this->a /= a;
}

KritiaEngine::Color::operator std::vector<float>() const {
	return std::vector<float>({ r,g,b,a });
}

std::ostream& KritiaEngine::operator<<(std::ostringstream& cout, Color& color) {
	cout << "(" << color.r << ", " << color.g << ", " << color.b << ", " << color.a << ")";
	return cout;
}

Color KritiaEngine::operator*(float a, const Color& color)
{
	return Color(a * color.r, a * color.g, a * color.b, a * color.a);
}

//----------------------Debug---------------------
std::ostringstream KritiaEngine::Debug::logStream = std::ostringstream();

KritiaEngine::Bound::Bound() {
	center = Vector3::Zero();
	size = Vector3(1, 1, 1);
}

Vector3 KritiaEngine::Bound::GetExtent() const {
	return 0.5 * size;
}

Vector3 KritiaEngine::Bound::GetMin() const {
	Vector3 c = center;
	return c - 0.5 * size;
}

Vector3 KritiaEngine::Bound::GetMax() const {
	Vector3 c = center;
	return c + 0.5 * size;
}


//--------------------------Bound-------------------------------
std::ostream& KritiaEngine::operator<<(std::ostringstream& cout, Bound& bound) {
	cout << "Center: ";
	cout << bound.center;
	cout << "Size: ";
	cout << bound.size;
	return cout;
}