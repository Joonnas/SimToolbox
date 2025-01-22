#include "a_math.h"

void aMath::identity(aMath::Mat3& matrix)
{
	for (int i = 0; i < sizeof(matrix.values) / sizeof(float); i++)
	{
		if (i == 0 || i == 4 || i == 8)
		{
			matrix.values[i] = 1.0f;
		}
		else
		{
			matrix.values[i] = 0.0f;
		}
	}
}
void aMath::identity(aMath::Mat4& matrix)
{
	for (int i = 0; i < sizeof(matrix.values) / sizeof(float); i++)
	{
		if (i == 0 || i == 5 || i == 10 || i == 15)
		{
			matrix.values[i] = 1;
		}
		else
		{
			matrix.values[i] = 0;
		}
	}
}

aMath::Vec2 aMath::add(const aMath::Vec2& a, const aMath::Vec2& b) { return aMath::Vec2(a.x + b.x, a.y + b.y); }
aMath::Vec2 aMath::subtract(const aMath::Vec2& a, const aMath::Vec2& b) { return aMath::Vec2(a.x - b.x, a.y - b.y); }
aMath::Vec2 aMath::mul(const aMath::Vec2& a, const aMath::Vec2& b) { return aMath::Vec2(a.x * b.x, a.y * b.y); }
aMath::Vec2 aMath::mul(float a, const aMath::Vec2& b) { return aMath::Vec2(a * b.x, a * b.y); }
float aMath::dot(const aMath::Vec2& a, const aMath::Vec2& b) { return ((a.x * b.x) + (a.y * b.y)); }

aMath::Vec3 aMath::add(const aMath::Vec3& a, const aMath::Vec3& b) { return aMath::Vec3(a.x + b.x, a.y + b.y, a.z + b.z); }
aMath::Vec3 aMath::subtract(const aMath::Vec3& a, const aMath::Vec3& b) { return aMath::Vec3(a.x - b.x, a.y - b.y, a.z - b.z); }
aMath::Vec3 aMath::mul(const aMath::Vec3& a, const aMath::Vec3& b) { return aMath::Vec3(a.x * b.x, a.y * b.y, a.z * b.z); }
aMath::Vec3 aMath::mul(float a, const aMath::Vec3& b) { return aMath::Vec3(a * b.x, a * b.y, a * b.z); }
aMath::Vec3 aMath::cross(const aMath::Vec3& a, const aMath::Vec3& b) { return aMath::Vec3((a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z), (a.x * b.y) - (a.y * b.x)); }
float aMath::dot(const aMath::Vec3& a, const aMath::Vec3& b) { return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z)); }

aMath::Vec4 aMath::add(const aMath::Vec4& a, const aMath::Vec4& b) { return aMath::Vec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
aMath::Vec4 aMath::subtract(const aMath::Vec4& a, const aMath::Vec4& b) { return aMath::Vec4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
aMath::Vec4 aMath::mul(const aMath::Vec4& a, const aMath::Vec4& b) { return aMath::Vec4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w); }
aMath::Vec4 aMath::mul(float a, const aMath::Vec4& b) { return aMath::Vec4(a * b.x, a * b.y, a * b.z, a * b.w); }
float aMath::dot(const aMath::Vec4& a, const aMath::Vec4& b) { return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w)); }

