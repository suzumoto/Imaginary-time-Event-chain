#include<cstdio>

int main(){
  FILE *fp;
  char filename[80] = "plot.gp";
  fp = std::fopen(filename,"w");
  fprintf(fp,"set xrange[-5.6691783*2:5.6691783*2]\n");
  fprintf(fp,"set yrange[0:100]\n");
  fprintf(fp,"set output \"../movie.gif\"\n");
  fprintf(fp,"set terminal gif animate optimize delay 2 size 480,360\n");
  for(int i = 0; i < 2000; ++i){
    fprintf(fp,"plot \"data%09d.dat\" w l\n",i);
  }
  fprintf(fp,"unset output\n");
  fprintf(fp,"unset terminal\n");
  std::fclose(fp);
  return 0;
}
    
