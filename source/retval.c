#include <stdio.h>

#define FAKE_BATTER "Batter1234 1 2 3  4   5  6  7  8  9   0   1   2  3  4  5"\
"  6    7"
#define REAL_BATTER "RodriguezI 4 0 0  0   0  2            1"
#define SPEC_1 "%10s %d %d %2d %3d %2d %2d %2d %2d %3d %3d %3d %2d %2d %2d"\
               " %2d %4d"
#define SPEC_2 "%10s %d %d  %d   %d  %d  %d  %d  %d   %d   %d   %d  %d  %d"\
               "  %d  %d    %d"

int main(void)
{

		if( ( 1.3 - ((int)1.3) ))
		  printf("It does.\n");
		else
			printf("It doesn't.\n");
		printf("%f\n", (1.3 - ((int)1.3)));

//		printf("atoi(\"NULL\") = %d\n", atoi("\0\0\0"));

//	char name[80];
//	int scanret, runs=0, hits=0, rbis=0, walks=0, strike_outs=0, doubles=0;
//	int homers=0, gdps=0, int_walks=0, beans=0, sac_hits=0, sac_flies=0;
//	int steals=0, caught_steals=0, afdi=0, triples=0;
//	
//	scanret =
//	sscanf(FAKE_BATTER, SPEC_2,
//	name, &runs, &hits, &rbis, &walks, &strike_outs,
//	&doubles, &triples,	&homers, &gdps, &int_walks, &beans,
//	&sac_hits, &sac_flies, &steals, &caught_steals,	&afdi);
//	
//	printf("scanret = %d out of 17\n", scanret);
//	
//	printf("\nPlayer: %s\n", name);
//	printf("Runs: %d\n", runs);
//	printf("Hits: %d\n", hits);
//	printf("Doubles: %d\n", doubles);
//	printf("Triples: %d\n", triples);
//	printf("Home runs: %d\n", homers);
//	printf("Runs batted in: %d\n", rbis);
//	printf("Strike outs: %d\n", strike_outs);
//	printf("Walks: %d\n", walks);
//	printf("Intentional walks: %d\n", int_walks);
//	printf("Hit by pitches: %d\n", beans);
//	printf("Sacrifice hits: %d\n", sac_hits);
//	printf("Sacrifice flies: %d\n", sac_flies);
//	printf("Ground into double plays: %d\n", gdps);
//	printf("Bases stolen: %d\n", steals);
//	printf("Caught stealing: %d\n", caught_steals);
//	printf("Award first on defensive interferences: %d\n", afdi);
	
	return 0;
}