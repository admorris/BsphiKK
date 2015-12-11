class Bs2PhiKKAcceptance
{
  public:
    Bs2PhiKKAcceptance();
    Bs2PhiKKAcceptance(const Bs2PhiKKAcceptance&);
    ~Bs2PhiKKAcceptance();
    double Evaluate(double,double,double,double);
    static double mKK_min;
    static double mKK_max;
  protected:
    double**** c;
    void createcoefficients();
    static int l_max;
    static int i_max;
    static int k_max;
    static int j_max;
};
