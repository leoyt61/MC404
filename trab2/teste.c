#include <stdio.h>

int pot(int x, int y){
    if (y == 1)
        return x;
    return x * pot(x, y-1);
}


double arctan(int n, double x){
    if (n == 0){
        if (x >= -1 && x <= 1)
            return x;
        else
            return 1/x;
    }
    else{
        if (x >= -1 && x <= 1)
            return pot(-1, n)*(pot(x, 2*n + 1))/(2*n + 1) + arctan(n-1, x);
        else
            return pot(-1, n)/((2*n + 1)*pot(-1, 2*n + 1)) + arctan(n-1, x);    
    }
}

int main(){
    double x = 35;
    double y = -19;
    double i = arctan(500, x/y);    
    i = (i*180)/3.141592653590;
    if (i >= 1)
        i = 90 - i;
    else if (i <= -1)
        i = -90 - i;
    printf("%lf", i);
}