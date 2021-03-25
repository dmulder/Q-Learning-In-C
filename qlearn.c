/*
   Copyright (C) 2020 Soham Bhure <soham.bhure18@vit.edu>
   Copyright (C) 2021 David Mulder <dmulder@samba.org>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "qlearn.h"

double gammaLR = 0.8;
int max_index[8];

int randrange(int start, int stop)
{
	int width = stop - start;
	return start + (rand() % width);
}

double update(int current_state, int action, double rMatrix[][8],
	      double qMatrix[][8])
{
	int i = 0, j = 0, k = 0, index_of_max;
	double temp_max = 0.0, max_value = 0.0, sumA = 0.0;

	//Collecting all the indexes where we have max in action row
	for (i = 0; i < 8; i++) {
		max_index[i] = 0;

		if (temp_max == qMatrix[action][i]) {
			max_index[j] = i;
			j++;
		} else if (temp_max < qMatrix[action][i]) {
			j = 0;
			temp_max = qMatrix[action][i];
			max_index[j] = i;
			j++;
		}
	}

	//Select a random out of all maximum
	int a = randrange(0, 8) % j;
	index_of_max = max_index[a];

	max_value = qMatrix[action][index_of_max];

	//Main updation
	qMatrix[current_state][action] =
	    rMatrix[current_state][action] + (gammaLR * max_value);
	temp_max = 0.0;
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (qMatrix[i][j] > temp_max) {
				temp_max = qMatrix[i][j];
			}
		}
	}

	if (temp_max > 0) {
		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				sumA = sumA + (qMatrix[i][j] / temp_max);
			}
		}

		sumA = sumA * 100;
		return sumA;
	} else {
		return 0.0;
	}
}

int available_actions(int state, int available_acts[], double rMatrix[][8])
{
	int k = 0, j = 0;
	while (j < 8) {
		if (rMatrix[state][j] >= 0.0) {
			available_acts[k] = j;
			k++;
		}
		j++;
	}
	printf("\n");
	return k;
}

int sample_next_action(int size, int available_acts[])
{
	int a = randrange(0, 8);
	int next_action = available_acts[a % size];
	return next_action;
}
