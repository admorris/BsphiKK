class Bs2PhiKKAcceptance
{
  public:
    Bs2PhiKKAcceptance(double,double);
    Bs2PhiKKAcceptance(const Bs2PhiKKAcceptance&);
    ~Bs2PhiKKAcceptance();
    double Evaluate(double,double,double,double);
  protected:
    double**** c;
    void createcoefficients();
    double mKK_min;
    double mKK_max;
    static int l_max;
    static int i_max;
    static int k_max;
    static int j_max;
};
