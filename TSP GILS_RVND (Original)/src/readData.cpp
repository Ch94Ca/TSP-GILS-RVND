#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cmath>

using namespace std;

double CalcEucDist ( double *X, double *Y, int I, int J );
double CalcAttDist ( double *X, double *Y, int I, int J );
void CalcLatLong ( double *X, double *Y, int n, double *latit, double* longit );
double CalcGeoDist ( double *latit, double *longit, int I, int J );

void readData( int argc, char** argv, int* Dimension, double ***Mdist )
{

     if (argc < 2) 
     {
         cout << "\nMissing Params\n";
         cout << " ./exec [Instance] "<< endl;
         exit(1);
     }

     if (argc > 2) 
     {
          cout << "\nToo many params\n";
          cout << " ./exec [Instance] " << endl;
         exit(1);
     }

    int N;
    string arquivo, ewt;

     char *instancia;
     instancia = argv[1];

    ifstream in( instancia, ios::in);

	if (!in) 
    {
		cout << "File Cannot be opened\n";
		exit(1);
    }

    while ( arquivo.compare("DIMENSION:") != 0 && arquivo.compare("DIMENSION" ) != 0 ) 
    {
        in >> arquivo;
    }

    if ( arquivo.compare("DIMENSION" ) == 0 )  in >> arquivo;

    in >> N;

    while ( arquivo.compare("EDGE_WEIGHT_TYPE:") != 0 && arquivo.compare("EDGE_WEIGHT_TYPE" ) != 0 ) 
    {
        in >> arquivo;
    }

    if ( arquivo.compare("EDGE_WEIGHT_TYPE" ) == 0 )  in >> arquivo;

    in >> ewt;

    double *x = new double [N+1];
    double *y = new double [N+1];

    double **dist = new double*[N+1];

    for ( int i = 0; i < N+1; i++ ) 
    {
        dist [i] = new double [N+1];
    }

    if ( ewt == "EXPLICIT" ) 
    {

        while ( arquivo.compare("EDGE_WEIGHT_FORMAT:") != 0 && arquivo.compare("EDGE_WEIGHT_FORMAT" ) != 0 ) 
        {
            in >> arquivo;
        }

        string ewf;
        if ( arquivo.compare("EDGE_WEIGHT_FORMAT" ) == 0 )  in >> arquivo;
        in >> ewf;

        if ( ewf == "FUNCTION" ) 
        {
            cout << "FUNCTION - Not supported!" << endl; 
        }
        else if ( ewf == "FULL_MATRIX" ) 
        {

            while ( arquivo.compare("EDGE_WEIGHT_SECTION") != 0 ) 
            {
                in >> arquivo;
            }

            for ( int i = 1; i < N+1; i++ ) 
            {
                for ( int j = 1; j < N+1; j++ ) 
                {
                    in >> dist[i][j];
                }
            }
        }

        else if ( ewf == "UPPER_ROW" ) 
        {

            while ( arquivo.compare("EDGE_WEIGHT_SECTION") != 0 ) 
            {
                in >> arquivo;
            }

            for ( int i = 1; i < N; i++ ) 
            {
                for ( int j = i+1; j < N+1; j++ ) 
                {
                    in >> dist[i][j];
                    dist[j][i] = dist[i][j];
                }
            }

            for ( int i = 1; i < N+1; i++ ) 
            {
                dist[i][i] = 0;
            }

        }

        else if ( ewf == "LOWER_ROW" ) 
        {

            while ( arquivo.compare("EDGE_WEIGHT_SECTION") != 0 ) 
            {
                in >> arquivo;
            }

            for ( int i = 2; i < N+1; i++ ) 
            {
                for ( int j = 1; j < i; j++ ) 
                {
                    in >> dist[i][j];
                    dist[j][i] = dist[i][j];
                }
            }

            for ( int i = 1; i < N+1; i++ ) 
            {
                dist[i][i] = 0;
            }
        }

        else if ( ewf == "UPPER_DIAG_ROW" ) 
        {

            while ( arquivo.compare("EDGE_WEIGHT_SECTION") != 0 ) 
            {
                in >> arquivo;
            }

            for ( int i = 1; i < N+1; i++ ) 
            {
                for ( int j = i; j < N+1; j++ ) 
                {
                    in >> dist[i][j];
                    dist[j][i] = dist[i][j];
                }
            }
        }

        else if ( ewf == "LOWER_DIAG_ROW" ) 
        {

            while ( arquivo.compare("EDGE_WEIGHT_SECTION") != 0 ) 
            {
                in >> arquivo;
            }

            for ( int i = 1; i < N+1; i++ ) 
            {
                for ( int j = 1; j <= i; j++ ) 
                {
                    in >> dist[i][j];
                    dist[j][i] = dist[i][j];
                }
            }
        }

        else if ( ewf == "UPPER_COL" ) 
        {

            while ( arquivo.compare("EDGE_WEIGHT_SECTION") != 0 ) 
            {
                in >> arquivo;
            }

            for ( int j = 2; j < N+1; j++ ) 
            {
                for ( int i = 1; i < j; i++ ) 
                {
                    in >> dist[i][j];
                    dist[j][i] = dist[i][j];
                }
            }

            for ( int i = 1; i < N+1; i++ ) 
            {
                dist[i][i] = 0;
            }

        }

        else if ( ewf == "LOWER_COL" ) 
        {

            while ( arquivo.compare("EDGE_WEIGHT_SECTION") != 0 ) 
            {
                in >> arquivo;
            }

            for ( int j = 1; j < N; j++ ) 
            {
                for ( int i = j+1; i < N+1; j++ ) 
                {
                    in >> dist[i][j];
                    dist[j][i] = dist[i][j];
                }
            }

            for ( int i = 1; i < N+1; i++ ) 
            {
                dist[i][i] = 0;
            }

        }

        else if ( ewf == "UPPER_DIAG_COL" ) 
        {

            while ( arquivo.compare("EDGE_WEIGHT_SECTION") != 0 ) 
            {
                in >> arquivo;
            }

            for ( int j = 1; j < N+1; j++ ) 
            {
                for ( int i = 1; i <= j; i++ ) 
                {
                    in >> dist[i][j];
                    dist[j][i] = dist[i][j];
                }
            }
        }

        else if ( ewf == "LOWER_DIAG_COL" ) 
        {

            while ( arquivo.compare("EDGE_WEIGHT_SECTION") != 0 ) 
            {
                in >> arquivo;
            }

            for ( int j = 1; j < N+1; j++ ) 
            {
                for ( int i = j; i < N+1; j++ ) 
                {
                    in >> dist[i][j];
                    dist[j][i] = dist[i][j];
                }
            }
        }

    }

    else if ( ewt == "EUC_2D" ) 
    {

        while ( arquivo.compare("NODE_COORD_SECTION") != 0 ) 
        {
            in >> arquivo;
        }
        int tempCity;
        for ( int i = 1; i < N+1; i++ ) 
        {
            in >> tempCity >> x[i] >> y[i];
        }

        for ( int i = 1; i < N+1; i++ ) 
        {
            for ( int j = 1; j < N+1; j++ ) 
            {
                dist[i][j] = floor ( CalcEucDist ( x, y, i, j ) + 0.5 );
            }
        }
    }

    else if ( ewt == "EUD_3D" ) 
    {
        cout << "EUC_3D - Not supported!" << endl; 
    }
    else if ( ewt == "MAX_2D" ) 
    {
        cout << "MAX_2D - Not supported!" << endl; 
    }
    else if ( ewt == "MAX_3D" ) 
    {
        cout << "MAX_3D - Not supported!" << endl; 
    }
    else if ( ewt == "MAN_2D" ) 
    {
        cout << "MAN_2D - Not supported!" << endl; 
    }
    else if ( ewt == "MAN_3D" ) 
    {
        cout << "MAN_3D - Not supported!" << endl; 
    }
    else if ( ewt == "CEIL_2D" ) 
    {

        while ( arquivo.compare("NODE_COORD_SECTION") != 0 ) 
        {
            in >> arquivo;
        }
        int tempCity;
        for ( int i = 1; i < N+1; i++ ) 
        {
            in >> tempCity >> x[i] >> y[i];
        }

        for ( int i = 1; i < N+1; i++ ) 
        {
            for ( int j = 1; j < N+1; j++ ) 
            {
                dist[i][j] = ceil ( CalcEucDist ( x, y, i, j ) );
            }
        }
    }
    else if ( ewt == "GEO" ) 
    {
        while ( arquivo.compare("NODE_COORD_SECTION") != 0 ) 
        {
            in >> arquivo;
        }

        int tempCity;
        for ( int i = 1; i < N+1; i++ ) 
        {
            in >> tempCity >> x[i] >> y[i];
        }

        double *latitude = new double [N+1];
        double *longitude = new double [N+1];

        CalcLatLong ( x, y, N, latitude, longitude );

        for ( int i = 1; i < N+1; i++ ) 
        {
            for ( int j = 1; j < N+1; j++ ) 
            {
                dist[i][j] = CalcGeoDist ( latitude, longitude, i, j );
            }
        }
    }

    else if ( ewt == "ATT" ) 
    {
        while ( arquivo.compare("NODE_COORD_SECTION") != 0 ) 
        {
            in >> arquivo;
        }

        int tempCity;
        int *tempX = new int [N+1];
        int *tempY = new int [N+1];

        for ( int i = 1; i < N+1; i++ ) 
        {
            in >> tempCity >> tempX[i] >> tempY[i];
            x[i]=tempX[i];
            y[i]=tempY[i];
        }

        for ( int i = 1; i < N+1; i++ ) 
        {
            for ( int j = 1; j < N+1; j++ ) 
            {
                dist[i][j] = CalcAttDist ( x, y, i, j );
            }
        }
    }
    else if ( ewt == "XRAY1" ) 
    {
        cout << "XRAY1 - Not supported!" << endl; 
    }
    else if ( ewt == "XRAY2" ) 
    {
        cout << "XRAY2 - Not supported!" << endl; 
    }
    else if ( ewt == "SPECIAL" ) 
    {
        cout << "SPECIAL - Not supported!" << endl; 
    }
    *Dimension = N;
    *Mdist = dist;
}

