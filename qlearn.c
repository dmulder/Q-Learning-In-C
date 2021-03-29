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

static double q_matrix_max(double **qMatrix, int state, int action_size)
{
	double temp_max = 0.0;
	for (int i = 0; i < action_size; i++) {
		if (temp_max < qMatrix[state][i]) {
			temp_max = qMatrix[state][i];
		}
	}

	return temp_max;
}

static void update(int current_state, int action, double **qMatrix,
		   double gamma, double lr,
		   double (*reward)(int state, int action), int action_size)
{
	double max_value = 0.0;

	max_value = q_matrix_max(qMatrix, current_state, action_size);

	/*
	 * Main update
	 */
	qMatrix[current_state][action] = reward(current_state, action) +
					 lr * (gamma * max_value);
}

/*
 * Explore by selecting a random action
 */
void q_learn_explore(double **qMatrix, int state,
		     double gamma, double lr,
		     int state_size, int action_size,
		     int (*select_action)(int state, int action_size),
		     double (*reward)(int state, int action))
{
	int action;
	if (select_action != NULL) {
		action = select_action(state, action_size);
	} else {
		action = randrange(0, action_size, 1);
	}
	return update(state, action, qMatrix, gamma, lr, reward, action_size);
}

static int q_matrix_max_action(double **qMatrix, int state, int action_size)
{
	double temp_max = 0.0;
	int index_of_max = 0;
	for (int i = 0; i < action_size; i++) {
		if (temp_max < qMatrix[state][i]) {
			index_of_max = i;
			temp_max = temp_max;
		}
	}
	return index_of_max;
}

/*
 * Exploit by selecting the action with max value in qMatrix
 */
void q_learn_exploit(double **qMatrix, int state,
		     double gamma, double lr,
		     int state_size, int action_size,
		     double (*reward)(int state, int action))
{
	int action;

	action = q_matrix_max_action(qMatrix, state, action_size);
	update(state, action, qMatrix, gamma, lr, reward, action_size);
}

/*
 * Explore and exploit, with an explore frequency of epsilon
 */
int q_learn(double **qMatrix, double epsilon, int state,
	    double gamma, double lr,
	    int state_size, int action_size,
	    int (*select_action)(int state, int action_size),
	    double (*reward)(int state, int action))
{
	double choice = (double)rand() / (double)RAND_MAX;
	if (choice < epsilon) {
		q_learn_explore(qMatrix, state, gamma, lr, state_size,
				action_size, select_action, reward);
	} else {
		q_learn_exploit(qMatrix, state, gamma, lr,
				state_size, action_size, reward);
	}
	return q_matrix_max_action(qMatrix, state, action_size);
}
