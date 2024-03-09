#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXIMUM 69
#define MINIMUM 3

void user_input(int *height_p, int *width_p, int *fence_height_p);
void build_roof(int width, int height);
void build_base(int width, int height, int fence_height);
void print_fence_top_bot(int fence_height);

int main()
{
  int height;
  int width;
  int fence_height = -1;
  user_input(&height, &width, &fence_height);
  build_roof(width, height);
  build_base(width, height, fence_height);
}

void user_input(int *height_p, int *width_p, int *fence_height_p)
{
  char space_test = {' '};
  int tester = scanf("%d %d%c%d", width_p, height_p, &space_test, fence_height_p);

  int height = *height_p;
  int width = *width_p;
  int fence_height = *fence_height_p;

  if (tester < 2)
  {
    fprintf(stderr, "Error: Chybny vstup!\n");
    exit(100);
  }
  else if (tester == 3 && space_test == ' ')
  {
    fprintf(stderr, "Error: Chybny vstup!\n");
    exit(100);
  }
  else if (height > MAXIMUM || height < MINIMUM || width > MAXIMUM || width < MINIMUM)
  {
    fprintf(stderr, "Error: Vstup mimo interval!\n");
    exit(101);
  }
  else if (width % 2 == 0)
  {
    fprintf(stderr, "Error: Sirka neni liche cislo!\n");
    exit(102);
  }
  else if ((height == width && fence_height >= height) || fence_height == 0)
  {
    fprintf(stderr, "Error: Neplatna velikost plotu!\n");
    exit(103);
  }
}

void build_roof(int width, int height)
{
  int roof_mid = 0;
  int roof_left = 0;
  int roof_left_build;
  while (roof_mid < width - 2)
  {
    if (roof_mid == 0)
    {
      for (roof_left_build = 1; roof_left_build <= ((width - 1) / 2) - roof_left; roof_left_build++)
      {
        printf(" ");
      }
      printf("X");
      roof_left += 1;
      roof_mid += 1;
    }
    else
    {
      for (roof_left_build = 1; roof_left_build <= (width - 1) / 2 - roof_left; roof_left_build++)
      {
        printf(" ");
      }
      printf("X");
      for (int i = 1; i <= roof_mid; i++)
      {
        printf(" ");
      }
      printf("X");
      roof_left += 1;
      roof_mid += 2;
    }
    printf("\n");
  }
}

void build_base(int width, int height, int fence_height)
{
  for (int height_build = 1; height_build <= height; height_build++)
  {
    if (height_build != 1 && height_build != height)
    {
      for (int wall_build = 1; wall_build <= width; wall_build++)
      {
        if (wall_build != 1 && wall_build != width)
        {
          if (fence_height > 0)
          {
            if (height_build % 2 == 0)
            {
              wall_build % 2 == 0 ? printf("o") : printf("*");
            }
            else
            {
              wall_build % 2 != 0 ? printf("o") : printf("*");
            }
          }
          else
          {
            printf(" ");
          }
        }
        else
        {
          printf("X");
        }
      }
      if (height_build == (height - fence_height) + 1 && height == width)
      {
        print_fence_top_bot(fence_height);
      }
      else if (height_build > (height - fence_height) && height == width)
      {
        fence_height % 2 == 0 ? printf(" ") : printf("|");
        if (fence_height % 2 == 0)
        {
          for (int fence_build = 0; fence_build < fence_height - 1; fence_build++)
          {
            fence_build % 2 == 0 ? printf("|") : printf(" ");
          }
        }
        else
        {
          for (int fence_build = 0; fence_build < fence_height - 1; fence_build++)
          {
            fence_build % 2 != 0 ? printf("|") : printf(" ");
          }
        }
      }
    }
    else
    {
      for (int floor_build = 1; floor_build <= width; floor_build++)
      {
        printf("X");
      }

      if (height == width)
      {
        if (height_build == height)
        {
          print_fence_top_bot(fence_height);
        }
      }
    }
    printf("\n");
  }
}

void print_fence_top_bot(int fence_height)
{
  fence_height % 2 == 0 ? printf("-") : printf("|");
  if (fence_height % 2 == 0)
  {
    for (int fence_build = 0; fence_build < fence_height - 1; fence_build++)
    {
      fence_build % 2 == 0 ? printf("|") : printf("-");
    }
  }
  else
  {
    for (int fence_build = 0; fence_build < fence_height - 1; fence_build++)
    {
      fence_build % 2 != 0 ? printf("|") : printf("-");
    }
  }
}
