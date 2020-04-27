#include "api_robot.h"

char* itoa(int value) {
    char * result;
    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= 10;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * 10)];
    } while ( value );

    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

int pow(int x, int y){
    if (y == 1)
        return x;
    return x * pow(x, y-1);
}

int sqrt(int y){
    int k = y/2;
    for (int j = 0; j < 10; j++){
        k = (k + (y/k))/2;
    }
    return k;
}

int abs(int x){
    if (x > 0)
        return x;
    return -x;
}

double continued_faction_arctan(int n, double x){
    double x2 = x*x;
    double d = n * 2 + 1;
    for (int k = n; k > 0; k--){
        double f = k * 2 - 1;
        d = f + k*k*x2/d;
    }
    return x / d;
}


double arctan(int n, double x){
    if (x > 1.0)
        return 3.141592653590/2.0 - continued_faction_arctan(n, 1/x);
    return continued_faction_arctan(n, x);
}

double getAngle(double x, double y){
    double i = arctan(20, x/y);    
    i = (i*180)/3.141592653590;
    if (x > 0 && y < 0)
        i+=180;
    else if (x < 0 && y < 0)
        i-=180;
    return i;
}

void print_coords(Vector3 p){
    int a = p.x;
    int b = p.y;
    int c = p.z;
    char s[20];
    puts("\nCoordenadas:\n");
    puts(itoa(a));
    puts(itoa(b));
    puts(itoa(c));
}

void print_angles(Vector3 a){
    int d = a.x;
    int e = a.y;
    int f = a.z;
    char s[20];
    puts("\nAngulos:\n");
    puts(itoa(d));
    puts(itoa(e));
    puts(itoa(f));

}

void avoid(Vector3 pos, Vector3 ang, int n, int friend, int a){
    int time;
    if (n == 0){
        if (ang.z >= 0){
            set_torque(0, 10);
            time = get_time();
            while (get_time() - time < 2000){}
            set_torque(10, 10);
            time = get_time();
            while (get_time() - time < 2000){}
        }
        else{
            set_torque(10, 0);
            time = get_time();
            while (get_time() - time < 2000){}
            set_torque(10, 10);
            time = get_time();
            while (get_time() - time < 2000){}
        }
    }
    else if (n == 1){
        set_torque(-20, -20);
        time = get_time();
        while (get_time() - time < 2000){}
        set_torque(10, 10);
        time = get_time();
        while (get_time() - time < 1000){}
        if ((ang.y >= 0 && a >= 0) || (ang.y <= 0 && a <= 0)){
            if (ang.y > a)
                set_torque(0, 20);
            else
                set_torque(20, 0);
            time = get_time();
            while (get_time() - time < 2500){}
        }
        else if (ang.y < 0 && a > 0){
            if (ang.y + 180 > a)
                set_torque(20, 0);
            else
                set_torque(0, 20);
            time = get_time();
            while (get_time() - time < 2500){}
        }
        else if (ang.y > 0 && a < 0){
            if (ang.y - 180 > a)
                set_torque(20, 0);
            else
                set_torque(0, 20);
            time = get_time();
            while (get_time() - time < 2500){}
        }
        set_torque(0, 0);
    }
    else if (n == 2){
        int b = getAngle((dangerous_locations[friend].x - pos.x), (dangerous_locations[friend].z - pos.z));
        if ((ang.y > 0 && b > 0) || (ang.y < 0 && b < 0)){
            if (ang.y >= b){
                set_torque(10, -10);
            }
            else if (ang.y < b){
                set_torque(-10, 10);
            }
        }
        else if (ang.y > 0 && b < 0){
            if (ang.y - 180 <= b){
                set_torque(10, -10);
            }
            else{
                set_torque(-10, 10);
            }
        }
        else if (a < 0 && b > 0){
            if (ang.y + 180 <= b){
                set_torque(-10, 10);
            }
            else{
                set_torque(10, -10);
            }
        time = get_time();
        while (get_time() - time < 2000);
        }
    set_head_servo(0, 31);
    time = get_time();
    while (get_time() - time < 100){}
    }
}


void keep_going(Vector3 pos, Vector3 ang, int friend, int cabeca, int a){
    int time;
    unsigned short dist = get_us_distance();
    char s[15];
    if ((ang.y > 0 && a > 0) || (ang.y < 0 && a < 0)){
        if (abs(ang.y - a) < 2)
            set_torque(10,10);
        else if (ang.y >= a)
            set_torque(-10, 10);
        else if (ang.y <= a)
            set_torque(10, -10);
    }
    else if (ang.y > 0 && a < 0){
        if (abs(ang.y - a) < 2)
            set_torque(10, 10);
        else if (ang.y - 180 == a){
            set_torque(10, -10);
            time = get_time();
            while (get_time() - time < 6000){}
        }
        else if (ang.y - 180 < a)
            set_torque(-10, 10);
        else if (ang.y -180 > a)
            set_torque(10, -10);
    }
    else if (ang.y < 0 && a > 0){
        if (abs(ang.y -a) < 1)
            set_torque(10, 10);
        else if (ang.y + 180 == a){
            set_torque(-10, 10);
            time = get_time();
            while (get_time() - time < 6000){}
        }
        else if (ang.y + 180 < a)
            set_torque(-10, 10);
        else if (ang.y + 180 > a)
            set_torque(10, -10);
    }
}


int main(){
    set_head_servo(2, 82);
    int cont = 0;
    char s[15];
    Vector3 pos;
    Vector3 ang;
    int cabeca = 78;
    int pin = 0;
    get_current_GPS_position(&pos);
    set_torque(-10,-10);
    int time = get_time();
    while (get_time() - time < 3000){}
    for (int i = 0; i < 5; i++){
            while ((abs(friends_locations[i].x - pos.x) > 5) || (abs(friends_locations[i].y - pos.y) > 5) || (abs(friends_locations[i].z - pos.z) > 5)){
            get_gyro_angles(&ang);
            if (ang.x > 180)
                ang.x-=360;
            if (ang.y > 180)
                ang.y-=360;
            if (ang.z > 180)
                ang.z-=360;
            if (cabeca == 72)
                cont = 0;
            else if (cabeca == 84)
                cont = 1;
            if (cont == 0)
                cabeca+=2;
            else if (cont == 1)
                cabeca-=2;   
            set_head_servo(2, cabeca);    
            puts(itoa(get_time()/1000));
            int a = getAngle((friends_locations[i].x - pos.x), (friends_locations[i].z - pos.z));
            if (abs(ang.x) >= 5 && abs(ang.z) >= 5)
                avoid(pos, ang, 0, 0, a);
            else if (abs(ang.x) < 5 && abs(ang.z) >= 5)
                a = ang.y;
            unsigned short d1;
            if (d1 = get_us_distance() < 400 && d1 != -1 && abs(ang.x) < 5)
                avoid(pos, ang, 1, 0, a);
            for (int j = 0; j < 5; j++){
                if (abs(dangerous_locations[j].x - pos.x) < 10 && abs(dangerous_locations[j].y - pos.y) < 10 && abs(dangerous_locations[j].z - pos.z) < 10)
                    avoid(pos, ang, 2, i, a);
            }
            get_current_GPS_position(&pos);
            keep_going(pos, ang, i, cabeca, a);
        }
        friends_locations[i].x = 1000000;
        friends_locations[i].y = 1000000;
        friends_locations[i].z = 1000000;
    }
    set_torque(0,0);
    while(1){} // Deixa esse while para nao travar
}