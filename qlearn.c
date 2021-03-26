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

static int randrange(int start, int stop, int step)
{
	int width = (stop - start)/step;
	return start + ((rand() % width)*step);
}

static double q_matrix_max(double **qMatrix, int state_size, int action)
{
	int *max_index = malloc(sizeof(int)*state_size);
	double temp_max = 0.0;
	int index_of_max;
	int j = 0;
	for (int i = 0; i < state_size; i++) {
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
	index_of_max = max_index[rand() % j];
	free(max_index);
	return qMatrix[action][index_of_max];
}

static double update(int current_state, int action, double **rMatrix,
		     double **qMatrix, int state_size, int action_size)
{
	double temp_max = 0.0, max_value = 0.0, sumA = 0.0;
	double gammaLR = 0.8;

	max_value = q_matrix_max(qMatrix, state_size, action);

	//Main updation
	qMatrix[current_state][action] =
	    rMatrix[current_state][action] + (gammaLR * max_value);
	for (int i = 0; i < state_size; i++) {
		for (int j = 0; j < action_size; j++) {
			if (qMatrix[i][j] > temp_max) {
				temp_max = qMatrix[i][j];
			}
		}
	}

	if (temp_max > 0) {
		for (int i = 0; i < state_size; i++) {
			for (int j = 0; j < action_size; j++) {
				sumA = sumA + (qMatrix[i][j] / temp_max);
			}
		}

		sumA = sumA * 100;
		return sumA;
	} else {
		return 0.0;
	}
}

static int available_actions(int state, int available_acts[], double **rMatrix,
			     int action_size)
{
	int k = 0, j = 0;
	while (j < action_size) {
		if (rMatrix[state][j] >= 0.0) {
			available_acts[k] = j;
			k++;
		}
		j++;
	}
	printf("\n");
	return k;
}

static int sample_next_action(int size, int available_acts[], int action_size)
{
	int a = randrange(0, action_size, 1);
	int next_action = available_acts[a % size];
	return next_action;
}

void q_learning_train(int epochs, int available_acts[], double scores[],
		      double **rMatrix, double **qMatrix, int state_size,
		      int action_size)
{
	int current_state, size_av_actions, action;
	double score;
	// Training the Q Matrix
	for (int i = 0; i < epochs; i++) {

		current_state = randrange(0, state_size, 1);
		size_av_actions = available_actions(current_state,
						    available_acts, rMatrix,
						    action_size);
		action = sample_next_action(size_av_actions, available_acts,
					    action_size);

		score = update(current_state, action, rMatrix, qMatrix,
			       state_size, action_size);
		scores[i] = score;

		printf("\nScore : %f", score);
	}
}
