#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 64

int colors[64][64*3];

// Main task/function used by each child processes; paints a given row of the
// color canvas by writing rgb color by number in range 0-255.
// returns if out of canvas bounds
void paint(int workID){
        if (workID < 0 || workID > 63){
                printf("Trying to paint out of bounds\n");
                return;
        }
        printf("Artist %d is painting\n",workID);

        for(int i =0; i < 64*3; i++){
                colors[workID][i] = rand()%255;
        }
}

// opens and writes the values stored in 2d color array to a new ppm file
// that can be translated into an image
void write_ppm(){
        FILE *fp;
        fp = fopen("threads.ppm","w+");

        fputs("P3\n",fp);
        fputs("64 64\n",fp);
        fputs("255\n",fp);

        for(int i =0; i < 64;i++){
                for(int j =0; j < 64*3; j++){
                        fprintf(fp,"%d",colors[i][j]);
                        fputs(" ",fp);          
                }
                fputs("\n",fp);
        }

        fclose(fp);        
}

// main point of program entry for parent thread call; creates 64 other threads all
// of which use paint function to write to canvas and create an image.
int main(int argc, char** argv){

        pthread_t tids[NTHREADS];

        for(int i =0; i < NTHREADS; i++){
                pthread_create(&tids[i], NULL, paint, i);
                printf("Thread %d created \n", tids[i]);
        }


        for(int i =0; i < NTHREADS; i++){
                pthread_join(tids[i], NULL);
                printf("Thread %d joined (terminated) \n", tids[i]);
        }

        printf("Masterpiece(vfork.ppm) is being assembled\n");
        write_ppm();

        return 0;
}

