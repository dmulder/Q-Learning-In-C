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

int randrange(int start, int stop, int step)
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

static void update(int current_state, int action, double **qMatrix,
		   double (*reward)(int state, int action),
		   int state_size, int action_size)
{
	double max_value = 0.0;
	double gammaLR = 0.8;

	max_value = q_matrix_max(qMatrix, state_size, action);

	//Main updation
	qMatrix[current_state][action] = reward(current_state, action) +
					 (gammaLR * max_value);
}

/*
 * Explore by selecting a random action
 */
void q_learn_explore(double **qMatrix, int state_size, int action_size,
		     int (*select_action)(int state, int action_size),
		     double (*reward)(int state, int action))
{
	int current_state, action;
	current_state = randrange(0, state_size, 1);
	if (select_action != NULL) {
		action = select_action(current_state, action_size);
	} else {
		action = randrange(0, action_size, 1);
	}
	update(current_state, action, qMatrix, reward,
	       state_size, action_size);
}


