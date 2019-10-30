
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

// Global array
// This is our 'canvas' we are painting on.
// It is 64 rows by 64 columns (each pixel stores 3 color components, R,G,B)
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
        fp = fopen("vfork.ppm","w+");

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

// main point of entry for parennt process; uses vfork to create
// 64 child processes that each paint to pre-initialized canvas.
int main(int argc, char** argv){

	int* integers = malloc(sizeof(int)*8000);
        int numberOfArtists = 64;
        pid_t pid;

        for(int i =0; i < numberOfArtists; i++){
                pid = vfork();

                if(pid==0){
                        paint(i);
                        exit(0);
                }


                printf("Child created: %d\n",pid);
        }


        printf("Masterpiece(vfork.ppm) is being assembled\n");

        write_ppm();
	free(integers);

        return 0;
}

