class Matrix{
public:
    int h_size;
    int w_size;
    double **mat;

    Matrix(int w=1, int h=1);
    Matrix(const Matrix &cp);
    ~Matrix();
    void reset();
};