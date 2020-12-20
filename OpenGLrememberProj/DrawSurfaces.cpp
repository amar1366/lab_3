#define _USE_MATH_DEFINES // for C
#include <chrono>
#include <vector>

#include"Draw.h"
#include"DrawPrism.h"
#include "MethodGauss.h"

//�����-���������
extern int alpha;
//����� ��������
extern bool texChange;
//id ��������
extern GLuint texId[2];
//���������/���������� ��������� �����
extern bool line;
extern double sharpness_t;


//����� ��� ������ ������, ����� 2 � 3 ��������
extern vector<Point> PointHB;


//������� ����� ����� ��� ������ ������
Point HermitCurve(Point P1, Point R1, Point P2, Point R2, double t) {
	double t_2 = t * t;
	double t_3 = t * t * t;

	double x = P1.x * (2 * t_3 - 3 * t_2 + 1) + P2.x * (-2 * t_3 + 3 * t_2) + R1.x * (t_3 - 2 * t_2 + t) + R2.x * (t_3 - t_2);
	double y = P1.y * (2 * t_3 - 3 * t_2 + 1) + P2.y * (-2 * t_3 + 3 * t_2) + R1.y * (t_3 - 2 * t_2 + t) + R2.y * (t_3 - t_2);
	double z = P1.z * (2 * t_3 - 3 * t_2 + 1) + P2.z * (-2 * t_3 + 3 * t_2) + R1.z * (t_3 - 2 * t_2 + t) + R2.z * (t_3 - t_2);

	return Point(x, y, z);
}

//���������� ����� ������ ������
//P1 - ����� ������ ������, P2 - ����� ����� ������
//R1 - ������ ������������ ������ ��� t = 0, R2 - ������ ����������� ������ ��� t = 1
//t - ���
//t_max - ������������ �������� t
vector<Point> PointHermitCurve(Point P1, Point R1, Point P2, Point R2, double t_max = 1) {
	vector<Point> points;
	t_max = 1 + sharpness_t * 0.001;

	for (double i = 0; i <= t_max; i += sharpness_t) {
		points.push_back(HermitCurve(P1, R1, P2, R2, i));
	}
	return points;
}

//������ ����� ����� ��� ������ ����� ������� �������
Point BezierCurve2(Point P1, Point P2, Point P3, double t) {
	double t_2 = t * t;
	double var = 1 - t;
	double var_2 = var * var;

	double x = P1.x * var_2 + 2 * P2.x * t * var + P3.x * t_2;
	double y = P1.y * var_2 + 2 * P2.y * t * var + P3.y * t_2;
	double z = P1.z * var_2 + 2 * P2.z * t * var + P3.z * t_2;

	return Point(x, y, z);
}

//���������� ����� ������ ����� ������� �������
//P1, P2, P3 - �����, t - ���, t_max - ������������ �������� t 
vector<Point> PointBezierCurve2(Point P1, Point P2, Point P3, double t_max = 1) {
	vector<Point> points;
	t_max = 1 + sharpness_t * 0.001;

	for (double i = 0; i <= t_max; i += sharpness_t) {
		points.push_back(BezierCurve2(P1, P2, P3, i));
	}
	return points;
}

//���������� ����� ������ ����� �������� �������
Point BezierCurve3(Point P0, Point P1, Point P2, Point P3, double t) {
	double t_2 = t * t;
	double t_3 = t_2 * t;
	double var = 1 - t;
	double var_2 = var * var;
	double var_3 = var_2 * var;

	double x = var_3 * P0.x + 3 * t * var_2 * P1.x + 3 * t_2 * var * P2.x + t_3 * P3.x;
	double y = var_3 * P0.y + 3 * t * var_2 * P1.y + 3 * t_2 * var * P2.y + t_3 * P3.y;
	double z = var_3 * P0.z + 3 * t * var_2 * P1.z + 3 * t_2 * var * P2.z + t_3 * P3.z;

	return Point(x, y, z);
}

//���������� ����� ������ ����� �������� �������
//���������� � ����� P0, ��������� ����������� � P1, ����� � P2 � ������������� � ����� P3
//t - ���, t_max - ������������ �������� t
vector<Point> PointBezierCurve3(Point P0, Point P1, Point P2, Point P3, double t_max = 1) {
	vector<Point> points;
	t_max = 1 + sharpness_t * 0.001;

	for (double i = 0; i <= t_max; i += sharpness_t) {
		points.push_back(BezierCurve3(P0, P1, P2, P3, i));
	}
	return points;
}

//������ ������ ������
void DrawHermitCurve(Point P1, Point R1, Point P2, Point R2) {
	//������ �����
	glLineWidth(3);

	R1 = Draw::SearchVector(P1, R1);
	R2 = Draw::SearchVector(P2, R2);

	//������ ������ ������
	glColor3d(0.4, 0.5, 0.6);
	glBegin(GL_LINE_STRIP);
	vector<Point> points = PointHermitCurve(P1, R1, P2, R2);
	for (int i = 0; i < (int)points.size(); i++) {
		points[i].DrawPoint();
	}
	glEnd();

	//������ �����
	glLineWidth(1);

}

