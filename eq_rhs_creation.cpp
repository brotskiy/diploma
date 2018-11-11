#include "eq_rhs_creation.h"

double intSin2D(const int ik, const double bnd)
{
  return bnd/2 - bnd * sin(2 * MY_PI * ik) / (4 * MY_PI * ik);
}

double intCosSin(const int i, const int ik, const double bnd)
{
  if (i == ik)
    return bnd * sin(MY_PI * ik) * sin(MY_PI * ik) / (2 * MY_PI * ik);
  else
  {
    double pi = i * MY_PI;
    double pik = ik * MY_PI;

    return bnd * (-ik + i*sin(pi)*sin(pik) + ik*cos(pi)*cos(pik)) / (MY_PI * (i*i - ik*ik));  // унарный минус!!
  }
}

double intSinSin(const int i, const int ik, const double bnd)
{
  if (i == ik)
    return bnd/2 - bnd * sin(2 * MY_PI * ik) / (4 * MY_PI * ik);
  else
  {
    double pi = i * MY_PI;
    double pik = ik * MY_PI;

    return bnd * (-i*cos(pi)*sin(pik) + ik*sin(pi)*cos(pik)) / (MY_PI * (i*i - ik*ik));
  }
}

double intSub(const int i, const int ii, const double a)
{
  return -a / (4*MY_PI*ii*i*(i - ii)) * (sin(2*ii*MY_PI)*(0.5*i*i - ii*i) - ii*ii*MY_PI*i + 0.5*ii*sin(2*i*MY_PI) - sin(2*i*MY_PI)*cos(ii*MY_PI)*cos(ii*MY_PI)*ii*i + cos(i*MY_PI)*cos(i*MY_PI)*sin(2*ii*MY_PI)*ii*i + ii*MY_PI*i*i);
}

double intSinCosSin(const int i, const int ii, const int ik, const double a)
{
  if ((ik != i + ii) && (ik != ii - i) && (ik != i - ii))
  {
    double dnm = ik*ik*ik*ik - 2*ik*ik*i*i + i*i*i*i - 2*ik*ik*ii - 2*ii*ii*i*i + ii*ii*ii*ii;

    double pik = ik * MY_PI;
    double pi = i * MY_PI;
    double pii = ii * MY_PI;

    return -a / (MY_PI*dnm) * (2*ik*ii*i*cos(pik)*cos(pi)*sin(pii) + cos(pik)*sin(pi)*cos(pii)*(ik*ik*ik - ik*ii*ii - i*i*ik) + sin(pik)*cos(pi)*cos(pii)*(i*i*i - ii*ii*i - ik*ik*i) + sin(pik)*sin(pi)*sin(pii)*(ik*ik*ii - ii*ii*ii + i*i*ii));
  }

  if (ik == i + ii)
    return -a / (4*MY_PI*ii*i*(ii + i)) * (sin(2*i*MY_PI)*cos(ii*MY_PI)*cos(ii*MY_PI)*ii*i + cos(i*MY_PI)*cos(i*MY_PI)*sin(2*ii*MY_PI)*ii*i + sin(2*ii*MY_PI)*(-0.5*i*i - ii*i) + 0.5*ii*ii*sin(2*i*MY_PI) - ii*i*i*MY_PI - ii*ii*i*MY_PI);

  if (ik == ii - i)
  {
    if (i != ii)
      return intSub(i, ii, a);
    else
      return 0;
  }

  if (ik == i - ii)
  {
    if (i != ii)
      return -1 * intSub(i, ii, a);
   else
      return 0;
  }
}