aMath::Vec3 aMath::mul(const aMath::Mat3& m, const aMath::Vec3& v)
{
	float val1 = (m.values[0] * v.x) + (m.values[1] * v.y) + (m.values[2] * v.z);
	float val2 = (m.values[3] * v.x) + (m.values[4] * v.y) + (m.values[5] * v.z);
	float val3 = (m.values[6] * v.x) + (m.values[7] * v.y) + (m.values[8] * v.z);
	aMath::Vec3 retVector(val1, val2, val3);
	return retVector;
}
aMath::Vec3 aMath::mul(const aMath::Vec3& v, const aMath::Mat3& m)
{
	float val1 = (v.x * m.values[0]) + (v.y * m.values[3]) + (v.z * m.values[6]);
	float val2 = (v.x * m.values[1]) + (v.y * m.values[4]) + (v.z * m.values[7]);
	float val3 = (v.x * m.values[2]) + (v.y * m.values[5]) + (v.z * m.values[8]);
	aMath::Vec3 retVector(val1, val2, val3);
	return retVector;
}
aMath::Vec4 aMath::mul(const aMath::Mat4& m, const aMath::Vec4& v)
{
	float val1 = (m.values[0] * v.x) + (m.values[1] * v.y) + (m.values[2] * v.z) + (m.values[3] * v.w);
	float val2 = (m.values[4] * v.x) + (m.values[5] * v.y) + (m.values[6] * v.z) + (m.values[7] * v.w);
	float val3 = (m.values[8] * v.x) + (m.values[9] * v.y) + (m.values[10] * v.z) + (m.values[11] * v.w);
	float val4 = (m.values[12] * v.x) + (m.values[13] * v.y) + (m.values[14] * v.z) + (m.values[15] * v.w);
	aMath::Vec4 retVector(val1, val2, val3, val4);
	return retVector;
}
aMath::Vec4 aMath::mul(const aMath::Vec4& v, const aMath::Mat4& m)
{
	float val1 = (v.x * m.values[0]) + (v.y * m.values[4]) + (v.z * m.values[8]) + (v.w * m.values[12]);
	float val2 = (v.x * m.values[1]) + (v.y * m.values[5]) + (v.z * m.values[9]) + (v.w * m.values[13]);
	float val3 = (v.x * m.values[2]) + (v.y * m.values[6]) + (v.z * m.values[10]) + (v.w * m.values[14]);
	float val4 = (v.x * m.values[3]) + (v.y * m.values[7]) + (v.z * m.values[11]) + (v.w * m.values[15]);
	aMath::Vec4 retVector(val1, val2, val3, val4);
	return retVector;
}

aMath::Mat3 aMath::mul(float a, const aMath::Mat3& m)
{
	aMath::Mat3 retMatrix;
	for (int i = 0; i < sizeof(m.values) / sizeof(float); i++)
	{
		retMatrix.values[i] = a * m.values[i];
	}
	return retMatrix;
}
aMath::Mat4 aMath::mul(float a, const aMath::Mat4& m)
{
	aMath::Mat4 retMatrix;
	for (int i = 0; i < sizeof(m.values) / sizeof(float); i++)
	{
		retMatrix.values[i] = a * m.values[i];
	}
	return retMatrix;
}

aMath::Mat3 aMath::mul(const aMath::Mat3& a, const aMath::Mat3& b)
{
	aMath::Mat3 retMatrix;
	for (int i = 0; i < 3; i++)
	{
		aMath::Vec3 vecA(a.values[(i * 3) + 0], a.values[(i * 3) + 1], a.values[(i * 3) + 2]);
		for (int j = 0; j < 3; j++)
		{
			aMath::Vec3 vecB(b.values[j], b.values[j + 3], b.values[j + 6]);
			retMatrix.values[(i * 3) + j] = aMath::dot(vecA, vecB);
		}
	}
	return retMatrix;
}
aMath::Mat4 aMath::mul(const aMath::Mat4& a, const aMath::Mat4& b)
{
	aMath::Mat4 retMatrix;
	for (int i = 0; i < 4; i++)
	{
		aMath::Vec4 vecA(a.values[(i * 4) + 0], a.values[(i * 4) + 1], a.values[(i * 4) + 2], a.values[(i * 4) + 3]);
		for (int j = 0; j < 4; j++)
		{
			aMath::Vec4 vecB(b.values[j], b.values[j + 4], b.values[j + 8], b.values[j + 12]);
			retMatrix.values[(i * 4) + j] = aMath::dot(vecA, vecB);
		}
	}
	return retMatrix;
}