//������ ������ ����� ������� �������
void DrawBezierCurve2(Point A, Point B, Point C) {

	//������ �����
	glLineWidth(3);

	//������ ������ ����� ������� �������
	glColor3d(0, 1, 0);
	glBegin(GL_LINE_STRIP);
	vector<Point> points = PointBezierCurve2(A, B, C);
	for (int i = 0; i < (int)points.size(); i++) {
		points[i].DrawPoint();
	}
	glEnd();

	//������ �����
	glLineWidth(1);

}

//������ ������ ����� �������� �������
void DrawBezierCurve3(Point A, Point B, Point C, Point D) {
	// ������ �����
	glLineWidth(3);

	//������ ������ ����� ������� �������
	glColor3d(0, 0, 1);
	glBegin(GL_LINE_STRIP);
	vector<Point> points = PointBezierCurve3(A, B, C, D);
	for (int i = 0; i < (int)points.size(); i++) {
		points[i].DrawPoint();
	}
	glEnd();

	//������ �����
	glLineWidth(1);

}

//����� ������������ ������� 3 �� 3
double SearchDeterminantMatrix3(double m[3][3]) {
	return m[0][0] * m[1][1] * m[2][2] +
		m[2][0] * m[0][1] * m[1][2] +
		m[1][0] * m[2][1] * m[0][2] -
		m[2][0] * m[1][1] * m[0][2] -
		m[0][0] * m[2][1] * m[1][2] -
		m[1][0] * m[0][1] * m[2][2];
}

//������ ���
void DrawSquare() {
	static Point XYZ = { 0, 0, 0 };

	double l = 0.5;
	Point A(l, l, -1 * l), B(l, -1 * l, -1 * l), C(-1 * l, -1 * l, -1 * l), D(-1 * l, l, -1 * l);
	Point AA(l, l, l), BB(l, -1 * l, l), CC(-1 * l, -1 * l, l), DD(-1 * l, l, l);

	Point O(2*l, 0, 0);
	Point X(2, 0, 0);
	Point Y(0, 2, 0);
	Point Z(0, 0, 2);

	glColor3d(0.1, 0.3, 0.3);
	glBegin(GL_QUADS);
	//���
	Draw::DrawQuads(A, B, C, D);
	//����
	Draw::DrawQuads(AA, BB, CC, DD);
	
	//�����
	glColor3d(0.4, 0.5, 0.3);
	Draw::DrawQuads(BB, B, A, AA, -1);
	glColor3d(0.5, 0.3, 0.6);
	Draw::DrawQuads(CC, C, B, BB, -1);
	glColor3d(0.7, 0.5, 0.1);
	Draw::DrawQuads(DD, D, C, CC, -1);
	glColor3d(0.8, 0.4, 0.8);
	Draw::DrawQuads(AA, A, D, DD, -1);

	glEnd();

	glColor3d(0.7, 0.3, 0.2);
	glBegin(GL_TRIANGLES);
	Draw::DrawTriangles_(O, AA, A);
	Draw::DrawTriangles_(O, BB, AA);
	Draw::DrawTriangles_(O, BB, B);
	Draw::DrawTriangles_(O, A, B);
	glEnd();

	if (line) {
		glLineWidth(3);
		glBegin(GL_LINES);
		glColor3d(1, 0, 0);
		Draw::DrawLine(XYZ, X);
		glColor3d(0, 1, 0);
		Draw::DrawLine(XYZ, Y);
		glColor3d(0, 0, 1);
		Draw::DrawLine(XYZ, Z);
		glEnd();
		glLineWidth(1);
	}

}

enum Method {
	Hermit,
	Bezier2,
	Bezier3
};

//������� ������
void MoveAxis(Point point, Point next_point) {
	Point dir = Draw::SearchVector(point, next_point);
	dir = Draw::VectorNormalization(dir);

	Point orig(1, 0, 0);
	Point rotX(dir.x, dir.y, 0);
	rotX = Draw::VectorNormalization(rotX);
	double cosU = Draw::ScalarProduct(orig, rotX);
	Point vecpr = Draw::VectorProduct(orig, rotX);

	double sinSign = vecpr.z / abs(vecpr.z);
	double U = acos(cosU) * 180.0 / M_PI * sinSign;
	double ZU = acos(dir.z) * 180.0 / M_PI - 90;

	glRotated(U, 0, 0, 1);
	glRotated(ZU, 0, 1, 0);

}

