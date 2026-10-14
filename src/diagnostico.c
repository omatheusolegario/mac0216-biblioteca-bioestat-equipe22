double sensibilidade(int vp, int fn)
{
    double sens = vp;
    return sens/(vp+fn);
}

double especificidade(int vn, int fp)
{
    double esp = vn;
    return esp/(vn+fp);
}

double vpp(int vp, int fp)
{
    double vpp = vp;
    return vpp/(vp+fp);
}

double vpn(int vn, int fn)
{
    double vpn = vn;
    return vpn/(vn+fn);
}