double CalcEucDist ( double *X, double *Y, int I, int J )
{
    return
    sqrt ( pow ( X[I] - X[J], 2 ) + pow ( Y[I] - Y[J], 2 ) );
}

double CalcAttDist ( double *X, double *Y, int I, int J )
{
    double rij, tij, dij;

    rij = sqrt ( ( pow ( X[I] - X[J], 2 ) + pow ( Y[I] - Y[J], 2 ) ) / 10 );
    tij = floor ( rij + 0.5 );

    if ( tij < rij )
        dij = tij + 1;
    else
        dij = tij;

    return dij;
}

void CalcLatLong ( double *X, double *Y, int n, double *latit, double* longit )
{
    double PI = 3.141592, min;
    int deg;

    for ( int i = 1; i < n+1; i++ ) 
    {
        deg = (int) X[i];
        min = X[i] - deg;
        latit[i] = PI * (deg + 5.0 * min / 3.0 ) / 180.0;
    }

    for ( int i = 1; i < n+1; i++ ) 
    {
        deg = (int) Y[i];
        min = Y[i] - deg;
        longit[i] = PI * (deg + 5.0 * min / 3.0 ) / 180.0;
    }
}

double CalcGeoDist ( double *latit, double *longit, int I, int J )
{
    double q1, q2, q3, RRR = 6378.388;

    q1 = cos( longit[I] - longit[J] );
    q2 = cos( latit[I] - latit[J] );
    q3 = cos( latit[I] + latit[J] );

    return
    (int) ( RRR * acos( 0.5*((1.0+q1)*q2 - (1.0-q1)*q3) ) + 1.0);
}