aMath::Mat3 aMath::add(const aMath::Mat3& a, const aMath::Mat3& b)
{
	aMath::Mat3 retMatrix;
	for (int i = 0; i < sizeof(a.values) / sizeof(float); i++)
	{
		retMatrix.values[i] = a.values[i] + b.values[i];
	}
	return retMatrix;
}
aMath::Mat3 aMath::subtract(const aMath::Mat3& a, const aMath::Mat3& b)
{
	aMath::Mat3 retMatrix;
	for (int i = 0; i < sizeof(a.values) / sizeof(float); i++)
	{
		retMatrix.values[i] = a.values[i] - b.values[i];
	}
	return retMatrix;
}
aMath::Mat4 aMath::add(const aMath::Mat4& a, const aMath::Mat4& b)
{
	aMath::Mat4 retMatrix;
	for (int i = 0; i < sizeof(a.values) / sizeof(float); i++)
	{
		retMatrix.values[i] = a.values[i] + b.values[i];
	}
	return retMatrix;
}
aMath::Mat4 aMath::subtract(const aMath::Mat4& a, const aMath::Mat4& b)
{
	aMath::Mat4 retMatrix;
	for (int i = 0; i < sizeof(a.values) / sizeof(float); i++)
	{
		retMatrix.values[i] = a.values[i] - b.values[i];
	}
	return retMatrix;
}

aMath::Vec4 aMath::multiply(const aMath::Mat4& a, const aMath::Vec4& b)
{
	aMath::Vec4 retVector;
	retVector.x = (a.values[0] * b.x) + (a.values[1] * b.y) + (a.values[2] * b.z) + (a.values[3] * b.w);
	retVector.y = (a.values[4] * b.x) + (a.values[5] * b.y) + (a.values[6] * b.z) + (a.values[7] * b.w);
	retVector.z = (a.values[8] * b.x) + (a.values[9] * b.y) + (a.values[10] * b.z) + (a.values[11] * b.w);
	retVector.w = (a.values[12] * b.x) + (a.values[13] * b.y) + (a.values[14] * b.z) + (a.values[15] * b.w);
	return retVector;
}
aMath::Mat4 aMath::multiply(const aMath::Mat4& a, const aMath::Mat4& b)
{
	aMath::Mat4 retMat;
	for (int i = 0; i < sizeof(retMat.values) / sizeof(float); i++)
	{
		retMat.values[i] = (a.values[((i / 4) * 4) + 0] * b.values[(i % 4) + 0]) + (a.values[((i / 4) * 4) + 1] * b.values[(i % 4) + 4]) + (a.values[((i / 4) * 4) + 2] * b.values[(i % 4) + 8]) + (a.values[((i / 4) * 4) + 3] * b.values[(i % 4) + 12]);
	}
	return retMat;
}

aMath::Mat4 aMath::translate(float x, float y, float z)
{
	aMath::Mat4 transformation;
	aMath::Vec3 translateVector(x, y, z);
	transformation.translate(translateVector);
	return transformation;
	// return multiply(transformation, position);
}
aMath::Mat4 aMath::translate(aMath::Vec3& pos) {return aMath::translate(pos.x, pos.y, pos.z);}

aMath::Mat4 aMath::scale(float x, float y, float z)
{
	aMath::Mat4 transformation;
	aMath::Vec3 scaleVector(x, y, z);
	transformation.scale(scaleVector);
	return transformation;
}
aMath::Mat4 aMath::scale(aMath::Vec3& s) {return aMath::scale(s.x, s.y, s.z);};

aMath::Mat4 aMath::rotateX(float angle) {
	aMath::Mat4 transformation;
	transformation.rotateX(angle);
	return transformation;
}
aMath::Mat4 aMath::rotateY(float angle) {
	aMath::Mat4 transformation;
	transformation.rotateY(angle);
	return transformation;
}
aMath::Mat4 aMath::rotateZ(float angle) {
	aMath::Mat4 transformation;
	transformation.rotateZ(angle);
	return transformation;
}

aMath::Mat4 aMath::orthographic(float left, float right, float top, float bottom, float near, float far)
{
	aMath::Mat4 orthographic_matrix;
	orthographic_matrix.values[0] = 2 / (right - left);
	orthographic_matrix.values[5] = 2 / (top - bottom);
	orthographic_matrix.values[10] = -2 / (far - near);
	orthographic_matrix.values[12] = -(right + left) / (right - left);
	orthographic_matrix.values[13] = -(top + bottom) / (top - bottom);
	orthographic_matrix.values[14] = -(far + near) / (far - near);
	return orthographic_matrix;
}