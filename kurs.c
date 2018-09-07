#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#define SZE 1000//Size array
#define chI (SZE * SZE/4)
void	free_mas_udlr(int **mas, int **udlr)
{
	int i;

	i = 0;
	while(i < SZE)
	{
		free(mas[i] = (int *)malloc(sizeof(int) * SZE));
		i++;
	}
	free(mas);

	i = 0;
	while(i < chI)
	{
		free(udlr[i]);
		i++;
	}
	free(udlr);
}
void	nprint(int **mas)
{
	int i;
	int j;
	i = 0;
	j = 0;
	while(i < SZE)
	{
		j = 0;
		while(j < SZE)
		{
			printf("%2d ", mas[i][j]);
			j++;
		}
		printf("\n");
		i++;
	}
	printf("\n");
}
void	random_mas(int **mas, int ps)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while(i < SZE)
	{
		j = 0;
		while(j < SZE)
		{
			if ((1 + rand()%100) <= ps)//1...100
				mas[i][j]= 1;
			else
				mas[i][j]= 0;
			j++;
		}
		i++;
	}
}
int 	have_up_friend(int **mas, int i, int j)
{
	if (i == 0)
		return 0;
	while(j < SZE && mas[i][j])
	{
		if(mas[i-1][j])
			return mas[i-1][j];
		j++;
	}
	return 0;
}
void	change_recursive(int **mas, int i, int j, int must, int shit)
{
	mas[i][j] = must;
	if(i > 0 && mas[i - 1][j] == shit)
		change_recursive(mas, i - 1, j, must, shit);
	if(i < SZE - 1 && mas[i + 1][j] == shit)
		change_recursive(mas, i + 1, j, must, shit);
	if(j > 0 && mas[i][j-1] == shit)
		change_recursive(mas, i, j-1, must, shit);
	if(j < SZE - 1 && mas[i][j+1] == shit)
		change_recursive(mas, i, j+1, must, shit);
}
void	time_change(int **mas, int i, int j, int **udlr)
{
	int 	shit;
	int 	must;
	int	up;
	int	now;
	j = SZE-1;
	while(j >= 0)
	{
		if(i > 0 && mas[i][j] && mas[i-1][j] && mas[i][j] != mas[i-1][j])
		{
			up = mas[i-1][j];
			now = mas[i][j];
			if(udlr[up][4] < udlr[now][4])
			{
				must = mas[i][j];
				shit = mas[i-1][j];
				change_recursive(mas, i-1, j, must, shit);
			}
			else
			{
				must = mas[i-1][j];
				shit = mas[i][j];
				change_recursive(mas, i, j, must, shit);
			}
			if(udlr[shit][0])
				udlr[must][0] = 1;
			if(udlr[shit][1])
				udlr[must][1] = 1;
			if(udlr[shit][2])
				udlr[must][2] = 1;
			if(udlr[shit][3])
				udlr[must][3] = 1;
			udlr[must][4] += udlr[shit][4];
		}
		j--;
	}}

void	write_udlr(int **udlr, int i, int j, int now)
{
	if(i == 0)
		udlr[now][0] = 1;
	if(i == SZE-1)
		udlr[now][1] = 1;
	if(j == 0)
		udlr[now][2] = 1;
	if(j == SZE-1)
		udlr[now][3] = 1;
	udlr[now][4] += 1;
}
int		this_perec(int **mas, int **udlr)
{
	int	i;
	int	j;
	int 	tmp;
	int 	cluster;
	int 	now;

	cluster = 2;
	i = 0;
	j = 0;
	while(i < SZE)
	{
		while(j < SZE)
		{
			while(j < SZE && !mas[i][j])
				j++;
			if (j < SZE)
			{
				tmp = have_up_friend(mas, i, j);
				if (tmp)
					now = tmp;
				else
				{
					now = cluster;
					cluster += 1;
				}
				while(j < SZE && mas[i][j])
				{
					write_udlr(udlr, i, j, now);
					mas[i][j] = now;
					j++;
				}
			}
		}
		time_change(mas, i, j, udlr);
		j = 0;
		i++;
	}
	i = 0;
	while(cluster >= 0)
	{
		if (udlr[cluster][4] > i && udlr[cluster][0] && udlr[cluster][1] && udlr[cluster][2] && udlr[cluster][3])
			i = udlr[cluster][4];
		cluster--;
	}
	return i;
}

int		null_udlr(int  **udlr)
{
	int i;

	i = 0;
	while(i < chI)
	{
		udlr[i][0] = 0;
		udlr[i][1] = 0;
		udlr[i][2] = 0;
		udlr[i][3] = 0;
		udlr[i][4] = 0;
		i++;
	}
}

int 	main()
{
	srand(time(NULL));
	FILE *sf;
	FILE *pf;
	int **udlr;
	int	**mas;
	int	tmp;
	int s;
	int perc;
	int i;
	int j;
	int start;
	i = 0;	
	mas = (int **)malloc(sizeof(int*) * SZE);
	while(i < SZE)
	{
		mas[i] = (int *)malloc(sizeof(int) * SZE);
		i++;
	}
	i = 0;
	udlr = (int **)malloc(sizeof(int*) * chI);
	while(i < chI)
	{
		udlr[i] = (int *)malloc(sizeof(int) * 5);
		i++;
	}
	sf = fopen("square.txt", "w");
	pf = fopen("perc.txt", "w");
	fprintf(sf, "%s\n", "Prob,Sq");
	fprintf(pf, "%s\n", "Prob,NumP");
	i = 0;
	while(i < 100)
	{
		j = 0;
		s = 0;
		tmp = 0;
		perc = 0;
		while(j < 100)
		{
			null_udlr(udlr);
			random_mas(mas, i);
			tmp = this_perec(mas, udlr);
			s += tmp;
			perc += tmp ? 1 : 0;
			printf("[%d] [%d]\n", i, j);
			j++;
		}
		s /= j;
		printf("s=%d p=%d\n", s, perc);
		fprintf(sf, "%d,%d\n",i,s);
		fprintf(pf, "%d,%d\n",i,perc);
		i++;
	}
	fclose(sf);
	fclose(pf);

	free_mas_udlr(mas, udlr);
	return 0;}