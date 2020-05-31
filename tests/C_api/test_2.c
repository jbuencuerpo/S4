#include <S4.h>
#include <math.h>

int main(int argc, char *argv[]){
	double freq = 1;
	if(argc > 1){
		freq = atof(argv[1]);
	}

	double eps[2];
    unsigned long N;
    N = 5;
	Simulation *S = (Simulation*)malloc(sizeof(Simulation));
    S->options.verbosity = 9;
	Simulation_Init(S);
	S->Lr[0] = 1;
	S->Lr[1] = 0;
	S->Lr[2] = 0;
	S->Lr[3] = 0;
	Simulation_MakeReciprocalLattice(S);
	Simulation_SetNumG(S, N);


	Material *M_air = Simulation_AddMaterial(S);
	eps[0] = 1; eps[1] = 0;
	Material_Init(M_air, "air", eps);
	Material *M_Si = Simulation_AddMaterial(S);
	eps[0] = 12; eps[1] = 0;
	Material_Init(M_Si, "Si", eps);

	Layer *L1 = Simulation_AddLayer(S);
	Layer *L2 = Simulation_AddLayer(S);
	Layer *L3 = Simulation_AddLayer(S);
	Layer *L4 = Simulation_AddLayer(S);
	Layer *L5 = Simulation_AddLayer(S);

	Layer_Init(L1, "l0", 0, "air", NULL);
	Layer_Init(L2, "l1", 0.5, "Si", NULL);
	Layer_Init(L3, "l2", 0.0, NULL, "Si");
	Layer_Init(L4, "l3", 0.5, NULL, "Si");
	Layer_Init(L5, "l4", 0, NULL, "air");
	//double center[2] = {0,0};
	//Simulation_AddLayerPatternCircle(S, L2, 0, center, 0.2);


	double k[2], pos[2], momentum[6]; 
	k[0] = 1;
	k[1] = 1;
	pos[0] = 0; 
    pos[1] = 0;

    for (int i=0; i<6; i++){
        momentum[i] = 0;
    }
    momentum[0] = 100.;
    //Where is the z??
	Simulation_MakeExcitationDipole(S, k, "l3",  pos, momentum);
	S->omega[0] = 2*3.1416*freq;
	S->omega[1] = 0;

	double powers[4];
	Simulation_GetPoyntingFlux(S, L3, 0, powers);
	printf("%g\t%g\n", freq, powers[0]);
    double r[3]; 
    r[0] = 0.1;
    r[1] = 0.2;
    r[2] = 0.3;
    double *Em;
    double *Hm;
    double E[6], H[6];
    double Et[6], Ht[6];
    Et[0] = 0.0; 
    Et[1] = 0.0; 
    Et[2] = 0.0; 
    Et[3] = 0.0; 
    Et[4] = 0.0; 
    Et[5] = 0.0; 
    Ht[0] = 0.0; 
    Ht[1] = 0.0; 
    Ht[2] = 0.0; 
    Ht[3] = 0.0; 
    Ht[4] = 0.0; 
    Ht[5] = 0.0; 

    Em = (double*)malloc(sizeof(double) *2*3 * N);
    Hm = (double*)malloc(sizeof(double) *2*3 * N);
//
    Simulation_GetFieldByG(S, r, Em, Hm);
    Simulation_GetField(S, r, E, H);

    for (int i=0; i<N; i++){
        for (int j=0; j<6; j++){
            //printf("%i\n", i*6+j);
            Et[j] += Em[i*6+j];
            Ht[j] += Hm[i*6+j];
        }
    }
	printf("%e\t%e\t%e\t\n", E[0], E[1], E[2]);
	printf("%e\t%e\t%e\t\n", Em[0], Em[1], Em[2]);
	printf("%e\t%e\t%e\t\n", Et[0], Et[1], Et[2]);
	Simulation_Destroy(S);
	free(S);
	return 0;
}