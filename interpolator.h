#ifndef IBN_INTERPOLATOR_H
#define IBN_INTERPOLATOR_H

class linear_interpolator
{
  std::vector<double> X;
  std::vector<double> Y;
  public:
    linear_interpolator(int n, double * x, double *y)
    {
      X.resize(n);
      Y.resize(n);
      for(int i=0;i<n;i++)
      {
        X[i] = x[i];
        Y[i] = y[i];
        std::cout << x[i] << " " << y[i] << std::endl;
      }
    }
    linear_interpolator(const std::vector<double> & x, const std::vector<double> &y)
    {
      X = x;
      Y = y;
    }
    linear_interpolator(const std::list<double> & x, const std::list<double> &y)
    {
      X.resize(x.size());
      Y.resize(y.size());
      int i=0; 
      for(std::list<double>::const_iterator it=x.begin(); it!=x.end(); it++) X[i++]=*it;
      i=0;
      for(std::list<double>::const_iterator it=y.begin(); it!=y.end(); it++) Y[i++]=*it;
    }

    double operator()(double x)
    {
      if(x<=X.front()) return Y.front();
      if(x>=X.back()) return Y.back();
      unsigned i=X.size()-1;
      while(X[i]>x) i--;
      double x1=X[i];
      double y1=Y[i];
      double x2=X[i+1];
      double y2=Y[i+1];
      return y1+(x-x1)*(y2-y1)/(x2-x1);
    }

    double get(double x)
    {
      int a=0;
      int b=X.size()-1;
      if(x<=X[a]) return Y[a];
      if(x>=X[b]) return Y[b];
      while((a+1)!=b)
      {
        int c = (a+b)/2;
        if(X[c]==x || X[c]==x) return Y[c];
        if(X[c]<x) a=c;
        if(X[c]>x) b=c;
      }
      return Y[a]+(x-X[a])*(Y[a]-Y[b])/(X[b]-X[a]);
    }

    double get2(double x, int &a, int&b)
    {
      if(a+1!=b)
      {
        int c= (a+b)/2;
        if(X[c]<x) get2(x,c,b);
        else       get2(x,a,c);
      }
    }


    double operator()(double x, double y0, double dy)
    {
      if(x<=X.front()) return Y.front();
      if(x>=X.back()) return Y.back();
      unsigned i=X.size()-1;
      while(X[i]>x) i--;
      double x1=X[i];
      double y1=Y[i];
      double x2=X[i+1];
      double y2=Y[i+1];
      if(fabs(y1-y2)>dy)
      {
        if(fabs(y0-y1)<dy) return y1;
        if(fabs(y0-y2)<dy) return y2;
      }
      return y1+(x-x1)*(y2-y1)/(x2-x1);
    }
};


double lin(double x, int N, double *X)
{
  int a=0;
  int b=N-1;
  if(x<=X[a]) return X[a];
  if(x>=X[b]) return X[b];
  while((a+1)!=b)
  {
    int c = (a+b)/2;
    if(X[c]==x) return x;
    if(X[c]<x) a=c;
    if(X[c]>x) b=c;
  }
  return a+(x-X[a])/(X[b]-X[a]);
}
#endif
