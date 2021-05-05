#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
int main(int argc, char *argv[])
{

  int server = 1;
  int Q = 0;
  int total = 0;
  double lamda = 0.01;
  double mu = 0.01;
  double delta = 0.01;
  double arrival_prob = 0;
  double depature_prob = 0;
  double arrival_num = 0;
  int arrival;
  double block_num = 0;
  int depature_num = 0;
  int service_current = 0;
  int time_unit = 100000;
  int count_lam = 0;
  int count_mu = 0;
  double count = time_unit / delta;
  double blocking_prob[10];
  double blocking_prob_average[24];
  double erlang[24];
  int print = 0;
  srand(time(NULL));
  Q = atoi(argv[1]);

  FILE *outfp;
  outfp = fopen("BProbabilityTable.txt", "w");

  for (int i = 0; i < 2; i++)
  {
    for (server = 1; server <= 10; server += 4)
    {

      if (server == 9)
      {
        server++;
      }
      if (i == 0)
      {
        total = server;
      }
      else if (i == 1)
      {
        total = server * 2;
      }
      count_lam = 0;
      lamda = 0.01;
      while (count_lam < 4)
      {
        count_lam++;
        count_mu = 0;
        mu = 0.01;
        while (count_mu < 6)
        {
          count_mu++;
          for (int j = 0; j < 10; j++)
          {
            arrival_num = 0;
            block_num = 0;
            service_current = 0;
            int i = 0;
            while (i < count)
            {
              double x = (double)rand() / (RAND_MAX + 1.0);
              double y = (double)rand() / (RAND_MAX + 1.0);

              arrival_prob = 1 - exp(-lamda * delta);
              if (service_current >= server)
              {
                depature_prob = (1 - exp(-mu * delta * server));
              }
              else
              {
                depature_prob = 1 - exp(-mu * delta * service_current);
              }

              if (x <= arrival_prob && service_current < total)
              {
                arrival_num++;
                service_current++;
              }
              else if (x <= arrival_prob && service_current >= total)
              {
                arrival_num++;
                block_num++;
              }

              if (y <= depature_prob)
              {
                if (service_current >= 1)
                  service_current--;
              }

              i++;
            }
            printf("block_num:%f\n", block_num);
            printf("arrival_num:%f\n", arrival_num);
            if (arrival_num == 0)
            {
              blocking_prob[j] = 0;
            }
            else
            {
              blocking_prob[j] = block_num / arrival_num;
            }
          }
          double sum = 0;
          for (int j = 0; j < 10; j++)
          {
            sum += blocking_prob[j];
          }
          blocking_prob_average[print] = sum / 10;
          erlang[print] = lamda / mu;

          printf("BP:%f\n", blocking_prob_average[print]);
          printf("Erlang:%f\n", erlang);
          mu = mu * 4;
          print++;
          if (print == 24)
          {
            fprintf(outfp, "S = %2d      ", server);
            for (int i = 0; i < 24; i++)
            {
              fprintf(outfp, "BP[%2d]      ", i + 1);
            }

            fprintf(outfp, "\n");
            fprintf(outfp, "            ");
            for (int i = 0; i < 24; i++)
            {
              fprintf(outfp, "%-12f", blocking_prob_average[i]);
            }

            fprintf(outfp, "\n\n");
            fprintf(outfp, "Q = %2d      ", total - server);
            for (int i = 0; i < 24; i++)
            {
              fprintf(outfp, "Erlang[%2d]  ", i + 1);
            }
            fprintf(outfp, "\n");
            fprintf(outfp, "            ");
            for (int i = 0; i < 24; i++)
            {
              fprintf(outfp, "%-12f", erlang[i]);
            }
            fprintf(outfp, "\n\n\n");
            print = 0;
          }
        }
        lamda = lamda * 10;
      }
    }
  }
  fclose(outfp);
  return 0;
}