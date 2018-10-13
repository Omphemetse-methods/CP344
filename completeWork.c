#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include "Classes.h"
#define KWHT  "\x1B[37m"
#define KRED  "\x1B[31m"
#define KBLU  "\x1B[34m"
#define RESET "\x1B[0m"
#define L 25
#define T 0.5

/* GLOBAL DECLARATIONS: */
int N = L*L;
int lat[L*L];
int XNN = 1;
int YNN = L;
double prob[5];                              // Flip probability for given temperature
double beta = 1.0 / T;                       // Inverse temperature 1/kT
int sweepsInt = 0;                           // sweep time
/* quantities i want to measure */
int Eu = 0;
int mag = 0;
double c = 0;
double Ti = T;
double x = 0;
/* files im writing data to  */
FILE * fm;
FILE * fc;
FILE * fx;

/* function declaration */
void init_lattice();
void display_latttice();
void initialize_prob();
void sweep();
void get_magnetization();
void get_energy();
void init_energy();
void get_data();
void get_magnetization();
void get_magentic_susceptibility();
void get_specific_heat();


/*********************************************************************/
/* Main Method */
void main() {

    init_lattice();
    initialize_prob();
    init_energy();

    // write data to files
    fm = fopen ("magnetization.csv", "w");
    fprintf(fm, "%s, %s, %s\n", "sweepsTime",  "magnetization", "Temparature");

    fc = fopen ("specif_heat.csv", "w");
    fprintf(fc, " %s, %s\n", "Temperature", "specif_heat");

    //fx = fopen ("magnetic_susceptibility.csv", "w");
    //fprintf(fx, " %s, %s\n", "Temperature", "specif_heat");

    /* run sweeps for couple of times */
    for (int i = 0; i < 1000; i++) {
        sweep();
    }

    // close all files
    fclose(fm);
    fclose(fc);

    // after writing to my magnetization.csv call python
    system("python handleData.py");

}

/*********************************************************************/
/* function for initialising my lattice */
void init_lattice() {

    /* specify the initial state of the lattice */
    int opt;
    printf("Enter initial state of opt  : ");
    scanf("%d",&opt);

    srand(time(0));

    /* set lattice states */
    for ( int i = 0; i < N; i++) {
        /* Depending on the initial temparature set set*/
        if ( opt == 1  ) {
            /* spin up */
            lat[i] = 1;
        } else if ( opt == -1) {
            /* spin down */
            lat[i] = -1;
        } else {
            /* random spin */
            int num = rand() % 10;
            if ( num < 5 ) {
                lat[i] = -1;
            } else {
                lat[i] = 1;
            }
        }
    };


}

/*********************************************************************/
void display_latttice() {
    printf("The Ising Model of ferromagntes \n");
    printf("\n");
    for ( int i = 0; i < N; i++) {
        if ( i % L == 0 && i != 0) printf("\n");
        if ( lat[i] == 1 ) printf(KRED " ↑ " RESET);
        if ( lat[i] == -1) printf(KBLU " ↓ " RESET);
    }
    printf(" \n");
    printf(" \n");

    get_magnetization();
    get_energy();
    get_specific_heat();
    // now incremet temperature
    Ti += 0.001;
    //get_magentic_susceptibility();

    /*
        magnetic susceptibility, specific heat,  magnetization, energy(Eu)
        all this quanties are availbale,
        get_data() write  them into their specific files
    */
    get_data();

    system("clear");
    usleep(50000);
}

/*********************************************************************/
void initialize_prob() {
  int i;
  for (i=2; i < 5; i += 2) prob[i] = exp(-2*beta*i);
}

/*********************************************************************/
void sweep() {
  int i, k;
  int nn,sum,delta;

  for (k = 0; k < N; k++) {

    /* Choose a site */
    /* You should replace drandom here and below with a random
       number generator that has been tested and performs
       satisfactorily */

    i = rand() % N;

    /* Calculate the sum of the neighbouring spins */
    if ((nn = i+XNN) >= N)   nn -= N;
        sum = lat[nn];
    if ((nn = i-XNN) <  0)   nn += N;
        sum += lat[nn];
    if ((nn = i+YNN) >= N)   nn -= N;
        sum += lat[nn];
    if ((nn = i-YNN) <  0)   nn += N;
        sum += lat[nn];

    /* Calculate the change in energy */
    delta = sum*lat[i];

    /* Decide whether to flip the spin */
    if (delta <= 0) {
      lat[i] = -lat[i];
  } else if (1.0 * rand() / RAND_MAX <  prob[delta]) {
      lat[i] = -lat[i];
    }
  }

  /* display my lattice */
  display_latttice(lat);

};

/*  initialize energy */
void init_energy() {
    int nn;
    for (int k = 0; k < N; k++) {
      int i = rand() % N;
      /* Calculate the sum of the neighbouring spins */
      if ((nn = i+XNN) >= N)   nn -= N;
        Eu += lat[nn]*lat[i];
      if ((nn = i-XNN) <  0)   nn += N;
        Eu += lat[nn]*lat[i];
      if ((nn = i+YNN) >= N)   nn -= N;
        Eu += lat[nn]*lat[i];
      if ((nn = i-YNN) <  0)   nn += N;
        Eu += lat[nn]*lat[i];
    }
}

/* get the Energy */
void get_energy() {
    int nn, sum, delta, Ev;
    for (int k = 0; k < N; k++) {
      int i = rand() % N;
      /* Calculate the sum of the neighbouring spins */
      if ((nn = i+XNN) >= N)   nn -= N;
        sum = lat[nn];
      if ((nn = i-XNN) <  0)   nn += N;
        sum += lat[nn];
      if ((nn = i+YNN) >= N)   nn -= N;
        sum += lat[nn];
      if ((nn = i-YNN) <  0)   nn += N;
        sum += lat[nn];

      /* Calculate the change in energy */
      delta = sum*lat[i];

      /* Decide whether to flip the spin */
      if (delta <= 0) {
        Eu += delta;
    } else if (1.0 * rand() / RAND_MAX <  prob[delta]) {
        Eu += delta;
      }
    }

    printf("Ev = Eu + delta: %d\n", Eu  );


}

/* get the magnetization */
void get_magnetization() {
    mag = 0;
    for ( int i = 0; i < N; i++) {
        mag = mag + lat[i];
    }
    printf("Magnetization: %f\n", 1.0*mag/N );
}

/* specific heat */
void get_specific_heat() {
    double E2 = pow(Eu, 2) / N;
    double E = pow(Eu /N, 2);
    c = (pow(beta, 2) / N )* ( E2 - E );
}

/* magnetic susceptibility */
void get_magentic_susceptibility() {

}

void get_data() {
    sweepsInt += 1;
    printf("time t:%d\n", sweepsInt );
    fprintf(fm, "%d, %f, %f\n", sweepsInt,  1.0*mag/N, Ti );

    fprintf(fc, "%f, %f\n", Ti, c );
}