//������ ��������� �� ������ ���� ����
void AnimationObjectCurve(vector<Point> points, vector<double> t, Method method) {
	vector<Point> PointsCurve;
	Point point(0, 0, 0);
	Point next_point(0, 0, 0);
	Point last_point(0, 0, 0);

	double t_max = t[0], t_max_next = t[1];

	switch (method) {
	case Hermit:
		if (line) {
			DrawHermitCurve(points[0], points[1], points[2], points[3]);
		}

		glPushMatrix();

		point = HermitCurve(points[0], Draw::SearchVector(points[0], points[1]), points[2], Draw::SearchVector(points[2], points[3]), t_max);
		next_point = HermitCurve(points[0], Draw::SearchVector(points[0], points[1]), points[2], Draw::SearchVector(points[2], points[3]), t_max_next);
		point.Translated();
		MoveAxis(point, next_point);
		DrawSquare();
		
		glPopMatrix();
		break;

	case Bezier2:
		if (line) {
			DrawBezierCurve2(points[0], points[1], points[2]);
		}

		glPushMatrix();
		point = BezierCurve2(points[0], points[1], points[2], t_max);
		next_point = BezierCurve2(points[0], points[1], points[2], t_max_next);

		point.Translated();
		MoveAxis(point, next_point);
		DrawSquare();

		glPopMatrix();
		break;

	case Bezier3:
		if (line) {
			DrawBezierCurve3(points[0], points[1], points[2], points[3]);
		}

		glPushMatrix();

		point = BezierCurve3(points[0], points[1], points[2], points[3], t_max);
		next_point = BezierCurve3(points[0], points[1], points[2], points[3], t_max_next);

		point.Translated();
		MoveAxis(point, next_point);
		DrawSquare();

		glPopMatrix();
		break;
	}
}

//���������
//������ ������� ���������� ����������
int factorial(int N)
{
	if (N < 0)
		return 0;
	if (N == 0)
		return 1;
	else
		return N * factorial(N - 1);
}

//������� ������ ����������
double FunctionBernsteinBasis(int n, int i, double t) {
	return (factorial(n) / (factorial(n - i) * factorial(i))) * pow(t, i) * pow(1 - t, n - i);
}

//����������� ������ �������
Point FunctionBezierSurface(vector<vector<Point>> mas, double u, double v) {
	Point R(0, 0, 0);
	int row = (int)mas.size();
	for (int i = 0; i < row; i++) {
		int col = (int)mas[i].size();
		for (int j = 0; j < col; j++) {
			double Bi = FunctionBernsteinBasis(row - 1, i, u);
			double Bj = FunctionBernsteinBasis(col - 1, j, v);
			R.x += Bj * Bi * mas[i][j].x;
			R.y += Bj * Bi * mas[i][j].y;
			R.z += Bj * Bi * mas[i][j].z;
		}
	}
	return R;
}

//�������� ���������� ������� ����� ����������
double Search_delta_time() {
	static auto end_render = std::chrono::steady_clock::now();
	auto cur_time = std::chrono::steady_clock::now();
	auto deltatime = cur_time - end_render;
	double delta = 1.0 * std::chrono::duration_cast<std::chrono::microseconds>(deltatime).count() / 1000000;
	end_render = cur_time;
	return delta;
}


void Draw() {

	static double t_max = 0;
	static double t_max_next = 0;
	static bool flag_tmax = true;

	//��������� �������
	double delta_time = Search_delta_time();
	double go = delta_time / 5; //t_max ���������� = 1 �� 5 ������
	//t_max ���� �� ���� ���������� �� 0 �� 1 ���������� �� ����� � �����
	if (flag_tmax) {
		t_max += go;
		t_max_next = t_max + go;
		if (t_max > 1) {
			t_max = 1;
			flag_tmax = false;
		}
		if (t_max_next > 1) {
			t_max_next = 1;
		}
	}
	else {
		t_max -= go; 
		t_max_next = t_max - go;
		if (t_max < 0) {
			t_max = 0;
			flag_tmax = true;
		}
		if (t_max_next < 0) {
			t_max_next = 0;
		}
	}

	vector<double> t = {t_max, t_max_next };

	//��������, �������� ������� �� ������ ���� ����
	//������ ������ ������
	AnimationObjectCurve(vector<Point> {PointHB[0], PointHB[1], PointHB[2], PointHB[3]}, t, Hermit);
	AnimationObjectCurve(vector<Point> {PointHB[4], PointHB[5], PointHB[6], PointHB[7]}, t, Hermit);
	
	//������ ������ ����� 2 �������
	AnimationObjectCurve(vector<Point> {PointHB[8], PointHB[9], PointHB[10]}, t, Bezier2);
	AnimationObjectCurve(vector<Point> {PointHB[11], PointHB[12], PointHB[13]}, t, Bezier2);

	//������ ������ ����� 3 �������
	AnimationObjectCurve(vector<Point> {PointHB[14], PointHB[15], PointHB[16], PointHB[17]}, t, Bezier3);
	AnimationObjectCurve(vector<Point> {PointHB[18], PointHB[19], PointHB[20], PointHB[21]}, t, Bezier3);

}