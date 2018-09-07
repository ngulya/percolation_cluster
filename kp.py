import random

def have_up_friend(m, i, j2):
	if m[i-1][j2] == -1:
		return 0
	j22 = j2
	while mas[i][j22] > 0:
		if mas[i-1][j22] > 0:
			return mas[i-1][j22]
		j22 += 1
	return 0

def write_udlr(udlr_a, i,j,now):
	if i == 1:
		udlr_a[now][0] = 1
	if i == size:
		udlr_a[now][1] = 1
	if j == 1:
		udlr_a[now][2] = 1
	if j == size:
		udlr_a[now][3] = 1
	udlr_a[now][4] += 1

def time_change(mas, i, udlr_a):
	j = size
	while j > 0:
		if mas[i][j] > 0 and mas[i-1][j] > 0 and mas[i][j] != mas[i-1][j]:
			up = mas[i-1][j]
			now = mas[i][j]
			j2 = 1
			while j2 < size + 1:
				if mas[i-1][j] == up:
					mas[i-1][j] = now
				j2 += 1
			if udlr_a[up][0] == 1:
				udlr_a[now][0] = 1
			if udlr_a[up][1] == 1:
				udlr_a[now][1] = 1
			if udlr_a[up][2] == 1:
				udlr_a[now][2] = 1
			if udlr_a[up][3] == 1:
				udlr_a[now][3] = 1
			udlr_a[now][4] += udlr_a[up][4]
			udlr_a[up][0] = 0
			udlr_a[up][1] = 0
			udlr_a[up][2] = 0
			udlr_a[up][3] = 0
			udlr_a[up][4] = 0
		j -= 1

def this_is_per(mas, udlr_a):
	cluster = 2
	i = 1
	j = 1
	tmp = 0
	while i < size + 1:
		while j < size + 1:
			j2 = j
			while j2 < size + 1 and mas[i][j2] == 0:
				j2 += 1
			j = j2
			if mas[i][j2] != -1:
				tmp = have_up_friend(mas, i, j2)
			if tmp != 0:
				now = tmp
			else:
				now = cluster
				cluster += 1
			while mas[i][j] > 0:
				write_udlr(udlr_a, i, j, now)
				mas[i][j] = now
				j += 1
			j += 1
		time_change(mas, i, udlr_a)
		j = 1
		i += 1
	i = 0
	while cluster > 0:
		if udlr_a[cluster][4] > i and udlr_a[cluster][3] == 1 and udlr_a[cluster][2] == 1 and udlr_a[cluster][1] == 1 and udlr_a[cluster][0] == 1:		
			i = udlr_a[cluster][4]
			if i > 40000:
				print(cluster, i)
				print_m()
				exit()
		cluster -= 1
	return i

def print_m():
	global mas, size
	i = 1
	j = 1
	while i < size + 1:
		j = 1
		while j < size + 1:
			print(mas[i][j],'	', end='')
			j += 1
		print("\n")
		i += 1

def mass(Probability, mas, size):
	i = 1
	j = 1
	while i < size + 1:
		j = 1
		while j < size + 1:
			if random.random() < Probability:
				mas[i][j] = 1
			else:
				mas[i][j] = 0
			j += 1
		i += 1

def udlr_a_NULL(udlr_a, size):
	maxs = int(size*size/2)
	i = 0
	j = 0
	while i < maxs:
		j = 0
		while j < 5:
			udlr_a[i][j] = 0
			j += 1
		i += 1

random.seed(34)
size = 100
mas = []
for i in range(size + 2):
	mas.append([])
	for j in range(size + 2):
		if i == 0 or j == 0 or i == size + 1 or j == size + 1:
			mas[i].append(-1)
		else:
			mas[i].append(0)
udlr_a = []
maxs = int(size*size/2)
for i in range(maxs):
	udlr_a.append([])
	for j in range(5):
		udlr_a[i].append(0)


sq = open('square.csv', 'w')

re = open('NumP.csv', 'w')
i = 0
sq.write('Prob,Sq\n')
re.write('Prob,NumP\n')
while i < 100:
	j = 0
	s = 0
	tmp = 0
	perc = 0
	while j < 25:
		udlr_a_NULL(udlr_a, size)
		mass(float(i/100), mas, size)
		tmp = this_is_per(mas, udlr_a)
		s += tmp
		if tmp != 0:
			perc += 1
		j += 1
	s /= j
	print(i, s, perc)
	sq.write(str(i) + ',' + str(s) + '\n')
	re.write(str(i) + ',' + str(perc)+ '\n')
	i += 1
sq.close()
re.close()