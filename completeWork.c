#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#define GRN   "\x1B[32m"
#define BLU   "\x1B[34m"
#define KYEL  "\x1B[33m"
#define RESET "\x1B[0m"
#define L 20
#define T 2.5

/* GLOBAL DECLARATIONS: */
int N = L*L;
int lat[L*L];
int XNN = 1;
int YNN = L;
double prob[5];            // Flip probability for given temperature
double beta = 1.0 / T;     // Inverse temperature 1/kT
int Eu = 0;                // initil energy of the sysetm
int mag = 0;
FILE * fp;
int sweepsInt = 0;


/* function declaration */
void init_lattice();
void display_latttice();
void initialize_prob();
void sweep();
void get_magnetization();
void get_energy();
void init_energy();
void get_data();


/*********************************************************************/
/* Main Method */
void main() {

    init_lattice();
    initialize_prob();
    init_energy();

    fp = fopen ("magnetization.csv", "w");
    fprintf(fp, "%s, %s\n", "sweepsTime",  "magnetization");

    /* run sweeps for couple of times */
    for (int i = 0; i < 1000; i++) {
        sweep();
    }

    fclose(fp);
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
        if ( lat[i] == 1 ) printf(KYEL " ↑ " RESET);
        if ( lat[i] == -1) printf(BLU " ↓ " RESET);
    }
    printf(" \n");
    printf(" \n");

    get_magnetization();
    get_energy();

    get_data();

    system("clear");
    usleep(90000);
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

/**************************************************
 initialize energy
 *******************************************/
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

/**************************************************
    get the Energy
***************************************************/
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

/**************************************************
    get the magnetization
***************************************************/
void get_magnetization() {
    mag = 0;
    for ( int i = 0; i < N; i++) {
        mag = mag + lat[i];
    }
    printf("Magnetization: %f\n", 1.0*mag/N );
}


void get_data() {
    sweepsInt += 1;
    fprintf(fp, "%d, %f\n", sweepsInt,  1.0*mag/N );
}
