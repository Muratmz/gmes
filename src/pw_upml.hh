/* This implementation is based on the following article.
 * S. Gedney, "Perfectly Matched Layer Absorbing Boundary Conditions,"
 * Computational Electrodynamics: The Finite-Difference Time-Domain Method,
 * Third Edition, A. Taflove and S.C. Hagness, Artech House Publishers,
 * 2005, pp. 273-328.
 */

#ifndef PW_UPML_HH_
#define PW_UPML_HH_

#include "pw_material.hh"

#define ex(i,j,k) ex[ex_y_size==1?0:((i)*ex_y_size+(j))*ex_z_size+(k)]
#define ey(i,j,k) ey[ey_z_size==1?0:((i)*ey_y_size+(j))*ey_z_size+(k)]
#define ez(i,j,k) ez[ez_x_size==1?0:((i)*ez_y_size+(j))*ez_z_size+(k)]
#define hx(i,j,k) hx[hx_y_size==1?0:((i)*hx_y_size+(j))*hx_z_size+(k)]
#define hy(i,j,k) hy[hy_z_size==1?0:((i)*hy_y_size+(j))*hy_z_size+(k)]
#define hz(i,j,k) hz[hz_x_size==1?0:((i)*hz_y_size+(j))*hz_z_size+(k)]

namespace gmes
{
  template <typename T> class UpmlElectric: public MaterialElectric<T>
  {
  public:
    UpmlElectric(double epsilon,
		 double c1_in, double c2_in, double c3_in, double c4_in,
		 double c5_in, double c6_in):
      eps(epsilon),
      c1(c1_in), c2(c2_in), c3(c3_in), c4(c4_in), c5(c5_in), c6(c6_in), d(0)
    {
    }

    double get_epsilon() const
    {
      return eps;
    }

    void set_epsilon(double epsilon)
    {
      eps = epsilon;
    }

  protected:
    double eps;
    double c1, c2, c3, c4, c5, c6;
    T d;
  };

  template <typename T> class UpmlEx: public UpmlElectric<T>
  {
  public:
    UpmlEx(double epsilon, double c1,
	   double c2, double c3, double c4, double c5, double c6):
      UpmlElectric<T>(epsilon, c1, c2, c3, c4, c5, c6)
    {
    }

    T update(T * const ex, int ex_x_size, int ex_y_size, int ex_z_size,
	     const T * const hz, int hz_x_size, int hz_y_size, int hz_z_size,
	     const T * const hy, int hy_x_size, int hy_y_size, int hy_z_size,
	     double dy, double dz, double dt, double n, int i, int j, int k)
    {
      const T dstore(d);

      d = c1 * d + c2 * ((hz(i+1,j+1,k) - hz(i+1,j,k)) / dy - 
			 (hy(i+1,j,k+1) - hy(i+1,j,k)) / dz);
      ex(i,j,k) = c3 * ex(i,j,k) + c4 * (c5 * d - c6 * dstore) / eps;

      return ex(i,j,k);
    }

  protected:
    using UpmlElectric<T>::eps;
    using UpmlElectric<T>::c1;
    using UpmlElectric<T>::c2;
    using UpmlElectric<T>::c3;
    using UpmlElectric<T>::c4;
    using UpmlElectric<T>::c5;
    using UpmlElectric<T>::c6;
    using UpmlElectric<T>::d;
  };

  template <typename T> class UpmlEy: public UpmlElectric<T>
  {
  public:
    UpmlEy(double epsilon, double c1,
	   double c2, double c3, double c4, double c5, double c6):
      UpmlElectric<T>(epsilon, c1, c2, c3, c4, c5, c6)
    {
    }

    T update(T * const ey, int ey_x_size, int ey_y_size, int ey_z_size,
	     const T * const hx, int hx_x_size, int hx_y_size, int hx_z_size,
	     const T * const hz, int hz_x_size, int hz_y_size, int hz_z_size,
	     double dz, double dx, double dt, double n, int i, int j, int k)
    {
      const T dstore(d);

      d = c1 * d + c2 * ((hx(i,j+1,k+1) - hx(i,j+1,k)) / dz - 
			 (hz(i+1,j+1,k) - hz(i,j+1,k)) / dx);
      ey(i,j,k) = c3 * ey(i,j,k) + c4 * (c5 * d - c6 * dstore) / eps;

      return ey(i,j,k);
    }

  protected:
    using UpmlElectric<T>::eps;
    using UpmlElectric<T>::c1;
    using UpmlElectric<T>::c2;
    using UpmlElectric<T>::c3;
    using UpmlElectric<T>::c4;
    using UpmlElectric<T>::c5;
    using UpmlElectric<T>::c6;
    using UpmlElectric<T>::d;
  };

  template <typename T> class UpmlEz: public UpmlElectric<T>
  {
  public:
    UpmlEz(double epsilon, double c1,
	   double c2, double c3, double c4, double c5, double c6):
      UpmlElectric<T>(epsilon, c1, c2, c3, c4, c5, c6)
    {
    }

    T update(T * const ez, int ez_x_size, int ez_y_size, int ez_z_size,
		const T * const hy, int hy_x_size, int hy_y_size, int hy_z_size,
		const T * const hx, int hx_x_size, int hx_y_size, int hx_z_size,
		double dx, double dy, double dt, double n, int i, int j, int k)
    {
      const T dstore(d);

      d = c1 * d + c2 * ((hy(i+1,j,k+1) - hy(i,j,k+1)) / dx - 
			 (hx(i,j+1,k+1) - hx(i,j,k+1)) / dy);
      ez(i,j,k) = c3 * ez(i,j,k) + c4 * (c5 * d - c6 * dstore) / eps;
      
      return ez(i,j,k);
    }

  protected:
    using UpmlElectric<T>::eps;
    using UpmlElectric<T>::c1;
    using UpmlElectric<T>::c2;
    using UpmlElectric<T>::c3;
    using UpmlElectric<T>::c4;
    using UpmlElectric<T>::c5;
    using UpmlElectric<T>::c6;
    using UpmlElectric<T>::d;
  };

