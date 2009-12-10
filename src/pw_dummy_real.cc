#include "pw_dummy_real.hh"
#include "constants.hh"

#define inplace_field(i,j,k) inplace_field[((i)*inplace_dim2+(j))*inplace_dim3+(k)]

using namespace gmes;

void DummyElectricReal::update(double * const inplace_field, int inplace_dim1, int inplace_dim2, int inplace_dim3,
		const double * const in_field1, int in1_dim1, int in1_dim2, int in1_dim3,
		const double * const in_field2, int in2_dim1, int in2_dim2, int in2_dim3,
		double d1, double d2, double dt, double t)
{
	return;
}

void DummyMagneticReal::update(double * const inplace_field, int inplace_dim1, int inplace_dim2, int inplace_dim3,
		const double * const in_field1, int in1_dim1, int in1_dim2, int in1_dim3,
		const double * const in_field2, int in2_dim1, int in2_dim2, int in2_dim3,
		double d1, double d2, double dt, double t)
{
	return;
}