  template <typename T> class UpmlMagnetic: public MaterialMagnetic<T>
  {
  public:
    UpmlMagnetic(double mu,
		 double c1_in, double c2_in, double c3_in, double c4_in,
		 double c5_in, double c6_in):
      mu(mu),
      c1(c1_in), c2(c2_in), c3(c3_in), c4(c4_in), c5(c5_in), c6(c6_in), b(0)
    {
    }

    double get_mu() const
    {
      return mu;
    }

    void set_mu(double mu)
    {
      this->mu = mu;
    }

  protected:
    double mu;
    double c1, c2, c3, c4, c5, c6;
    T b;
  };

  template <typename T> class UpmlHx: public UpmlMagnetic<T>
  {
  public:
    UpmlHx(double mu, double c1, double c2,
	   double c3, double c4, double c5, double c6):
      UpmlMagnetic<T>(mu, c1, c2, c3, c4, c5, c6)
    {
    }

    T update(T * const hx, int hx_x_size, int hx_y_size, int hx_z_size,
	     const T * const ez, int ez_x_size, int ez_y_size, int ez_z_size,
	     const T * const ey, int ey_x_size, int ey_y_size, int ey_z_size,
	     double dy, double dz, double dt, double n, int i, int j, int k)
    {
      const T bstore(b);

      b = c1 * b - c2 * ((ez(i,j,k-1) - ez(i,j-1,k-1)) / dy - 
			 (ey(i,j-1,k) - ey(i,j-1,k-1)) / dz);
      hx(i,j,k) = c3 * hx(i,j,k) + c4 * (c5 * b - c6 * bstore) / mu;

      return hx(i,j,k);
    }

  protected:
    using UpmlMagnetic<T>::mu;
    using UpmlMagnetic<T>::c1;
    using UpmlMagnetic<T>::c2;
    using UpmlMagnetic<T>::c3;
    using UpmlMagnetic<T>::c4;
    using UpmlMagnetic<T>::c5;
    using UpmlMagnetic<T>::c6;
    using UpmlMagnetic<T>::b;
  };

  template <typename T> class UpmlHy: public UpmlMagnetic<T>
  {
  public:
    UpmlHy(double mu, double c1, double c2,
	   double c3, double c4, double c5, double c6):
      UpmlMagnetic<T>(mu, c1, c2, c3, c4, c5, c6)
    {
    }

    T update(T * const hy, int hy_x_size, int hy_y_size, int hy_z_size,
	     const T * const ex, int ex_x_size, int ex_y_size, int ex_z_size,
	     const T * const ez, int ez_x_size, int ez_y_size, int ez_z_size,
	     double dz, double dx, double dt, double n, int i, int j, int k)
    {
      const T bstore(b);

      b = c1 * b - c2 * ((ex(i-1,j,k) - ex(i-1,j,k-1)) / dz - 
			 (ez(i,j,k-1) - ez(i-1,j,k-1)) / dx);
      hy(i,j,k) = c3 * hy(i,j,k) + c4 * (c5 * b - c6 * bstore) / mu;

      return hy(i,j,k);
    }

  protected:
    using UpmlMagnetic<T>::mu;
    using UpmlMagnetic<T>::c1;
    using UpmlMagnetic<T>::c2;
    using UpmlMagnetic<T>::c3;
    using UpmlMagnetic<T>::c4;
    using UpmlMagnetic<T>::c5;
    using UpmlMagnetic<T>::c6;
    using UpmlMagnetic<T>::b;
  };

  template <typename T> class UpmlHz: public UpmlMagnetic<T>
  {
  public:
    UpmlHz(double mu, double c1, double c2,
	   double c3, double c4, double c5, double c6):
      UpmlMagnetic<T>(mu, c1, c2, c3, c4, c5, c6)
    {
    }

    T update(T * const hz, int hz_x_size, int hz_y_size, int hz_z_size,
	     const T * const ey, int ey_x_size, int ey_y_size, int ey_z_size,
	     const T * const ex, int ex_x_size, int ex_y_size, int ex_z_size,
	     double dx, double dy, double dt, double n, int i, int j, int k)
    {
      const T bstore(b);

      b = c1 * b - c2 * ((ey(i,j-1,k) - ey(i-1,j-1,k)) / dx - 
			 (ex(i-1,j,k) - ex(i-1,j-1,k)) / dy);
      hz(i,j,k) = c3 * hz(i,j,k) + c4 * (c5 * b - c6 * bstore) / mu;
      
      return hz(i,j,k);
    }

  protected:
    using UpmlMagnetic<T>::mu;
    using UpmlMagnetic<T>::c1;
    using UpmlMagnetic<T>::c2;
    using UpmlMagnetic<T>::c3;
    using UpmlMagnetic<T>::c4;
    using UpmlMagnetic<T>::c5;
    using UpmlMagnetic<T>::c6;
    using UpmlMagnetic<T>::b;
  };
}

#undef ex
#undef ey
#undef ez
#undef hx
#undef hy
#undef hz

#endif /*PW_UPML_HH_*/
